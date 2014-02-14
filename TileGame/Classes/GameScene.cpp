#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "Enemy.h"

using namespace cocos2d;

CCScene* Game::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Game *layer = Game::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    HudLayer *hud = new HudLayer();
    hud->init();
    scene->addChild(hud);
    layer->_hud = hud;
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Game::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pickup.caf");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("hit.caf");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("move.caf");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("TileMap.caf");
    
    _tileMap = new CCTMXTiledMap();

    // Area and Stage
    int stage = 1;
    int area = 1;

    CCString* tileMapStr = CCString::createWithFormat("TileMap_%d_%d.tmx", stage, area);
    CCLOG("%s", tileMapStr->getCString());
    _tileMap->initWithTMXFile(tileMapStr->getCString());
    _background = _tileMap->layerNamed("Background");
    _foreground = _tileMap->layerNamed("Foreground");

    _meta = _tileMap->layerNamed("Meta");
    _meta->setVisible(false);
    
    this->addChild(_tileMap);
    
    CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Objects");
    
    if(objectGroup == NULL){
        CCLog("tile map has no objects object layer");
        return false;
    }
    
    CCPoint playerPoint = objectGroup->getPointByName("PlayerSpawnPoint");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Player.plist");
    _player = new CCSprite();
    _player->initWithSpriteFrameName("Player_right_1.png");
    _player->setPosition(playerPoint);
    
    _animationCache = CCAnimationCache::sharedAnimationCache();
    // TODO:
    Player *player = new Player();
    _animationCache = player->setupAnimations();
    this->addChild(_player);
    
    // TODO:
    // enemy
    CCPoint enemyPoint = objectGroup->getPointByName("EnemySpawnPoint");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Enemy.plist");
    _enemy = new CCSprite();
    _enemy->initWithSpriteFrameName("Enemy_right_1.png");
    _enemy->setPosition(enemyPoint);
    
    _animationCache = player->setupAnimations();
    this->addChild(_enemy);
    
    this->setViewPointCenter(_player->getPosition());
    
    this->setTouchEnabled(true);
    
    return true;
}

// TODO:
void Game::setupEnemyAnimations(const char* enemyName)
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
}

void Game::setViewPointCenter(CCPoint position)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);
    
    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
    this->setPosition(viewPoint);
}

#pragma mark - handle touches

void Game::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool Game::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    return true;
}

void Game::setPlayerPosition(CCPoint position)
{
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
    _player->setPosition(position);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("move.caf");
}

