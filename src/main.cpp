// TEAM NAME: EVANDE
// TEAM MEMBER: 김현주 (2017310457), 서유영(2017313099)

#pragma comment( lib, "Bullet2FileLoader.lib" )
#pragma comment( lib, "Bullet3Collision.lib" )
#pragma comment( lib, "Bullet3Common.lib" )
#pragma comment( lib, "Bullet3Dynamics.lib" )
#pragma comment( lib, "Bullet3Geometry.lib" )
#pragma comment( lib, "Bullet3OpenCL_clew.lib" )
#pragma comment( lib, "BulletCollision.lib" )
#pragma comment( lib, "BulletDynamics.lib" )
#pragma comment( lib, "BulletExampleBrowserLib.lib" )
#pragma comment( lib, "BulletFileLoader.lib" )
#pragma comment( lib, "BulletInverseDynamics.lib" )
#pragma comment( lib, "BulletInverseDynamicsUtils.lib" )
#pragma comment( lib, "BulletRobotics.lib" )
#pragma comment( lib, "BulletSoftBody.lib" )
#pragma comment( lib, "BulletWorldImporter.lib" )
#pragma comment( lib, "LinearMath.lib" )
#pragma comment( lib, "OpenGLWindow.lib" )

#include "cgmath.h"			// slee's simple math library
#include "cgut2.h"			// slee's OpenGL utility
#include "trackball.h"
#include "assimp_loader.h"
#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "stage4.h"
//#include "btBulletDynamicsCommon.h"

//*******************************************************************
// forward declarations for freetype text
void text_init();
void render_text(std::string text, GLint x, GLint y, GLfloat scale, vec4 color);

//*******************************************************************
// global constants
static const char* window_name = "cgmodel - assimp for *.obj and *.3ds";
static const char* vert_shader_path = "../bin/shaders/models.vert";
static const char* frag_shader_path = "../bin/shaders/models.frag";

//*******************************************************************
// include stb_image with the implementation preprocessor definition
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//*******************************************************************
// window objects
GLFWwindow* window = nullptr;
ivec2		window_size = ivec2(720, 480);	// initial window size

//*******************************************************************
// OpenGL objects
GLuint	program = 0;	// ID holder for GPU program

//*******************************************************************
// global variables
int		frame = 0;		// index of rendering frames
bool	show_texcoord = false;
bool	b_wireframe = false;	// this is the default
int		select_color = 3;
vec3	new_eye = vec3(0);
bool	is_shiftkey = false;
bool	is_ctrlkey = false;
int		screen = 0;
int		stage = 0;
bool	clear = false;
bool	reset = false;
bool	game_over = false;
bool	isPrintPr = false;
bool	isPrintEp = false;
float	a = 0.0f;
float	s = 4.0f;
int		p = 600;
stage_1 ts{};
stage_2 ts2{};
stage_3 ts3{};
stage_4 ts4{};
irrklang::ISound* tracking1;
irrklang::ISoundEngine* engine;
std::vector<irrklang::ISoundSource*> bgm_list;
std::vector<irrklang::ISoundSource*> sounds_list;

//*******************************************************************
// scene objects
//mesh2* pMesh = nullptr;
camera		cam;
trackball	tb;

