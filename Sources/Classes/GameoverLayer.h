#ifndef GameoverLayer_h__
#define GameoverLayer_h__
#include "cocos2d.h"
#include "PlayLayer.h"
USING_NS_CC;

class GameoverLayer:public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	GameoverLayer();
	~GameoverLayer();
	void btnBack_Click(Ref* psender);
	CREATE_FUNC(GameoverLayer);
};
#endif // GameoverLayer_h__