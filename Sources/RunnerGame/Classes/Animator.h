#ifndef Animator_h__
#define Animator_h__
#include "cocos2d.h"
#include "ComponentObject.h"
USING_NS_CC;
using namespace std;


class Animator:public ComponentObject
{
private:
	map<string,Animate*> _listAnimation;
	Animate* _currentAnimate;
public:
	Animator();
	~Animator();
	CC_SYNTHESIZE_READONLY(Speed*, _playingAction, PlayingAction);
	CC_SYNTHESIZE_READONLY(string, _playingActionName, PlayingActionName);
	static Animator* create();
	inline void addAction(string name, Animate* action){ _listAnimation[name] = action; }
	void addAction(string name, int imageCount, string imagePath, float animationDelay = 0.2f);
	Action* getActionByName(string name);
	void playActionByName(string name, float duration = 0.1f, bool isRepeat = false, bool returnIdle = false);
	void stopCurrentAction();
	void stopActionByName(string name);
};
#endif // Animator_h__