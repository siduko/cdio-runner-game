#ifndef ComponentObject_h__
#define ComponentObject_h__
#include "cocos2d.h"
USING_NS_CC;

class ComponentObject:public Node
{
public:
	CC_SYNTHESIZE(bool, _enabled, Enabled);
	ComponentObject();
	~ComponentObject();
	static ComponentObject* create();
	void update(float dt);
};
#endif // ComponentObject_h__