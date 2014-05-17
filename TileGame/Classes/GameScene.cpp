#include "GameScene.h"
#include "SimpleAudioEngine.h"

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
    
    this->scheduleUpdate();
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
    _player = new Player();
    _player->initWithSpriteFrameName("Player_right_1.png");
    _player->setPosition(playerPoint);
    
    _animationCache = CCAnimationCache::sharedAnimationCache();
    _animationCache = _player->setupAnimations();
    this->addChild(_player);
    
    // enemy
    CCPoint enemyPoint = objectGroup->getPointByName("EnemySpawnPoint");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Enemy.plist");
    _enemy = new Enemy();
    _enemy->initWithSpriteFrameName("Enemy_right_1.png");
    _enemy->setPosition(enemyPoint);
    
    _animationCache = _enemy->setupAnimations("Enemy");
    this->addChild(_enemy);
    
    this->setViewPointCenter(_player->getPosition());
    
    this->setTouchEnabled(true);
    
    return true;
}

//cpp内
void Game::update(float delta)
{
    // ここに記入されたモノを、定期的に呼び出す
    _enemy->randomWalk(_meta, _tileMap);
    
    // map moving
    this->setViewPointCenter(_player->getPosition());
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

// do not move to Player.h
void Game::setPlayerPosition(CCPoint position, CCFiniteTimeAction* sequence)
{
    CCPoint tileCoord = this->tileCoordForPosition(position);
    int tileGid = _meta->tileGIDAt(tileCoord);
    
    if (tileGid) {
        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
        if (properties) {
            // obstacle
            CCString *collision = new CCString();
            *collision = *properties->valueForKey("Collidable");
            // Moveable
            CCString *move = new CCString();
            *move = *properties->valueForKey("Moveable");
            // Breakable
            CCString *breakable = new CCString();
            *breakable = *properties->valueForKey("Breakable");
            
            if ((collision && collision->compare("True") == 0) ||
                 (move && move->compare("True") == 0) ||
                 (breakable && breakable->compare("True") == 0)) {
                // 動けない音を出す
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("hit.caf");
                this->finishAnimation();
                CCLog("Can't move");
                return;
            }
            
            // item get
            CCString *collectable = new CCString();
            
            *collectable = *properties->valueForKey("Collectable");
            if (collectable && (collectable->compare("True") == 0)) {
                // 取り除く
                _meta->removeTileAt(tileCoord);
                _foreground->removeTileAt(tileCoord);
                _numCollected++;
                _hud->numCollectedChanged(_numCollected);
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pickup.caf");
            }
        }
    }
    
    _player->runAction(sequence);
    // not hit only　赤にする
    this->setTileEffect(position);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("move.caf");
}

// 赤をタップした？
void Game::attackBlock(CCPoint point)
{
    CCPoint tileCoord = this->tileCoordForPosition(point);;
    
    int tileGid = _meta->tileGIDAt(tileCoord);
    if (tileGid) {
        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
        if (properties) {
            // obstacle
            CCString *collision = new CCString();
            *collision = *properties->valueForKey("Collidable");
            
            // Moveable
            //CCString *move = new CCString();
            //*move = *properties->valueForKey("Moveable");
            // Breakable
            CCString *breakable = new CCString();
            *breakable = *properties->valueForKey("Breakable");

            // 殴れる判定
            if (breakable && breakable->compare("True") == 0) {
            //if (collision && (collision->compare("True") == 0)) {
                CCLog("Attackable");
                // TODO:
                
                // ブロック消す
                //_meta->removeTileAt(tileCoord);
                //_foreground->removeTileAt(tileCoord);
                /*CCSprite *sprite = _background->tileAt(tileCoord);
                if(sprite->getTag() == TAPPABLE)
                {
                    CCLog("OKsssss");
                }*/
            }
        }
    }
}

void Game::block()
{
}
    
// タイルを連鎖で壊す
void Game::breakBlock(CCPoint point)
{
    
}

// TODO:
// Moveable(移動できる、分散)のタイル


// Breakable(破壊可能)のタイルを赤にする
void Game::setTileEffect(CCPoint position)
{
    // stage and area
    _hud->setStageLabel(1, 1);
    _hud->setLifeLabel(1);
    
    CCPoint tileCoord = this->tileCoordForPosition(position);
    // キャラクターの上下左右の座標
    CCPointArray *pointArray = CCPointArray::create(4);
    pointArray->addControlPoint(ccp(0,1));
    pointArray->addControlPoint(ccp(-1,0));
    pointArray->addControlPoint(ccp(1,0));
    pointArray->addControlPoint(ccp(0,-1));
    
    for(int i = 0; i < pointArray->count(); i++){
        CCPoint point = pointArray->getControlPointAtIndex(i);
        //CCLog("(%f,%f)", point.x, point.y);
            
        CCPoint tmpTileCoord = ccp(tileCoord.x + point.x, tileCoord.y + point.y);
        if(tmpTileCoord.x < 0 || tmpTileCoord.x > _tileMap->getMapSize().width -1 ||
           tmpTileCoord.y < 0 || tmpTileCoord.y > _tileMap->getMapSize().height - 1)
        {
            continue;
        }

        //CCLog("(%f,%f)", tmpTileCoord.x, tmpTileCoord.y);
        //CCLog("TileX %f, TileY %f", _tileMap->getMapSize().width, _tileMap->getMapSize().height);
        
        int tileGid = _meta->tileGIDAt(tmpTileCoord);
        
        if (tileGid) {
            CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
            if (properties) {
                // obstacle
                CCString *collision = new CCString();
                // Breakableを赤
                *collision = *properties->valueForKey("Breakable");
            
                if (collision && (collision->compare("True") == 0)) {
                    CCSprite *sprite = _background->tileAt(tmpTileCoord);
                    //sprite->setTag(TAPPABLE);
                    // red
                    sprite->setColor(ccRED);
                }
            }
        }
    }
    
    // 白に戻す
    // まず詰める
    /*
      ◯は赤なので変化させない
      |6,0|6,1|6,2|6,3|6,4|6,5|6,6|
      |5,0|5,1|5,2|5,3|5,4|5,5|5,6|
      |4,0|4,1|4,2|◯4,3|4,4|4,5|4,6|
      |3,0|3,1|◯3,2|3,3|◯3,4|3,5|3,6|
      |2,0|2,1|2,2|◯2,3|2,4|2,5|2,6|
      |1,0|1,1|1,2|1,3|1,4|1,5|1,6|
      |0,0|0,1|0,2|0,3|0,4|0,5|0,6|
    */
    CCPointArray *revertPointArray = CCPointArray::create(255);
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if((i == 3 && j == 2) || (i == 3 && j == 4) ||
               (i == 2 && j == 3) || (i == 4 && j == 3)
            )
            {
                continue;
            }
            revertPointArray->addControlPoint(ccp(-3 + j, -3 + i));
        }
    }

    // 詰めたものから適用
    for(int i = 0; i < revertPointArray->count(); i++) {
        CCPoint point = revertPointArray->getControlPointAtIndex(i);
        //CCLog("(%f,%f)", point.x, point.y);
        
        CCPoint tmpTileCoord = ccp(tileCoord.x + point.x, tileCoord.y + point.y);
        
        // 画面端はエラーになるので適用させない
        if(tmpTileCoord.x < 0 || tmpTileCoord.x > _tileMap->getMapSize().width - 1||
           tmpTileCoord.y < 0 || tmpTileCoord.y > _tileMap->getMapSize().height - 1)
        {
            continue;
        }
        
        //CCLog("(%f,%f)", tmpTileCoord.x, tmpTileCoord.y);
        
        int tileGid = _meta->tileGIDAt(tmpTileCoord);
        
        if (tileGid) {
            CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
            if (properties) {
                // obstacle
                CCString *collision = new CCString();
                // Breakableのみ戻す
                *collision = *properties->valueForKey("Breakable");
                
                if (collision && (collision->compare("True") == 0)) {
                    CCSprite *sprite = _background->tileAt(tmpTileCoord);
                    // 白で塗る
                    sprite->setColor(ccWHITE);
                    //sprite->setTag(NOT_TAPPABLE);
                }
            }
        }
    }
}

