#ifndef HubLayer_h__
#define HubLayer_h__
#include "cocos2d.h"
#include "Utils.h"
#include "Player.h"
#include "LevelsLayer.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace std;
using namespace ui;
using namespace CocosDenshion;

class HubLayer:public Layer
{
private:
	Text* lbScore, *lbVelocity, *lbEffectTimer,*lbHealth;
	ImageView* effectImage;
	Player* player;
public:
	LoadingBar* powerJump;
	Sprite* angleJump;
	virtual bool init();
	void setScore(string value);
	inline void setPlayer(Player* player){ this->player = player; }
	inline Player* getPlayer(){
		return this->player;
	}
	virtual void update(float delta);
	HubLayer();
	~HubLayer();
	CREATE_FUNC(HubLayer);
};

#endif // HubLayer_h__