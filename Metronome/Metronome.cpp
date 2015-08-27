#include "Metronome.h"

const string Beat::BEAT_HI		= "hi.mp3";
const string Beat::BEAT_SNARE	= "snare.mp3";
const string Beat::BEAT_WOOD	= "wood.mp3";

Metronome& Metronome::getInstance()
{
	static Metronome instance;
	return instance;
}

Metronome::Metronome()
{
	mBpm = 60;
	mBeatName = Beat::BEAT_HI;
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
	if (validateBpm(bpm))
	{
		mBpm = bpm;
	}
}

string Metronome::GetBeat() const
{
	return mBeatName;
}

void Metronome::SetBeat(std::string beatName)
{
	mBeatName = beatName;
}

void Metronome::Start()
{
	Start(mBpm, mBeatName);
}

void Metronome::Start(int bpm)
{
	Start(bpm, mBeatName);
}

void Metronome::Start(int bpm, string beatName)
{
	if (!validateBpm(bpm))
	{
		throw BpmOutOfRangeException();
	}

	mtx.lock();
	if (mRunning)
	{
		mtx.unlock();
		throw AlreadyRunningException();
	}

	FMOD_RESULT result = mSystem->createSound(Common_MediaPath(beatName.c_str()), FMOD_DEFAULT, 0, &mSound);
	ERRCHECK(result);

	mSound->setMode(FMOD_LOOP_NORMAL);
	mSound->setLoopPoints(0, FMOD_TIMEUNIT_MS, 60000 / bpm, FMOD_TIMEUNIT_MS);

	mSystem->playSound(mSound, 0, false, &mChannel);

	mRunning = true;
	mtx.unlock();
}

void Metronome::Stop()
{
	mtx.lock();
	if (!mRunning)
	{
		mtx.unlock();
		throw NotRunningException();
	}
	
	mRunning = false;
	mChannel->stop();
	mSound->release();
	mtx.unlock();
}

bool Metronome::validateBpm(int bpm)
{
	if (bpm < 60 || bpm > 220)
	{
		throw BpmOutOfRangeException();
	}
	return true;
}