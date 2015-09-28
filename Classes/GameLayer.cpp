#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
USING_NS_CC;
bool GameLayer::init()
{
	if (!LayerColor::init()) {
		return false;
	}
	scheduleOnce(SEL_SCHEDULE(&GameLayer::playBgMusic), 0);
	_prev = Vec2::ZERO;
	isTipable = true;
	memset(_map, 0, xCount*yCount*sizeof(int));
	initUI();
	initMap();
	return true;
}
bool GameLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){ 
	return true;
}
void GameLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event){}
void GameLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
	float x = touch->getLocation().x;
	float y = touch->getLocation().y;
	//屏幕坐标转换为地图坐标
	auto point = screen2Index(x, y);

	CCLOG("touch point index x:%d, y:%d", static_cast<int>(point.x), static_cast<int>(point.y));

	//判断连通与否
	//当前位置有精灵
	if (_map[(int)point.x][(int)point.y] > 0) {
		//上一次点击的精灵不是空
		if (!_prev.equals(Vec2::ZERO)) {
			CCLOG("compare point x:%d, y:%d", static_cast<int>(point.x), static_cast<int>(point.y));
			//可以和这次点击的精灵消去
			if (link(_prev, point)) {
				CCLOG("path point count :%d", _path.size());
				int tag = genSpriteTag(point.x, point.y);
				auto selectedIcon = getChildByTag(tag);
				selectedIcon->setScale(1.2);
				selectedIcon->setLocalZOrder(101);

				drawLine();
			}
			//不能和这次点击的精灵消去
			else{
				//原来的变小
				int tag = genSpriteTag(_prev.x, _prev.y);
				auto selectedIcon = getChildByTag(tag);
				// 恢复原大小
				selectedIcon->setScale(1.0);
				// 恢复原Z序
				selectedIcon->setLocalZOrder(100);

				//当前的变大
				tag = genSpriteTag(point.x, point.y);
				selectedIcon = getChildByTag(tag);
				// 放大1.2倍
				selectedIcon->setScale(1.2);
				// Z序提前，放在所有精灵前面
				selectedIcon->setLocalZOrder(101);

				//原来的等于现在的
				_prev = point;
			}
		}
		else{
			CCLOG("add a point");
			_prev = point;
			int tag = genSpriteTag(point.x, point.y);
			auto selectedIcon = getChildByTag(tag);
			selectedIcon->setScale(1.2);
			selectedIcon->setLocalZOrder(101);
		}
	}
}
void GameLayer::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event){}
void GameLayer::drawLine()
{

	// 画线
	if (_path.size() >= 2) {
		Vec2 *vecs = new Vec2[_path.size()];
		for (int i = 0; i < _path.size(); i++) {
			vecs[i] = index2Screen(_path.at(i).x, _path.at(i).y);
		}

		// 随机给线条一个颜色
		auto color = CCRANDOM_0_1();

		// 循环画线段，只有线段可以设置线条宽度
		for (int i = 0; i < _path.size(); i++) {
			if (i > 0) {
				_draw->drawSegment(vecs[i - 1], vecs[i], 5, Color4F(color, color, color, 1));
			}
		}

		// 连通的两个图标对应的地图数组置0
		Vec2 p1 = _path.front();
		_map[(int)p1.x][(int)p1.y] = 0;
		Vec2 p2 = _path.back();
		_map[(int)p2.x][(int)p2.y] = 0;

		// 删掉线段的顶点数组
		delete[] vecs;

		// 清除连通的图标，同时清除路径点
		scheduleOnce(SEL_SCHEDULE(&GameLayer::clearMatched), 0.2);
	}
}

void GameLayer::clearMatched(float dt)
{
	_draw->clear();

	Vec2 p1 = _path.front();
	Vec2 p2 = _path.back();

	auto x1 = (int)p1.x;
	auto y1 = (int)p1.y;
	auto tag1 = genSpriteTag(x1, y1);
	removeChildByTag(tag1);

	auto x2 = (int)p2.x;
	auto y2 = (int)p2.y;
	auto tag2 = genSpriteTag(x2, y2);
	removeChildByTag(tag2);

	_prev = Vec2::ZERO;
	_path.clear();

	while (isDead()){
		changeMap();
	}
}

