#ifndef Enemy_h__
#define Enemy_h__
#include "cocos2d.h"
#include "GameObject.h"
#include "DataController.h"
#include "Utils.h"
USING_NS_CC;



class Enemy :public GameObject
{
public:
	static enum EnemyType
	{
		Move, Fly, Stand
	};

	static enum EnemyState
	{
		Idle, Moving, BeHit, Dead, TargetDetected
	};

	CC_SYNTHESIZE(EnemyType, _enemyType, EnemyType);
	CC_SYNTHESIZE(EnemyState, _enemyState, EnemyState);
	CC_SYNTHESIZE(GameObject*, _target, Target);
	CC_SYNTHESIZE(float, _actionTimeout, ActionTimeout);
	CC_SYNTHESIZE(float, _moveTimeout, MoveTimeout);
	CC_SYNTHESIZE(float, _moveVelocity, MoveVelocity);
	CC_SYNTHESIZE(bool, _resetActionTimeout, ResetActionTimeout);

	Enemy(ValueMap properties);
	~Enemy();
	static Enemy* create(ValueMap properties);
	void update(float dt);
};
#endif // Enemy_h__