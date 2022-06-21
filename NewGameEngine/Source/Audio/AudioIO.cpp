// SDL and FFTW handling is implemented in the same fashion as  arcogabbo did it in https://github.com/arcogabbo/musicvisualizer in C language.
// The code was transferred from C-code to C++ code.
// Since SDL and FFTW are both written in C. Raw pointers instead of smart pointers are utilized in this section of the code.

#include "AudioIO.h"
#include <implot.h>


AudioIO::AudioIO()
{
	SDL_Init(SDL_INIT_AUDIO);
}

AudioIO::~AudioIO()
{
	// Stop Audio Thread before cleaning up memory
	SDL_LockAudioDevice(m_deviceId);
	
	// Clean up memory if data in the audio buffer exists.
	if (audio->m_haveData) {
		// Free WAV buffer.f
		SDL_FreeWAV(m_buffer);
		SDL_CloseAudioDevice(m_deviceId);
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		SDL_Quit();
		audio->m_haveData = false;
		if (audio->inL) {
			fftw_free(audio->inL);
		}
		if (audio->outL) {
			fftw_free(audio->outL);
		}
		
		if (audio->isStereoSepar&&audio->outR) {
			fftw_free(audio->outR);
		}
		if (audio->isStereoSepar&&audio->inR) {
			fftw_free(audio->inR);
		}
		// clean up after fftw.
		fftw_destroy_plan(audio->planL);
		fftw_destroy_plan(audio->planR);
		fftw_cleanup();
		// delete audio object and stop pointing to the object.
		delete audio;
		audio = nullptr;
		
	}

}



bool AudioIO::OpenFile(std::string fileName)
{
	m_fileName = fileName.c_str();

	// If not loaded return
	if (SDL_LoadWAV(m_fileName, &m_dataType, &m_buffer, &m_length) == nullptr) {
		return false;

	}
	else {
		audio->m_haveData = true;
		m_dataFormat = m_dataType.format;
		if (m_dataType.channels == 2) {
			audio->isStereoSepar = true;
		}
		else {
			audio->isStereoSepar = false;
		}
		m_dataType.samples = SAMPLE_NUM;

		// init fftw input and output arrays 
		if (audio->inL) {
			fftw_free(audio->inL);
		}
		if (audio->outL) {
			fftw_free(audio->outL);
		}
		if (audio->isStereoSepar && audio->outR) {
			fftw_free(audio->outR);
		}
		if (audio->isStereoSepar && audio->inR) {
			fftw_free(audio->inR);
		}
		audio->inL = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * SAMPLE_NUM);
		audio->outL = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * SAMPLE_NUM);

		if (audio->isStereoSepar) {
			audio->outR = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * SAMPLE_NUM);
			audio->inR = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * SAMPLE_NUM);
		}
		// Initialize plan of fftw
		if (audio->planL)
			fftw_destroy_plan(audio->planL);
		if (audio->planR)
			fftw_destroy_plan(audio->planR);

		fftw_cleanup();

		audio->planL = fftw_plan_dft_1d(SAMPLE_NUM, audio->inL, audio->outL, FFTW_FORWARD, FFTW_MEASURE);
		if (audio->isStereoSepar) {
			audio->planR = fftw_plan_dft_1d(SAMPLE_NUM, audio->inR, audio->outR, FFTW_FORWARD, FFTW_MEASURE);
		}
		// User Data (AudioData*) passed to SDL audio function 
		m_dataType.userdata = audio;
		// Assign callback function (function pointer)
		m_dataType.callback = myCallback;
		
		// Other audio parameters
		audio->position = m_buffer;
		audio->length = m_length;
		audio->format = m_dataFormat;
		audio->silence = m_dataType.silence;
		return true;
	}

	

	
}

void AudioIO::PlayAudio()
{
	// Init device ID
	m_deviceId = SDL_OpenAudioDevice(NULL, 0, &m_dataType, NULL, 0);
	int success = SDL_QueueAudio(m_deviceId, m_buffer, m_length);
	SDL_PauseAudioDevice(m_deviceId, 0);
}

void AudioIO::PlayPausedAudio()
{
	SDL_PauseAudioDevice(m_deviceId, 0);
}

void AudioIO::PauseAudio()
{
	SDL_PauseAudioDevice(m_deviceId, 1);
}

