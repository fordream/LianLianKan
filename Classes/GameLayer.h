#ifndef  _GAME_LAYER_H_
#define  _GAME_LAYER_H_
#include "cocos2d.h"
#include "Global.h"
USING_NS_CC;
class GameLayer :public cocos2d::LayerColor{
public:

	virtual bool init();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

	void update(float dt);
	CREATE_FUNC(GameLayer);
	void initUI();
	void initMap();
	
private:
	void playBgMusic(float);
	void playBgMusic1(float);
	void playBgMusic2(float);
	void change();
	void drawMap();
	void changeMap();

	// «∑Ò «À¿æ÷
	bool isDead();
	cocos2d::ProgressTimer *_progress;
	cocos2d::Label *_numberTime;
	cocos2d::DrawNode* _draw;
	cocos2d::Vec2 _prev;
	int _map[xCount][yCount];// = { { 0 } };

	std::vector<cocos2d::Vec2> _vecExpand1;
	std::vector<cocos2d::Vec2> _vecExpand2;
	std::vector<cocos2d::Vec2> _selected;
	std::vector<cocos2d::Vec2> _path;
	bool link(cocos2d::Vec2 v1, cocos2d::Vec2 v2);
	bool linkD(cocos2d::Vec2 v1, cocos2d::Vec2 v2);
	void expandX(cocos2d::Vec2 v, std::vector<cocos2d::Vec2> &vector);
	void expandY(cocos2d::Vec2 v, std::vector<cocos2d::Vec2> &vector);
	void autoClear(cocos2d::Ref*);
	cocos2d::Vec2 index2Screen(int x, int y);
	cocos2d::Vec2 screen2Index(float x, float y);
	int genSpriteTag(int x, int y);
};
#endif