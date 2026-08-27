#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>
#include "Program.h"
#include "Object3D.h"
#include "Camera.h"
#include "ObjectCreator.h"
#include "Input.h"
#include "CameraControl.h"
#include "AudioManager.h"
#include "GuiScreen.h"
GLFWwindow* window = nullptr;
int main() {
	//--GLFW INIT--
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(1080, 720, "gui_sample", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return 2;
	}
	//--SHADERS--
	//-Main
	std::ifstream vertex("res\\vertex.glsl");
	std::ifstream fragment("res\\fragment.glsl");
	Shader vert(&vertex, Shader::Type::Vertex);
	vert.compile();
	Shader frag(&fragment, Shader::Type::Fragment);
	frag.compile();
	Program program(&vert, &frag);
	if (!program.link()) {
		return 3;
	}
	//-GUI
	std::ifstream vertexG("res\\vertex_nolight.glsl");
	std::ifstream fragmentG("res\\fragment_nolight.glsl");
	Shader vertG(&vertexG, Shader::Type::Vertex);
	vertG.compile();
	Shader fragG(&fragmentG, Shader::Type::Fragment);
	fragG.compile();
	Program programG(&vertG, &fragG);
	if (!programG.link()) {
		return 3;
	}
	glUseProgram(program.getId());
	//--LIGHT--
	Lighting li(Vector3(0.5f, 2, 2), &program);
	Vector3& spec = li.dirLight.getSpecular();
	spec.setX(0);
	spec.setY(0);
	spec.setZ(0);
	Vector3 dir = li.dirLight.getDirection();
	dir.setX(1.0f);
	dir.setY(-2.0f);
	dir.setZ(-1.0f);
	Vector3& pos = li.pointLights[0]->getPosition();
	pos.setX(2048);
	pos.setY(2048);
	pos.setZ(2048);
	//--CAMERA--
	RenderUtils re(&program, &li, window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	Camera cam(&re);
	cam.setCameraPos(Vector3(0.5f, 2, 2));
	cam.lookAt(Vector3(0, 0, 0));
	//--OBJECTS--
	Texture cat(&program, "res\\out.png", Texture::Config());
	Texture buttonN({0, 0xff, 0, 0xff}, &program, Texture::Config());
	Texture buttonH({0, 0xdd, 0, 0xff}, &program, Texture::Config());
	Texture buttonP({0, 0xaa, 0, 0xff}, &program, Texture::Config());
	std::unique_ptr<Object3D> box = ObjectCreator::createBox(Vector3(0, 0.5f, 0), Vector3(1, 1, 1), &cam, &cat);
	cam.perspective(80, (float) width / (float) height, 0.1f, 100.0f);
	//--INPUT--
	Input input(window);
	input.addKeyboardCallback([](int key, int action) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	});
	CameraControl cc(&input);
	cam.setCameraControl(&cc);
	//--SOUND--
	AudioManager am;
	cam.setAudioManager(&am);
	Sound* click = am.createSound("res\\mouse-click-classic.mp3");
	//--GUI--
	Camera orthoCam(&re);
	std::shared_ptr<GuiButton> button = std::make_shared<GuiButton>(Vector2(75, 40), Vector2(150, 80), &orthoCam, &buttonN, [click]() {
		click->play();
	});
	button.get()->hoverTexture = &buttonH;
	button.get()->pressedTexture = &buttonP;
	GuiScreen gui(&re, &input, Vector2(width, height));
	gui.addButton(button);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(program.getId());
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		box->begin();
		box->render(&program, false);
		cam.updateFrustum();

		gui.draw(&programG);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}