#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "cocos2d.h"

enum class CollideFlag
{
	none, finish, goal, mirror
};

class StageObject : public cocos2d::Ref
{
protected:
	StageObject();
	virtual ~StageObject();
    virtual bool init();    
public:
	CREATE_FUNC(StageObject);
	CC_SYNTHESIZE(int, _x, X);
	CC_SYNTHESIZE(int, _y, Y);
	virtual int getRight() { return _x; };
	virtual int getLeft() { return _x; };
	virtual int getUp() { return _y; };
	virtual int getBottom() { return _y; };
	virtual bool setPosition(int x, int y) { _x = x; _y = y; return true; };
	virtual void physics(cocos2d::Vec2& point, cocos2d::Vec2& a);
	virtual bool settingCollide(cocos2d::Vec2& point, int radius);
	virtual CollideFlag collide(cocos2d::Vec2& start, cocos2d::Vec2& end, cocos2d::Vec2& vec);
	virtual void draw(cocos2d::Node* target);
	virtual void update();
};

#endif  //__OBJECT_H__//