void GameLayer::update(float dt){
	if (isWin()) {
		gameOver("You Win!");
		return;
	}
	// 当前进度
	int current = _progress->getPercentage();
	// 为零就gameover
	if (current == 0) {
		// game over
		gameOver("Game Over");
		return;
	}
	else{
		// 减一
		current--;
		_progress->setPercentage(current);
		char time[3];
		sprintf(time, "%d", current);
		_numberTime->setString(time);
	}
}
void GameLayer::playBgMusic(float)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(s_music_Back2new, false);
	scheduleOnce(SEL_SCHEDULE(&GameLayer::playBgMusic1), 60);
}

void GameLayer::playBgMusic1(float)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(s_music_Back3new, false);
	scheduleOnce(SEL_SCHEDULE(&GameLayer::playBgMusic2), 62);
}

void GameLayer::playBgMusic2(float)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(s_music_mainmenu, false);
	scheduleOnce(SEL_SCHEDULE(&GameLayer::playBgMusic), 65);
}
void GameLayer::initUI(){
	auto textureCache = Director::getInstance()->getTextureCache();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//时间条背景框
	auto progressFrame = Sprite::createWithTexture(textureCache->getTextureForKey(s_time_slot));
	progressFrame->setAnchorPoint(Vec2(0, 0));
	progressFrame->setPosition(120, visibleSize.height - 50);
	addChild(progressFrame);


	//时间条精灵
	auto sprite = Sprite::createWithTexture(textureCache->getTextureForKey(s_time_bars));
	_progress = ProgressTimer::create(sprite);
	_progress->setAnchorPoint(Vec2(0, 0));
	//类型，条形
	_progress->setType(ProgressTimer::Type::BAR);
	_progress->setPosition(120, visibleSize.height - 50);
	// 水平变化
	_progress->setMidpoint(Vec2(0, 0));
	// 一次一个单位
	_progress->setBarChangeRate(Vec2(1, 0));
	// 初始100
	_progress->setPercentage(100);
	addChild(_progress);

	// 时间数字
	_numberTime = Label::createWithSystemFont("100", "Thonburi", 24);
	_numberTime->setAnchorPoint(Vec2(0, 0));
	_numberTime->setPosition(400, visibleSize.height - 48);
	_numberTime->setColor(Color3B::BLACK);
	addChild(_numberTime);

	auto tip = Sprite::createWithTexture(textureCache->getTextureForKey(s_game_leisure));
	auto menuItemSprite = MenuItemSprite::create(tip, tip, CC_CALLBACK_1(GameLayer::autoClear, this));
	menuItemSprite->setPosition(visibleSize.width - tip->getContentSize().width/2,
		visibleSize.height - tip->getContentSize().height / 2);
	auto menu = Menu::create(menuItemSprite, nullptr);
	menu->setAnchorPoint(Vec2(1, 1));
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameLayer::onTouchCancelled, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_draw = DrawNode::create();
	addChild(_draw, 1000);
	_prev = Vec2::ZERO;
	schedule(SEL_SCHEDULE(&GameLayer::update), 1.0);
}
void GameLayer::autoClear(cocos2d::Ref* spender){
	// 左右抖动的动画
	if (isTipable){
		isTipable = false;
		auto rote = RotateBy::create(0.05, 20);
		auto seq = Sequence::create(rote, rote->reverse(), rote->reverse(), rote->clone(), 
			CallFunc::create([&]{isTipable = true; }), nullptr);
		((Sprite*)spender)->runAction(seq);
		//在isDead函数中，就已经计算出了路径
		if (isDead()) {
			CCLOG("die-----");
			changeMap();
		}
		else{
			drawLine();
		}
	}
	
}

