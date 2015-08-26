#include <string>
#include <exception>
#include "fmod.hpp"
#include "common.h"

using namespace std;

class Metronome
{
private:
	const string BEAT_HI	= "hi.mp3";
	const string BEAT_SNARE = "snare.mp3";
	const string BEAT_WOOD	= "wood.mp3";

	int				mBpm;
	bool			mRunning;
	string			mBeatName;

	FMOD::System    *mSystem;
	FMOD::Sound     *mSound;
	FMOD::Channel   *mChannel;

	Metronome();

	Metronome(Metronome const&) = delete;
	void operator=(Metronome const&) = delete;

public:
	static Metronome& getInstance();

	void Initialize();
	int GetBpm() const;
	void SetBpm(int bpm);
	void SetBeat(string beatName);
	void Start();
	void Start(int bpm, string beatName);
	void Stop();
};

class BpmOutOfRangeException : public exception
{
	virtual const char* what() const throw()
	{
		return "BPM is out of range. BPM must be between 60~220.";
	}
}bpmOutOfRangeException;

class AlreadyRunningException : public exception
{
	virtual const char* what() const throw()
	{
		return "Metronome is already running.";
	}
}alreadyRunningException;
