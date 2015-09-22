#ifndef  _BACKGROUND_LAYER_H_
#define  _BACKGROUND_LAYER_H_
#include "cocos2d.h"

class BackGroundLayer :public cocos2d::Layer{
public:

	virtual bool init();

	virtual void update(float);

	CREATE_FUNC(BackGroundLayer);

	void play(cocos2d::Ref*);
private:
	cocos2d::Sprite* _bird;
};

#endif