#ifndef HubLayer_h__
#define HubLayer_h__
#include "cocos2d.h"
#include "Utils.h"
#include "Player.h"
#include "JoyStick.h"
USING_NS_CC;
using namespace std;
class HubLayer:public Layer
{
private:
	Size sceneSize;
	LabelTTF* lbScore,*lbVelocity;
	Player* player;
public:
	virtual bool init();
	void setScore(string value);
	inline void setPlayer(Player* player){ this->player = player; }
	virtual void update(float delta);
	HubLayer();
	~HubLayer();
	CREATE_FUNC(HubLayer);
};
#endif // HubLayer_h__