void AudioIO::SwitchAudioFile(std::string filename)
{
	// Reset Audio
	SDL_CloseAudioDevice(m_deviceId);
	SDL_FreeWAV(m_buffer);


	audio->m_haveData = false;

	//MEMORY LEAKED SOLVED
	if (audio->inL) {
		fftw_free(audio->inL);
		audio->inL = nullptr;
	}
	if (audio->inR) {
		fftw_free(audio->inL);
		audio->inL = nullptr;
	}
	if (audio->outL) {
		fftw_free(audio->outL);
		audio->outL = nullptr;
	}

	if (audio->outR) {
		fftw_free(audio->outR);
		audio->outR = nullptr;
	}

	// play new file
	OpenFile(filename);
	PlayAudio();
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


/// <summary>
///		callback function needed for SDL implementation
/// </summary>
/// <param name="userData"> pointer to Audio Data is passed as a parameter </param>
/// <param name="stream"> audio stream </param>
/// <param name="len"> length of the </param>
void AudioIO::myCallback(void* userData, Uint8* stream, int len)
{
	
	// cast void* back to AudioData*
	auto audio = reinterpret_cast<AudioData*>(userData);
	// wrapper around
	struct wrapper wrap;

	if (audio->length == 0)
		return;

	//fill in wrapper
	wrap.stream = stream;
	wrap.audio = audio;

	if(audio->m_haveData)
		output(wrap); // if audio is playing, pass it to the FFT processor.

	Uint32 length = (Uint32)len;
	length = (length > audio->length ? audio->length : length);

	SDL_memcpy(stream, audio->position, length);

	audio->position += length;
	audio->length -= length;

	
	
}

void AudioIO::output(struct wrapper arg)
{

	struct wrapper wrap = arg;
	// Put every sample to audio in[] array .
	for (int i = 0; i < SAMPLE_NUM; i++)
	{

		//getting values from stream and applying hann windowing function
		double multiplier = 0.5 * (1 - cos(2 * 3.14 * i / SAMPLE_NUM));
		
		wrap.audio->inL[i][0] = Get16bitAudioSample(
			wrap.stream,
			wrap.audio->format) * multiplier;
		wrap.audio->inL[i][1] = 0.0;

		wrap.stream += 2;//skip 2*8 = 16 bits.
		if (wrap.audio->isStereoSepar) {
			wrap.audio->inR[i][0] = Get16bitAudioSample(
				wrap.stream,
				wrap.audio->format) * multiplier;
			wrap.audio->inR[i][1] = 0.0;
			wrap.stream += 2;//skip 2*8 = 16 bits.
		}
		

	}
	// Execute FFT transform.
	fftw_execute(wrap.audio->planL);
	if (wrap.audio->isStereoSepar) {
		fftw_execute(wrap.audio->planR);
	}
	// Number of samples in each frequency domain
	int countB = 1, 
		countM = 1, 
		countT = 1;

	// Accumulative magnitude in each domain
	double cumB = 0,
	       cumM = 0,
		   cumT = 0;
	
	// For every Output sample.
	for (int i = 0; i < SAMPLE_NUM / 2; i++) {
		
		double re = wrap.audio->outL[i][0];
		double im = wrap.audio->outL[i][1];




		double freq = ((double)i) * SAMPLE_RATE / ((double)SAMPLE_NUM); // Approximate frequency
		double magn = sqrt(re * re + im * im); //magnitude (convert from imaginary numbers to real numbers)

		

		// if magnitude > threshold, use it to calculate average.
		if (magn > MAGN_THRESHOLD) {
			if (freq > BASS_START && freq < BASS_END) {
				cumB += magn;
				countB++;
			}
			if (freq > MID_START && freq < MID_END) {
				cumM += magn;
				countM++;
			}
			if (freq > TREBLE_START) {
				cumT += magn;
				countT++;
			}
		}
		
		
			wrap.audio->freq[i] = freq;
			wrap.audio->magn[i] = magn;
		
		
	}
	// Calculate average (accumulative magnitude/amount of samples in the domain) 
	double avB = cumB / countB,
		avM = cumM / countM,
		avT = cumT / countT;

	// If average equals to 0, set to default
	if (avB == 0.0 && avM == 0.0 && avT == 0.0) {
		avB = 0.75;
		avM = 0.5;
		avT = 0.1;
	}
	// Store it in the wrapper.
	wrap.audio->averageB = avB;
	wrap.audio->averageM = avM;
	wrap.audio->averageT = avT;

	if (wrap.audio->isStereoSepar) {


		// Number of samples in each frequency domain
		int countBR = 1,
			countMR = 1,
			countTR = 1;

		// Accumulative magnitude in each domain
		double	cumBR = 0,
				cumMR = 0,
				cumTR = 0;
		for (int i = 0; i < SAMPLE_NUM / 2; i++) {

			double re = wrap.audio->outR[i][0];
			double im = wrap.audio->outR[i][1];

			double freq = ((double)i) * SAMPLE_RATE / ((double)SAMPLE_NUM); // Approximate frequency
			double magn = sqrt(re * re + im * im); //magnitude (convert from imaginary numbers to real numbers)

			// if magnitude > threshold, use it to calculate average.
			if (magn > MAGN_THRESHOLD) {
				if (freq > BASS_START && freq < BASS_END) {
					cumBR += magn;
					countBR++;
				}
				if (freq > MID_START && freq < MID_END) {
					cumMR += magn;
					countMR++;
				}
				if (freq > TREBLE_START) {
					cumTR += magn;
					countTR++;
				}
			}

			// Put every sample to
			//if (freq<=11100) {

			wrap.audio->freqR[i] = freq;
			wrap.audio->magnR[i] = magn;
			
		}
		double	avBR = cumBR / countBR,
				avMR = cumMR / countMR,
				avTR = cumTR / countTR;

		// If average equals to 0, set to default
		if (avBR == 0.0 && avMR == 0.0 && avTR == 0.0) {
			avBR = 0.75;
			avMR = 0.5;
			avTR = 0.1;
		}
		// Store it in the wrapper.
		wrap.audio->averageBR = avBR;
		wrap.audio->averageMR = avMR;
		wrap.audio->averageTR = avTR;

	}
	
	
}