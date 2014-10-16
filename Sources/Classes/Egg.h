#ifndef Egg_h__
#define Egg_h__
#include "cocos2d.h"
#include "Definitions.h"
USING_NS_CC;

class Egg:public Sprite
{
public:
	Egg();
	~Egg();
	static Egg* create();
};
#endif // Egg_h__