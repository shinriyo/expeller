//
//  Player.cpp
//  TileGame
//
//  Created by 臣輔 杉田 on 2/13/14.
//
//

#include "Player.h"
#include "SimpleAudioEngine.h"

CCAnimationCache* Player::setupAnimations()
{
    CCAnimation *pAnimationFront = CCAnimation::create();
    CCAnimation *pAnimationBack  = CCAnimation::create();
    CCAnimation *pAnimationLeft  = CCAnimation::create();
    CCAnimation *pAnimationRight = CCAnimation::create();
    CCAnimation *pAnimationAttack = CCAnimation::create();
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame *frame;
    
    // 手前 FRONT
    frame = cache->spriteFrameByName("Player_front_1.png");
    pAnimationFront->addSpriteFrame(frame);
    frame = cache->spriteFrameByName("Player_front_2.png");
    pAnimationFront->addSpriteFrame(frame);
    
    // 上 BACK
    frame = cache->spriteFrameByName("Player_back_1.png");
    pAnimationBack->addSpriteFrame(frame);
    frame = cache->spriteFrameByName("Player_back_2.png");
    pAnimationBack->addSpriteFrame(frame);
    
    // 右
    frame = cache->spriteFrameByName("Player_right_1.png");
    pAnimationRight->addSpriteFrame(frame);
    frame = cache->spriteFrameByName("Player_right_2.png");
    pAnimationRight->addSpriteFrame(frame);
    
    // 左
    frame = cache->spriteFrameByName("Player_left_1.png");
    pAnimationLeft->addSpriteFrame(frame);
    frame = cache->spriteFrameByName("Player_left_2.png");
    pAnimationLeft->addSpriteFrame(frame);
    
    // 攻撃
    frame = cache->spriteFrameByName("Player_attack_1.png");
    pAnimationAttack->addSpriteFrame(frame);
    frame = cache->spriteFrameByName("Player_attack_2.png");
    pAnimationAttack->addSpriteFrame(frame);
    
    pAnimationFront->setDelayPerUnit(0.5f);
    pAnimationBack->setDelayPerUnit(0.5f);
    pAnimationLeft->setDelayPerUnit(0.5f);
    pAnimationRight->setDelayPerUnit(0.5f);
    pAnimationAttack->setDelayPerUnit(0.5f);

    CCAnimationCache *animationCache = CCAnimationCache::sharedAnimationCache();
    animationCache->addAnimation( pAnimationFront, "P_FRONT" );
    animationCache->addAnimation( pAnimationBack,  "P_BACK" );
    animationCache->addAnimation( pAnimationLeft,  "P_LEFT" );
    animationCache->addAnimation( pAnimationRight, "P_RIGHT" );
    animationCache->addAnimation( pAnimationAttack, "P_ATTACK" );

    return animationCache;
}

// TODO:
void Player::runAnimation(const char* name)
{
    CCAnimationCache *animationCache = CCAnimationCache::sharedAnimationCache();
    CCAnimation *pAnimation = animationCache->animationByName(name);
    CCRepeatForever *pAction = CCRepeatForever::create( CCAnimate::create(pAnimation) );
    this->runAction(pAction);
}

// TODO:
void Player::setMoving(CCPoint position, CCFiniteTimeAction* sequence)
{
    /*
    CCPoint tileCoord = this->tileCoordForPosition(position);
    int tileGid = _meta->tileGIDAt(tileCoord);
    
    if (tileGid) {
        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
        if (properties) {
            // obstacle
            CCString *collision = new CCString();
            *collision = *properties->valueForKey("Collidable");
            
            if (collision && (collision->compare("True") == 0)) {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("hit.caf");
                this->finishAnimation();
                return;
            }
            
            // item get
            CCString *collectible = new CCString();
            
            *collectible = *properties->valueForKey("Collectable");
            if (collectible && (collectible->compare("True") == 0)) {
                _meta->removeTileAt(tileCoord);
                _foreground->removeTileAt(tileCoord);
                _numCollected++;
                _hud->numCollectedChanged(_numCollected);
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pickup.caf");
            }
        }
    }
    */
    this->runAction(sequence);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("move.caf");
}

void Player::finishAnimation()
{
    _isMoveable = true;
}