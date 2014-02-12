#include "HudLayer.h"

using namespace cocos2d;

bool HudLayer::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        // collected
        _collectedLabel = new CCLabelTTF();
        _collectedLabel->initWithString("0", "Verdana-Bold", 18.0);
        _collectedLabel->setColor(ccc3(0,0,0));
        
        int collectedMargin = 10;
        _collectedLabel->setPosition(ccp(winSize.width - (_collectedLabel->getContentSize().width/2) - collectedMargin, _collectedLabel->getContentSize().height/2 + collectedMargin));
        this->addChild(_collectedLabel);
        
        // area-stage
        _stageLabel = new CCLabelTTF();
        _stageLabel->initWithString("0", "Verdana-Bold", 18.0);
        _stageLabel->setColor(ccc3(0,0,0));

        int margin = 10;
        _stageLabel->setPosition(ccp(winSize.width - (_stageLabel->getContentSize().width/2) - margin, _stageLabel->getContentSize().height/2 + margin));
        this->addChild(_stageLabel);
        
        // life
        _lifeLabel = new CCLabelTTF();
        _lifeLabel->initWithString("0", "Verdana-Bold", 18.0);
        _lifeLabel->setColor(ccc3(0,0,0));

        _lifeLabel->setPosition(ccp(winSize.width - (_lifeLabel->getContentSize().width/2) - margin, _lifeLabel->getContentSize().height/2 + margin));
        this->addChild(_lifeLabel);
    }
    
    return true;
}

void HudLayer::numCollectedChanged(int numCollected)
{
    CCString *labelCollected = new CCString();
    labelCollected->initWithFormat("%d", numCollected);
    _collectedLabel->setString(labelCollected->getCString());
}

void HudLayer::setStageLabel(int area, int stage)
{
    CCString *labelStage = new CCString();
    labelStage->initWithFormat("%d-%d", area, stage);
    _stageLabel->setString(labelStage->getCString());
}

void HudLayer::setLifeLabel(int life)
{
    CCString *labelLife = new CCString();
    labelLife->initWithFormat("LIFE x %d", life);
    _stageLabel->setString(labelLife->getCString());
}