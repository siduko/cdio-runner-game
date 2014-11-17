#ifndef HubLayer_h__
#define HubLayer_h__
#include "cocos2d.h"
#include "Utils.h"
#include "Player.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace std;
using namespace ui;

class HubLayer:public Layer
{
private:
	Size sceneSize;
	Text* lbScore,*lbVelocity;
	ImageView* effectImage;
	Player* player;
public:
	LoadingBar* powerJump;
	virtual bool init();
	void setScore(string value);
	inline void setPlayer(Player* player){ this->player = player; }
	virtual void update(float delta);
	HubLayer();
	~HubLayer();
	CREATE_FUNC(HubLayer);
};
#endif // HubLayer_h__