#pragma once
#ifndef __STAGE3_H__
#define __STAGE3_H__

#pragma comment( lib, "irrKlang.lib" )

#include "sphere.h"
#include "game_object.h"
#include "irrKlang/irrKlang.h"

static const char* track_mesh_obj3 = "../bin/mesh/Stages/stage3/tracks/stage3_track.obj";
static const char* start_floor_mesh_obj3 = "../bin/mesh/Stages/stage3/tracks/stage3_start_floor_objects.obj";
static const char* end_floor_mesh_obj3 = "../bin/mesh/Stages/stage3/tracks/stage3_end_floor_objects.obj";
static const char* start_wall_mesh_obj3 = "../bin/mesh/Stages/stage3/tracks/stage3_start_wall.obj";
static const char* end_wall_mesh_obj3 = "../bin/mesh/Stages/stage3/tracks/stage3_end_wall.obj";
static const char* start_trigger_mesh_obj3 = "../bin/mesh/Stages/stage3/tracks/stage3_start_trigger.obj";
static const char* end_trigger_mesh_obj3 = "../bin/mesh/Stages/stage3/tracks/stage3_end_trigger.obj";
static const char* background_mesh_obj3 = "../bin/mesh/Stages/stage3/tracks/background3.obj";

extern bool clear;
extern bool game_over;

class stage_3 {

private:
	btVector3 start_pos;
	game_object* player;
	vec4 player_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	float start_time;
	float booster_start_time;
	float soju_start_time;
	bool on_carpet = false;

	float force = 0.15f;
	float max_speed = 30.0f;
	float time_limit = 180.0f;

	bool isStart = false;
	bool isEnd = false;
	int soju_sign = 1;
	int clk;

