#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Screen.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "Object3D.hpp"
#include "Skybox.hpp"

const int MS_FRAME = 16;
const float SPEED_STEP = 0.1;

int startTime;
int lag = 0;
float speed = 0.5;

int season = 0; // 0: winter, 1: spring, 2: summer, 3: fall
int seasonTime = 100;
int seasonTimer = 0;

glm::mat4 world(1.0f);

Camera camera(
	glm::vec3(0.0f, 0.5f, 3.0f), // position
	glm::vec3(0.0f, 0.0f, -1.0f) // direction (forward)
);

Screen screen(
	glm::radians(45.0f), // fovy
	600,  // width
	400,  // height
	0.2f, // zNear
	6000  // zFar
);

Light light;
Skybox skybox;
Scene scene;

int button_pressed = 0; // 1 if a button is currently being pressed.
int GLUTmouse[2] = { 0, 0 };

GLuint renderStyle;

Shader objectShader;
Shader lightShader;
Shader skyboxShader;
Shader normalShader;

void updateRenderStyle(GLuint value) {
	renderStyle = value;
	objectShader.setUniform("renderStyle", value);
}

void updateWorldMatrix(Shader& shader) {
	shader.setUniform("world", world);
}

void updateMaxDayNight(Shader& shader) {
	glm::vec3 pos = scene.getLight().getDiffusePosition();
	shader.setUniform("maxDayNight", pos.y);
}

void updateDayNight(Shader& shader) {
	glm::vec3 pos = scene.getLight().getDiffusePosition();
	shader.setUniform("dayNight", pos.y);
}

void updateRenderStyle() {
	updateRenderStyle((renderStyle + 1) % 2);
}

/**
 * This function is called when a mouse button is pressed.
 */
void mouse(int, int state, int x, int y) {
	// Remember button state
	button_pressed = (state == GLUT_DOWN) ? 1 : 0;

	// Remember mouse position
	GLUTmouse[0] = x;
	GLUTmouse[1] = screen.getHeight() - y;
}

/**
 * This function is called when the mouse is dragged.
 */
void mousedrag(int x, int y) {
	// Invert y coordinate
	y = screen.getHeight() - y;

	//change in the mouse position since last time
	int dx = x - GLUTmouse[0];
	int dy = y - GLUTmouse[1];

	GLUTmouse[0] = x;
	GLUTmouse[1] = y;

	if (dx == 0 && dy == 0) return;
	if (button_pressed == 0) return;

	camera.rotate(dx,dy);

	glutPostRedisplay();
}

void mouseWheel(int, int dir, int, int) {
	if (dir > 0) {
		camera.move(glm::vec2(0,1));
	}
	else {
		camera.move(glm::vec2(0,-1));
	}

	glutPostRedisplay();
}

/**
 * This function is called when a key is pressed.
 */
void keyboard(unsigned char key, int, int) {
	switch(key) {
	case 27: // Escape to quit
		exit(0);
		break;
	case 'w':
		objectShader.use();
		updateRenderStyle();
		break;
	case 'z':
		world = glm::translate(world, glm::vec3(0.0f, 0.05f, 0.f));
		break;
	case 'd':
		world = glm::translate(world, glm::vec3(0.05f, 0.f, 0.f));
		break;
	case 'q':
		world = glm::translate(world, glm::vec3(-0.05f, 0.f, 0.f));
		break;
	case 's':
		world = glm::translate(world, glm::vec3(0.f, -0.05f, 0.f));
		break;
	case 'a':
		world = glm::rotate(world, 0.05f, glm::vec3(0.f, -0.05f, 0.f));
		scene.rotateLight(0.05f, glm::vec3(0.f, -0.05f, 0.f));
		break;
	case 'e':
		world = glm::rotate(world, 0.05f, glm::vec3(0.05f, 0.f, 0.f));
		scene.rotateLight(0.05f, glm::vec3(0.05f, 0.f, 0.f));
		break;
	case 'r':
		camera = Camera(
			glm::vec3(0.0f, 0.0f, 3.0f), // position
			glm::vec3(0.0f, 0.0f, -1.0f) // direction (forward)
		);
		break;
	case 'p':
		if (speed + SPEED_STEP < 5) {
			speed += SPEED_STEP;
		}
		break;
	case 'm':
		if (speed - SPEED_STEP >= 0) {
			speed -= SPEED_STEP;
		}
		break;
	}
	glutPostRedisplay();
}

/**
 * This function is called when an arrow key is pressed.
 */
