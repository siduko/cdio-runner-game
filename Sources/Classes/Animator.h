#ifndef Animator_h__
#define Animator_h__
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

typedef pair<string, Action*> AnimationPair;

class Animator:public Node
{
private:
	vector<AnimationPair> _listAnimation;
	AnimationPair _playingAction;
public:
	Animator();
	~Animator();
	static Animator* create();
	inline void addAction(string name, Action* action){ _listAnimation.push_back(make_pair(name, action)); }
	void addAction(string name, int imageCount, string imagePath, float animationDelay = 0.2f, bool isRepeat = true,bool isFlipX = false,bool isFlipY = false);
	inline string getPlayingActionName(){ return _playingAction.first; }
	inline Action* getPlayingSelectedAction(){ return _playingAction.second; }
	Action* getActionByName(string name);
	Action* getActionByIndex(int index);
	void playActionByName(string name);
	void playActionByIndex(int index);
	void stopCurrentAction();
	void stopActionByName(string name);
	void stopActionByIndex(int index);
};
#endif // Animator_h__