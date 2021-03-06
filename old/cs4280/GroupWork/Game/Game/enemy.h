#ifndef __ENEMY_H
#define __ENEMY_H

/*
	ENEMY.H

	The CEnemy class

	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 3/30/2001
	Description: The CEnemy class

*/

#include "entity.h"
// Phase 15 - Comment out the following
// Phase 19 - Uncomment the following
#include "audiosystem.h"
#include "vector.h"

class CPlayer;
class CTerrain;

enum AIState_t
{
	AI_UNCARING,	// enemy is not scared and does not care
	AI_SCARED,	// enemy is scared and running away
	AI_DEAD
};

class CEnemy : public CEntity
{
private:

protected:
     int hitPoints;           // hit points the enemy has left
     float distFromPlayer;    // distance this enemy is from player
     float runSpeed;          // speed of enemy when running
     AIState_t aiState;       // state of enemy thought

     virtual void OnProcessAI() {}

     void OnCollision(CObject *collisionObject)
     {
          // if this enemy collides with another enemy
          if (typeid(*collisionObject) == typeid(CEnemy))
          {
               modelState = MODEL_IDLE;
               velocity = CVector(0.0, 0.0, 0.0);
          }
          // if this enemy collides with the terrain (always)
          else if (typeid(*collisionObject) == typeid(CTerrain))
          {
               position.y = ((CTerrain*)collisionObject)->GetHeight(position.x, position.z) + size;
          }
          else
          {
          }
     }

public:
     CPlayer *player;

     CEnemy() 
     {
          hitPoints = 100;    // start with 100 hit points
          isDead = false;     // enemy starts off alive
          velocity = CVector(0.0, 0.0, 0.0); // speed of enemy
          runSpeed = velocity.z;
          SetState(MODEL_IDLE);    // idle state
          direction = 0.0f;        // point north
          player = NULL;
     }

     ~CEnemy() {}

     void ProcessAI() { OnProcessAI(); }
     void SetPlayer(CPlayer *p) { player = p; }
};

#endif