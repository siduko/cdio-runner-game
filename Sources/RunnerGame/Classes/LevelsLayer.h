#ifndef LevelsLayer_h__
#define LevelsLayer_h__
#include "cocos2d.h"
#include "SlidingMenuGrid.h"
#include "PlayLayer.h"
#include "Utils.h"
USING_NS_CC;
using namespace std;


class LevelsLayer:public Layer
{
private:
	SlidingMenuGrid* sliding;
public:
	LevelsLayer();
	~LevelsLayer();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LevelsLayer);
	void btnLevel_Clicked(Object* pSender);
};
#endif // LevelsLayer_h__

