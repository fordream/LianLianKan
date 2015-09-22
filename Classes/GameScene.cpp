#include "GameScene.h"
#include "BackGroundLayer.h"
USING_NS_CC;

bool GameScene::init()
{
	// 先调父类的init函数
	// 这里可以知道，init()函数是一个继承下来的函数，而且是一个虚函数，子类可以重写这个函数
	if (!Scene::init()) {
		return false;
	}
	_backgroundLayer = BackGroundLayer::create();

	// 添加到当前层
	addChild(_backgroundLayer);


	return true;
}