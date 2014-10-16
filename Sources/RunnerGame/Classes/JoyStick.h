#ifndef JoyStick_h__
#define JoyStick_h__
#include "cocos2d.h"
USING_NS_CC;

enum JoyStickEnum
{
	DEFAULT,
	D_UP,
	D_DOWN,
	D_LEFT,
	D_RIGHT,
	D_LEFT_UP,
	D_LEFT_DOWN,
	D_RIGHT_UP,
	D_RIGHT_DOWN
};

class JoyStick : public Layer
{
public:
	void onRun();
	void onDisable();
	void setDieRadius(float radius);
	void setFailRadius(float radius);
	void setVisibleJoystick(bool visible);
	void setAutoPosition(bool value);
	float getVelocity();
	std::function<void(JoyStickEnum,float)> onDirection;
	static JoyStick* create(std::string chassisPath, std::string dotPath);
	void initWithJoystick(std::string chassisPath, std::string dotPath);
protected:
	float _radius;
	float _failradius;
	bool isMoveOut;
	bool isDieRadius;
	bool isAutoPosition;
	JoyStickEnum _direction;
	Sprite* _chassis;
	Sprite* _touchDot;
	EventListenerTouchOneByOne* listener;

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	virtual void onExit();
	
	void setTouchDotPosition(Vec2 vec1, Vec2 vec2);

	void changeAngle(Vec2 position);
	void callDirectionFun();
	void resetState();
};
#endif // JoyStick_h__

