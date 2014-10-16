#ifndef Item_h__
#define Item_h__
#include "cocos2d.h"
#include "Definitions.h"
#include "GameObject.h"
USING_NS_CC;
using namespace std;

class Item:public GameObject
{
public:
	CC_SYNTHESIZE(int, _score, Score);
	void setInfo(ValueMap properties);
	Item();
	~Item();
	static Item* create();
};
#endif // Item_h__

