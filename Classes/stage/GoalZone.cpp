#include <math.h>

#include "GoalZone.h"

#include "system/MyUtils.h"

USING_NS_CC;

/*
 * Constructor
 */
GoalZone::GoalZone()
{
	_goalZone = NULL;
}

/*
 * Destructor
 */
GoalZone::~GoalZone()
{
	CC_SAFE_RELEASE_NULL(_goalZone);
}

/*
 * Initialize
 */
bool GoalZone::init()
{
    if ( !StageObject::init() )
    {
        return false;
    }

    return true;
}

 CollideFlag GoalZone::collide(cocos2d::Vec2 & start, cocos2d::Vec2 & end, cocos2d::Vec2 & vec)
 {
	 Vec2 v = end - start;
	 Vec2 a = Vec2(_x, _y) - start;
	 Vec2 b = Vec2(_x, _y) - end;
	 float d = abs(v.cross(a)) / v.getLength();
	 vec.set(start + (v * (v.dot(a) / (v.x * v.x + v.y * v.y))));
	 if (d <= _radius) {
		 if (v.dot(a) * v.dot(b) <= 0) return CollideFlag::goal;
		 if (a.x * a.x + a.y * a.y < d * d || b.x * b.x + b.y * b.y < d * d) return CollideFlag::goal;
	 }
	 return CollideFlag::none;
 }

 /*
 * Draw
 */
 void GoalZone::draw(Node* target)
{
 	//Create animetion
	auto anime = Animation::create();
	for(int y=0; y<5; y++)
	{
		for(int x=0; x<5; x++)
		{
			anime->addSpriteFrame(SpriteFrame::create("image/goal.png", Rect(0 + 160*x, 0 + 160*y, 160, 160)));
		}
	}
	anime->setDelayPerUnit(0.04f);
	anime->setRestoreOriginalFrame(true);

	//Create sprite
	auto goalZone = Sprite::create("image/goal.png", Rect(0, 0, 160, 160));
	goalZone->setPosition(_x, _y);
	goalZone->setScale(_radius / 80.0f * 1.6f);
	goalZone->runAction(
		RepeatForever::create(
			Animate::create(anime)
		)
	);
	setGoalZone(goalZone);
	target->addChild(goalZone, 20);
}

 void GoalZone::explosion()
 {
	 Node* parent = _goalZone->getParent();
	 _goalZone->removeFromParent();

	 //Create animetion
	 auto anime = Animation::create();
	 for (int y = 0; y<4; y++)
	 {
		 for (int x = 0; x<4; x++)
		 {
			 anime->addSpriteFrame(SpriteFrame::create("image/explosion.png", Rect(0 + 250 * x, 0 + 250 * y, 250, 250)));
		 }
	 }
	 anime->setDelayPerUnit(0.1f);
	 anime->setRestoreOriginalFrame(true);

	 //Create sprite
	 auto goalZone = Sprite::create("image/explosion.png", Rect(0, 0, 250, 250));
	 goalZone->setPosition(_x, _y);
	 goalZone->setScale(_radius / 80.0f * 1.6f);
	 goalZone->runAction(
		Sequence::create(
			Animate::create(anime),
			CallFunc::create([goalZone] 
			{
				 goalZone->removeFromParent();
			}),
		NULL)
	 );
	 parent->addChild(goalZone, 20);
 }

