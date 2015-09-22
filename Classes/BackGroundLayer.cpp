#include "BackGroundLayer.h"
#include "Global.h"
#include "GameLayer.h"
USING_NS_CC;

bool BackGroundLayer::init()
{
	if (!Layer::init()) {
		return false;
	}
	// 获取屏幕可视区域
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//纹理缓存
	auto textureCache = Director::getInstance()->getTextureCache();

	//帧缓存
	auto spriteFrameCache = SpriteFrameCache::getInstance();

	//背景
	auto background = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound));
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(background);


	//白云1
	auto cloud1 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud1));
	cloud1->setAnchorPoint(Vec2(0, 0));
	cloud1->setPosition(0, visibleSize.height - cloud1->getContentSize().height);
	addChild(cloud1);

	//白云倒影
	auto cloudShadow = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud1));
	cloudShadow->setAnchorPoint(Vec2(0, 0));
	//垂直翻转
	cloudShadow->setFlippedY(true);
	//设置透明度
	cloudShadow->setOpacity(40);
	cloudShadow->setPosition(0, visibleSize.height - cloud1->getContentSize().height * 2);
	addChild(cloudShadow);

	// 白云2
	auto cloud2 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud2));
	// 设置锚点，左下角
	cloud2->setAnchorPoint(Vec2(0, 0));
	cloud2->setPosition(cloud1->getContentSize().width, visibleSize.height - cloud2->getContentSize().height);
	addChild(cloud2);


	// 岛
	auto land = Sprite::createWithTexture(textureCache->getTextureForKey(s_island));
	land->setAnchorPoint(Vec2(1, 0));
	land->setPosition(visibleSize.width - 40, visibleSize.height - land->getContentSize().height - 47 * 2);
	addChild(land);
	// 岛倒影
	auto landShadow = Sprite::createWithTexture(textureCache->getTextureForKey(s_island));
	landShadow->setAnchorPoint(Vec2(1, 0));
	landShadow->setFlippedY(true);
	landShadow->setOpacity(40);
	landShadow->setPosition(visibleSize.width - 40, visibleSize.height - land->getContentSize().height - 78 * 2);
	addChild(landShadow);

	// 取第一帧
	auto frame = spriteFrameCache->getSpriteFrameByName("sailing_boat1.png");
	// 用帧创建一个船精灵
	auto boat = Sprite::createWithSpriteFrame(frame);
	boat->setPosition(visibleSize.width - boat->getContentSize().width - 50 * 2, 230 * 2);
	// 创建一个帧动画
	auto animation = Animation::create();
	// 循环取船的序列帧
	for (int i = 1; i < 4; i++) {
		char bname[64];
		sprintf(bname, "sailing_boat%d.png", i);
		animation->addSpriteFrame(spriteFrameCache->getSpriteFrameByName(bname));
	}
	// 设置每一个动画帧的间隔
	animation->setDelayPerUnit(0.5);
	// 动画完毕后保持第一帧
	animation->setRestoreOriginalFrame(true);
	addChild(boat);
	// 创建一个动作
	auto animate = Animate::create(animation);
	boat->runAction(RepeatForever::create(animate));
	// 向左移动
	auto moveBy = MoveBy::create(10, Vec2(-240, 0));
	// 水平翻转，船掉头
	auto flipxAction = FlipX::create(true);
	// 组合动画，船来回游荡，并会掉头
	auto action = Sequence::create(moveBy, flipxAction, moveBy->reverse(), flipxAction->reverse(), nullptr);
	boat->runAction(RepeatForever::create(action));

	// 第二条船，动画同上，不过是在左边
	auto boat2 = Sprite::createWithSpriteFrame(frame);
	boat2->setFlippedX(true);
	boat2->setPosition(100, 400);
	addChild(boat2);
	// 这里不能直接用animate，因为boat1已经使用，这里必须要用另一个动作，所以clone一下
	boat2->runAction(animate->clone());
	auto moveBy2 = MoveBy::create(12, Vec2(300, 0));

	// 这里的flipx跟boat1相反，所以这里先reverse一下，后面不reverse
	auto action2 = Sequence::create(moveBy2, flipxAction->clone()->reverse(), moveBy2->reverse(), flipxAction->clone(), nullptr);
	boat2->runAction(RepeatForever::create(action2));

	// 沙滩
	auto beach = Sprite::createWithTexture(textureCache->getTextureForKey(s_beach_adornment));
	beach->setPosition(visibleSize.width / 2, beach->getContentSize().height / 2);
	addChild(beach);

	// 海浪1
	auto wave = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_sea1));
	wave->setOpacity(0);
	wave->setPosition(visibleSize.width / 2, 140 * 2);
	addChild(wave);

	// 海浪2
	auto wave2 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_sea2));
	wave2->setPosition(visibleSize.width / 2, 100 * 2);
	wave2->setOpacity(0);
	addChild(wave2);

	auto waveMoveBy = MoveBy::create(4.0, Vec2(0, -80));
	// 由暗到明
	auto fadeIn = FadeIn::create(3.0);
	// 由明到暗
	auto fadeOut = FadeOut::create(1.0);
	// 放到初始位置
	auto place1 = Place::create(Vec2(visibleSize.width / 2, 140 * 2));
	// 同时进行动作，时间长度由最长的动作决定
	auto spawn = Spawn::create(waveMoveBy, Sequence::create(fadeIn, fadeOut, nullptr), nullptr);
	// 海浪2的动作
	auto spawn2 = Spawn::create(waveMoveBy->clone()->reverse(), Sequence::create(fadeIn->clone(), fadeOut->clone(), nullptr), nullptr);
	// 海浪2的初始位置
	auto place2 = Place::create(Vec2(visibleSize.width / 2, 100 * 2));
	// target 动作，指定tartget的动作
	auto targetAction = TargetedAction::create(wave2, Sequence::create(spawn2, place2, nullptr));
	// 延迟2s
	auto delay = DelayTime::create(2.0);
	wave->runAction(RepeatForever::create(Sequence::create(spawn, targetAction, place1, delay, nullptr)));


	// 鸟

	auto birdFrame = spriteFrameCache->getSpriteFrameByName("seagull1.png");

	_bird = Sprite::createWithSpriteFrame(birdFrame);

	auto point = Vec2(CCRANDOM_0_1() * visibleSize.width, 0);

	_bird->setPosition(point);

	// 创建帧动画
	auto birdAnimation = Animation::create();

	// 循环取鸟的序列帧
	for (int i = 1; i < 4; i++) {
		char bname[64];
		sprintf(bname, "seagull%d.png", i);
		birdAnimation->addSpriteFrame(spriteFrameCache->getSpriteFrameByName(bname));
	}
	// 设置每一个动画帧的间隔
	birdAnimation->setDelayPerUnit(0.3);
	// 动画完毕后保持第一帧
	birdAnimation->setRestoreOriginalFrame(true);
	addChild(_bird);
	// 创建一个动作
	auto birdAnimate = Animate::create(birdAnimation);
	_bird->runAction(RepeatForever::create(birdAnimate));
	_bird->runAction(MoveBy::create(2, Vec2(CCRANDOM_0_1() * visibleSize.width, point.y + visibleSize.height + 100)));
	// 启用默认调度器，调用系统update函数来刷新
	scheduleUpdate();
	// 播放按钮
	auto play = Sprite::createWithTexture(textureCache->getTextureForKey(s_button_play));
	auto playS = Sprite::createWithTexture(textureCache->getTextureForKey(s_button_play_s));

	// 菜单按钮接一个回调函数
	auto menuItem = MenuItemSprite::create(play, playS, CC_CALLBACK_1(BackGroundLayer::play, this));
	auto menu = Menu::create(menuItem, nullptr);
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(menu,3,"play");
	return true;
}

void BackGroundLayer::update(float dt)
{
	// 如果海鸥飞出屏幕，就重新设置其位置，重新执行飞的动作
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (_bird->getPosition().y > visibleSize.height + 20) {
		auto point = Vec2(CCRANDOM_0_1() * visibleSize.width, 10);
		_bird->setPosition(point);
		_bird->runAction(MoveBy::create(floor(5 * CCRANDOM_0_1()), 
			Vec2(CCRANDOM_0_1() * visibleSize.width, point.y + visibleSize.height)));
	}
}

void BackGroundLayer::play(cocos2d::Ref* ref){
	auto gameLayer = GameLayer::create();
	addChild(gameLayer);
	this->removeChildByName("play");
}