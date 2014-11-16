#ifndef Player_h__
#define Player_h__
#include "cocos2d.h"
#include "DataController.h"
#include "GameObject.h"
#include "Animator.h"
#include "EffectComponent.h"
USING_NS_CC;
using namespace std;
enum PlayerState
{
	Idle, Running, Jumping, Falling, Hurt, Swim, Duck, Climb
};
class Player :public GameObject
{
public:
	Player(ValueMap properties);
	~Player();
	CC_SYNTHESIZE(PlayerState, _playerState, PlayerState);
	CC_SYNTHESIZE(float, _actionTimeOut, ActionTimeOut);
	CC_SYNTHESIZE(bool, _allowJump, AllowJump);
	CC_SYNTHESIZE(float, _limitVelocity,LimitVelecity)
	CC_SYNTHESIZE(int, _score, Score);
	CC_SYNTHESIZE(float, _acceleration, Acceleration);
	CC_SYNTHESIZE(int, _health, Health);
	CC_SYNTHESIZE(int, _maxHealth, MaxHealth);
	CC_SYNTHESIZE(bool, _autoControlVelocity, AutoControlVelocity);
	static Player* create(ValueMap properties);
	inline float getVelocity(){ 
		return this->getPhysicsBody()->getVelocity().x;
	};
	inline void setVelocity(float value){
		this->getPhysicsBody()->setVelocity(ccp(value, this->getPhysicsBody()->getVelocity().y));
	};
	void jump();
	void update(float dt);
};
#endif // Player_h__

