#pragma once

#include <al.h>
#include <alc.h>

class RED_EXPORT Sound
{
public:
	Sound(ALuint source, ALuint buffer);
	void Play();
	void Pause();
	void Stop();
	bool isPlaying();
	~Sound();
private:
	ALuint source;
	ALuint buffer;
};