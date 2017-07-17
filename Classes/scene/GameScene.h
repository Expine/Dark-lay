#ifndef __GAME_SCENE_H_
#define __GAME_SCENE_H_

#include "cocos2d.h"

#include "stage/Planet.h"
#include "stage/Stage.h"

class Game : public cocos2d::Layer
{
private:
	int _cursol;
	void drawStage();
	cocos2d::EventListenerTouchOneByOne* createMoveListener(cocos2d::Sprite* back, Planet* pl, int i);
protected:
	Game();
	virtual ~Game();
    virtual bool init();    
public:
	static cocos2d::Scene* createScene(Stage* stage, cocos2d::CallFunc* cf);
    CREATE_FUNC(Game);	
	CC_SYNTHESIZE_RETAIN(cocos2d::CallFunc*, _cf, Cf);
	CC_SYNTHESIZE_RETAIN(Stage*, _stage, Stage);
	CC_SYNTHESIZE_RETAIN(Planet*, _planet, Planet);
};

#endif // __GAME_SCENE_H_
