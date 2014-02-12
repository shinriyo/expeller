#ifndef __HUDLAYER_H__
#define __HUDLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

class HudLayer : public cocos2d::CCLayer
{
private:
    CCLabelTTF *_collectedLabel;
    CCLabelTTF *_stageLabel;
    CCLabelTTF *_lifeLabel;
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HudLayer);
    
    void numCollectedChanged(int numCollected);
    void setStageLabel(int area, int stage);
    void setLifeLabel(int life);
};

#endif // __HUDLAYER_H__