void Game::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    if(!_isMoveable) return;
    
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);
    
    // 赤をタップした？
    this->attackBlock(touchLocation);
    
    CCPoint playerPos = _player->getPosition();
    CCPoint diff = ccpSub(touchLocation, playerPos);
    _player->stopAllActions();
    CCPoint newPos;
    
    if (abs(diff.x) > abs(diff.y)) {
        if (diff.x > 0) {
            // right
            _player->runAnimation("P_RIGHT");
            playerPos.x += _tileMap->getTileSize().width;
            _player->setDirection(Player::P_Right);
        } else {
            // left
            _player->runAnimation("P_LEFT");
            playerPos.x -= _tileMap->getTileSize().width;
            _player->setDirection(Player::P_Left);
        }
    } else {
        if (diff.y > 0) {
            // up
            // BACK
            _player->runAnimation("P_BACK");
            playerPos.y += _tileMap->getTileSize().height;
            _player->setDirection(Player::P_Back);
        } else {
            // down
            // FRONT
            _player->runAnimation("P_FRONT");
            playerPos.y -= _tileMap->getTileSize().height;
            _player->setDirection(Player::P_Front);
        }
    }
    
    CCMoveTo* move = CCMoveTo::create(SPEED, playerPos);
    CCCallFuncN *func = CCCallFuncN::create(this, callfuncN_selector(Game::finishAnimation));
    CCFiniteTimeAction* sequence = CCSequence::create(move, func, NULL);
    
    // safety check on the bounds of the map
    if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        playerPos.y >= 0 &&
        playerPos.x >= 0 )
    {
        _isMoveable = false;
        this->setPlayerPosition(playerPos, sequence);
    }
}

void Game::finishAnimation()
{
    _player->stopAllActions();
    
    // 上下左右で止まる
    switch(_player->getDirection()) {
        break; case Player::P_Right: { _player->initWithSpriteFrameName("Player_right_1.png"); }
        break; case Player::P_Left: { _player->initWithSpriteFrameName("Player_left_1.png"); }
        break; case Player::P_Back: { _player->initWithSpriteFrameName("Player_back_1.png"); }
        break; case Player::P_Front: { _player->initWithSpriteFrameName("Player_front_1.png"); }
        break; default: { _player->initWithSpriteFrameName("Player_right_1.png"); }
    }
    
    _isMoveable = true;
}

CCPoint Game::tileCoordForPosition(CCPoint position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    return ccp(x, y);
}
