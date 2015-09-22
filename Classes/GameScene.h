#ifndef  _GAME_SCENE_H_
#define  _GAME_SCENE_H_
#include "cocos2d.h"

class GameScene :public cocos2d::Scene{
public:

	virtual bool init();

	void play(void);

	CREATE_FUNC(GameScene);
private:
	cocos2d::Layer* _backgroundLayer;
};

#endif