void keyboard2(int key, int, int) {
	switch(key) {
	case GLUT_KEY_UP:
		camera.move(glm::vec2(0,1));
		break;
	case GLUT_KEY_DOWN:
		camera.move(glm::vec2(0,-1));
		break;
	case GLUT_KEY_LEFT:
		camera.move(glm::vec2(-1,0));
		break;
	case GLUT_KEY_RIGHT:
		camera.move(glm::vec2(1,0));
		break;
	}
	glutPostRedisplay();
}

void reshape(int width, int height) {
	screen.reshape(width, height);

	objectShader.use();
	screen.uniformProjectionMatrix(objectShader);

	lightShader.use();
	screen.uniformProjectionMatrix(lightShader);

	skyboxShader.use();
	screen.uniformProjectionMatrix(skyboxShader);

	normalShader.use();
	screen.uniformProjectionMatrix(normalShader);
}

/**
 * Main loop function.
 */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - startTime;
	lag += deltaTime;
	startTime = currentTime;
	if (lag > MS_FRAME) {
		seasonTimer++;
		if (seasonTimer == seasonTime) {
			season = (season + 1) % 4;
			seasonTimer = 0;
		}
//		scene.rotateLight(0.01f * speed * deltaTime, glm::vec3(0.05f, 0.0f, 0.0f));
		lag -= MS_FRAME;
	}

	objectShader.use();
	updateWorldMatrix(objectShader);
	updateDayNight(objectShader);
	camera.uniformViewMatrix(objectShader);
	camera.uniformPosition(objectShader);
	scene.uniformObjects(objectShader);
	scene.displayObjects(objectShader, season);

	skyboxShader.use();
	updateWorldMatrix(skyboxShader);
	updateDayNight(skyboxShader);
	camera.uniformViewMatrix(skyboxShader);
	camera.uniformPosition(skyboxShader);
	skybox.setUniform(skyboxShader);
	skybox.display();

	lightShader.use();
	updateWorldMatrix(lightShader);
	camera.uniformViewMatrix(lightShader);
	camera.uniformPosition(lightShader);
	scene.uniformLight(lightShader);
	scene.displayLight();

	/*
	normalShader.use();
	updateWorldMatrix(normalShader);
	camera.uniformViewMatrix(normalShader);
	scene.displayObjects(normalShader);
	//*/

	glFlush();
	glutPostRedisplay();
}

/**
 * Initializes resources (vertices, shaders) which will later be displayed or
 * used for rendering.
 */
void initResources() {
	objectShader.init("src/shaders/object.vs", "src/shaders/object.frag");
	lightShader.init("src/shaders/light.vs", "src/shaders/light.frag");
	skyboxShader.init("src/shaders/skybox.vs", "src/shaders/skybox.frag");

	normalShader.init(
		"src/shaders/normals.vs", "src/shaders/normals.frag",
		"src/shaders/normals.gs"
	);

	objectShader.use();
	updateRenderStyle(0);
	screen.uniformProjectionMatrix(objectShader);
	updateMaxDayNight(objectShader);

	lightShader.use();
	screen.uniformProjectionMatrix(lightShader);

	skyboxShader.use();
	screen.uniformProjectionMatrix(skyboxShader);
	updateMaxDayNight(skyboxShader);

	normalShader.use();
	screen.uniformProjectionMatrix(normalShader);

//	Object3D obj1(
//		"res/obj/apple.obj",
//		glm::vec3(0.0f),
//		"res/textures/wool.jpg"
//	);
//	Object3D obj2(
//		"res/obj/apple.obj",
//		glm::vec3(1.0f, 0.0f, 0.0f),
////		"res/textures/wallHappy.jpg"
//		glm::vec3(1.0f, 0.0f, 0.0f)
//	);

//	scene.add(obj1);
//	scene.add(obj2);

	glClearColor(0.4, 0.4, 0.4, 0);
}

/**
 * Initializes GLUT (window & callbacks), GLEW (extensions), and some OpenGL
 * features.
 */
void initLibraries(int argc, char* argv[]) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitContextVersion (3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("OpenGL Application");
	glutReshapeWindow(screen.getWidth(), screen.getHeight());

	// GLUT events callbacks
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard2);
	glutMotionFunc(mousedrag);
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);
}

int main(int argc, char* argv[]) {
	initLibraries(argc, argv);

	light = Light(
		"res/obj/apple.ctm",
		glm::vec3(0.0f, 1.5f, 0.0f),
		1.0f,
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
	light.setScale(0.2);
	skybox = Skybox(glm::vec3(0.0f, 4.0f, 0.0f));
	scene = Scene(0.1, light);
	initResources();

	startTime = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();

	return EXIT_SUCCESS;
}
