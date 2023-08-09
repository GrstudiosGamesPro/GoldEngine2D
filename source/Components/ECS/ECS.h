#ifndef ECSCOMPONENT_H
#define ECSCOMPONENT_H
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>


class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static_assert (std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual ~Component() {}
};

class Entity
{
private:
	//Manager& manager;
	bool active = true;
	std::vector<Component*> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
	GroupBitset groupBitset;

public:
	std::string ObjectName = "New Entity";
	std::string ObjectTag = "None";
	int objectID = 1;

	//Entity(Manager& mManager) : manager(mManager) {}

	void update()
	{
		for (auto& c : components) c->update();
	}
	void draw()
	{
		for (auto& c : components) c->draw();
	}

	bool isActive() const { return active; }
	void destroy() { active = false; }

	bool hasGroup(Group mGroup)
	{
		return groupBitset[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		groupBitset[mGroup] = false;
	}


	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c = new T(std::forward<TArgs>(mArgs)...);
		c->entity = this;
		components.emplace_back(c);

		componentArray[getComponentTypeID<T>()] = c;
		componentBitset[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	template <typename T> bool hasComponent() const
	{
		return componentBitset[getComponentTypeID<T>()];
	}

	template <typename T>
	bool removeComponent()
	{
		if (hasComponent<T>())
		{
			auto& component = getComponent<T>();
			componentBitset[getComponentTypeID<T>()] = false;
			componentArray[getComponentTypeID<T>()] = nullptr;
			return true;
		}
		return false;
	}
};
#endif