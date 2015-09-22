#include "WelcomeScene.h"
#include "WelcomeLayer.h"
USING_NS_CC;

bool WelcomeScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	auto welcomeLayer = WelcomeLayer::create();

	// 添加到当前层
	addChild(welcomeLayer);

	return true;
}