void GameLayer::initMap()
{
	// 这里x用来控制地图数组的每格的值，y用来控制两次反转，即每两个格标记同一个值
	int x = 1;
	int y = 0;
	// 数组下标从0开始，这里从1开始遍历，那么最外面一圈就是0不变
	for (int i = 1; i < xCount - 1; i++) {
		for (int j = 1; j < yCount - 1; j++) {
			// 地图数组赋值
			_map[i][j] = x;
			// y控制反转，x控制每格值增加，增大到图片总数后从再1开始递增
			if (y == 1) {
				x++;
				y = 0;
				if (x == iconCount) {
					x = 1;
				}
			}
			else{
				y = 1;
			}
		}
	}
	change();
	drawMap();
}

void GameLayer::change(){
	// 随机种子
	srand((unsigned int)time(NULL));

	// 临时变量，用来交换两个位置的数据
	int tempX, tempY, tempM;

	// 遍历地图数组，随机交换位置
	for (int x = 1; x < xCount - 1; x++){
		for (int y = 1; y < yCount - 1; y++) {
			tempX = 1 + (int)(CCRANDOM_0_1() * (xCount - 2));
			tempY = 1 + (int)(CCRANDOM_0_1() * (yCount - 2));
			tempM = _map[x][y];
			_map[x][y] = _map[tempX][tempY];
			_map[tempX][tempY] = tempM;
		}
	}
		
	while(isDead()) {
		changeMap();
	}
}
void GameLayer::drawMap(){
	// 绘制棋盘上精灵
	for (int x = 0; x < xCount; x++) {
		for (int y = 0; y < yCount; y++) {
			if (_map[x][y] > 0) {
				char iconName[64];
				// 格式化图片名
				sprintf(iconName, "%d.png", _map[x][y]);
				auto position = index2Screen(x, y);
				// 所有图片已经加到帧缓存，这里直接用名字取一帧，创建一个精灵
				auto icon = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(iconName));
				//icon->setAnchorPoint(Vec2(1, 1));
				icon->setPosition(position);
				// 设置一个tag，方便后续识别点击的是那个图标
				int tag = genSpriteTag(x, y);
				// 这是Z序和tag值，Z序设置大一些确保图标在其他精灵的前面
				addChild(icon, 100, tag);
			}
		}
	}
}

bool GameLayer::isWin()
{
	for (int x = 0; x < xCount; x++) {
		for (int y = 0; y < yCount; y++) {
			if (_map[x][y] != 0) {
				return false;
			}
		}
	}
	return true;
}

