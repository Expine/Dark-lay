#ifndef __GOAL_ZONE_H__
#define __GOAL_ZONE_H__

#include "cocos2d.h"

#include "Object.h"

class GoalZone : public StageObject
{
private:
protected:
	GoalZone();
	virtual ~GoalZone();
    virtual bool init();    
public:
	CREATE_FUNC(GoalZone);
	CC_SYNTHESIZE(int, _radius, Radius);
	CC_SYNTHESIZE_RETAIN(cocos2d::Sprite*, _goalZone, GoalZone);
	virtual int getRight() { return _x + _radius; };
	virtual int getLeft() { return _x - _radius; };
	virtual int getUp() { return _y + _radius; };
	virtual int getBottom() { return _y - _radius; };
	virtual CollideFlag collide(cocos2d::Vec2& start, cocos2d::Vec2& end, cocos2d::Vec2& vec);
	virtual void draw(cocos2d::Node* target);
	void explosion();
};

#endif // __GOAL_ZONE_H__