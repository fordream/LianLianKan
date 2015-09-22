#include "WelcomeLayer.h"
#include "Global.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
USING_NS_CC;
bool WelcomeLayer::init(){
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto loading = Sprite::create(s_loading_logo);
	loading->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(loading);
	loading->setOpacity(50);

	// 一个由暗到明的动画
	auto fadeIn = FadeIn::create(3);
	// 创建一个一直重复的动画
	loading->runAction(RepeatForever::create(Sequence::create(fadeIn, fadeIn->reverse(), NULL)));

	auto spriteFrameCache = SpriteFrameCache::getInstance();
	// 纹理缓存
	auto textureCache = Director::getInstance()->getTextureCache();
	textureCache->addImage(s_backgound);
	textureCache->addImage(s_backgound_cloud1);
	textureCache->addImage(s_backgound_cloud2);
	textureCache->addImage(s_backgound_sea1);
	textureCache->addImage(s_backgound_sea2);
	textureCache->addImage(s_beach_adornment);
	textureCache->addImage(s_island);
	textureCache->addImage(s_button_play);
	textureCache->addImage(s_button_play_s);
	textureCache->addImage(s_time_bars);
	textureCache->addImage(s_time_slot);
	textureCache->addImage(s_game_dialog);
	textureCache->addImage(s_game_leisure);

	spriteFrameCache->addSpriteFramesWithFile(s_boat_plist);
	spriteFrameCache->addSpriteFramesWithFile(s_seagull_plist);
	spriteFrameCache->addSpriteFramesWithFile(s_icon_plist);

	// 加载声音资源
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_Back2new);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_Back3new);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_class);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_mainmenu);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(s_music_2);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(s_music_3);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(s_music_4);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(s_music_12);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(s_music_14);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(s_music_15);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(s_music_16);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(s_music_19);

	scheduleOnce(SEL_SCHEDULE(&WelcomeLayer::loadingDone), 2);
	return true;
}

void WelcomeLayer::loadingDone(float delta){
	auto scene = GameScene::create();
	auto animateScene = TransitionMoveInB::create(0.5f, scene);
	Director::getInstance()->replaceScene(animateScene);
}