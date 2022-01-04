#pragma once

#include "framework.h"

#define _CRT_SECURE_NO_WARNINGS
#include "Sound.h"
#include <cstdio>
#include <iostream>
#include <Windows.h>
#include <al.h>
#include <alc.h>

using namespace std;

class RED_EXPORT SoundTest
{
public:
	SoundTest();
	Sound* LoadSound(char*);
	void printError(const char*, int = 0);
	~SoundTest();
private:
	ALCdevice *device;
	ALCcontext *context;
};