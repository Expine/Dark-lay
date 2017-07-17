#ifndef __PLANET_H__
#define __PLANET_H__

#include "cocos2d.h"

#include "Object.h"
#include "stage/Stage.h"

enum class PlanetType
{
	BlackHole, Mercury, Mars, Venus, Earth, Uranus, Neptune, Saturn, Jupiter
};

const float RADIUS[] = { 9, 11, 13, 15, 17, 19, 21, 23, 25 };
const float ATTRACTION[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };
//const float ATTRACTION[] = { 10, 3, 5, 5, 6, 7, 8, 9, 10 };

class Planet : public StageObject
{
private:
	bool _putable;
protected:
	Planet();
	virtual ~Planet();
    virtual bool init();    
public:
	CREATE_FUNC(Planet);
	CC_SYNTHESIZE_READONLY(PlanetType, _type, Type);
	CC_SYNTHESIZE_READONLY(float, _radius, Radius);
	CC_SYNTHESIZE_READONLY(float, _attraction, Attraction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Sprite*, _sprite, Sprite);
	CC_SYNTHESIZE_RETAIN(cocos2d::Sprite*, _xSprite, XSprite);
	virtual bool setPosition(int x, int y, Stage* stage);
	void setType(PlanetType type);
	virtual int getRight() { return _x + _radius; };
	virtual int getLeft() { return _x - _radius; };
	virtual int getUp() { return _y + _radius; };
	virtual int getBottom() { return _y - _radius; };
	virtual void physics(cocos2d::Vec2& point, cocos2d::Vec2& a);
	virtual bool settingCollide(cocos2d::Vec2& point, int radius);
	virtual CollideFlag collide(cocos2d::Vec2& start, cocos2d::Vec2& end, cocos2d::Vec2& vec);
	virtual void draw(cocos2d::Node* target);

	inline bool isPutable() { return _putable; };
};

#endif // __PLANET_H__