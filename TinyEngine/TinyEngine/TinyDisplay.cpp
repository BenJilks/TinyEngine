#include "TinyDisplay.h"
using namespace TinyEngine;

/* Initializes the display library */
void TinyDisplay::Init()
{
	glfwInit();
}

TinyDisplay::TinyDisplay(unsigned int width, unsigned int height, string title, bool is_fullscreen)
{
	/* Set global values */
	m_width = width;
	m_height = height;
	m_title = title;
	m_should_close = false;

	/* Create GLFW window and initialize OpenGL */
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWmonitor* monitor = nullptr;
	if (is_fullscreen)
		monitor = glfwGetPrimaryMonitor();

	m_window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
	glfwMakeContextCurrent(m_window);
	gladLoadGL();

	glfwSwapInterval(1);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Enable OpenGL settings */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

/* Update the display */
void TinyDisplay::Update()
{
	/* Poll input from the window */
	glfwPollEvents();

	/* Swap the screen buffers */
	glfwSwapBuffers(m_window);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/* Returns if a key is down */
bool TinyDisplay::IsKeyDown(unsigned int key)
{
	return glfwGetKey(m_window, key);
}

/* Returns the mouses position as a 2D vector */
Vec2f TinyDisplay::GetMousePosition()
{
	double x, y;
	glfwGetCursorPos(m_window, &x, &y);
	return Vec2f(x, y);
}

/* Sets the clear colour of the screen */
void TinyDisplay::SetClearColour(Colour colour)
{
	glClearColor(colour.r(), colour.g(), colour.b(), colour.a());
}

/* Returns if the window should be closed */
int TinyDisplay::ShouldWindowClose()
{
	return glfwWindowShouldClose(m_window) || m_should_close;
}

/* Returns the resolution of the primary monitor */
Vec2i TinyDisplay::GetMonitorSize()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	return Vec2i(mode->width, mode->height);
}