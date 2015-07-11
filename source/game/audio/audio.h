#ifndef GAME_AUDIO_H
#define GAME_AUDIO_H

#include "SDL/SDL.h"

/*
Audio in SDL is extremely low level, this is great in that it'll be a learning exercise to implement
as much myself as possible, but bad in that audio is often this obscure black art in game dev.

SDL has a callback which is run in a separate thread and which we have to implement to fill the audio buffer
Since there are weird challenges that have to be met when working with threads and since locks and semaphores are so
expensive, the implementation will need to be completely thread safe without them. The best thing I can think at the moment is
that the audio thread be responsible for reading and advancing the playback. The main thread will queue up what to play, but
nothing gets removed from that queue without the audio thread doing so.
*/

#define FREQUENCY 44100
#define SAMPLE_RATE 2048
#define SOUND_TIME 0.20

static Sint8* bounceAudioData = NULL; // This will hold the mythical bleep and bloop of pong
static Uint32 bounceAudioLength; // The length of the buffer, allows us to free it when done

static Uint32 bounceRemaining = 0; // Remaining amount of sound to play
static SDL_RWops* output = NULL;

void InitAudio();
void DestroyAudio();

void generateSound(double frequency);
void generateNoise();
void generateSquare(double frequency);

void audioCallback(void* userData /* unused*/, Uint8* stream, int len);

void startRecording();
void stopRecording();

#endif