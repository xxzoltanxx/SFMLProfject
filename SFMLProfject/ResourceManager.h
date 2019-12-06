#pragma once
#include <unordered_map>
#include <fstream>

template <typename Derived, typename T> class ResourceManager
{
public:
	ResourceManager()
	{
	}
	T* getResource(std::string resource)
	{
		return resources[resource].first;
	}

	T* request(std::string resource)
	{
		if (resources.find(resource) == resources.end())
		{
			T* res = load(resource);
			resources.emplace(resource, std::make_pair(res, 0));
		}
		++resources[resource].second;
		return resources[resource].first;

	}
	void release(std::string resource)
	{
		--resources[resource].second;
		if (resources[resource].second == 0)
		{
			delete resources[resource].first;
			resources.erase(resources.find(resource));
		}
	}
	void release(T* item)
	{
		auto iter = std::find_if(resources.begin(), resources.end(), [&item](std::pair<std::string, std::pair<T*, int>> val) -> bool { return val.second.first == item; });
		--iter->second.second;
		if (iter->second.second == 0)
		{
			delete iter->second.first;
			resources.erase(iter->first);
		}
	}

protected:
	T* load(std::string resource)
	{
		return static_cast<Derived*>(this)->load(resource);
	}
	std::unordered_map<std::string, std::string> paths;
	std::unordered_map<std::string, std::pair<T*,int>> resources;
};