	struct result_callback : public btCollisionWorld::ContactResultCallback {
		btScalar result_callback::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,
			int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
			return 0;
		}
	};

	std::vector<vertex> create_sphere_vertices(uint n_tess) {
		std::vector<vertex> vs = { {vec3(0), vec3(0,0,-1.0f), vec2(0.5f)} };

		for (uint th = 0; th <= n_tess; th++) {
			float tt = PI * th / float(n_tess), tc = cos(tt), ts = sin(tt);
			for (uint p = 0; p <= n_tess * 2; p++) {
				float pp = PI * 2.0f * p / float(2.0f * n_tess), pc = cos(pp), ps = sin(pp);
				vs.push_back({
					vec3(ts * pc, ts * ps, tc),
					vec3(ts * pc, ts * ps, tc),
					vec2(p / float(2 * n_tess), 1 - th / float(n_tess))
					});
			}
		}

		return vs;
	}

	void init_world() {
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		overlappingPairCache = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver;
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
		dynamicsWorld->setGravity(btVector3(0, -9, 0));
	}

	void create_items_list() {
		items_list.push_back("../bin/mesh/Stages/stage3/items/aplus1.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/aplus2.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/aplus3.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/aplus4.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/aplus5.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/aplus6.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/aplus7.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/aplus8.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/aplus9.obj");
		
		items_list.push_back("../bin/mesh/Stages/stage3/items/coin1.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/coin2.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/coin3.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/coin4.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/coin5.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/coin6.obj");
		items_list.push_back("../bin/mesh/Stages/stage3/items/coin7.obj");
		
	}

	void create_obstacles_list() {
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus1.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus2.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus3.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus4.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus5.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus6.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus7.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus8.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus9.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus10.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus11.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus12.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus13.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus14.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/bus15.obj");

		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet1.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet2.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet3.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet4.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet5.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet6.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet7.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet8.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet9.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet10.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet11.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet12.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet13.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet14.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet15.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/carpet16.obj");

		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju1.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju2.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju3.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju4.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju5.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju6.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju7.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju8.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju9.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju10.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju11.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju12.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju13.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju14.obj");
		obstacles_list.push_back("../bin/mesh/Stages/stage3/obstacle/soju15.obj");

	}

	void init_camera(camera& cam, game_object* player) {
		btTransform transform;
		player->gBody->getMotionState()->getWorldTransform(transform);
		btVector3 pos = transform.getOrigin();

		cam.eye = vec3(pos.getX(), pos.getY(), pos.getZ()) + vec3(0, 30 * sin(PI / 6), -30 * cos(PI / 6));
		cam.at = vec3(pos.getX(), pos.getY(), pos.getZ());
		cam.up = vec3(0, 1, 0);
		cam.view_matrix = mat4::look_at(cam.eye, cam.at, cam.up);
	}

	mesh2* make_sphere() {
		uint n_tess = 32;

		mesh2* pMesh = new mesh2();

		pMesh->vertex_list = create_sphere_vertices(n_tess);

		uint N = 2 * n_tess + 1;
		for (uint i = 0; i < n_tess; i++) {
			for (uint j = 1; j <= n_tess * 2; j++) {
				pMesh->index_list.push_back(i * N + j);
				pMesh->index_list.push_back((i + 1) * N + j);
				pMesh->index_list.push_back((i + 1) * N + j + 1);

				pMesh->index_list.push_back(i * N + j + 1);
				pMesh->index_list.push_back(i * N + j);
				pMesh->index_list.push_back((i + 1) * N + j + 1);
			}
		}

		glGenBuffers(1, &pMesh->vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, pMesh->vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(decltype(pMesh->vertex_list)::value_type) * pMesh->vertex_list.size(), pMesh->vertex_list.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &pMesh->index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(pMesh->index_list)::value_type) * pMesh->index_list.size(), pMesh->index_list.data(), GL_STATIC_DRAW);


		return pMesh;
	}

	mesh2* make_object_from_file(const char* file_path) {
		mesh2* fMesh = load_model(file_path);
		return fMesh;
	}

	game_object* make_gameobject(mesh2* _mesh, btRigidBody* _body, bool _is_transparent = false, bool _contact_response = true, int _item_type = 0, int _obstacle_type = 0) {
		game_object* go = new game_object();
		go->gMesh = _mesh;
		go->gBody = _body;
		go->is_transparent = _is_transparent;
		go->item_type = _item_type;
		go->obstacle_type = _obstacle_type;
		if (!_contact_response) {
			go->gBody->setCollisionFlags(go->gBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
		go->gBody->setActivationState(DISABLE_DEACTIVATION);

		return go;
	}

	btRigidBody* make_rigidbody_from_shape(btCollisionShape* colShape, float _mass, vec3 origin) {
		btTransform tf;
		tf.setIdentity();

		btScalar mass(_mass);

		btVector3 localInertia(0, 0, 0);
		colShape->calculateLocalInertia(mass, localInertia);
		tf.setOrigin(btVector3(origin.x, origin.y, origin.z));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(tf);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
		return body;
	}

	btRigidBody* make_sphere_rigidbody() {
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		return make_rigidbody_from_shape(colShape, 2.f, vec3(0, 10, -30));
	}

	btRigidBody* make_convexhull_rigidbody(mesh2* gMesh) {

		btTriangleMesh* mesh = new btTriangleMesh();

		for (uint i = 0; i < gMesh->index_list.size(); i += 3) {
			int index0 = gMesh->index_list[i];
			int index1 = gMesh->index_list[i + 1];
			int index2 = gMesh->index_list[i + 2];

			btVector3 vertex0(gMesh->vertex_list[index0].pos.x,
				gMesh->vertex_list[index0].pos.y,
				gMesh->vertex_list[index0].pos.z);
			btVector3 vertex1(gMesh->vertex_list[index1].pos.x,
				gMesh->vertex_list[index1].pos.y,
				gMesh->vertex_list[index1].pos.z);
			btVector3 vertex2(gMesh->vertex_list[index2].pos.x,
				gMesh->vertex_list[index2].pos.y,
				gMesh->vertex_list[index2].pos.z);

			mesh->addTriangle(vertex0, vertex1, vertex2);
		}

		btConvexShape* Shape = new btConvexTriangleMeshShape(mesh);

		return make_rigidbody_from_shape(Shape, 0.f, vec3(0, 0, 0));
	}

	btRigidBody* make_bvh_triangle_rigidbody(mesh2* gMesh) {

		btTriangleMesh* mesh = new btTriangleMesh();

		for (uint i = 0; i < gMesh->index_list.size(); i += 3) {
			int index0 = gMesh->index_list[i];
			int index1 = gMesh->index_list[i + 1];
			int index2 = gMesh->index_list[i + 2];

			btVector3 vertex0(gMesh->vertex_list[index0].pos.x,
				gMesh->vertex_list[index0].pos.y,
				gMesh->vertex_list[index0].pos.z);
			btVector3 vertex1(gMesh->vertex_list[index1].pos.x,
				gMesh->vertex_list[index1].pos.y,
				gMesh->vertex_list[index1].pos.z);
			btVector3 vertex2(gMesh->vertex_list[index2].pos.x,
				gMesh->vertex_list[index2].pos.y,
				gMesh->vertex_list[index2].pos.z);

			mesh->addTriangle(vertex0, vertex1, vertex2);
		}

		btBvhTriangleMeshShape* Shape = new btBvhTriangleMeshShape(mesh, true);

		return make_rigidbody_from_shape(Shape, 0.f, vec3(0, 0, 0));
	}


	void update_force(camera* cam) {
		vec4 dir_xz = vec4(normalize(cam->at - cam->eye), 1);
		dir_xz.y = 0;

		float delta = float(glfwGetTime()) - last_t;
		btVector3 k = player->gBody->getLinearVelocity();

		if (isDownPressed) {
			vec4 _dir = mat4::rotate(vec3(1, 0, 0), PI / 18) * -dir_xz;
			vec3 direction = vec3(_dir.x, _dir.y, _dir.z);

			vec3 tmp = direction * force * delta * 100;
			btVector3 dir = btVector3(tmp.x, tmp.y, tmp.z);
			player->gBody->applyCentralImpulse(dir);

			if (!engine->isCurrentlyPlaying(sounds_list[5])) {
				engine->play2D(sounds_list[5]);
			}
		}

		if (isUpPressed) {
			vec4 _dir = mat4::rotate(vec3(1, 0, 0), PI / 18) * dir_xz;
			vec3 direction = vec3(_dir.x, _dir.y, _dir.z);

			vec3 tmp = direction * force * delta * 100;
			btVector3 dir = btVector3(tmp.x, tmp.y, tmp.z);
			player->gBody->applyCentralImpulse(dir);
		}

		btVector3 velocity = player->gBody->getLinearVelocity();
		float speed = velocity.length();

		if (speed > max_speed) {
			player->gBody->setLinearVelocity(max_speed * velocity.normalize() * delta * 100);
		}
		if (speed == 0.f) {
			player->gBody->setLinearVelocity(0.1f * btVector3(dir_xz.x, dir_xz.y, dir_xz.z).normalize());
		}
	}

	void update_camera(camera* cam) {
		if (isLeftPressed) {
			vec3 _tmp = cam->eye - cam->at;
			vec4 r = (mat4::rotate(vec3(0, 1, 0), soju_sign * PI / 240) * vec4(_tmp, 1));
			cam->eye = vec3(r.x, r.y, r.z) + cam->at;
			cam->view_matrix = mat4::look_at(cam->eye, cam->at, cam->up);
		}

		if (isRightPressed) {
			vec3 _tmp = cam->eye - cam->at;
			vec4 r = (mat4::rotate(vec3(0, 1, 0), -soju_sign * PI / 240) * vec4(_tmp, 1));
			cam->eye = vec3(r.x, r.y, r.z) + cam->at;
			cam->view_matrix = mat4::look_at(cam->eye, cam->at, cam->up);
		}

		if (!isRightPressed || !isLeftPressed) {
			player->gBody->applyCentralImpulse(btVector3(0, 0, 0));
		}
	}

	void update_items_and_obstacles(camera* cam) {
		float t = float(glfwGetTime());

		if (booster_start_time && t - booster_start_time < 4.0f) {
			if (clk % 6 == 0) {
				generate_particles(particle_sphere, 5, player, vec3(0, 10.0f, -15.0f), 2.0f, vec3(0.8f, 0.5f, 0.1f), 0.2f, 0.03f, 0.08f);
			}
			clk++;
		}
		else {
			force = 0.15f;
			player->gBody->setRollingFriction(0.8f);
			max_speed = 30.0f;
		}

		if (!on_carpet)
			player->gBody->setRollingFriction(0.8f);

		if (t - soju_start_time > 10.0f && soju_sign < 0)
			soju_sign = 1;

		if (t - booster_start_time > 4.0f && !on_carpet && t - soju_start_time > 10.0f)
			player_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);

		if (max_speed == 30.0f) {
			cam->fovy *= 0.99f;
			if (cam->fovy < PI / 4)	cam->fovy = PI / 4;
		}
		if (max_speed == 50.0f) {
			cam->fovy *= 1.01f;
			if (cam->fovy > PI / 3)		cam->fovy = PI / 3;
		}
	}

	void generate_particles(mesh2* _mesh, int n_particles, game_object* parent, vec3 direction, float speed, vec3 color, float life, float min_scale = 0.5f, float max_scale = 1.5f) {
		float t = float(glfwGetTime());
		game_object* go = make_gameobject(make_sphere(), make_sphere_rigidbody(), false, false, 0, 0);

		for (int i = 0; i < n_particles; i++) {
			vec3 pos = vec3(rand_range(-1, 1), rand_range(-1, 1), rand_range(-1, 1));
			vec3 velocity = direction == vec3(0) ? vec3(rand_range(-2, 2), rand_range(-2, 2), rand_range(-2, 2))
				: direction + speed * vec3(rand_range(-1, 1), rand_range(-1, 1), rand_range(-1, 1));
			if (color.x == color.y && color.x == color.z)
				color += vec3(rand_range(-0.05f, 0.05f));
			else
				color += vec3(rand_range(-0.05f, 0.05f), rand_range(-0.05f, 0.05f), rand_range(-0.05f, 0.05f));
			color = vec3(clip(color.x, 0, 1), clip(color.y, 0, 1), clip(color.z, 0, 1));
			float scale = rand_range(min_scale, max_scale);
			life += rand_range(0, 0.1f);

			particle_list.push_back(new particle{ go, parent, pos, velocity, vec4(color, 1.0f), scale, mat4::identity(), life + t, false });
		}
	}

	void update_particles() {
		float t = float(glfwGetTime());
		float d = t - last_t;

		btTransform transform;
		player->gBody->getMotionState()->getWorldTransform(transform);
		btVector3 _center = transform.getOrigin();
		vec3 center = vec3(_center.getX(), _center.getY(), _center.getZ());

		for (particle* p : particle_list) {
			if (t > p->life) {
				p->is_dead = true;
				continue;
			}

			p->pos += d * p->velocity;
			p->model_matrix = mat4::translate(p->pos) * mat4::translate(center) * mat4::scale(p->scale);
		}

		particle_list.erase(std::remove_if(particle_list.begin(), particle_list.end(),
			[](particle* p) { return p->is_dead; }), particle_list.end());

	}

	bool check_player_collide_with(btCollisionObject* target_obj) {
		btCollisionObject* player_obj = player->gBody;

		result_callback r_callback;

		dynamicsWorld->performDiscreteCollisionDetection();
		dynamicsWorld->contactPairTest(player_obj, target_obj, r_callback);

		int n_manifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

		for (int i = 0; i < n_manifolds; i++) {
			btPersistentManifold* contact_manifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obj_a = (btCollisionObject*)contact_manifold->getBody0();
			btCollisionObject* obj_b = (btCollisionObject*)contact_manifold->getBody1();

			if (obj_a == player_obj && obj_b == target_obj) {
				return true;
			}
		}

		return false;
	}

	void reposition(camera cam) {
		btTransform initialTransform;

		initialTransform.setOrigin(btVector3(0, 10, -30));

		dynamicsWorld->removeCollisionObject(player->gBody);
		mesh2* sphere_mesh = make_sphere();
		game_object_list[0] = make_gameobject(sphere_mesh, make_sphere_rigidbody());

		btTransform transform;

		player = game_object_list.at(0);
		player->gBody->getMotionState()->getWorldTransform(transform);
		player->gBody->setRollingFriction(0.8f);
		start_pos = transform.getOrigin();

		init_camera(cam, player);
	}

public:

	std::vector<game_object*> game_object_list;
	std::vector<const char*> items_list;
	std::vector<const char*> obstacles_list;

	game_object* start_trigger;
	game_object* end_trigger;

	std::vector<game_object*> items;
	std::vector<game_object*> obstacles;
	std::vector<particle*> particle_list;

	mesh2* particle_sphere;
	float last_t;

	bool isDownPressed = false;
	bool isUpPressed = false;
	bool isLeftPressed = false;
	bool isRightPressed = false;

	stage_3() {};

	bool init(GLuint _program, GLFWwindow* _window, camera& cam) {
		init_world();
		create_items_list();
		create_obstacles_list();

		particle_sphere = make_sphere();

		mesh2* sphere_mesh = make_sphere();
		mesh2* track_mesh = make_object_from_file(track_mesh_obj3);
		mesh2* start_floor_mesh = make_object_from_file(start_floor_mesh_obj3);
		mesh2* end_floor_mesh = make_object_from_file(end_floor_mesh_obj3);
		mesh2* start_wall_mesh = make_object_from_file(start_wall_mesh_obj3);
		mesh2* end_wall_mesh = make_object_from_file(end_wall_mesh_obj3);
		mesh2* background_mesh = make_object_from_file(background_mesh_obj3);
		mesh2* start_trigger_mesh = make_object_from_file(start_trigger_mesh_obj3);
		mesh2* end_trigger_mesh = make_object_from_file(end_trigger_mesh_obj3);

		game_object_list.push_back(make_gameobject(sphere_mesh, make_sphere_rigidbody()));
		game_object_list.push_back(make_gameobject(track_mesh, make_bvh_triangle_rigidbody(track_mesh)));
		game_object_list.push_back(make_gameobject(start_floor_mesh, make_bvh_triangle_rigidbody(start_floor_mesh)));
		game_object_list.push_back(make_gameobject(end_floor_mesh, make_bvh_triangle_rigidbody(end_floor_mesh)));
		game_object_list.push_back(make_gameobject(start_wall_mesh, make_bvh_triangle_rigidbody(start_wall_mesh), true));
		game_object_list.push_back(make_gameobject(end_wall_mesh, make_bvh_triangle_rigidbody(end_wall_mesh), true));
		game_object_list.push_back(make_gameobject(background_mesh, make_bvh_triangle_rigidbody(background_mesh)));

		start_trigger = make_gameobject(start_trigger_mesh, make_bvh_triangle_rigidbody(start_trigger_mesh), true, false);
		end_trigger = make_gameobject(end_trigger_mesh, make_bvh_triangle_rigidbody(end_trigger_mesh), true, false);

		for (uint i = 0, aplus_cnt = 0; i < items_list.size(); i++) {
			mesh2* mesh = make_object_from_file(items_list[i]);
			game_object* tmp;
			if (aplus_cnt < 9) {
				tmp = make_gameobject(mesh, make_bvh_triangle_rigidbody(mesh), false, false, 1);
				aplus_cnt++;
			}
			else {
				tmp = make_gameobject(mesh, make_bvh_triangle_rigidbody(mesh), false, false, 2);
			}
			game_object_list.push_back(tmp);
			items.push_back(tmp);
		}

		for (uint i = 0, bus_cnt = 0, carpet_cnt = 0; i < obstacles_list.size(); i++) {
			mesh2* mesh = make_object_from_file(obstacles_list[i]);
			game_object* tmp;
			if (bus_cnt < 15) {
				tmp = make_gameobject(mesh, make_convexhull_rigidbody(mesh), false, true, 0, 1);
				bus_cnt++;
			}
			else if (carpet_cnt < 16) {
				tmp = make_gameobject(mesh, make_bvh_triangle_rigidbody(mesh), false, false, 0, 2);
				carpet_cnt++;
			}
			else {
				tmp = make_gameobject(mesh, make_bvh_triangle_rigidbody(mesh), false, false, 0, 3);
			}
			game_object_list.push_back(tmp);
			obstacles.push_back(tmp);
		}

		btTransform transform;

		player = game_object_list.at(0);
		player->gBody->getMotionState()->getWorldTransform(transform);
		player->gBody->setRollingFriction(0.8f);
		start_pos = transform.getOrigin();

		init_camera(cam, player);

		return true;
	}

	void update(GLuint _program, ivec2 window_size, camera& cam) {
		cam.aspect_ratio = window_size.x / float(window_size.y);
		cam.projection_matrix = mat4::perspective(cam.fovy, cam.aspect_ratio, cam.dNear, cam.dFar);

		float t = float(glfwGetTime());
		float scale = 1.0f;

		update_force(&cam);
		update_camera(&cam);
		update_items_and_obstacles(&cam);
		update_particles();

		if (check_player_collide_with(start_trigger->gBody)) {
			isStart = true;
			start_time = t;
			clear = false;
		}
		if (check_player_collide_with(end_trigger->gBody)) {
			isEnd = true;
			clear = true;
		}

		if (!isStart)	start_time = t;

		for (uint i = 0; i < items.size(); i++) {
			if (!items[i]->isCollided && check_player_collide_with(items[i]->gBody)) {
				if (items[i]->item_type == 1) {					// aplus: speed booster
					booster_start_time = t;
					force = 0.3f;
					max_speed = 50.0f;
					player_color = vec4(1.0f, 0.4f, 0.05f, 1.0f);
					engine->play2D(sounds_list[1]);
				}
				else if (items[i]->item_type == 2) {			// coin: increase time
					engine->play2D(sounds_list[3]);
					start_time += 2.0f;
					generate_particles(particle_sphere, 40, player, vec3(0), 4.0f, vec3(0.9f, 0.9f, 0.9f), 0.1f, 0.05f, 0.2f);
				}
				items[i]->isCollided = true;
			}
		}

		on_carpet = false;
		for (uint i = 0; i < obstacles.size(); i++) {
			if (!obstacles[i]->isCollided && check_player_collide_with(obstacles[i]->gBody)) {
				if (obstacles[i]->obstacle_type == 1) {			// bus: obstacle
					engine->play2D(sounds_list[0]);
				}
				else if (obstacles[i]->obstacle_type == 2) {	// carpet: friction up
					engine->play2D(sounds_list[2]);
					on_carpet = true;
					player_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
					btVector3 velocity = player->gBody->getLinearVelocity();
					player->gBody->applyCentralImpulse(-velocity * 0.8f);
					player->gBody->setRollingFriction(3.0f);
					generate_particles(particle_sphere, 100, player, vec3(0), 4.0f, vec3(0.2f, 1.0f, 0.2f), 0.1f, 0.05f, 0.2f);
				}
				else if (obstacles[i]->obstacle_type == 3) {	// soju: swap left/right
					engine->play2D(sounds_list[4]);
					soju_start_time = t;
					soju_sign = -1;
					player_color = vec4(0.7f, 0.2f, 0.8f, 1.0f);
					generate_particles(particle_sphere, 100, player, vec3(0), 4.0f, vec3(1.0f, 0.2f, 0.2f), 0.1f, 0.05f, 0.2f);
				}
				obstacles[i]->isCollided = true;
			}
		}

		last_t = t;

		GLint uloc;
		uloc = glGetUniformLocation(_program, "view_matrix");			if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);
		uloc = glGetUniformLocation(_program, "projection_matrix");		if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);

		glActiveTexture(GL_TEXTURE0);
	}

	void render(GLuint _program, camera cam) {
		const char* vertex_attrib[] = { "position", "normal", "texcoord" };
		size_t		attrib_size[] = { sizeof(vertex().pos), sizeof(vertex().norm), sizeof(vertex().tex) };

		dynamicsWorld->stepSimulation(1.f / 60.f, 10);

		for (game_object* go : game_object_list) {

			go->update(dynamicsWorld);

			GLint uloc;
			uloc = glGetUniformLocation(_program, "model_matrix");		if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, go->model_matrix);
			uloc = glGetUniformLocation(_program, "is_transparent");	if (uloc > -1) glUniform1i(uloc, go->is_transparent);

			for (size_t k = 0, kn = std::extent<decltype(vertex_attrib)>::value, byte_offset = 0; k < kn; k++, byte_offset += attrib_size[k - 1])
			{
				GLuint loc = glGetAttribLocation(_program, vertex_attrib[k]); if (loc >= kn) continue;
				glEnableVertexAttribArray(loc);
				glBindBuffer(GL_ARRAY_BUFFER, go->gMesh->vertex_buffer);
				glVertexAttribPointer(loc, attrib_size[k] / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)byte_offset);
			}

			for (size_t k = 0, kn = go->gMesh->geometry_list.size(); k < kn; k++) {
				geometry& g = go->gMesh->geometry_list[k];

				if (g.mat->textures.diffuse) {
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, g.mat->textures.diffuse->id);
					glUniform1i(glGetUniformLocation(_program, "TEX"), 0);	 // GL_TEXTURE0
					glUniform1i(glGetUniformLocation(_program, "use_texture"), true);
				}
				else {
					glUniform4fv(glGetUniformLocation(_program, "diffuse"), 1, (const float*)(&g.mat->diffuse));
					glUniform1i(glGetUniformLocation(_program, "use_texture"), false);
				}

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->gMesh->index_buffer);
				glDrawElements(GL_TRIANGLES, g.index_count, GL_UNSIGNED_INT, (GLvoid*)(g.index_start * sizeof(GLuint)));
			}

			if (go->gMesh->geometry_list.size() == 0) {
				glUniform4fv(glGetUniformLocation(_program, "diffuse"), 1, (const float*)player_color);
				glUniform1i(glGetUniformLocation(_program, "use_texture"), false);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go->gMesh->index_buffer);
				glDrawElements(GL_TRIANGLES, go->gMesh->vertex_list.size() * 6, GL_UNSIGNED_INT, nullptr);
			}
		}

		for (particle* p : particle_list) {
			GLint uloc;
			uloc = glGetUniformLocation(_program, "model_matrix");		if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, p->model_matrix);
			uloc = glGetUniformLocation(_program, "is_transparent");	if (uloc > -1) glUniform1i(uloc, p->obj->is_transparent);

			for (size_t k = 0, kn = std::extent<decltype(vertex_attrib)>::value, byte_offset = 0; k < kn; k++, byte_offset += attrib_size[k - 1])
			{
				GLuint loc = glGetAttribLocation(_program, vertex_attrib[k]); if (loc >= kn) continue;
				glEnableVertexAttribArray(loc);
				glBindBuffer(GL_ARRAY_BUFFER, p->obj->gMesh->vertex_buffer);
				glVertexAttribPointer(loc, attrib_size[k] / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)byte_offset);
			}

			glUniform4fv(glGetUniformLocation(_program, "diffuse"), 1, (const float*)p->color);
			glUniform1i(glGetUniformLocation(_program, "use_texture"), false);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->obj->gMesh->index_buffer);
			glDrawElements(GL_TRIANGLES, p->obj->gMesh->vertex_list.size() * 6, GL_UNSIGNED_INT, nullptr);
		}
	}

	bool close() {
		for (game_object* go : game_object_list) {
			free(go->gMesh);
		}
		delete_texture_cache();

		return true;
	}

	void reset(camera cam) {
		clear = false;
		reposition(cam);
		
		for (uint i = 0; i < items.size(); i++) {
			items[i]->isCollided = false;
		}

		for (uint i = 0; i < obstacles.size(); i++) {
			obstacles[i]->isCollided = false;
		}
	}

	bool check_game_over() {
		game_object* player = game_object_list[0];
		btTransform transform;
		player->gBody->getMotionState()->getWorldTransform(transform);
		btVector3 pos = transform.getOrigin();

		if ((isStart && time_left() <= 0)) {
			isStart = false;
			isEnd = false;
			return true;
		}
		return false;
	}

	float time_left() {
		return max(time_limit - ((float)glfwGetTime() - start_time), 0);
	}

	float get_player_speed() {
		btVector3 velocity = player->gBody->getLinearVelocity();
		return velocity.length();
	}
};

#endif