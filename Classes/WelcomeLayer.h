#ifndef  _WELCOME_LAYER_H_
#define  _WELCOME_LAYER_H_
#include "cocos2d.h"

class WelcomeLayer :public cocos2d::Layer{
public:

	virtual bool init();

	CREATE_FUNC(WelcomeLayer);

	void loadingDone(float);
};

#endif