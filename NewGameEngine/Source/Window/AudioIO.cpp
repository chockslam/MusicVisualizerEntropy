#include "AudioIO.h"
#include <implot.h>

AudioIO::AudioIO()
{
	/// <summary>
	/// Default parameters
	/// </summary>
	SDL_Init(SDL_INIT_AUDIO);
	m_SamplesNum = 2048;
}

AudioIO::~AudioIO()
{
	if (m_haveData) {
		SDL_FreeWAV(m_buffer);
	}
}



bool AudioIO::OpenFile(const char* fileName)
{
	m_fileName = fileName;

	if (SDL_LoadWAV(m_fileName, &m_dataType, &m_buffer, &m_length) == nullptr) {
		return false;

	}
	else {
		m_haveData = true;
		m_dataFormat = m_dataType.format;
		
		//m_dataType.freq = sampleRate;
		m_dataType.samples = m_SamplesNum;


		audio->in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_SamplesNum);
		audio->out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_SamplesNum);


		audio->plan = fftw_plan_dft_1d(m_SamplesNum, audio->in, audio->out, FFTW_FORWARD, FFTW_MEASURE);
		m_dataType.userdata = audio;
		m_dataType.callback = myCallback;
		
		audio->position = m_buffer;
		audio->length = m_length;
		audio->format = m_dataFormat;
		audio->silence = m_dataType.silence;
		return true;
	}

	

	
}

void AudioIO::PlayAudio()
{
	m_deviceId = SDL_OpenAudioDevice(NULL, 0, &m_dataType, NULL, 0);
	//auto error = SDL_GetError();
	int success = SDL_QueueAudio(m_deviceId, m_buffer, m_length);
	SDL_PauseAudioDevice(m_deviceId, 0);
}


double AudioIO::Get16bitAudioSample(Uint8* bytebuffer, SDL_AudioFormat format)
{
	Uint16 val = 0x0;

	if (SDL_AUDIO_ISLITTLEENDIAN(format))
		val = (uint16_t)bytebuffer[0] | ((uint16_t)bytebuffer[1] << 8);
	else
		val = ((uint16_t)bytebuffer[0] << 8) | (uint16_t)bytebuffer[1];

	if (SDL_AUDIO_ISSIGNED(format))
		return ((int16_t)val) / 32768.0;

	return val / 65535.0;
}

void AudioIO::myCallback(void* userData, Uint8* stream, int len)
{
	
	
	struct AudioData* audio = (struct AudioData*)userData;
	//pthread_t th;
	struct wrapper wrap;

	if (audio->length == 0)
		return;


	//SDL_memset(stream, 0, len);
	SDL_MixAudio(stream, audio->position, len, SDL_MIX_MAXVOLUME/20);

	//stream = audio->position;

	wrap.stream = stream;
	wrap.audio = audio;

	output(wrap);

	Uint32 length = (Uint32)len;
	length = (length > audio->length ? audio->length : length);

	SDL_memcpy(stream, audio->position, length);

	audio->position += length;
	audio->length -= length;
}

void AudioIO::output(struct wrapper arg)
{

	//smphSignalCallBackToThread.acquire();
	struct wrapper wrap = arg;
	
	for (int i = 0; i < m_SamplesNum; i++)
	{

		//getting values from stream and applying hann windowing function
		double multiplier = 0.5 * (1 - cos(2 * 3.14 * i / m_SamplesNum));
		
		wrap.audio->in[i][0] = Get16bitAudioSample(
			wrap.stream,
			wrap.audio->format) * multiplier;
		wrap.audio->in[i][1] = 0.0;

		wrap.stream += 2;

	}

	fftw_execute(wrap.audio->plan);

	
	int countB = 0, 
		countM = 0, 
		countT = 0;

	float cumB = 0,
	      cumM = 0,
		  cumT = 0;
	
	for (int i = 0; i < m_SamplesNum / 2; i++) {
		double re = wrap.audio->out[i][0];
		double im = wrap.audio->out[i][1];
		double freq = ((double)i) * sampleRate / ((double)m_SamplesNum);
		double magn = sqrt(re * re + im * im);

		if (freq > 50.0 && freq < 250.0) {
			cumB += magn;
			countB++;
		}
		if (freq > 250.0 && freq < 4000.0) {
			cumM += magn;
			countM++;
		}
		if (freq > 4000.0) {
			cumT += magn;
			countT++;
		}
		
		wrap.audio->freq[i] = freq;
		wrap.audio->magn[i] = magn;

	}

	wrap.audio->averageB = cumB / countB;
	wrap.audio->averageM = cumM / countM;
	wrap.audio->averageT = cumT / countT;



	printf("");

}