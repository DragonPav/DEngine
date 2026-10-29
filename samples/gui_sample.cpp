#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
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
#include "BMFontManager.h"
#include "GuiText.h"
GLFWwindow* window = nullptr;
int width, height;
bool saveScreenshot() {
	std::vector<unsigned char> frameBuffer(width * height * 4);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.data());
	stbi_flip_vertically_on_write(1);
	int stride = width * 4;
	return stbi_write_png("screenshot.png", width, height, 4, frameBuffer.data(), stride);
}
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
	std::ifstream vertexG("res\\vertex_gui.glsl");
	std::ifstream fragmentG("res\\fragment_gui.glsl");
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
	//--CAMERA--
	RenderUtils re(&program, &li, window);
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	Camera cam(&re);
	cam.setCameraPos(Vector3(0.5f, 2, 2));
	cam.lookAt(Vector3(0, 0, 0));
	//--OBJECTS--
	Texture cat(&program, "res\\out.png", Texture::Config());
	Texture buttonN({0, 0xff, 0, 0xff}, &programG, Texture::Config());
	std::unique_ptr<Object3D> box = ObjectCreator::createBox(Vector3(0, 0.5f, 0), Vector3(1, 1, 1), &cam, &cat);
	box->setup();
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
	std::unique_ptr<Sound> click = am.createSound("res\\mouse-click-classic.mp3");
	std::shared_ptr<Sound> click2 = std::move(click);
	//--GUI--
	Camera orthoCam(&re);
	std::shared_ptr<GuiButton> button = std::make_shared<GuiButton>(Vector2(75, 40), Vector2(150, 80), &orthoCam, &buttonN, [click2]() {
		click2->play();
		if (!saveScreenshot()) {
			std::cerr << "Screenshot not saved" << std::endl;
		}
	});
	BMFontManager fontManager("res\\Main.fnt", "res\\Main.png", &programG);
	std::shared_ptr<GuiText> text = std::make_shared<GuiText>(L"Сделать скриншот", Vector2(0, 0), Vector2(0.2f, 0.2f), &orthoCam, &fontManager);
	text->setTintColor(Vector4(1, 0, 0, 1));
	GuiScreen gui(&re, &input, Vector2(width, height));
	gui.addButton(button);
	gui.addElement(text);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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