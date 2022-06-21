
#pragma once
#include <SDL2\SDL.h>
#include <vector>
#include <complex>
#include <fftw3.h>
#include <memory>
#include "../Common/AudioConst.h"


/// <summary>
/// Class represents Audio driver with additional functionality that handles FFT transform.
/// Singleton implementation written in https://stackoverflow.com/questions/1008019/c-singleton-design-pattern/1008289.
/// </summary>
class AudioIO {
	friend class Application; // Application class can access its private members

public:
	static AudioIO& getInstance()
	{
		static AudioIO instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}
private:
	AudioIO();                    // Constructor? (the {} brackets) are needed here.
	~AudioIO();                    // Constructor? (the {} brackets) are needed here.
	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.
public:
	AudioIO(AudioIO const&) = delete;
	void operator=(AudioIO const&) = delete;

	// Note: Scott Meyers mentions in his Effective Modern
	//       C++ book, that deleted functions should generally
	//       be public as it results in better error messages
	//       due to the compilers behavior to check accessibility
	//       before deleted status


	/// <summary>
	/// Audio data struct. Passed to SDL audio as a user data. 
	/// </summary>
	struct AudioData {
		Uint8* position;
		Uint32 length;
		SDL_AudioFormat format;
		Uint8 silence;
		fftw_plan planL;
		fftw_plan planR;

		fftw_complex* inL; // Time domain
		fftw_complex* inR; // Time domain
		fftw_complex* outL; // Frequency domain
		fftw_complex* outR; // Frequency domain

		double freq[SAMPLE_NUM/2]; // X-axis - frequency
		double magn[SAMPLE_NUM/2]; // Y-axis - magnitude
		double freqR[SAMPLE_NUM/2]; // X-axis - frequency
		double magnR[SAMPLE_NUM/2]; // Y-axis - magnitude

		double averageB; // average of bass domain
		double averageM; // average of mid-range domain
		double averageT; // average of treble domain
		
		double averageBR; // average of bass domain
		double averageMR; // average of mid-range domain
		double averageTR; // average of treble domain


		bool isStereoSepar = true;
		bool m_haveData; // if buffer exists

	};
	/// <summary>
	/// Wrapper around audio data.
	/// </summary>
	struct wrapper {
		Uint8* stream;
		struct AudioData* audio;
	};


	bool OpenFile(std::string fileName);			// Open .wav file
	void PlayAudio();								// Play Audio
	void PlayPausedAudio();							// Play if audio is paused.
	void PauseAudio();								// Pause audio.
	void SwitchAudioFile(std::string filename);		// Switch between files.

	AudioData* getAudioData() { return this->audio; };


	static double Get16bitAudioSample(Uint8* bytebuffer, SDL_AudioFormat format); // 16-bit audio sample
	static void myCallback(void*, Uint8*, int);									  // callback function passed (needed for SDL implementation)
	static void output(struct wrapper arg);		

	

private:
	// variables needed for implementations of SDL audio processing.
	SDL_AudioDeviceID m_deviceId;
	SDL_AudioSpec m_dataType;
	SDL_AudioFormat m_dataFormat;
	Uint32 m_length;
	Uint8* m_buffer;



	AudioData* audio = new AudioData();

	// name of the played .wav file.
	const char* m_fileName;

};