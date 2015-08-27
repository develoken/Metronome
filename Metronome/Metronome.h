#include <string>
#include <mutex>
#include <exception>
#include "fmod.hpp"
#include "common.h"

using namespace std;

class Beat
{
public:
	const static string BEAT_HI;
	const static string BEAT_SNARE;
	const static string BEAT_WOOD;
};

class Metronome
{
private:
	mutex			mtx;

	int				mBpm;
	bool			mRunning;
	string			mBeatName;

	FMOD::System    *mSystem;
	FMOD::Sound     *mSound;
	FMOD::Channel   *mChannel;

	Metronome();
	bool validateBpm(int bpm);

	Metronome(Metronome const&) = delete;
	void operator=(Metronome const&) = delete;

public:
	static Metronome& getInstance();

	void Initialize();
	int GetBpm() const;
	void SetBpm(int bpm);
	string GetBeat() const;
	void SetBeat(string beatName);
	void Start();
	void Start(int bpm);
	void Start(int bpm, string beatName);
	void Stop();
};

class BpmOutOfRangeException : public exception
{
	virtual const char* what() const throw()
	{
		return "BPM is out of range. BPM must be between 60~220";
	}
};

class BpmInvalidException : public exception
{
	virtual const char* what() const throw()
	{
		return "BPM value is invalid";
	}
};

class AlreadyRunningException : public exception
{
	virtual const char* what() const throw()
	{
		return "Metronome is already running";
	}
};

class NotRunningException : public exception
{
	virtual const char* what() const throw()
	{
		return "Metronome is not running";
	}
};