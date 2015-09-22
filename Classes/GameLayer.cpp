#include "GameLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
bool GameLayer::init()
{
	if (!LayerColor::init()) {
		return false;
	}
	scheduleOnce(SEL_SCHEDULE(&GameLayer::playBgMusic), 0);

	initUI();

	initMap();
	return true;
}
bool GameLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){ 
	auto point = touch->getLocation();
	CCLOG("Location point x=%f, y=%f", point.x, point.y);
	return true;
}
void GameLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event){}
void GameLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){}
void GameLayer::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event){}


void GameLayer::update(float dt){}
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
void GameLayer::autoClear(cocos2d::Ref*){

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
	int ix = ceil(x / iconSize);
	int iy = ceil(y / iconSize);

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