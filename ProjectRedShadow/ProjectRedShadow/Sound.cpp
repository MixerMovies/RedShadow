#include "Sound.h"
#include <iostream>

Sound::Sound(ALuint source, ALuint buffer)
{
	Sound::source = source;
	Sound::buffer = buffer;
}

void Sound::Play()
{
	alSourcePlay(source);
	std::cout<<alGetError()<<std::endl;
}

void Sound::Pause()
{
	alSourcePause(source);
}

void Sound::Stop()
{
	alSourceStop(source);
}

bool Sound::isPlaying()
{
	ALenum state;

	alGetSourcei(source, AL_SOURCE_STATE, &state);

	return (state == AL_PLAYING);
}

Sound::~Sound()
{
	alDeleteSources(1, &source);
}
