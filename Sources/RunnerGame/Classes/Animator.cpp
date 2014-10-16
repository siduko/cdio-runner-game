#include "Animator.h"


Animator::Animator()
{
}

Animator::~Animator()
{
	for (pair<string, Action*> a : _listAnimation)
		a.second->release();
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

void Animator::playActionByName(string name)
{
	Node* _parent = this->getParent();
	if (_parent){
		for (pair<string, Action*> a : _listAnimation)
		{
			if (a.first == name)
			{
				if (_playingAction != a){
					_parent->stopAction(_playingAction.second);
					_parent->runAction(a.second);
					_playingAction = a;
				}
				return;
			}
		}
	}
}

void Animator::playActionByIndex(int index)
{
	Node* _parent = this->getParent();
	if (_parent  && index < _listAnimation.size()){
		if (_playingAction != _listAnimation[index]){
			_parent->stopAction(_playingAction.second);
			_parent->runAction(_listAnimation[index].second);
			_playingAction = _listAnimation[index];
		}
	}
}

void Animator::addAction(string name, int imageCount, string imagePath, float animationDelay /*= 0.2f*/, bool isRepeat /*= true*/, bool isFlipX /*= false*/, bool isFlipY /*= false*/)
{
	Sprite* test;
	Vector<SpriteFrame*> animFrames(imageCount);
	char str[100] = { 0 };
	for (int i = 1; i <= imageCount; i++)
	{
		sprintf(str, imagePath.c_str(), i);
		test = Sprite::create(str);
		test->setFlipX(isFlipX);
		test->setFlipY(isFlipY);
		auto frame = test->getSpriteFrame();
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, animationDelay);
	Action* temp = Animate::create(animation);
	if (isRepeat)
		temp = RepeatForever::create(Animate::create(animation));
	else
		temp = Sequence::createWithTwoActions(Animate::create(animation), CallFunc::create([this](){this->playActionByName("idle"); }));
	temp->retain();
	_listAnimation.push_back(make_pair(name, temp));
}

void Animator::stopCurrentAction()
{
	Node* _parent = this->getParent();
	if (_parent  ){
		_parent->stopAction(_playingAction.second);
	}
}

void Animator::stopActionByName(string name)
{
	Node* _parent = this->getParent();
	if (_parent){
		Action* a = getActionByName(name);
		if (a!=NULL)
		_parent->stopAction(a);
	}
}

void Animator::stopActionByIndex(int index)
{
	Node* _parent = this->getParent();
	if (_parent){
		Action* a = getActionByIndex(index);
		if (a != NULL)
		_parent->stopAction(a);
	}
}

Action* Animator::getActionByName(string name)
{
	for (AnimationPair a:_listAnimation)
	if (a.first == name)
		return a.second;
	return NULL;
}

Action* Animator::getActionByIndex(int index)
{
	if (index < _listAnimation.size())
		return _listAnimation[index].second;
	return NULL;
}
