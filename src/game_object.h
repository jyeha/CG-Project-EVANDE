#pragma once
#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "btBulletDynamicsCommon.h"

typedef struct _game_object {
	mesh2*			gMesh;
	btRigidBody*	gBody;
	mat4			model_matrix = mat4::identity();
	bool			is_transparent = false;
	int				item_type = 0;							// 0: not item		1: aplus	2: coin
	int				obstacle_type = 0;						// 0: not obstacle	1: bus		2: carpet	3: soju
	bool			isCollided = false;
	//mat4 scale_matrix;
	//mat4 rotate_matrix;

	void update(btDiscreteDynamicsWorld* world);

}game_object;

inline void game_object::update(btDiscreteDynamicsWorld* world) {
	btTransform transform;
	gBody->getMotionState()->getWorldTransform(transform);

	mat4 translate_matrix = { 1, 0, 0, float(transform.getOrigin().getX()),
					0, 1, 0, float(transform.getOrigin().getY()),
					0, 0, 1, float(transform.getOrigin().getZ()),
					0, 0, 0, 1 };

	model_matrix = translate_matrix;
}

typedef struct _particle {
	game_object* obj;
	game_object* parent;
	vec3 pos;
	vec3 velocity;
	vec4 color;
	float scale;
	mat4 model_matrix;
	float life;
	bool is_dead;

}particle;

#endif