#ifndef Player_h__
#define Player_h__
#include "cocos2d.h"
#include "Definitions.h"
#include "GameObject.h"
#include "Animator.h"
#include "EffectComponent.h"
USING_NS_CC;
using namespace std;
enum PlayerState
{
	Idle,Running,Jumping,Hurt
};
class Player :public GameObject
{
private:
	PlayerState _playerState;
public:
	Player(ValueMap properties);
	~Player();
	CC_SYNTHESIZE(bool, _allowJump, AllowJump);
	CC_SYNTHESIZE(float, _limitVelocity,LimitVelecity)
	CC_SYNTHESIZE(int, _score, Score);
	CC_SYNTHESIZE(float, _acceleration, Acceleration);
	CC_SYNTHESIZE(int, _health, Health);
	CC_SYNTHESIZE(int, _maxHealth, MaxHealth);
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

