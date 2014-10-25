#ifndef EntityManager_h__
#define EntityManager_h__
#include "cocos2d.h"
#include "ComponentObject.h"
USING_NS_CC;
using namespace std;

class EntityManager:public Node
{
private:
	map<string, ComponentObject*> _components;
public:
	EntityManager();
	~EntityManager();
	static EntityManager* create();
	void addComponentObject(string name,ComponentObject* component);
	void removeComponentObject(string name);
	ComponentObject* getComponentObjectByName(string name);
	bool checkComponentObjectExist(string name);
	void update(float dt);
};
#endif // EntityManager_h__