void Game::setTileEffect(CCPoint position)
{
    // TODO:
    // stage and area
    _hud->setStageLabel(1, 1);
    _hud->setLifeLabel(1);
    
    CCPoint tileCoord = this->tileCoordForPosition(position);
    
    CCPointArray *pointArray = CCPointArray::create(8);
    pointArray->addControlPoint(ccp(-1,1));
    pointArray->addControlPoint(ccp(0,1));
    pointArray->addControlPoint(ccp(1,1));
    pointArray->addControlPoint(ccp(-1,0));
    pointArray->addControlPoint(ccp(1,0));
    pointArray->addControlPoint(ccp(-1,-1));
    pointArray->addControlPoint(ccp(0,-1));
    pointArray->addControlPoint(ccp(1,-1));
    
    for(int i = 0; i < pointArray->count(); i++){
        CCPoint point = pointArray->getControlPointAtIndex(i);
        CCLog("(%f,%f)", point.x, point.y);
            
        CCPoint tmpTileCoord = ccp(tileCoord.x + point.x, tileCoord.y + point.y);
        if(tmpTileCoord.x < 0 || tmpTileCoord.x > _tileMap->getMapSize().width -1 ||
           tmpTileCoord.y < 0 || tmpTileCoord.y > _tileMap->getMapSize().height - 1)
        {
            continue;
        }
        CCLog("(%f,%f)", tmpTileCoord.x, tmpTileCoord.y);
        CCLog("TileX %f, TileY %f", _tileMap->getMapSize().width, _tileMap->getMapSize().height);
        int tileGid = _meta->tileGIDAt(tmpTileCoord);
        
        if (tileGid) {
            CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
            if (properties) {
                // obstacle
                CCString *collision = new CCString();
                *collision = *properties->valueForKey("Collidable");
            
                if (collision && (collision->compare("True") == 0)) {
                    CCSprite *sprite = _background->tileAt(tmpTileCoord);
                    // red
                    sprite->setColor(ccc3(255, 90, 90));
                    //sprite->setOpacity(2);
                }
            }
        }
    }
    
    // 白に戻す
    CCPointArray *revertPointArray = CCPointArray::create(255);
    revertPointArray->addControlPoint(ccp(-2,2));
    revertPointArray->addControlPoint(ccp(-1,2));
    revertPointArray->addControlPoint(ccp(-0,2));
    revertPointArray->addControlPoint(ccp(1,2));
    revertPointArray->addControlPoint(ccp(2,2));
    revertPointArray->addControlPoint(ccp(-2,1));
    revertPointArray->addControlPoint(ccp(2,1));
    revertPointArray->addControlPoint(ccp(-2,0));
    revertPointArray->addControlPoint(ccp(2,0));
    revertPointArray->addControlPoint(ccp(-2,-1));
    revertPointArray->addControlPoint(ccp(2,-1));
    revertPointArray->addControlPoint(ccp(-2,-2));
    revertPointArray->addControlPoint(ccp(-1,-2));
    revertPointArray->addControlPoint(ccp(0,-2));
    revertPointArray->addControlPoint(ccp(1,-2));
    revertPointArray->addControlPoint(ccp(2,-2));
    
    for(int i = 0; i < revertPointArray->count(); i++){
        CCPoint point = revertPointArray->getControlPointAtIndex(i);
        CCLog("(%f,%f)", point.x, point.y);
        
        CCPoint tmpTileCoord = ccp(tileCoord.x + point.x, tileCoord.y + point.y);
        
        if(tmpTileCoord.x < 0 || tmpTileCoord.x > _tileMap->getMapSize().width - 1||
           tmpTileCoord.y < 0 || tmpTileCoord.y > _tileMap->getMapSize().height - 1)
        {
            continue;
        }
        CCLog("(%f,%f)", tmpTileCoord.x, tmpTileCoord.y);
        int tileGid = _meta->tileGIDAt(tmpTileCoord);
        
        if (tileGid) {
            CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
            if (properties) {
                // obstacle
                CCString *collision = new CCString();
                *collision = *properties->valueForKey("Collidable");
                
                if (collision && (collision->compare("True") == 0)) {
                    CCSprite *sprite = _background->tileAt(tmpTileCoord);
                    // white
                    sprite->setColor(ccc3(255, 255, 255));
                    //sprite->setOpacity(2);
                }
            }
        }
    }
    
}

void Game::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);
    
    CCPoint playerPos = _player->getPosition();
    CCPoint diff = ccpSub(touchLocation, playerPos);
    _player->stopAllActions();
    
    if ( abs(diff.x) > abs(diff.y) ) {
        if (diff.x > 0) {
            // right
            runPlayerAcrion("RIGHT");
            //_player->setFlipX(false);
            playerPos.x += _tileMap->getTileSize().width;
        } else {
            // left
            runPlayerAcrion("LEFT");
            //_player->setFlipX(true);
            playerPos.x -= _tileMap->getTileSize().width;
        }
    } else {
        if (diff.y > 0) {
            // up
            // BACK
            runPlayerAcrion("BACK");
            playerPos.y += _tileMap->getTileSize().height;
        } else {
            // down
            // FRONT
            runPlayerAcrion("FRONT");
            playerPos.y -= _tileMap->getTileSize().height;
        }
    }
    
    // safety check on the bounds of the map
    if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        playerPos.y >= 0 &&
        playerPos.x >= 0 )
    {
        this->setPlayerPosition(playerPos);
        
        // not hit only
        this->setTileEffect(playerPos);
    }
    
    this->setViewPointCenter(_player->getPosition());
}

void Game::runPlayerAcrion(const char* name)
{
    CCAnimation *pAnimation = _animationCache->animationByName(name);
    CCRepeatForever *pAction = CCRepeatForever::create( CCAnimate::create(pAnimation) );
    _player->runAction(pAction);
}

CCPoint Game::tileCoordForPosition(CCPoint position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    return ccp(x, y);
}
