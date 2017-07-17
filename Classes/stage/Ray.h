#ifndef __RAY_H__
#define __RAY_H__

#include "cocos2d.h"

#include "Object.h"
#include "Stage.h"

class StageRay : public StageObject
{
private:
	static const float dt;
	static const int MAX_ITEM = 100000;
	void redrawNode();
	std::vector<cocos2d::Vec2> _points;
protected:
	StageRay();
	virtual ~StageRay();
    virtual bool init();    
public:
	CREATE_FUNC(StageRay);
	CC_SYNTHESIZE(bool, _goalFlag, GoalFlag);
	CC_SYNTHESIZE_READONLY(float, _initX, InitX);
	CC_SYNTHESIZE_READONLY(float, _initY, InitY);
	void setInitVec(cocos2d::Vec2 v);
	CC_SYNTHESIZE_RETAIN(Stage*, _stage, Stage);
	CC_SYNTHESIZE_RETAIN(cocos2d::DrawNode*, _drawNode, DrawNode);
	virtual void draw(cocos2d::Node* target);
	virtual void update();
	void animationWithGoal();
};


#endif // __RAY_H__