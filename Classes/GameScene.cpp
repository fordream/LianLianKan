#include "GameScene.h"
#include "BackGroundLayer.h"
USING_NS_CC;

bool GameScene::init()
{
	// �ȵ������init����
	// �������֪����init()������һ���̳������ĺ�����������һ���麯�������������д�������
	if (!Scene::init()) {
		return false;
	}
	_backgroundLayer = BackGroundLayer::create();

	// ��ӵ���ǰ��
	addChild(_backgroundLayer);


	return true;
}