#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Sound.h"
#include <cstdio>
#include <iostream>
#include <Windows.h>
#include <al.h>
#include <alc.h>

using namespace std;

class SoundTest
{
public:
	SoundTest();
	Sound* LoadSound(char*);
	void printError(char*, int = 0);
	~SoundTest();
private:
	ALCdevice *device;
	ALCcontext *context;
};