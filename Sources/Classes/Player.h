#ifndef Player_h__
#define Player_h__
#include "cocos2d.h"
#include "Definitions.h"
#include "GameObject.h"
USING_NS_CC;
using namespace std;
enum PlayerState
{
	Idle,Running,Jumping,Hurt
};
class Player:public GameObject
{
public:
	Player();
	~Player();
	CC_SYNTHESIZE(bool, _allowJump, AllowJump);
	void setSpeed(float value);
	void setInfo(ValueMap properties);
	static Player* create();
	void move(Vec2 vec);
	void jump(Vec2 vec);
};
#endif // Player_h__

