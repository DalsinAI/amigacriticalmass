// Description:
//   Stage manager controls movement from levelpack to levelpack and 
//   level to level.
//
// Copyright (C) 2001 Frank Becker
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation;  either version 2 of the License,  or (at your option) any  later
// version.
//
// This program is distributed in the hope that it will be useful,  but  WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details
//
#include <Trace.hpp>
#include <GameState.hpp>
#include <StageManager.hpp>
#include <EnemyFactory.hpp>
#include <Skill.hpp>
#include <XMLHelper.hpp>

#include <Random.hpp>
#include <ParticleInfo.hpp>
#include <Particles.hpp>
#include <ParticleGroup.hpp>
#include <ParticleGroupManager.hpp>

bool StageManager::init( void)
{
    return findLevelPacks();
}

void StageManager::reset( void)
{
    //catch used skill changes via runtime config
    SkillS::instance()->updateSkill();

    _levelPackIterator = _levelPackList.begin();
    loadNextLevelPack();
    activateLevel();
}

void StageManager::update( void)
{
    static ParticleGroup *bonus =
	ParticleGroupManagerS::instance()->getParticleGroup( BONUS_GROUP);

    //no armor piercing needed for rookie
    if( (GameState::skill != Skill::eRookie) &&
        (HeroS::instance()->getArmorPierce() <= 1.0f))
    {
	if( Random::rangef0_1() < 0.001f)
	{
	    LOG_INFO << "ArmorPierce" << endl;
	    float posX = (Random::rangef0_1()-0.5f) * 60.0f;
	    bonus->newParticle( "ArmorPierce", posX, 49.0f, -100.0f);
	}
    }

#if 0
    if( Random::rangef0_1() < 0.001f)
    {
	LOG_INFO << "WeaponUpgrade" << endl;
	float posX = (Random::rangef0_1()-0.5f) * 60.0f;
	bonus->newParticle( "WeaponUpgrade", posX, 49.0f, -100.0f);
    }
#endif

    if( GameState::numObjects == 0)
    {
	_activeLevel = _activeLevel->NextSibling();
	if( ! _activeLevel)
	{
	    loadNextLevelPack();
	}
	activateLevel();
    }
}

bool StageManager::findLevelPacks( void)
{
    list<string> rList;
    ResourceManagerS::instance()->getResourceList( rList);

    list<string>::iterator i;
    for( i=rList.begin(); i!=rList.end(); i++)
    {
	string resourceName = *i;

	string::size_type end = resourceName.length()-8; // strlen("Pack.xml")
	string::size_type find = resourceName.find("Pack.xml");
	if( (find!=string::npos) && (find==end))
	{
	    LOG_INFO << "Adding LevelPack [" << resourceName << "]" << endl; 
	    _levelPackList.insert( _levelPackList.end(), resourceName);
	}
    }


    _levelPackIterator = _levelPackList.begin();

    bool foundSome = _levelPackIterator != _levelPackList.end();
    if( !foundSome)
    {
	LOG_WARNING << "No levelpacks found in resource file!" << endl; 
	string defaultLevelPack = "levelpacks/CritterPack.xml";
	_levelPackList.insert( _levelPackList.end(), defaultLevelPack);
    }

    return true;
}

bool StageManager::loadNextLevelPack( void)
{
    XTRACE();

    if( _levelPackIterator == _levelPackList.end())
    {
	_levelPackIterator = _levelPackList.begin();
	//when we wrap around, increment skill
	SkillS::instance()->incrementSkill();
    }

    delete _activeLevelPack;

    string levelPackName = *_levelPackIterator;
    _activeLevelPack = XMLHelper::load( levelPackName);

    if( !_activeLevelPack)
    {
	_activeLevel = 0;
	return false;
    }

    TiXmlNode *levelPack = _activeLevelPack->FirstChild("LevelPack");
    _activeLevel = levelPack->ToElement()->FirstChild();

    //advance to next level pack
    _levelPackIterator++;

    if( !_activeLevelPack)
    {
	LOG_ERROR << "No level pack found!" << endl; 
	return false;
    }

    return true;
}

bool StageManager::activateLevel( void)
{
    XTRACE();

    if( !_activeLevel)
    {
	return false;
    }
#if 0
    //catch used skill changes via runtime config
    SkillS::instance()->updateSkill();
#endif

    TiXmlElement* levelNode = _activeLevel->ToElement();
    _activeLevelName = *levelNode->Attribute("Name");
    LOG_INFO << "Level '" << _activeLevelName 
             << "' by " << *levelNode->Attribute("Author") << endl; 

    static ParticleGroup *effects = 
	ParticleGroupManagerS::instance()->getParticleGroup( EFFECTS_GROUP2);
    ParticleInfo pi;
    pi.position.x =  0.0;
    pi.position.y =  0.0;
    pi.position.z =-50.0;
    pi.text = _activeLevelName;

    pi.color.x = 1.0;
    pi.color.y = 1.0;
    pi.color.z = 1.0;

    pi.extra.y = 0.1f;
    pi.extra.z = 0.1f;

    effects->newParticle( "StatusMessage", pi);

    GameState::numObjects = 0;
    _levelStartTime = GameState::stopwatch.getTime();

    TiXmlNode *enemyNode = _activeLevel->FirstChild();
    while( enemyNode)
    {
        EnemyFactory::createEnemy( enemyNode);
	enemyNode = enemyNode->NextSibling();
    }

    return true;
}
