#include "Metronome.h"

Metronome& Metronome::getInstance()
{
	static Metronome instance;
	return instance;
}

Metronome::Metronome()
{
	mBpm = 60;
	mBeatName = BEAT_HI;
	mRunning = false;
}

void Metronome::Initialize()
{
	FMOD_RESULT result;
	
	result = FMOD::System_Create(&mSystem);
	ERRCHECK(result);

	void *extradriverdata = 0;

	Common_Init(&extradriverdata);
	result = mSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
	ERRCHECK(result);
}

int Metronome::GetBpm() const
{
	return mBpm;
}

void Metronome::SetBpm(int bpm)
{
	if (bpm < 60 || bpm > 220)
	{
		throw bpmOutOfRangeException;
	}

	mBpm = bpm;
}

void Metronome::SetBeat(std::string beatName)
{
	mBeatName = beatName;
}

void Metronome::Start()
{
	Start(mBpm, mBeatName);
}

void Metronome::Start(int bpm, string beatName)
{
	if (mRunning)
	{
		throw alreadyRunningException;
	}

	mRunning = true;
	
	FMOD_RESULT result = mSystem->createSound(Common_MediaPath(beatName.c_str()), FMOD_DEFAULT, 0, &mSound);
	ERRCHECK(result);

	mSound->setMode(FMOD_LOOP_NORMAL);
	mSound->setLoopPoints(0, FMOD_TIMEUNIT_MS, 60000 / bpm, FMOD_TIMEUNIT_MS);

	mSystem->playSound(mSound, 0, false, &mChannel);
}

void Metronome::Stop()
{
	mChannel->stop();
	mSound->release();
	mRunning = false;
}