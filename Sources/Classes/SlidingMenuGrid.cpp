#include "SlidingMenuGrid.h"


SlidingMenuGrid::SlidingMenuGrid()
{
}


SlidingMenuGrid::~SlidingMenuGrid()
{
}

bool SlidingMenuGrid::onTouchBegan(Touch *touch, Event *unused_event)
{
	touchOrigin = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	if (state == Waiting)
	{
		// Activate the menu item if we are touching one.
		if (!bMoving)
		{
			selectedItem = GetItemWithinTouch(touch);
			if (selectedItem)
			{
				selectedItem->selected();
			}
		}
		state = TrackingTouch;
	}
	return true;
}

void SlidingMenuGrid::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (GetItemWithinTouch(touch) == NULL && selectedItem)
	{
		//Touch Cancelled
		selectedItem->unselected();
		selectedItem = NULL;
		state = Waiting;
		return;
	}

	if (GetItemWithinTouch(touch) != NULL && selectedItem)
	{
		return;
	}
	// Calculate the current touch point during the move.
	touchStop = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());// Distance between the origin of the touch and current touch point.    
	fMoveDelta = (bVerticalPaging) ? (touchStop.y - touchOrigin.y) : (touchStop.x - touchOrigin.x);// Set our position.    
	setPosition(GetPositionOfCurrentPageWithOffset(fMoveDelta));
	bMoving = true;
}

void SlidingMenuGrid::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (bMoving)
	{
		bMoving = false;
		{
			// Do we have multiple pages?
			if (iPageCount > 1 && (fMoveDeadZone < abs(fMoveDelta)))
			{
				// Are we going forward or backward?
				bool bForward = (fMoveDelta < 0) ? true : false;

				// Do we have a page available?
				if (bForward && (iPageCount>iCurrentPage + 1))
				{
					// Increment currently active page.
					iCurrentPage++;
				}
				else if (!bForward && (iCurrentPage > 0))
				{
					// Decrement currently active page.
					iCurrentPage--;
				}
				// Start sliding towards the current page.
				moveToCurrentPage();
			}
			else
			{
				if (selectedItem)
				{
					selectedItem->unselected();
					selectedItem->activate();
				}
			}
		}
	}
	else
	{
		//Only touch
		if (selectedItem)
		{
			selectedItem->unselected();
			selectedItem->activate();
			//selectedItem=NULL;			
		}
	}
	state = Waiting;
}

void SlidingMenuGrid::onTouchCancelled(Touch *touch, Event *unused_event)
{
	if (selectedItem)
	{
		selectedItem->unselected();
		selectedItem = NULL;
		state = Waiting;
	}
}

void SlidingMenuGrid::buildGrid(int cols, int rows)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int col = 0, row = 0;
	auto child = pMenu->getChildren();
	CCObject* item;
	iPageCount = 0;
	for(auto item : child)
	{
		MenuItemSprite* getItem = (MenuItemSprite*)item;
		getItem->setPosition(ccp(getPosition().x + col * padding.x + (iPageCount * winSize.width), getPosition().y - row * padding.y));
		++col;
		if (col == cols)
		{
			col = 0;
			++row;
			if (row == rows)
			{
				iPageCount++;
				col = 0;
				row = 0;
			}
		}
	}
}

void SlidingMenuGrid::buildGridVertical(int cols, int rows)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int col = 0, row = 0;
	auto child = pMenu->getChildren();
	CCObject* item;
	iPageCount = 0;
	for(auto item:child)
	{
		CCMenuItemSprite* getItem = (CCMenuItemSprite*)item;
		getItem->setPosition(ccp(getPosition().x + col * padding.x, getPosition().y - row * padding.y + (iPageCount * winSize.height)));
		++col;
		if (col == cols)
		{
			col = 0;
			++row;
			if (row == rows)
			{
				iPageCount++;
				col = 0;
				row = 0;
			}
		}
	}
}

void SlidingMenuGrid::moveToCurrentPage()
{
	Size winSize = CCDirector::sharedDirector()->getWinSize();
	// Perform the action
	EaseBounceOut* action = EaseBounceOut::create(MoveTo::create(fAnimSpeed, GetPositionOfCurrentPage()));
	runAction(action);
}

Point SlidingMenuGrid::GetPositionOfCurrentPage()
{
	Size winSize = Director::sharedDirector()->getWinSize();
	return (bVerticalPaging) ? ccp(menuOrigin.x, menuOrigin.y - (iCurrentPage*winSize.height)) : ccp((menuOrigin.x - (iCurrentPage*winSize.width)), menuOrigin.y);
}

