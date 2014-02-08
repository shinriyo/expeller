//
//  OptionScene.h
//  TileGame
//
//  Created by 17age forever on 2/8/14.
//
//

#ifndef __TileGame__OptionScene__
#define __TileGame__OptionScene__

#include "cocos2d.h"
#include "TitleScene.h"

class Option : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    void menuTitleCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Option);
};

#endif /* defined(__TileGame__OptionScene__) */
