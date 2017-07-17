#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"

#include "stage/Stage.h"

class Title : public cocos2d::Layer
{
private:
	int _stageNumber;
	int _cursol;
	cocos2d::Vector<Stage*> _stageList;
	cocos2d::Sprite* _stageSprite;

	void rightArrow(cocos2d::Sprite* stage);
	void leftArrow(cocos2d::Sprite* stage);
protected:
	Title();
	virtual ~Title();
    virtual bool init();    
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(Title);	
	CC_SYNTHESIZE_RETAIN(Stage*, _stage, Stage);
	void resetStageSprite();
};

#endif // __TITLE_SCENE_H__
