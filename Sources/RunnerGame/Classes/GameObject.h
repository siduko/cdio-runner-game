#ifndef GameObject_h__
#define GameObject_h__
#include "cocos2d.h"
#include "EntityManager.h"
#include "Animator.h"
USING_NS_CC;


class GameObject:public Sprite
{
protected:
	EntityManager* _entityManager;
public:
	GameObject();
	GameObject(ValueMap properties);
	~GameObject();
	inline EntityManager* getEntityManager(){ return _entityManager; }
	static GameObject* create();
	static GameObject* create(ValueMap properties);
	void update(float dt);
};
#endif // GameObject_h__

