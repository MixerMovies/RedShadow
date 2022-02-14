#include "SoundTest.h"

SoundTest::SoundTest() :
	device(nullptr),
	context(nullptr)
{
	device = alcOpenDevice(nullptr);
	if (!device)
	{
		printError("no sound device");
		return;
	}

	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	if (!context)
	{
		printError("no sound context");
		return;
	}
}

Sound* SoundTest::LoadSound(char* path)
{
	FILE *fp = NULL;
	fopen_s(&fp, path, "rb");
	char type[4];
	DWORD size, chunkSize;
	short formatType, channels;
	DWORD sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	DWORD datasize;

	fread(type, sizeof(char), 4, fp);
	string typecompare = string(type + '\0').substr(0,4);
	if (typecompare.compare("RIFF") != 0)
	{
		printError("No RIFF");
		return new Sound(0,0);
	}
	
	fread(&size, sizeof(DWORD), 1, fp);
	fread(type, sizeof(char), 4, fp);
	typecompare = string(type + '\0').substr(0, 4);
	if (typecompare.compare("WAVE") != 0)
	{
		printError("not WAVE");
		return new Sound(0, 0);
	}

	fread(type, sizeof(char), 4, fp);
	typecompare = string(type + '\0').substr(0, 4);
	if (typecompare.compare("fmt ") != 0)
	{
		printError("not fmt ");
		return new Sound(0, 0);
	}

	fread(&chunkSize, sizeof(DWORD), 1, fp);
	fread(&formatType, sizeof(short), 1, fp);
	fread(&channels, sizeof(short), 1, fp);
	fread(&sampleRate, sizeof(DWORD), 1, fp);
	fread(&avgBytesPerSec, sizeof(DWORD), 1, fp);
	fread(&bytesPerSample, sizeof(short), 1, fp);
	fread(&bitsPerSample, sizeof(short), 1, fp);

	fread(type, sizeof(char), 4, fp);
	typecompare = string(type + '\0').substr(0, 4);
	if (typecompare.compare("data") != 0)
	{
		printError("Missing DATA");
		return new Sound(0, 0);
	}

	fread(&datasize, sizeof(DWORD), 1, fp);

	unsigned char* buf = new unsigned char[datasize];
	fread(buf, sizeof(BYTE), datasize, fp);
	fclose(fp);

	ALuint source;
	ALuint buffer;
	ALuint frequency = sampleRate;
	ALenum format = 0;

	alGenBuffers(1, &buffer);
	alGenSources(1, &source);

	if (bitsPerSample == 8)
	{
		if (channels == 1)
			format = AL_FORMAT_MONO8;
		else if (channels == 2)
			format = AL_FORMAT_STEREO8;
	}
	else if (bitsPerSample == 16)
	{
		if (channels == 1)
			format = AL_FORMAT_MONO16;
		else if (channels == 2)
			format = AL_FORMAT_STEREO16;
	}

	std::cout << "Chunk Size: " << chunkSize << "\n";
	std::cout << "Format Type: " << formatType << "\n";
	std::cout << "Channels: " << channels << "\n";
	std::cout << "Sample Rate: " << sampleRate << "\n";
	std::cout << "Average Bytes Per Second: " << avgBytesPerSec << "\n";
	std::cout << "Bytes Per Sample: " << bytesPerSample << "\n";
	std::cout << "Bits Per Sample: " << bitsPerSample << "\n";
	std::cout << "Data Size: " << datasize << "\n";

	alBufferData(buffer, format, buf, datasize, frequency);
	delete[] buf;

	ALfloat SourcePos[] = { 0, 0, 0 };
	ALfloat SourceVel[] = { 0, 0, 0 };
	ALfloat ListenerPos[] = { 0, 0, 0 };
	ALfloat ListenerVel[] = { 0, 0, 0 };
	ALfloat ListenerOri[] = { 0, 0, -1, 0, 1, 0};

	alListenerfv(AL_POSITION, ListenerPos);
	alListenerfv(AL_VELOCITY, ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);

	alSourcei(source, AL_BUFFER, buffer);
	alSourcef(source, AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 1.0f);
	alSourcefv(source, AL_POSITION, SourcePos);
	alSourcefv(source, AL_VELOCITY, SourceVel);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	Sound *sound = new Sound(source, buffer);

	return sound;
}

void SoundTest::printError(const char* msg, int error)
{
	cout << msg << "\n";
}

SoundTest::~SoundTest()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}
