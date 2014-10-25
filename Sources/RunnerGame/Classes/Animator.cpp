#include "Animator.h"


Animator::Animator()
{
	_playingAction = NULL;
	_currentAnimate = NULL;
	_playingActionName = "";
}

Animator::~Animator()
{
	for (pair<string, Action*> a : _listAnimation)
		a.second->release();
	_listAnimation.clear();
}

Animator* Animator::create()
{
	Animator *node = new Animator();
	if (node)
	{
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return nullptr;
}

void Animator::playActionByName(string name, float duration /*= 0.1f*/, bool isRepeat /*= false*/, bool returnIdle /*= false*/)
{
	Node* _parent = this->getParent();
	if (_parent){
		Animate* act = _listAnimation[name];
		if (act && _currentAnimate != act)
		{
			act->setDuration(duration);
			if (_playingAction)
				_parent->stopAction(_playingAction);
			if (isRepeat)
			{
				Action* temp = RepeatForever::create(act);
				_parent->runAction(temp);
				_playingAction = temp;
				_playingActionName = name;
			}
			else
			{
				if (returnIdle)
				{
					Action* temp = Sequence::createWithTwoActions(act, CallFunc::create([this](){this->playActionByName("idle"); }));
					_parent->runAction(temp);
					_playingAction = temp;
					_playingActionName = name;
				}
				else
				{
					_parent->runAction(act);
					_playingAction = act;
					_playingActionName = name;
				}
			}
			_currentAnimate = act;
		}
	}

}

void Animator::addAction(string name, int imageCount, string imagePath, float animationDelay /*= 0.2f*/)
{
	Sprite* test;
	Vector<SpriteFrame*> animFrames(imageCount);
	char str[100] = { 0 };
	for (int i = 1; i <= imageCount; i++)
	{
		sprintf(str, imagePath.c_str(), i);
		test = Sprite::create(str);
		auto frame = test->getSpriteFrame();
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, animationDelay);
	Animate* temp = Animate::create(animation);
	temp->retain();
	_listAnimation[name] = temp;
}

void Animator::stopCurrentAction()
{
	Node* _parent = this->getParent();
	if (_parent){
		_parent->stopAction(_playingAction);
	}
}

void Animator::stopActionByName(string name)
{
	Node* _parent = this->getParent();
	if (_parent){
		Action* a = getActionByName(name);
		if (a != NULL)
			_parent->stopAction(a);
	}
}

Action* Animator::getActionByName(string name)
{
	return _listAnimation[name];
}
