#include "EntityManager.h"


EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
	_components.clear();
}

bool EntityManager::checkComponentObjectExist(string name)
{
	for (auto c : _components)
	{
		if (c.first == name)
			return true;
	}
	return false;
}

void EntityManager::addComponentObject(string name, ComponentObject* component)
{
	if (!checkComponentObjectExist(name))
	{
		//CCLOG("Added component: %s", name.c_str());
		_components[name]= component;
		if (this->getParent())
			this->getParent()->addChild(component);
	}
}

void EntityManager::removeComponentObject(string name)
{
	if (this->getParent() && _components[name])
	{
		this->getParent()->removeChild(_components[name]);
		_components.erase(name);
	}
}

void EntityManager::update(float dt)
{
	for (auto c : _components)
	{
		if (c.second->getEnabled())
			c.second->update(dt);
	}
}

ComponentObject* EntityManager::getComponentObjectByName(string name)
{
	return _components[name];
}

EntityManager* EntityManager::create()
{
	EntityManager *node = new EntityManager();
	if (node)
	{
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return nullptr;
}
