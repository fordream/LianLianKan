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
	//��Ļ����ת��Ϊ��ͼ����
	auto point = screen2Index(x, y);

	CCLOG("touch point index x:%d, y:%d", static_cast<int>(point.x), static_cast<int>(point.y));

	//�ж���ͨ���
	//��ǰλ���о���
	if (_map[(int)point.x][(int)point.y] > 0) {
		//��һ�ε���ľ��鲻�ǿ�
		if (!_prev.equals(Vec2::ZERO)) {
			CCLOG("compare point x:%d, y:%d", static_cast<int>(point.x), static_cast<int>(point.y));
			//���Ժ���ε���ľ�����ȥ
			if (link(_prev, point)) {
				CCLOG("path point count :%d", _path.size());
				int tag = genSpriteTag(point.x, point.y);
				auto selectedIcon = getChildByTag(tag);
				selectedIcon->setScale(1.2);
				selectedIcon->setLocalZOrder(101);

				drawLine();
			}
			//���ܺ���ε���ľ�����ȥ
			else{
				//ԭ���ı�С
				int tag = genSpriteTag(_prev.x, _prev.y);
				auto selectedIcon = getChildByTag(tag);
				// �ָ�ԭ��С
				selectedIcon->setScale(1.0);
				// �ָ�ԭZ��
				selectedIcon->setLocalZOrder(100);

				//��ǰ�ı��
				tag = genSpriteTag(point.x, point.y);
				selectedIcon = getChildByTag(tag);
				// �Ŵ�1.2��
				selectedIcon->setScale(1.2);
				// Z����ǰ���������о���ǰ��
				selectedIcon->setLocalZOrder(101);

				//ԭ���ĵ������ڵ�
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

	// ����
	if (_path.size() >= 2) {
		Vec2 *vecs = new Vec2[_path.size()];
		for (int i = 0; i < _path.size(); i++) {
			vecs[i] = index2Screen(_path.at(i).x, _path.at(i).y);
		}

		// ���������һ����ɫ
		auto color = CCRANDOM_0_1();

		// ѭ�����߶Σ�ֻ���߶ο��������������
		for (int i = 0; i < _path.size(); i++) {
			if (i > 0) {
				_draw->drawSegment(vecs[i - 1], vecs[i], 5, Color4F(color, color, color, 1));
			}
		}

		// ��ͨ������ͼ���Ӧ�ĵ�ͼ������0
		Vec2 p1 = _path.front();
		_map[(int)p1.x][(int)p1.y] = 0;
		Vec2 p2 = _path.back();
		_map[(int)p2.x][(int)p2.y] = 0;

		// ɾ���߶εĶ�������
		delete[] vecs;

		// �����ͨ��ͼ�꣬ͬʱ���·����
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
	// ��ǰ����
	int current = _progress->getPercentage();
	// Ϊ���gameover
	if (current == 0) {
		// game over
		gameOver("Game Over");
		return;
	}
	else{
		// ��һ
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

	//ʱ����������
	auto progressFrame = Sprite::createWithTexture(textureCache->getTextureForKey(s_time_slot));
	progressFrame->setAnchorPoint(Vec2(0, 0));
	progressFrame->setPosition(120, visibleSize.height - 50);
	addChild(progressFrame);


	//ʱ��������
	auto sprite = Sprite::createWithTexture(textureCache->getTextureForKey(s_time_bars));
	_progress = ProgressTimer::create(sprite);
	_progress->setAnchorPoint(Vec2(0, 0));
	//���ͣ�����
	_progress->setType(ProgressTimer::Type::BAR);
	_progress->setPosition(120, visibleSize.height - 50);
	// ˮƽ�仯
	_progress->setMidpoint(Vec2(0, 0));
	// һ��һ����λ
	_progress->setBarChangeRate(Vec2(1, 0));
	// ��ʼ100
	_progress->setPercentage(100);
	addChild(_progress);

	// ʱ������
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
	// ���Ҷ����Ķ���
	if (isTipable){
		isTipable = false;
		auto rote = RotateBy::create(0.05, 20);
		auto seq = Sequence::create(rote, rote->reverse(), rote->reverse(), rote->clone(), 
			CallFunc::create([&]{isTipable = true; }), nullptr);
		((Sprite*)spender)->runAction(seq);
		//��isDead�����У����Ѿ��������·��
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
	// ����x�������Ƶ�ͼ�����ÿ���ֵ��y�����������η�ת����ÿ��������ͬһ��ֵ
	int x = 1;
	int y = 0;
	// �����±��0��ʼ�������1��ʼ��������ô������һȦ����0����
	for (int i = 1; i < xCount - 1; i++) {
		for (int j = 1; j < yCount - 1; j++) {
			// ��ͼ���鸳ֵ
			_map[i][j] = x;
			// y���Ʒ�ת��x����ÿ��ֵ���ӣ�����ͼƬ���������1��ʼ����
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
	// �������
	srand((unsigned int)time(NULL));

	// ��ʱ������������������λ�õ�����
	int tempX, tempY, tempM;

	// ������ͼ���飬�������λ��
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
	// ���������Ͼ���
	for (int x = 0; x < xCount; x++) {
		for (int y = 0; y < yCount; y++) {
			if (_map[x][y] > 0) {
				char iconName[64];
				// ��ʽ��ͼƬ��
				sprintf(iconName, "%d.png", _map[x][y]);
				auto position = index2Screen(x, y);
				// ����ͼƬ�Ѿ��ӵ�֡���棬����ֱ��������ȡһ֡������һ������
				auto icon = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(iconName));
				//icon->setAnchorPoint(Vec2(1, 1));
				icon->setPosition(position);
				// ����һ��tag���������ʶ���������Ǹ�ͼ��
				int tag = genSpriteTag(x, y);
				// ����Z���tagֵ��Z�����ô�һЩȷ��ͼ�������������ǰ��
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
	// ֹͣupdate�������ȡ�
	unschedule(SEL_SCHEDULE(&GameLayer::update));

	// ����һ����ɫ��
	auto layer = LayerColor::create(Color4B(100, 100, 100, 100));
	// ê��Ĭ�������½�
	layer->setPosition(0, 0);
	addChild(layer, 1000);

	// �Ի��򱳾�
	auto sprite = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(s_game_dialog));
	sprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	layer->addChild(sprite);

	// Game Over ��ʾ
	auto titleSprite = Label::createWithSystemFont(title, "Thonburi", 35);
	titleSprite->setPosition(layer->getContentSize().width / 2, sprite->getContentSize().height - 100);
	titleSprite->setColor(Color3B::RED);
	layer->addChild(titleSprite);

	// Font Item
	MenuItemFont::setFontName("fonts/Marker Felt.ttf");
	// Play Again ��ť
	auto item1 = MenuItemFont::create("Play Again", CC_CALLBACK_0(GameLayer::playAgain, this));
	auto color_action = TintBy::create(0.5f, 0, -255, -255);
	auto color_back = color_action->reverse();
	auto seq = Sequence::create(color_action, color_back, nullptr);
	item1->runAction(RepeatForever::create(seq));

	auto menu = Menu::create(item1, nullptr);

	menu->setPosition(layer->getContentSize().width / 2, sprite->getContentSize().height / 2);

	layer->addChild(menu);

	// Game Over����Ӵ����¼�
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	// �̵������¼���������Ĳ㲻����Ӧ�˴δ���
	listener->setSwallowTouches(true);
	// Lambda���ʽʵ�ִ����������յģ���Ϊ����Ҫ������ʵ��
	listener->onTouchBegan = [](Touch*touch, Event* event){return true; };
	listener->onTouchMoved = [](Touch*touch, Event* event){};
	listener->onTouchEnded = [](Touch*touch, Event* event){};
	listener->onTouchCancelled = [](Touch*touch, Event* event){};
	// �ӵ�GameOver��
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
	// �������
	srand((unsigned int)time(NULL));

	// ��ʱ������������������λ�õ�����
	int tempX, tempY, tempM;

	// ������ͼ���飬�������λ��
	for (int x = 1; x < xCount - 1; x++){
		for (int y = 1; y < yCount - 1; y++) {
			tempX = 1 + (int)(CCRANDOM_0_1() * (xCount - 2));
			tempY = 1 + (int)(CCRANDOM_0_1() * (yCount - 2));
			tempM = _map[x][y];
			_map[x][y] = _map[tempX][tempY];
			_map[tempX][tempY] = tempM;


			// ��������λ�ã�����tagֵ
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
				// ���Ϊ�գ���ͼ�����ӦֵҪ��0
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
	// ·���㼯����һ��vector
	_path.clear();

	// �жϵ�����������ǲ���ͼ��һ��
	if (_map[static_cast<int>(v1.x)][static_cast<int>(v1.y)] == _map[static_cast<int>(v2.x)][static_cast<int>(v2.y)]) {
		// ֱ��
		if (linkD(v1, v2)) {
			_path.push_back(v1);
			_path.push_back(v2);
			return true;
		}

		// һ���սǣ��Խǿ�ֱ��
		auto p = Vec2(v1.x, v2.y);
		if (_map[static_cast<int>(p.x)][static_cast<int>(p.y)] == 0) {
			if (linkD(v1, p) && linkD(p, v2)) {
				_path.push_back(v1);
				_path.push_back(p);
				_path.push_back(v2);
				return true;
			}
		}

		// һ���սǣ��Խǿ�ֱ��
		p = Vec2(v2.x, v1.y);
		if (_map[static_cast<int>(p.x)][static_cast<int>(p.y)] == 0) {
			if (linkD(v1, p) && linkD(p, v2)) {
				_path.push_back(v1);
				_path.push_back(p);
				_path.push_back(v2);
				return true;
			}
		}

		// X��չ���ж��Ƿ��п�ֱ���ĵ�
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

		// Y��չ���ж��Ƿ��п�ֱ���ĵ�
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
	// X����һ�£����ɨ��Y���꣬�м��������ͨ·����ô����ֱ��
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

	// Y����һ�£����ɨ��X���꣬�м��������ͨ·����ô����ֱ��
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