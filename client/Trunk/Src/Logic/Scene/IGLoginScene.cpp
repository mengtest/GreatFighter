#include "IGLoginScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;


IGLoginScene::~IGLoginScene()
{
	delete m_loginManager;
	m_loginManager = nullptr;
}

Scene* IGLoginScene::createScene()
{
    auto scene = Scene::create();

    auto layer = IGLoginScene::create();

    scene->addChild(layer);

    return scene;
}

bool IGLoginScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

	m_loginUI = IGLoginUI::create();
	addChild(m_loginUI);

	m_loginManager = new IGLoginManager();

    return true;
}

void IGLoginScene::onEnter()
{
	Scene::onEnter();

	m_loginManager->onEnter();
}

void IGLoginScene::onExit()
{
	Scene::onExit();

	m_loginManager->onExit();
}
