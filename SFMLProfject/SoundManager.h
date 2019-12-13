#pragma once
#include <SFML/Audio.hpp>
#include "State.h"
#include "AudioManager.h"

struct SoundProperties
{
	std::string name;
	float attenuation;
	float pitch;
	float minDistance;
};

struct SoundData
{
	SoundData(std::string nam) : name(nam), isPaused(false) {}
	std::string name;
	bool isPaused;
};

using PropertyHolder = std::unordered_map<std::string, SoundProperties>;
using StateSounds = std::unordered_map<unsigned int, std::pair<sf::Sound*, SoundData>>;
using Sounds = std::unordered_map<State, StateSounds>;
using Music = std::unordered_map<State, std::pair<sf::Music*, SoundData>>;
using RecycledSounds = std::vector<std::pair<std::pair<sf::Sound*, std::string>, unsigned int>>;

class SoundManager
{
public:
	unsigned int playSound(const std::string& sound, const sf::Vector3f& position, bool loop, bool isRelative);
	bool playSound(const unsigned int& soundID);
	bool stopSound(const unsigned int& soundID);
	bool pauseSound(const unsigned int& soundID);
	bool setPosition(const unsigned int& soundID, const sf::Vector3f& newpos);

	void switchState(State newState);
	void eraseState(State state);
	bool recycleSound(const std::string& soundName, const unsigned int& soundID, sf::Sound* sound);

	void playMusic(const std::string& music, bool loop);
	void stopMusic();
	void update(float dt);

	static const int maxSounds = 150;
	static const int maxRecycled = 75;
private:
	unsigned int createSound(const std::string& sound);
	Sounds mSounds;
	Music mMusic;
	RecycledSounds mRecycledSounds;
	PropertyHolder mProperties;

	State mCurrentState;
	unsigned int lastID;
	unsigned int currentSoundsPlaying;

	float frameCounter = 0;
};