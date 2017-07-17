#ifndef __WALL_H__
#define __WALL_H__

#include "cocos2d.h"

#include "Object.h"

class Wall : public StageObject
{
private:
	cocos2d::DrawNode* _drawNode;
protected:
	Wall();
	virtual ~Wall();
    virtual bool init();    
public:
	CREATE_FUNC(Wall);
	CC_SYNTHESIZE(int, _endX, EndX);
	CC_SYNTHESIZE(int, _endY, EndY);
	virtual int getRight() { return (_x > _endX) ? _x : _endX; };
	virtual int getLeft() { return (_x < _endX) ? _x : _endX; };
	virtual int getUp() { return (_y > _endY) ? _y : _endY; };
	virtual int getBottom() { return (_y < _endY) ? _y : _endY; };
	virtual bool setPosition(int x, int y)
	{
		int centerX = (_x + _endX) / 2;
		int centerY = (_y + _endY) / 2;
		_x = _x + (x - centerX);
		_y = _y + (y - centerY);
		_endX = _endX + (x - centerX);
		_endY = _endY + (y - centerY);
		return true;
	};
	virtual bool settingCollide(cocos2d::Vec2& point, int radius);
	virtual CollideFlag collide(cocos2d::Vec2& start, cocos2d::Vec2& end, cocos2d::Vec2& vec);
	virtual void draw(cocos2d::Node* target);
};

#endif // __WALL_H__