//*******************************************************************
void update()
{
	glUseProgram(program);

	if (screen == 0) {
		a = abs(sin(float(glfwGetTime()) * 2.5f));
	}
	else if (screen == 1) {
		p--;
		a = abs(sin(float(glfwGetTime()) * 2.5f));
	}
	else if (screen == 2 && !reset && !game_over) {
		game_object* player = ts.game_object_list.at(0);
		btTransform transform;
		player->gBody->getMotionState()->getWorldTransform(transform);
		btVector3 pos = transform.getOrigin();

		vec3 diff = vec3(pos.getX(), pos.getY(), pos.getZ()) - cam.at;

		cam.eye += diff;
		cam.at += diff;

		cam.view_matrix = mat4::look_at(cam.eye, cam.at, cam.up);

		stage = 2;
		if(!clear)	game_over = ts.check_game_over();
		ts.update(program, window_size, cam);
	}
	else if (screen == 2 && reset) {
		reset = false;
		ts.reset(cam);
	}
	else if (screen == 3 && !reset && !game_over) {
		game_object* player = ts2.game_object_list.at(0);
		btTransform transform;
		player->gBody->getMotionState()->getWorldTransform(transform);
		btVector3 pos = transform.getOrigin();

		vec3 diff = vec3(pos.getX(), pos.getY(), pos.getZ()) - cam.at;

		cam.eye += diff;
		cam.at += diff;

		cam.view_matrix = mat4::look_at(cam.eye, cam.at, cam.up);

		stage = 3;
		if (!clear)	game_over = ts2.check_game_over();
		ts2.update(program, window_size, cam);
	}
	else if (screen == 3 && reset) {
		reset = false;
		ts2.reset(cam);
	}
	else if (screen == 4 && !reset && !game_over) {
		game_object* player = ts3.game_object_list.at(0);
		btTransform transform;
		player->gBody->getMotionState()->getWorldTransform(transform);
		btVector3 pos = transform.getOrigin();

		vec3 diff = vec3(pos.getX(), pos.getY(), pos.getZ()) - cam.at;

		cam.eye += diff;
		cam.at += diff;

		cam.view_matrix = mat4::look_at(cam.eye, cam.at, cam.up);

		stage = 4;
		if (!clear)	game_over = ts3.check_game_over();
		ts3.update(program, window_size, cam);
	}
	else if (screen == 4 && reset) {
		reset = false;
		ts3.reset(cam);
	}
	else if (screen == 5 && !reset && !game_over) {
		game_object* player = ts4.game_object_list.at(0);
		btTransform transform;
		player->gBody->getMotionState()->getWorldTransform(transform);
		btVector3 pos = transform.getOrigin();

		vec3 diff = vec3(pos.getX(), pos.getY(), pos.getZ()) - cam.at;

		cam.eye += diff;
		cam.at += diff;

		cam.view_matrix = mat4::look_at(cam.eye, cam.at, cam.up);

		stage = 5;
		if (!clear)	game_over = ts4.check_game_over();
		ts4.update(program, window_size, cam);
	}
	else if (screen == 5 && reset) {
		reset = false;
		ts4.reset(cam);
	}
	else if (screen == 6) {
		s = abs(sin(float(glfwGetTime()) * 2.5f));
		// clear
	}
}

