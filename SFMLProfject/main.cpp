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

struct SoundProperties
{
	std::string name;
	float attenuation;
	float pitch;
	float minDistance;
};

struct SoundData
{
	std::string name;
	bool isPaused;
};

using PropertyHolder = std::unordered_map<std::string, SoundProperties>;
using SoundsState = std::unordered_map<unsigned int, std::pair<sf::Sound*, SoundData>>;
using Sounds = std::unordered_map<State, SoundsState>;
using Music = std::unordered_map<State, std::pair<sf::Music*, SoundData>>;
using RecycledSounds = std::vector<std::pair<std::pair<sf::Sound*, std::string>, unsigned int>>;

