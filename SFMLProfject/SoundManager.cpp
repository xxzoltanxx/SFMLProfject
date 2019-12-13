#include "SoundManager.h"
#include "Utilities.h"

void SoundManager::eraseState(State state)
{
	auto soundIter = mSounds[state].begin();
	while (soundIter != mSounds[state].end())
	{
		recycleSound(soundIter->second.second.name, soundIter->first, soundIter->second.first);
		++soundIter;
	}
	auto found = mMusic.find(state);
	if (found != mMusic.end())
	{
		delete found->second.first;
		mMusic.erase(found);
		--currentSoundsPlaying;
	}
}

void SoundManager::update(float dt)
{
	frameCounter += dt;
	if (frameCounter > 0.33f)
	{
		frameCounter = 0;
		auto soundIter = mSounds[mCurrentState].begin();
		while (soundIter != mSounds[mCurrentState].end())
		{
			if (!soundIter->second.first->getStatus())
			{
				recycleSound(soundIter->second.second.name, soundIter->first, soundIter->second.first);
				soundIter = mSounds[mCurrentState].erase(soundIter);
			}
			else
			{
				soundIter++;
			}
		}
		auto musicIter = mMusic.find(mCurrentState);
		if (musicIter != mMusic.end())
		{
			if (!musicIter->second.first->getStatus())
			{
				delete musicIter->second.first;
				mMusic.erase(musicIter);
				--currentSoundsPlaying;
			}
		}
	}
}

void SoundManager::stopMusic()
{
	auto found = mMusic.find(mCurrentState);
	if (found != mMusic.end())
	{
		delete found->second.first;
		mMusic.erase(found);
		--currentSoundsPlaying;
	}
}

void SoundManager::playMusic(const std::string& music, bool loop)
{
	stopMusic();
	sf::Music* newMusic = new sf::Music();
	newMusic->openFromFile(AudioManager::get()->getPath(music));
	newMusic->setLoop(loop);
	newMusic->setRelativeToListener(true);
	newMusic->play();
	mMusic[mCurrentState] = std::make_pair(newMusic, SoundData(music));
	++currentSoundsPlaying;
}

bool SoundManager::recycleSound(const std::string& soundName, const unsigned int& soundID, sf::Sound* sound)
{
	if (mRecycledSounds.size() > maxRecycled)
	{
		--currentSoundsPlaying;
		delete sound;
		return false;
	}
	else
	{
		mRecycledSounds.push_back(std::make_pair(std::make_pair(sound, soundName), soundID));
		return true;
	}
}

void SoundManager::switchState(State newState)
{
	for (auto iter = mSounds[mCurrentState].begin(); iter != mSounds[mCurrentState].end();)
	{
		pauseSound(iter->first);
	}
	auto music = mMusic.find(mCurrentState);
	if (music != mMusic.end())
	{
		music->second.first->pause();
	}
	mCurrentState = newState;
	for (auto iter = mSounds[mCurrentState].begin(); iter != mSounds[mCurrentState].end();)
	{
		playSound(iter->first);
	}
	auto music = mMusic.find(mCurrentState);
	if (music != mMusic.end())
	{
		music->second.first->play();
	}
}

bool SoundManager::setPosition(const unsigned int& soundID, const sf::Vector3f& newpos)
{
	if (mSounds[mCurrentState].find(soundID) != mSounds[mCurrentState].end())
	{
		mSounds[mCurrentState][soundID].first->setPosition(newpos);
		return true;
	}
	return false;
}

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

unsigned int SoundManager::playSound(const std::string& sound, const sf::Vector3f& position, bool loop, bool isRelative)
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
	return id;
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
		soundPtr->setBuffer(*AudioManager::get()->request(getProperty(sound).name));
		soundID = ++lastID;
		++currentSoundsPlaying;
		mSounds[mCurrentState][soundID] = std::make_pair(soundPtr, SoundData(sound));
		return soundID;
	}
	return 0;
}

void SoundManager::clearAll()
{
	for (auto soundBegin = mSounds.begin(); soundBegin != mSounds.end(); ++soundBegin)
	{
		auto soundIter = soundBegin->second.begin();
		auto soundIterEnd = soundBegin->second.end();
		while (soundIter != soundIterEnd)
		{
			delete soundIter->second.first;
			++soundIter;
		}
	}
	mSounds.clear();
	for (auto recycledBegin = mRecycledSounds.begin(); recycledBegin != mRecycledSounds.end(); ++recycledBegin)
	{
		delete recycledBegin->first.first;
	}
	mRecycledSounds.clear();
	for (auto musicBegin = mMusic.begin(); musicBegin != mMusic.end(); ++musicBegin)
	{
		delete musicBegin->second.first;
	}
	mMusic.clear();
	currentSoundsPlaying = 0;
}

SoundProperties& SoundManager::getProperty(const std::string& sound)
{
	auto found = mProperties.find(sound);
	if (found == mProperties.end())
	{
		SoundProperties properties;
		std::ifstream ifs(Utils::GetWorkingDirectory() + "Audio/" + sound + ".sound");
		ifs >> properties;
		mProperties[sound] = properties;
	}
	return mProperties[sound];
}