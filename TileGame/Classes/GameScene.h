#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include "HudLayer.h"

using namespace cocos2d;

class Game : public cocos2d::CCLayer
{
    virtual void update(float delta);
private:
    CCTMXTiledMap *_tileMap;
    
    CCTMXLayer *_background;
    
    Player *_player;
    
    // TODO: array?
    Enemy *_enemy;
    
    CCTMXLayer *_meta;
    
    CCTMXLayer *_foreground;
    
    HudLayer *_hud;
    
    int _numCollected;
    /*const int NOT_TAPPABLE = 0;
    const int TAPPABLE = 1;*/
    CCAnimationCache *_animationCache;

    bool _isMoveable = true;
    void finishAnimation();
    float const SPEED = 0.3f;
    
    void attackBlock(CCPoint point);
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    void setViewPointCenter(CCPoint position);
    
    void registerWithTouchDispatcher();
    
//    void setPlayerPosition(CCPoint position);
    void setPlayerPosition(CCPoint position, CCFiniteTimeAction* sequence);

    void setTileEffect(CCPoint position);
    bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    
    void ccTouchEnded(CCTouch *touch, CCEvent *event);
    
    CCPoint tileCoordForPosition(CCPoint position);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Game);
};

#endif // __Game_SCENE_H__
