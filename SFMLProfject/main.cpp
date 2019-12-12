#include <unordered_map>
#include <vector>
#include <functional>

#include "Game.h"

int main()
{
	Game game;
	game.run();
}

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
	bool playSound(const std::string& sound, const sf::Vector3f& position, bool loop, bool isRelative);
	bool playSound(const unsigned int& soundID);
	bool stopSound(const unsigned int& soundID);
	bool pauseSound(const unsigned int& soundID);
	bool setPosition(const unsigned int& soundID, const sf::Vector3f& newpos);

	void switchState(State newState);
	void recycleSound(const std::string& soundName, const unsigned int& soundID, sf::Sound* sound);

	void playMusic(const std::string& music);
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
};

bool SoundManager::playSound(const unsigned int& soundID)
{
	if (mSounds[mCurrentState].find(soundID) != mSounds[mCurrentState].end())
	{
		mSounds[mCurrentState][soundID].first->play();
		mSounds[mCurrentState][soundID].second.isPaused = false;
		return true;
	}
	return false;
}

bool SoundManager::stopSound(const unsigned int& soundID)
{
	if (mSounds[mCurrentState].find(soundID) != mSounds[mCurrentState].end())
	{
		mSounds[mCurrentState][soundID].first->stop();
		mSounds[mCurrentState][soundID].second.isPaused = true;
		return true;
	}
	return false;
}

bool SoundManager::pauseSound(const unsigned int& soundID)
{
	if (mSounds[mCurrentState].find(soundID) != mSounds[mCurrentState].end())
	{
		mSounds[mCurrentState][soundID].first->pause();
		mSounds[mCurrentState][soundID].second.isPaused = true;
		return true;
	}
	return false;
}

bool SoundManager::playSound(const std::string& sound, const sf::Vector3f& position, bool loop, bool isRelative)
{
	unsigned int id = createSound(sound);
	if (id)
	{
		SoundProperties& properties = mProperties[sound];
		sf::Sound* soundB = mSounds[mCurrentState][id].first;
		soundB->setAttenuation(properties.attenuation);
		soundB->setPitch(properties.pitch);
		soundB->setMinDistance(properties.minDistance);
		soundB->setLoop(loop);
		soundB->setRelativeToListener(isRelative);
		soundB->play();
		return true;
	}
	return false;
}

unsigned int SoundManager::createSound(const std::string& sound)
{
	sf::Sound* soundPtr = nullptr;
	unsigned int soundID;
	for (auto iter = mRecycledSounds.begin(); iter != mRecycledSounds.end(); ++iter)
	{
		if (iter->first.second == sound)
		{
			soundPtr = iter->first.first;
			soundID = iter->second;
			mRecycledSounds.erase(iter);
			break;
		}
	}
	if (soundPtr != nullptr)
	{
		mSounds[mCurrentState][soundID] = std::make_pair(soundPtr, SoundData(sound));
		return soundID;
	}
	else if (currentSoundsPlaying <= maxSounds)
	{
		soundPtr = new sf::Sound();
		soundPtr->setBuffer(*AudioManager::get()->request(mProperties[sound].name));
		soundID = ++lastID;
		++currentSoundsPlaying;
		mSounds[mCurrentState][soundID] = std::make_pair(soundPtr, SoundData(sound));
		return soundID;
	}
	return 0;
}