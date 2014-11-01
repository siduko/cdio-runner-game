#ifndef ChapterLayer_h__
#define ChapterLayer_h__
#include "cocos2d.h"
#include "SlidingMenuGrid.h"
USING_NS_CC;

class ChapterLayer:public Layer
{
public:
	ChapterLayer();
	~ChapterLayer();
	virtual bool init();
	CREATE_FUNC(ChapterLayer);
	static Scene* createScene();
	void btnLevel_Clicked(Object* pSender);
};
#endif // ChapterLayer_h__