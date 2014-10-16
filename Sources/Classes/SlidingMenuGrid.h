#ifndef SlidingMenuGrid_h__
#define SlidingMenuGrid_h__
#include "cocos2d.h"
USING_NS_CC;

enum SlidingMenuGridState
{
	Waiting, TrackingTouch, Cancelled
};

class SlidingMenuGrid:public Layer
{
protected:
	void buildGrid(int cols, int rows);
	void buildGridVertical(int cols, int rows);
	void moveToCurrentPage();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);

	Point GetPositionOfCurrentPage();
	Point GetPositionOfCurrentPageWithOffset(float offset);
	MenuItemSprite* GetItemWithinTouch(Touch* touch);

	MenuItem *selectedItem; // Menu item that was selected/active.

	Point padding; // Padding in between menu items. 
	Point menuOrigin; // Origin position of the entire menu grid.
	Point touchOrigin; // Where the touch action began.
	Point touchStop; // Where the touch action stopped.

	int iPageCount; // Number of pages in this grid.
	int iCurrentPage; // Current page of menu items being viewed.

	bool bMoving; // Is the grid currently moving?
	bool bSwipeOnlyOnMenu; // Causes swiping functionality to only work when siping on top of the menu items instead of entire screen.
	bool bVerticalPaging; // Disabled by default. Allows for pages to be scrolled vertically instead of horizontal.

	float fMoveDelta; // Distance from origin of touch and current frame.
	float fMoveDeadZone; // Amount they need to slide the grid in order to move to a new page.
	float fAnimSpeed; // 0.0-1.0 value determining how slow/fast to animate the paging.

	SlidingMenuGridState state;

	int iMenuRows;
	Menu * pMenu;
	EventListenerTouchOneByOne* touchListener;
public:
	SlidingMenuGrid();
	~SlidingMenuGrid();
	virtual bool init();
	virtual void onExit();
	static SlidingMenuGrid* menuWithArray(Vector<MenuItemSprite*> items, int cols, int rows, CCPoint pos, CCPoint pad, bool vertical = false);
	bool initWithArray(Vector<MenuItemSprite*> items, int cols, int rows, CCPoint pos, CCPoint pad, bool vertical);
	float GetSwipeDeadZone();
	void SetSwipeDeadZone(float fValue);
	bool IsVerticallyPaged();
	void SetVerticalPaging(bool bValue);
	int getPageCount(){ return iPageCount; }

	void gotoPage(int iPage = 0);
	void setPageLabel(int iPage, LabelTTF * pLabel);//Add label on the page

};
#endif // SlidingMenuGrid_h__