void render()
{
	// clear screen (with background color) and clear depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// notify GL that we use our own program
	glUseProgram(program);
		if(game_over && !clear) {
			if (!engine->isCurrentlyPlaying(bgm_list[7])) {
				tracking1->stop();
				tracking1 = engine->play2D(bgm_list[7], false, false, true);
			}
			render_text("GAME OVER", 75, 300, 3.0f, vec4(1.0f, 0.0f, 0.0f, 1.0f));
			render_text("Press R to restart", 300, 400, 0.5f, vec4(0.5f, 0.5f, 0.5f, a));
		}
		else {
			if (screen == 0) {
				render_text("**DOKIDOKI**", 50, 200, 1.8f, vec4(0.2f, 0.2f, 0.65f, 1.0f));
				render_text("*SOFTWARE CLUB*", 50, 270, 1.8f, vec4(0.2f, 0.2f, 0.65f, 1.0f));
				render_text("Press Enter to Start", 200, 350, 0.8f, vec4(0.4f, 0.4f, 0.4f, a));
				if (!engine->isCurrentlyPlaying(bgm_list[5])) {
					tracking1 = engine->play2D(bgm_list[5], false, false, true);
				}
			}
			else if (screen == 1) {
				render_text("Once upon a time,", 50, p, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("there was a boy", 50, p+50, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("who strongly believe that", 50, p + 100, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("he can do whatever he wants", 50, p + 150, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("when he goes to university.", 50, p + 200, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));

				render_text("Due to the true belief,", 50, p + 250, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("the boy devote his whole", 50, p + 300, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("three years of high school", 50, p + 350, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("to study SAT", 50, p + 400, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));

				render_text("Delighting with the higher score,", 50, p + 500, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("than he expected,", 50, p + 550, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("he applies to his dream,", 50, p + 600, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("<< S.O.F.T.W.A.R.E.M.A.J.O.R >>", 50, p + 650, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("SKKU software department!!", 50, p + 700, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));

				render_text("On the day of acceptance announcement", 50, p + 800, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("he could listen to the song", 50, p + 850, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("- WE ARE THE CHAMPION!!!", 50, p + 900, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));

				render_text("With a pounding heart,", 50, p + 1000, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("the boy meets the first week of university.", 50, p + 1050, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("However, something is different", 50, p + 1100, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("from his dream...", 50, p + 1150, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));

				render_text("What kind of things are", 50, p + 1250, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("waiting for him...?", 50, p + 1300, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));

				render_text("Can the boy escap....", 50, p + 1400, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("no, graduate from the university?", 50, p + 1450, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));

				// cmd translate
				if (!isPrintPr) {
					printf("==================== PROLOGUE ====================\n");
					printf("대학에 가면 다 할 수 있다는 말에 속아넘어가 고등학교 3년을 꼬박 수능 공부에 바친 00.\n\n");
					printf("예상보다 높은 수능 점수에 기뻐하며 줄곧 꿈꺼왔던 『소.프.트.웨.어.중.점.학.과』 성균관대학교 소프트웨어학과에 지원한다!\n\n");
					printf("합격 발표의 날, 그의 귓가에 흘러들어오는 We are the Champion!!\n\n");
					printf("떨리는 마음으로 대학 첫 주를 맞이한다.\n그런데 뫄뫄가 생각한 것과는 뭔가 다르다?\n\n");
					printf("그런 뫄뫄의 앞에서 기다리고 있는 것은...?\n과연 뫄뫄는 성공적으로 대학을 탈추..\n아니 졸업할 수 있을까?\n\n");
					printf("====================== END =======================\n");
					isPrintPr = true;
				}

				render_text("Press Enter to Skip", 300, 500, 0.5f, vec4(0.4f, 0.4f, 0.4f, a));
				if (!engine->isCurrentlyPlaying(bgm_list[0])) {
					tracking1->stop();
					tracking1 = engine->play2D(bgm_list[0], false, false, true);
				}
			}
			else if (screen == 2 && !reset) {
				ts.render(program, cam);
				if (clear) {
					if (!engine->isCurrentlyPlaying(bgm_list[6])) {
						tracking1->stop();
						tracking1 = engine->play2D(bgm_list[6], false, false, true);
					}
					render_text("Press Enter", 250, 250, 1.0f, vec4(1.0f, 0.92f, 0.016f, 1.0f));
					render_text("to fulfill military duty", 120, 300, 1.0f, vec4(1.0f, 0.92f, 0.016f, 1.0f));
					render_text("or to bokhak", 250, 350, 1.0f, vec4(1.0f, 0.92f, 0.016f, 1.0f));
				}
				else {
					if (!engine->isCurrentlyPlaying(bgm_list[1])) {
						tracking1->stop();
						tracking1 = engine->play2D(bgm_list[1], false, false, true);
					}
					char buff[100];
					float t = ts.time_left();
					vec3 rgb = t > 10 ? (t > 30 ? vec3(0.2f, 0.8f, 0.2f) : vec3(0.7f, 0.7f, 0.1f)) : vec3(0.8f, 0.2f, 0.2f);

					std::snprintf(buff, sizeof(buff), "%d:%d:%d", (int)t / 60, (int)t % 60, (int)(t * 100) % 100);
					render_text(std::string(buff), 10, 30, 0.5f, vec4(rgb, 1.0f));
					//render_text(std::string(buff), 10, 10, 0.5f, vec4(0.5f, 0.8f, 0.2f, 1.0f));

					std::snprintf(buff, sizeof(buff), "%3.0f km/h", ts.get_player_speed() * 2.5f);
					render_text(std::string(buff), 650, 530, 0.7f, vec4(0.2f, 0.2f, 0.2f, 1.0f));
				}
			}
			else if (screen == 3 && !reset) {
				ts2.render(program, cam);
				if (clear) {
					if (!engine->isCurrentlyPlaying(bgm_list[6])) {
						tracking1->stop();
						tracking1 = engine->play2D(bgm_list[6], false, false, true);
					}
					render_text("Press Enter", 250, 250, 1.0f, vec4(1.0f, 0.92f, 0.016f, 1.0f));
					render_text("to go to graduate school", 110, 300, 1.0f, vec4(1.0f, 0.92f, 0.016f, 1.0f));
				}
				else {
					if (!engine->isCurrentlyPlaying(bgm_list[2])) {
						tracking1->stop();
						tracking1 = engine->play2D(bgm_list[2], false, false, true);
					}
					char buff[100];
					float t = ts2.time_left();
					vec3 rgb = t > 10 ? (t > 30 ? vec3(0.2f, 0.8f, 0.2f) : vec3(0.7f, 0.7f, 0.1f)) : vec3(0.8f, 0.2f, 0.2f);

					std::snprintf(buff, sizeof(buff), "%d:%d:%d", (int)t / 60, (int)t % 60, (int)(t * 100) % 100);
					render_text(std::string(buff), 10, 30, 0.5f, vec4(rgb, 1.0f));
					//render_text(std::string(buff), 10, 10, 0.5f, vec4(0.5f, 0.8f, 0.2f, 1.0f));

					std::snprintf(buff, sizeof(buff), "%3.0f km/h", ts2.get_player_speed() * 2.5f);
					render_text(std::string(buff), 650, 530, 0.7f, vec4(0.2f, 0.2f, 0.2f, 1.0f));
				}
			}
			else if (screen == 4 && !reset) {
				ts3.render(program, cam);
				if (clear) {
					if (!engine->isCurrentlyPlaying(bgm_list[6])) {
						tracking1->stop();
						tracking1 = engine->play2D(bgm_list[6], false, false, true);
					}
					render_text("Press Enter", 250, 250, 1.0f, vec4(1.0f, 0.92f, 0.016f, 1.0f));
					render_text("to graduate", 250, 300, 1.0f, vec4(1.0f, 0.92f, 0.016f, 1.0f));
				}
				else {
					if (!engine->isCurrentlyPlaying(bgm_list[3])) {
						tracking1->stop();
						tracking1 = engine->play2D(bgm_list[3], false, false, true);
					}
					char buff[100];
					float t = ts3.time_left();
					vec3 rgb = t > 10 ? (t > 30 ? vec3(0.2f, 0.8f, 0.2f) : vec3(0.7f, 0.7f, 0.1f)) : vec3(0.8f, 0.2f, 0.2f);

					std::snprintf(buff, sizeof(buff), "%d:%d:%d", (int)t / 60, (int)t % 60, (int)(t * 100) % 100);
					render_text(std::string(buff), 10, 30, 0.5f, vec4(rgb, 1.0f));
					//render_text(std::string(buff), 10, 10, 0.5f, vec4(0.5f, 0.8f, 0.2f, 1.0f));

					std::snprintf(buff, sizeof(buff), "%3.0f km/h", ts3.get_player_speed() * 2.5f);
					render_text(std::string(buff), 650, 530, 0.7f, vec4(0.2f, 0.2f, 0.2f, 1.0f));
				}
			}
			else if (screen == 5 && !reset) {
				ts4.render(program, cam);
				if (clear) {
					if (!engine->isCurrentlyPlaying(bgm_list[6])) {
						tracking1->stop();
						tracking1 = engine->play2D(bgm_list[6], false, false, true);
					}
					render_text("Press Enter", 250, 250, 1.0f, vec4(1.0f, 0.92f, 0.016f, 1.0f));
					render_text("to see epilogue", 200, 300, 1.0f, vec4(1.0f, 0.92f, 0.016f, 1.0f));
				}
				else {
					if (!engine->isCurrentlyPlaying(bgm_list[4])) {
						tracking1->stop();
						tracking1 = engine->play2D(bgm_list[4], false, false, true);
					}
					char buff[100];
					float t = ts4.time_left();
					vec3 rgb = t > 10 ? (t > 30 ? vec3(0.2f, 0.8f, 0.2f) : vec3(0.7f, 0.7f, 0.1f)) : vec3(0.8f, 0.2f, 0.2f);

					std::snprintf(buff, sizeof(buff), "%d:%d:%d", (int)t / 60, (int)t % 60, (int)(t * 100) % 100);
					render_text(std::string(buff), 10, 30, 0.5f, vec4(rgb, 1.0f));
					//render_text(std::string(buff), 10, 10, 0.5f, vec4(0.5f, 0.8f, 0.2f, 1.0f));

					std::snprintf(buff, sizeof(buff), "%3.0f km/h", ts4.get_player_speed() * 2.5f);
					render_text(std::string(buff), 650, 530, 0.7f, vec4(0.2f, 0.2f, 0.2f, 1.0f));
				}
			}
			else if (screen == 6) {
				if (!engine->isCurrentlyPlaying(bgm_list[5])) {
					tracking1->stop();
					tracking1 = engine->play2D(bgm_list[5], false, false, true);
				}
				// The boy was finally able to graduate the SKKU. But, the journey does not end here. Now, he has to ready to get a job...
				render_text("The boy was finally able to", 50, 100, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("graduate the SKKU", 50, 150, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("But, the journey does not end here.", 50, 200, 0.6f, vec4(0.0f, 0.0f, 0.0f, 1.0f));
				render_text("Now, he has to ready to get a job...", 50, 250, 0.6f, vec4(1.0f, 0.0f, 0.0f, 1.0f));


				// cmd translate
				if (!isPrintEp) {
					printf("==================== EPILOGUE ====================\n");
					printf("드디어 무사히 졸업을 마친 00.\n\n");
					printf("그러나 아직 여정은 끝나지 않았다.\n\n");
					printf("그에겐 아직 취준생의 길이 남아있으니...\n\n");
					printf("====================== END =======================\n");
					isPrintEp = true;
				}
			}
		}

	glfwSwapBuffers(window);
}

void reshape(GLFWwindow* window, int width, int height)
{
	// set current viewport in pixels (win_x, win_y, win_width, win_height)
	// viewport: the window area that are affected by rendering 
	window_size = ivec2(width, height);
	glViewport(0, 0, width, height);
}

void print_help()
{
	printf("\n\n[help]\n");
	printf("- press ESC or 'q' to terminate the program\n");
	printf("- press F1 or 'h' to see help\n");
	printf("- press UP to go ahead\n");
	printf("- press DOWN to go back\n");
	printf("- press LEFT or RIGHT to change direction\n");
	printf("- press R to reset stage\n");
	printf("\n");
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)	glfwSetWindowShouldClose(window, GL_TRUE);
		else if (key == GLFW_KEY_H || key == GLFW_KEY_F1)	print_help();
		//else if (key == GLFW_KEY_W)
		//{
		//	b_wireframe = !b_wireframe;
		//	glPolygonMode(GL_FRONT_AND_BACK, b_wireframe ? GL_LINE : GL_FILL);
		//	printf("> using %s mode\n", b_wireframe ? "wireframe" : "solid");
		//}
		else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
			is_shiftkey = true;
		}
		else if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
			is_ctrlkey = true;
		}
		else if (key == GLFW_KEY_UP) {
			ts.isUpPressed = true;
			ts2.isUpPressed = true;
			ts3.isUpPressed = true;
			ts4.isUpPressed = true;
		}
		else if (key == GLFW_KEY_DOWN) {
			ts.isDownPressed = true;
			ts2.isDownPressed = true;
			ts3.isDownPressed = true;
			ts4.isDownPressed = true;
		}
		else if (key == GLFW_KEY_LEFT) {
			ts.isLeftPressed = true;
			ts2.isLeftPressed = true;
			ts3.isLeftPressed = true;
			ts4.isLeftPressed = true;
		}
		else if (key == GLFW_KEY_RIGHT) {
			ts.isRightPressed = true;
			ts2.isRightPressed = true;
			ts3.isRightPressed = true;
			ts4.isRightPressed = true;
		}
		else if (key == GLFW_KEY_ENTER) {
			if (screen < 2 || clear) {
				screen++;
				clear = false;
			}
		}
		else if (key == GLFW_KEY_R) {
			reset = true;
			game_over = false;
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
			is_shiftkey = false;
		}
		else if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
			is_ctrlkey = false;
		}
		else if (key == GLFW_KEY_UP) {
			ts.isUpPressed = false;
			ts2.isUpPressed = false;
			ts3.isUpPressed = false;
			ts4.isUpPressed = false;
		}
		else if (key == GLFW_KEY_DOWN) {
			ts.isDownPressed = false;
			ts2.isDownPressed = false;
			ts3.isDownPressed = false;
			ts4.isDownPressed = false;
		}
		else if (key == GLFW_KEY_LEFT) {
			ts.isLeftPressed = false;
			ts2.isLeftPressed = false;
			ts3.isLeftPressed = false;
			ts4.isLeftPressed = false;
		}
		else if (key == GLFW_KEY_RIGHT) {
			ts.isRightPressed = false;
			ts2.isRightPressed = false;
			ts3.isRightPressed = false;
			ts4.isRightPressed = false;
		}
	}
}

void mouse(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT || ((button == GLFW_MOUSE_BUTTON_LEFT) && is_shiftkey)) {
		dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
		vec2 npos = vec2(float(pos.x) / float(window_size.x - 1), float(pos.y) / float(window_size.y - 1));
		if (action == GLFW_PRESS)			tb.begin_zoom(cam.view_matrix, npos.x, npos.y, cam.eye);
		else if (action == GLFW_RELEASE)	cam.eye = tb.end_zoom(cam.view_matrix);
	}

	else if (button == GLFW_MOUSE_BUTTON_MIDDLE || ((button == GLFW_MOUSE_BUTTON_LEFT) && is_ctrlkey)) {
		dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
		vec2 npos = vec2(float(pos.x) / float(window_size.x - 1), float(pos.y) / float(window_size.y - 1));
		if (action == GLFW_PRESS)			tb.begin_pan(cam.view_matrix, npos.x, npos.y, cam.eye, cam.at);
		else if (action == GLFW_RELEASE) {
			std::vector<vec3> tmp = tb.end_pan(cam.view_matrix);
			cam.eye = tmp[0];
			cam.at = tmp[1];
		}
	}

	else if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
		vec2 npos = vec2(float(pos.x) / float(window_size.x - 1), float(pos.y) / float(window_size.y - 1));
		if (action == GLFW_PRESS)			tb.begin(cam.view_matrix, npos.x, npos.y);
		else if (action == GLFW_RELEASE)	cam.eye = tb.end(cam.view_matrix);
	}
}

void motion(GLFWwindow* window, double x, double y)
{
	if (!tb.is_tracking() && !tb.is_zooming() && !tb.is_panning()) return;
	vec2 npos = vec2(float(x) / float(window_size.x - 1), float(y) / float(window_size.y - 1));
	cam.view_matrix = tb.update(npos.x, npos.y, cam.at);
}

bool user_init()
{
	// log hotkeys
	print_help();

	// init GL states
	glClearColor(100 / 255.0f, 150 / 255.0f, 200 / 255.0f, 1.0f);	// set clear color
	glEnable(GL_CULL_FACE);								// turn on backface culling
	glEnable(GL_DEPTH_TEST);								// turn on depth tests
	//glEnable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// setup freetype
	text_init();


	return true;
}

void user_finalize()
{
	delete_texture_cache();
}

int main(int argc, char* argv[]) {
	// initialization
	if (!glfwInit()) { printf("[error] failed in glfwInit()\n"); return 1; }

	// create window and initialize OpenGL extensions
	if (!(window = cg_create_window(window_name, window_size.x, window_size.y))) { glfwTerminate(); return 1; }
	if (!cg_init_extensions(window)) { glfwTerminate(); return 1; }	// version and extensions

	// initializations and validations
	if (!(program = cg_create_program(vert_shader_path, frag_shader_path))) { glfwTerminate(); return 1; }	// create and compile shaders/program

	if (!ts.init(program, window, cam)) { printf("Failed to init stage\n"); glfwTerminate(); return 1; }
	if (!ts2.init(program, window, cam)) { printf("Failed to init stage\n"); glfwTerminate(); return 1; }
	if (!ts3.init(program, window, cam)) { printf("Failed to init stage\n"); glfwTerminate(); return 1; }
	if (!ts4.init(program, window, cam)) { printf("Failed to init stage\n"); glfwTerminate(); return 1; }
	
	if (!user_init()) { printf("Failed to user_init()\n"); glfwTerminate(); return 1; }					// user initialization

	// register event callbacks
	glfwSetWindowSizeCallback(window, reshape);	// callback for window resizing events
	glfwSetKeyCallback(window, keyboard);			// callback for keyboard events
	glfwSetMouseButtonCallback(window, mouse);	// callback for mouse click inputs
	glfwSetCursorPosCallback(window, motion);		// callback for mouse movement

	// enters rendering/event loop
	for (frame = 0; !glfwWindowShouldClose(window); frame++)
	{
		glfwPollEvents();	// polling and processing of events
		update();			// per-frame update
		render();			// per-frame render
	}

	//// finalize test stage
	ts.close();
	ts2.close();
	ts3.close();
	ts4.close();


	// normal termination
	user_finalize();
	cg_destroy_window(window);

	return 0;
}