bool GameLayer::isDead(){
	for (int y = 1; y < yCount-1; y++) {
		for (int x = 1; x < xCount-1; x++) {
			if (_map[x][y] != 0) {
				for (int j = y; j < yCount - 1; j++) {
					if (j == y) {
						for (int i = x + 1; i < xCount - 1; i++) {
							if (_map[i][j] == _map[x][y] && link(Vec2(x, y), Vec2(i, j))) {
								return false;
							}
						}
					}
					else{
						for (int i = 1; i < xCount - 1; i++) {
							if (_map[i][j] == _map[x][y] && link(Vec2(x, y), Vec2(i, j))) {
								return false;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

void GameLayer::gameOver(std::string title)
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 停止update函数调度。
	unschedule(SEL_SCHEDULE(&GameLayer::update));

	// 创建一个颜色层
	auto layer = LayerColor::create(Color4B(100, 100, 100, 100));
	// 锚点默认是左下角
	layer->setPosition(0, 0);
	addChild(layer, 1000);

	// 对话框背景
	auto sprite = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(s_game_dialog));
	sprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	layer->addChild(sprite);

	// Game Over 提示
	auto titleSprite = Label::createWithSystemFont(title, "Thonburi", 35);
	titleSprite->setPosition(layer->getContentSize().width / 2, sprite->getContentSize().height - 100);
	titleSprite->setColor(Color3B::RED);
	layer->addChild(titleSprite);

	// Font Item
	MenuItemFont::setFontName("fonts/Marker Felt.ttf");
	// Play Again 按钮
	auto item1 = MenuItemFont::create("Play Again", CC_CALLBACK_0(GameLayer::playAgain, this));
	auto color_action = TintBy::create(0.5f, 0, -255, -255);
	auto color_back = color_action->reverse();
	auto seq = Sequence::create(color_action, color_back, nullptr);
	item1->runAction(RepeatForever::create(seq));

	auto menu = Menu::create(item1, nullptr);

	menu->setPosition(layer->getContentSize().width / 2, sprite->getContentSize().height / 2);

	layer->addChild(menu);

	// Game Over层添加触摸事件
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	// 吞掉触摸事件，即下面的层不会响应此次触摸
	listener->setSwallowTouches(true);
	// Lambda表达式实现触摸函数，空的，因为不需要做触摸实现
	listener->onTouchBegan = [](Touch*touch, Event* event){return true; };
	listener->onTouchMoved = [](Touch*touch, Event* event){};
	listener->onTouchEnded = [](Touch*touch, Event* event){};
	listener->onTouchCancelled = [](Touch*touch, Event* event){};
	// 加到GameOver层
	dispatcher->addEventListenerWithSceneGraphPriority(listener, layer);

}

void GameLayer::playAgain()
{
	auto scene = GameScene::create();
	Director::getInstance()->replaceScene(TransitionRotoZoom::create(1.2, scene));
}

int GameLayer::genSpriteTag(int x, int y){
	return y*xCount + x;
}

void GameLayer::changeMap()
{
	// 随机种子
	srand((unsigned int)time(NULL));

	// 临时变量，用来交换两个位置的数据
	int tempX, tempY, tempM;

	// 遍历地图数组，随机交换位置
	for (int x = 1; x < xCount - 1; x++){
		for (int y = 1; y < yCount - 1; y++) {
			tempX = 1 + (int)(CCRANDOM_0_1() * (xCount - 2));
			tempY = 1 + (int)(CCRANDOM_0_1() * (yCount - 2));
			tempM = _map[x][y];
			_map[x][y] = _map[tempX][tempY];
			_map[tempX][tempY] = tempM;


			// 交换精灵位置，交换tag值
			int tag1 = genSpriteTag(x, y);
			int tag2 = genSpriteTag(tempX, tempY);

			auto pos1 = index2Screen(x, y);
			auto pos2 = index2Screen(tempX, tempY);

			auto pic1 = getChildByTag(tag1);
			auto pic2 = getChildByTag(tag2);

			if (pic1) {
				pic1->setPosition(pos2);
				pic1->setTag(tag2);
			}
			else{
				// 如果为空，地图数组对应值要置0
				_map[tempX][tempY] = 0;
			}

			if (pic2) {
				pic2->setPosition(pos1);
				pic2->setTag(tag1);
			}
			else{
				_map[x][y] = 0;
			}
		}
	}
		
}

Vec2 GameLayer::index2Screen(int x, int y)
{
	return Vec2(x * iconSize, y * iconSize);
}

Vec2 GameLayer::screen2Index(float x, float y)
{
	int ix = ceil((x - iconSize / 2) / iconSize);

	int iy = ceil((y - iconSize / 2) / iconSize);

	if (ix < xCount && iy < yCount) {
		return Vec2(ix, iy);
	}
	else{
		return Vec2::ZERO;
	}
}


bool GameLayer::link(cocos2d::Vec2 v1, cocos2d::Vec2 v2)
{
	if (v1.equals(v2)) {
		return false;
	}
	// 路径点集，是一个vector
	_path.clear();

	// 判断点击的两个点是不是图案一致
	if (_map[static_cast<int>(v1.x)][static_cast<int>(v1.y)] == _map[static_cast<int>(v2.x)][static_cast<int>(v2.y)]) {
		// 直连
		if (linkD(v1, v2)) {
			_path.push_back(v1);
			_path.push_back(v2);
			return true;
		}

		// 一个拐角，对角可直连
		auto p = Vec2(v1.x, v2.y);
		if (_map[static_cast<int>(p.x)][static_cast<int>(p.y)] == 0) {
			if (linkD(v1, p) && linkD(p, v2)) {
				_path.push_back(v1);
				_path.push_back(p);
				_path.push_back(v2);
				return true;
			}
		}

		// 一个拐角，对角可直连
		p = Vec2(v2.x, v1.y);
		if (_map[static_cast<int>(p.x)][static_cast<int>(p.y)] == 0) {
			if (linkD(v1, p) && linkD(p, v2)) {
				_path.push_back(v1);
				_path.push_back(p);
				_path.push_back(v2);
				return true;
			}
		}

		// X扩展，判断是否有可直连的点
		expandX(v1, _vecExpand1);
		expandX(v2, _vecExpand2);

		for (auto pt1 : _vecExpand1) {
			for (auto pt2 : _vecExpand2) {
				if (pt1.x == pt2.x) {
					if (linkD(pt1, pt2)) {
						_path.push_back(v1);
						_path.push_back(pt1);
						_path.push_back(pt2);
						_path.push_back(v2);
						return true;
					}
				}
			}
		}

		// Y扩展，判断是否有可直连的点
		expandY(v1, _vecExpand1);
		expandY(v2, _vecExpand2);
		for (auto pt1 : _vecExpand1) {
			for (auto pt2 : _vecExpand2) {
				if (pt1.y == pt2.y) {
					if (linkD(pt1, pt2)) {
						_path.push_back(v1);
						_path.push_back(pt1);
						_path.push_back(pt2);
						_path.push_back(v2);
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool GameLayer::linkD(Vec2 v1, Vec2 v2)
{
	// X坐标一致，逐个扫描Y坐标，中间如果都是通路，那么可以直连
	if (v1.x == v2.x) {
		int y1 = MIN(v1.y, v2.y);
		int y2 = MAX(v2.y, v2.y);
		bool flag = true;
		for (int y = y1 + 1; y < y2; y++) {
			if (_map[static_cast<int>(v1.x)][y] != 0) {
				flag = false;
				break;
			}
		}

		return flag;
	}

	// Y坐标一致，逐个扫描X坐标，中间如果都是通路，那么可以直连
	if (v1.y == v2.y) {
		int x1 = MIN(v1.x, v2.x);
		int x2 = MAX(v1.x, v2.x);
		bool flag = true;
		for (int x = x1 + 1; x < x2; x++) {
			if (_map[x][static_cast<int>(v1.y)] != 0) {
				flag = false;
				break;
			}
		}

		return flag;
	}
	return false;
}

void GameLayer::expandX(Vec2 v, std::vector<Vec2> &vector)
{
	vector.clear();
	for (int x = static_cast<int>(v.x) + 1; x < xCount; x++) {
		if (_map[x][static_cast<int>(v.y)] != 0) {
			break;
		}
		vector.push_back(Vec2(x, v.y));
	}
	for (int x = static_cast<int>(v.x) - 1; x >= 0; x--) {
		if (_map[x][static_cast<int>(v.y)] != 0) {
			break;
		}
		vector.push_back(Vec2(x, v.y));
	}
}

void GameLayer::expandY(Vec2 v, std::vector<Vec2> &vector)
{
	vector.clear();
	for (int y = static_cast<int>(v.y) + 1; y < yCount; y++) {
		if (_map[static_cast<int>(v.x)][y] != 0) {
			break;
		}
		vector.push_back(Vec2(v.x, y));
	}
	for (int y = static_cast<int>(v.y) - 1; y >= 0; y--) {
		if (_map[static_cast<int>(v.x)][y] != 0) {
			break;
		}
		vector.push_back(Vec2(v.x, y));
	}
}