Point SlidingMenuGrid::GetPositionOfCurrentPageWithOffset(float offset)
{
	Size winSize = Director::sharedDirector()->getWinSize();
	return (bVerticalPaging) ? ccp(menuOrigin.x, menuOrigin.y - (iCurrentPage*winSize.height) + offset) : ccp((menuOrigin.x - (iCurrentPage*winSize.width) + offset), menuOrigin.y);
}

MenuItemSprite* SlidingMenuGrid::GetItemWithinTouch(Touch* touch)
{
	Point touchLocation = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

	// Parse our menu items and see if our touch exists within one.
	auto child = pMenu->getChildren();
	for(auto item:child)
	{

		MenuItemSprite * getItem = static_cast<CCMenuItemSprite *>(item);

		Point local = getItem->convertToNodeSpace(touchLocation);
		Rect r = getItem->rect();
		r.origin = CCPointZero;// If the touch was within this item. Return the item.
		if (r.containsPoint(local))
		{
			return getItem;
		}
	}
	// Didn't touch an item. 
	return NULL;
}

bool SlidingMenuGrid::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

void SlidingMenuGrid::onExit()
{
	_eventDispatcher->removeEventListener(touchListener);
}

bool SlidingMenuGrid::initWithArray(Vector<MenuItemSprite*> items, int cols, int rows, CCPoint pos, CCPoint pad, bool vertical)
{
	if (!CCLayer::init())
	{
		return false;
	}

	selectedItem = NULL;
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SlidingMenuGrid::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SlidingMenuGrid::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(SlidingMenuGrid::onTouchCancelled, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(SlidingMenuGrid::onTouchMoved, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	int iTag;//Tag saved in every item in array of items
	pMenu = Menu::create();
	addChild(pMenu, 0);
	for (auto mi : items)
	{
		iTag = mi->getTag();
		pMenu->addChild(mi, 1, iTag);
	}
	padding = pad;
	iCurrentPage = 0;
	bMoving = false;
	iMenuRows = rows;
	menuOrigin = pos;
	fMoveDeadZone = 10;
	bVerticalPaging = vertical;
	fAnimSpeed = 1;
	state = Waiting;
	(bVerticalPaging) ? this->buildGridVertical(cols, rows) : this->buildGrid(cols, rows);
	this->setPosition(menuOrigin);
	return true;
}

float SlidingMenuGrid::GetSwipeDeadZone()
{
	return fMoveDeadZone;
}

void SlidingMenuGrid::SetSwipeDeadZone(float fValue)
{
	fMoveDeadZone = fValue;
}

bool SlidingMenuGrid::IsVerticallyPaged()
{
	return bVerticalPaging;
}

void SlidingMenuGrid::SetVerticalPaging(bool bValue)
{
	bVerticalPaging = bValue;
}

void SlidingMenuGrid::gotoPage(int iPage /*= 0*/)
{
	int iSetPage = iPage;
	if (iPage <= 0)
		iSetPage = 1;
	else if (iPage > iPageCount)
	{
		iSetPage = iPageCount;
	}
	if (bVerticalPaging)
		iCurrentPage = iPageCount - iSetPage;
	else
		iCurrentPage = iSetPage - 1;
	moveToCurrentPage();
}

void SlidingMenuGrid::setPageLabel(int iPage, LabelTTF *pLabel)
{
	addChild(pLabel, 10);
	if (!pLabel)
		return;
	int iSetPage = iPage;
	if (iPage <= 0)
		iSetPage = 1;
	else if (iPage > iPageCount)
	{
		iSetPage = iPageCount;
	}
	Size winSize = Director::sharedDirector()->getWinSize();
	Size labelSize = pLabel->getContentSize();
	Point pt;
	if (bVerticalPaging)
		pt = ccp(menuOrigin.x + labelSize.width / 2, menuOrigin.y - (iSetPage - 1)*winSize.height + iMenuRows*padding.y + labelSize.height / 2);
	else
		pt = ccp(menuOrigin.x + (iSetPage - 1)*winSize.width + labelSize.width / 2, winSize.height - menuOrigin.y - iMenuRows*padding.y - labelSize.height);
	pLabel->setPosition(pt);
}

SlidingMenuGrid* SlidingMenuGrid::menuWithArray(Vector<MenuItemSprite*> items, int cols, int rows, CCPoint pos, CCPoint pad, bool vertical /*= false*/)
{
	SlidingMenuGrid *slidingMenu = new SlidingMenuGrid();
	if (slidingMenu && slidingMenu->initWithArray(items, cols, rows, pos, pad, vertical))
	{
		slidingMenu->autorelease();
		return slidingMenu;
	}
	CC_SAFE_DELETE(slidingMenu);
	return NULL;
}
