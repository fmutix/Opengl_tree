#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>

#include <unistd.h>
#include <execinfo.h>
#include <signal.h>

#include "Shader.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Screen.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "Object3D.hpp"

glm::mat4 world(1.0f);

Camera camera(
	glm::vec3(0.0f, 0.0f, 3.0f), // position
	glm::vec3(0.0f, 1.0f, 0.0f), // up
	glm::vec3(0.0f, 0.0f, -1.0f) // forward
);

Screen screen(
	glm::radians(45.0f), // fovy
	600,  // width
	400,  // height
	0.2f, // zNear
	6000  // zFar
);

Light light;

Scene scene;

int button_pressed = 0; // 1 if a button is currently being pressed.
int GLUTmouse[2] = { 0, 0 };

GLuint renderStyle;

Shader shader;
Shader normalShader;

void updateRenderStyle(GLuint value) {
	renderStyle = value;
	shader.setUniform("renderStyle", value);
}

void updateWorldMatrix(Shader& shader) {
	shader.setUniform("world", world);
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

	float vx = (float) dx / screen.getWidth();
	float vy = (float) dy / screen.getHeight();
	glm::vec3 axis = camera.rotate(vx, vy);
	scene.rotateLight(vx, vy, axis);
	
	glutPostRedisplay();
}

void mouseWheel(int, int dir, int, int) {
	if (dir > 0) {
		camera.moveForwardBackward(0.02f);
	}
	else {
		camera.moveForwardBackward(-0.02f);
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
		shader.use();
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
	case 'p':
		world = glm::rotate(world, 0.05f, glm::vec3(0.f, -0.05f, 0.f));
		break;
	case 'm':
		world = glm::rotate(world, 0.05f, glm::vec3(0.05f, 0.f, 0.f));
		break;
	case 'r':
		camera.reset();
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
		camera.moveForwardBackward(0.1f);
		break;
	case GLUT_KEY_DOWN:
		camera.moveForwardBackward(-0.1f);
		break;
	case GLUT_KEY_LEFT:
		camera.moveLeftRight(0.1f);
		break;
	case GLUT_KEY_RIGHT:
		camera.moveLeftRight(-0.1f);
		break;
	}
	glutPostRedisplay();
}

void reshape(int width, int height) {
	screen.reshape(width, height);

	shader.use();
	screen.uniformProjectionMatrix(shader);

	normalShader.use();
	screen.uniformProjectionMatrix(normalShader);
}

/**
 * Main loop function.
 */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();
	updateWorldMatrix(shader);
	camera.uniformViewMatrix(shader);
	camera.uniformPosition(shader);
	scene.uniform(shader);
	scene.display(shader);

	/*
	normalShader.use();
	updateworldMatrix(normalShader);
	camera.uniformViewMatrix(normalShader);
	scene.display();
	//*/

	glFlush();
}

/**
 * Initializes resources (vertices, shaders) which will later be displayed or
 * used for rendering.
 */
void initResources() {
	shader.init("src/shaders/light.vs", "src/shaders/light.frag");
	
	normalShader.init(
		"src/shaders/normals.vs", "src/shaders/normals.frag",
		"src/shaders/normals.gs"
	);

	shader.use();
	updateRenderStyle(0);
	screen.uniformProjectionMatrix(shader);

	normalShader.use();
	screen.uniformProjectionMatrix(normalShader);

//	Object3D obj1("res/obj/apple.obj", glm::vec3(0.0f), glm::vec3(1.0f));
	Object3D obj1("res/obj/apple.obj", glm::vec3(0.0f), "res/textures/wool.jpg");
	Object3D obj2("res/obj/apple.obj", glm::vec3(1.0f, 0.0f, 0.0f), "res/textures/wallHappy.jpg");

	scene.add(obj1);
	scene.add(obj2);
	
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
	glDepthFunc(GL_LESS) ;
	glEnable(GL_MULTISAMPLE);
}

int main(int argc, char* argv[]) {
	initLibraries(argc, argv);
	
	light = Light("res/obj/cube.obj", glm::vec3(0.5f, 0.5f, 0.5f), 2.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	scene = Scene(0.1, light);
	initResources();

	glutMainLoop();

	return EXIT_SUCCESS;
}
