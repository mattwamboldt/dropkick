#include "Audio.h"
#include <fstream>
#include <string>
using namespace std;

void InitAudio()
{
	bounceAudioLength = (Uint32)(FREQUENCY * SOUND_TIME);
	bounceAudioData = new Sint8[bounceAudioLength];
	SDL_memset(bounceAudioData, 0, bounceAudioLength);
	bounceRemaining = 0;
}

void DestroyAudio()
{
	if (bounceAudioData != NULL)
	{
		delete[] bounceAudioData;
	}
}

void generateNoise()
{
	int max_amplitude = 127; // 8 bit audio
	for (Uint32 i = 0; i < bounceAudioLength; i++)
	{
		bounceAudioData[i] = (rand() * 255) - 127;
	}

	bounceRemaining = bounceAudioLength;
}

void generateSquare(double frequency)
{
	int max_amplitude = 127; // 8 bit audio
	double period = (M_PI * 2 * frequency) / FREQUENCY; //defines the change in amplitude over time

	for (Uint32 i = bounceRemaining; i < bounceAudioLength; i++)
	{
		double sineVal = SDL_sin(period * i) * 1;
		if (sineVal > 0.0)
		{
			bounceAudioData[i] = max_amplitude;
		}
		else
		{
			bounceAudioData[i] = -max_amplitude;
		}
	}

	bounceRemaining = bounceAudioLength;
}

void generateSound(double frequency)
{
	int max_amplitude = 127; // 8 bit audio
	double period = (M_PI * 2 * frequency) / FREQUENCY; //defines the change in amplitude over time
	for (Uint32 i = 0; i < bounceRemaining; i++)
	{
		int amplitude = (int)(max_amplitude * SDL_sin(period * i));
		bounceAudioData[i] = (bounceAudioData[i] + amplitude) / 2;
	}

	for (Uint32 i = bounceRemaining; i < bounceAudioLength; i++)
	{
		bounceAudioData[i] = (Uint8)(max_amplitude * SDL_sin(period * i));
	}

	bounceRemaining = bounceAudioLength;

	Uint32 endTrim = bounceRemaining - 1;
	
	//attempt to trim a click off the end when part of the wave doesn't stop on silence cleanly
	if (bounceAudioData[endTrim] > 0)
	{
		while (bounceAudioData[endTrim] > 0) bounceAudioData[endTrim--] = 0;
	}
	else if (bounceAudioData[endTrim] < 0)
	{
		while (bounceAudioData[endTrim] < 0) bounceAudioData[endTrim--] = 0;
	}
}

void audioCallback(void* userData /* unused*/, Uint8* stream, int len)
{
	SDL_memset(stream, 0, len); // silence

	if (bounceRemaining != 0)
	{
		Uint32 writeLen = len; //we make a copy so the file output will match the streem
		if (writeLen > bounceRemaining) writeLen = bounceRemaining;
		SDL_memcpy(stream, bounceAudioData + (bounceAudioLength - bounceRemaining), writeLen);
		bounceRemaining -= writeLen;
	}
	
	if (output != NULL)
	{
		output->write(output, stream, 1, len);
	}
}

void startRecording()
{
	stopRecording();
	output = SDL_RWFromFile("test.raw", "wb");
}

void stopRecording()
{
	if (output != NULL)
	{
		output->close(output);
		output = NULL;
	}
}
