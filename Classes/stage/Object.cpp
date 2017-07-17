#include "Object.h"

#include "system/MyUtils.h"

USING_NS_CC;

/*
 * Constructor
 */
StageObject::StageObject()
{
	_x = 0;
	_y = 0;
}

/*
 * Destructor
 */
StageObject::~StageObject(){}

/*
 * Initialize
 */
bool StageObject::init()
{
    return true;
}

/*
 * Draw
 */
void StageObject::draw(Node* target){}

/*
 * Update
 */
void StageObject::update(){}

/*
 * Physics
 */
void StageObject::physics(Vec2& v, Vec2& a) {}

bool StageObject::settingCollide(cocos2d::Vec2 & point, int radius)
{
	return false;
}

/*
 * Collide
 */
CollideFlag StageObject::collide(Vec2& start, Vec2& end, Vec2& vec) { return CollideFlag::none; }
