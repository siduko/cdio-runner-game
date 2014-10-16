#ifndef GameObject_h__
#define GameObject_h__
#include "cocos2d.h"
#include "Animator.h"
USING_NS_CC;


class GameObject:public Sprite
{
protected:
	Animator* _animator;
public:
	PhysicsBody* spriteBody;
	GameObject();
	~GameObject();
	inline Animator* getAnimator(){ return _animator; }
	void setInfo(ValueMap properties);
	static GameObject* create();
};
#endif // GameObject_h__

