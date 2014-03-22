//
//  Enemy.cpp
//  TileGame
//
//  Created by 臣輔 杉田 on 2/13/14.
//
//

#include "Enemy.h"
#include "SimpleAudioEngine.h"

Enemy::Enemy()
{
    
}

CCAnimationCache* Enemy::setupAnimations(const char* enemyName)
{
    CCAnimation *pAnimationFront = CCAnimation::create();
    CCAnimation *pAnimationBack  = CCAnimation::create();
    CCAnimation *pAnimationLeft  = CCAnimation::create();
    CCAnimation *pAnimationRight = CCAnimation::create();
    CCAnimation *pAnimationAttack = CCAnimation::create();
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame *frame;
    
    // 手前 FRONT
    CCString* tmpStr = CCString::createWithFormat("%s_front_1.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationFront->addSpriteFrame(frame);
    tmpStr = CCString::createWithFormat("%s_front_2.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationFront->addSpriteFrame(frame);
    
    // 上 BACK
    tmpStr = CCString::createWithFormat("%s_back_1.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationBack->addSpriteFrame(frame);
    tmpStr = CCString::createWithFormat("%s_back_2.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationBack->addSpriteFrame(frame);
    
    // 右
    tmpStr = CCString::createWithFormat("%s_right_1.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationRight->addSpriteFrame(frame);
    tmpStr = CCString::createWithFormat("%s_right_2.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationRight->addSpriteFrame(frame);
    
    // 左
    tmpStr = CCString::createWithFormat("%s_left_1.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationLeft->addSpriteFrame(frame);
    tmpStr = CCString::createWithFormat("%s_left_2.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationLeft->addSpriteFrame(frame);
    
    // 攻撃
    tmpStr = CCString::createWithFormat("%s_attack_1.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationAttack->addSpriteFrame(frame);
    tmpStr = CCString::createWithFormat("%s_attack_2.png", enemyName);
    frame = cache->spriteFrameByName(tmpStr->getCString());
    pAnimationAttack->addSpriteFrame(frame);
    
    pAnimationFront->setDelayPerUnit(0.5f);
    pAnimationBack->setDelayPerUnit(0.5f);
    pAnimationLeft->setDelayPerUnit(0.5f);
    pAnimationRight->setDelayPerUnit(0.5f);
    pAnimationAttack->setDelayPerUnit(0.5f);
    
    CCAnimationCache *animationCache = CCAnimationCache::sharedAnimationCache();
    animationCache = CCAnimationCache::sharedAnimationCache();
    animationCache->addAnimation( pAnimationFront, "E_FRONT" );
    animationCache->addAnimation( pAnimationBack,  "E_BACK" );
    animationCache->addAnimation( pAnimationLeft,  "E_LEFT" );
    animationCache->addAnimation( pAnimationRight, "E_RIGHT" );
    animationCache->addAnimation( pAnimationAttack, "E_ATTACK" );
    
    return animationCache;
}

void Enemy::finishAnimation()
{
    _isMoveable = true;
}

void Enemy::runAnimation(const char* name)
{
    CCAnimationCache *animationCache = CCAnimationCache::sharedAnimationCache();
    CCAnimation *pAnimation = animationCache->animationByName(name);
    CCRepeatForever *pAction = CCRepeatForever::create( CCAnimate::create(pAnimation) );
    this->runAction(pAction);
}

//void Enemy::randomWalk(CCTMXTiledMap *tileMap)
void Enemy::randomWalk(CCTMXLayer *meta, CCTMXTiledMap *tileMap)
{
    if(!_isMoveable) return;

    _isMoveable = false;
    float randomVerticalOrHorizon = CCRANDOM_0_1();
    
    float randomX = CCRANDOM_0_1();
    float randomY = CCRANDOM_0_1();
    CCPoint nowPos = this->getPosition();
    int tileWidth = tileMap->getTileSize().width;
    int tileHeight = tileMap->getTileSize().height;
    CCPoint newPos = ccp(nowPos.x + randomX, nowPos.y + randomY);
    CCPoint tileCoord = this->tileCoordForPosition(tileMap, newPos);

    int tileGid = meta->tileGIDAt(tileCoord);

    if (tileGid) {
        // TODO: "TMXLayer: invalid position"
        // error
//        CCDictionary *properties = tileMap->propertiesForGID(tileGid);
        /*
        if (properties) {
            // obstacle
            CCString *collision = new CCString();
            *collision = *properties->valueForKey("Collidable");

            // item get
            CCString *collectible = new CCString();
            
            if ((collision && (collision->compare("True") == 0)) ||
                (collectible && (collectible->compare("True") == 0))) {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("hit.caf");
                return;
            }
        }
        */
    }

    // "E_ATTACK"
    
    if(randomVerticalOrHorizon > 0.5f) {
        // horizon
        if(randomX > 0.5f) {
            newPos = ccp(nowPos.x + tileWidth, nowPos.y);
            this->runAnimation("E_LEFT");
        } else {
            newPos = ccp(nowPos.x + -tileWidth, nowPos.y);
            this->runAnimation("E_RIGHT");
        }
    } else {
        // vertical
        if(randomY > 0.5f) {
            newPos = ccp(nowPos.x, nowPos.y + tileHeight);
            this->runAnimation("E_FRONT");
        } else {
            newPos = ccp(nowPos.x, nowPos.y + tileHeight);
            this->runAnimation("E_BACK");
        }
    }

    CCMoveTo* move = CCMoveTo::create(SPEED, newPos);
    CCCallFuncN *func = CCCallFuncN::create(this, callfuncN_selector(Enemy::finishAnimation));
    CCFiniteTimeAction* sequence = CCSequence::create(move, func, NULL);

    if (newPos.x <= (tileMap->getMapSize().width * tileMap->getTileSize().width) &&
        newPos.y <= (tileMap->getMapSize().height * tileMap->getTileSize().height) &&
        newPos.y >= 0 &&
        newPos.x >= 0 )
    {
        this->runAction(sequence);
    }
}

// Move common?
CCPoint Enemy::tileCoordForPosition(CCTMXTiledMap *tileMap, CCPoint position)
{
    int x = position.x / tileMap->getTileSize().width;
    int y = ((tileMap->getMapSize().height * tileMap->getTileSize().height) - position.y) / tileMap->getTileSize().height;
    return ccp(x, y);
}