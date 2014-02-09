//
//  TitleScene.h
//  TileGame
//
//  Created by 17age forever on 2/8/14.
//
//

#ifndef __TileGame__TitleScene__
#define __TileGame__TitleScene__

#include "cocos2d.h"
#include "GameScene.h"
#include "OptionScene.h"

class Title : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // menu callback
    void menuStartCallback(CCObject* pSender);
    void menuOptionCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Title);
};

#endif /* defined(__TileGame__TitleScene__) */
