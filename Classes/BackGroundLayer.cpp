#include "BackGroundLayer.h"
#include "Global.h"
#include "GameLayer.h"
USING_NS_CC;

bool BackGroundLayer::init()
{
	if (!Layer::init()) {
		return false;
	}
	// ��ȡ��Ļ��������
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//������
	auto textureCache = Director::getInstance()->getTextureCache();

	//֡����
	auto spriteFrameCache = SpriteFrameCache::getInstance();

	//����
	auto background = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound));
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(background);


	//����1
	auto cloud1 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud1));
	cloud1->setAnchorPoint(Vec2(0, 0));
	cloud1->setPosition(0, visibleSize.height - cloud1->getContentSize().height);
	addChild(cloud1);

	//���Ƶ�Ӱ
	auto cloudShadow = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud1));
	cloudShadow->setAnchorPoint(Vec2(0, 0));
	//��ֱ��ת
	cloudShadow->setFlippedY(true);
	//����͸����
	cloudShadow->setOpacity(40);
	cloudShadow->setPosition(0, visibleSize.height - cloud1->getContentSize().height * 2);
	addChild(cloudShadow);

	// ����2
	auto cloud2 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud2));
	// ����ê�㣬���½�
	cloud2->setAnchorPoint(Vec2(0, 0));
	cloud2->setPosition(cloud1->getContentSize().width, visibleSize.height - cloud2->getContentSize().height);
	addChild(cloud2);


	// ��
	auto land = Sprite::createWithTexture(textureCache->getTextureForKey(s_island));
	land->setAnchorPoint(Vec2(1, 0));
	land->setPosition(visibleSize.width - 40, visibleSize.height - land->getContentSize().height - 47 * 2);
	addChild(land);
	// ����Ӱ
	auto landShadow = Sprite::createWithTexture(textureCache->getTextureForKey(s_island));
	landShadow->setAnchorPoint(Vec2(1, 0));
	landShadow->setFlippedY(true);
	landShadow->setOpacity(40);
	landShadow->setPosition(visibleSize.width - 40, visibleSize.height - land->getContentSize().height - 78 * 2);
	addChild(landShadow);

	// ȡ��һ֡
	auto frame = spriteFrameCache->getSpriteFrameByName("sailing_boat1.png");
	// ��֡����һ��������
	auto boat = Sprite::createWithSpriteFrame(frame);
	boat->setPosition(visibleSize.width - boat->getContentSize().width - 50 * 2, 230 * 2);
	// ����һ��֡����
	auto animation = Animation::create();
	// ѭ��ȡ��������֡
	for (int i = 1; i < 4; i++) {
		char bname[64];
		sprintf(bname, "sailing_boat%d.png", i);
		animation->addSpriteFrame(spriteFrameCache->getSpriteFrameByName(bname));
	}
	// ����ÿһ������֡�ļ��
	animation->setDelayPerUnit(0.5);
	// ������Ϻ󱣳ֵ�һ֡
	animation->setRestoreOriginalFrame(true);
	addChild(boat);
	// ����һ������
	auto animate = Animate::create(animation);
	boat->runAction(RepeatForever::create(animate));
	// �����ƶ�
	auto moveBy = MoveBy::create(10, Vec2(-240, 0));
	// ˮƽ��ת������ͷ
	auto flipxAction = FlipX::create(true);
	// ��϶������������ε��������ͷ
	auto action = Sequence::create(moveBy, flipxAction, moveBy->reverse(), flipxAction->reverse(), nullptr);
	boat->runAction(RepeatForever::create(action));

	// �ڶ�����������ͬ�ϣ������������
	auto boat2 = Sprite::createWithSpriteFrame(frame);
	boat2->setFlippedX(true);
	boat2->setPosition(100, 400);
	addChild(boat2);
	// ���ﲻ��ֱ����animate����Ϊboat1�Ѿ�ʹ�ã��������Ҫ����һ������������cloneһ��
	boat2->runAction(animate->clone());
	auto moveBy2 = MoveBy::create(12, Vec2(300, 0));

	// �����flipx��boat1�෴������������reverseһ�£����治reverse
	auto action2 = Sequence::create(moveBy2, flipxAction->clone()->reverse(), moveBy2->reverse(), flipxAction->clone(), nullptr);
	boat2->runAction(RepeatForever::create(action2));

	// ɳ̲
	auto beach = Sprite::createWithTexture(textureCache->getTextureForKey(s_beach_adornment));
	beach->setPosition(visibleSize.width / 2, beach->getContentSize().height / 2);
	addChild(beach);

	// ����1
	auto wave = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_sea1));
	wave->setOpacity(0);
	wave->setPosition(visibleSize.width / 2, 140 * 2);
	addChild(wave);

	// ����2
	auto wave2 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_sea2));
	wave2->setPosition(visibleSize.width / 2, 100 * 2);
	wave2->setOpacity(0);
	addChild(wave2);

	auto waveMoveBy = MoveBy::create(4.0, Vec2(0, -80));
	// �ɰ�����
	auto fadeIn = FadeIn::create(3.0);
	// ��������
	auto fadeOut = FadeOut::create(1.0);
	// �ŵ���ʼλ��
	auto place1 = Place::create(Vec2(visibleSize.width / 2, 140 * 2));
	// ͬʱ���ж�����ʱ�䳤������Ķ�������
	auto spawn = Spawn::create(waveMoveBy, Sequence::create(fadeIn, fadeOut, nullptr), nullptr);
	// ����2�Ķ���
	auto spawn2 = Spawn::create(waveMoveBy->clone()->reverse(), Sequence::create(fadeIn->clone(), fadeOut->clone(), nullptr), nullptr);
	// ����2�ĳ�ʼλ��
	auto place2 = Place::create(Vec2(visibleSize.width / 2, 100 * 2));
	// target ������ָ��tartget�Ķ���
	auto targetAction = TargetedAction::create(wave2, Sequence::create(spawn2, place2, nullptr));
	// �ӳ�2s
	auto delay = DelayTime::create(2.0);
	wave->runAction(RepeatForever::create(Sequence::create(spawn, targetAction, place1, delay, nullptr)));


	// ��

	auto birdFrame = spriteFrameCache->getSpriteFrameByName("seagull1.png");

	_bird = Sprite::createWithSpriteFrame(birdFrame);

	auto point = Vec2(CCRANDOM_0_1() * visibleSize.width, 0);

	_bird->setPosition(point);

	// ����֡����
	auto birdAnimation = Animation::create();

	// ѭ��ȡ�������֡
	for (int i = 1; i < 4; i++) {
		char bname[64];
		sprintf(bname, "seagull%d.png", i);
		birdAnimation->addSpriteFrame(spriteFrameCache->getSpriteFrameByName(bname));
	}
	// ����ÿһ������֡�ļ��
	birdAnimation->setDelayPerUnit(0.3);
	// ������Ϻ󱣳ֵ�һ֡
	birdAnimation->setRestoreOriginalFrame(true);
	addChild(_bird);
	// ����һ������
	auto birdAnimate = Animate::create(birdAnimation);
	_bird->runAction(RepeatForever::create(birdAnimate));
	_bird->runAction(MoveBy::create(2, Vec2(CCRANDOM_0_1() * visibleSize.width, point.y + visibleSize.height + 100)));
	// ����Ĭ�ϵ�����������ϵͳupdate������ˢ��
	scheduleUpdate();
	// ���Ű�ť
	auto play = Sprite::createWithTexture(textureCache->getTextureForKey(s_button_play));
	auto playS = Sprite::createWithTexture(textureCache->getTextureForKey(s_button_play_s));

	// �˵���ť��һ���ص�����
	auto menuItem = MenuItemSprite::create(play, playS, CC_CALLBACK_1(BackGroundLayer::play, this));
	auto menu = Menu::create(menuItem, nullptr);
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(menu,3,"play");
	return true;
}

void BackGroundLayer::update(float dt)
{
	// �����Ÿ�ɳ���Ļ��������������λ�ã�����ִ�зɵĶ���
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