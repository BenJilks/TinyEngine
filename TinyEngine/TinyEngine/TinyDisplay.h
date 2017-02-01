#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "TinyMaths.h"
using namespace std;

namespace TinyEngine
{
	/* Create and handle a GLFW window and inputs */
	class TinyDisplay
	{
	public:
		TinyDisplay(unsigned int width, unsigned int height, string title, bool is_fullscreen = false);

		/* Initializes the display library */
		static void Init();

		/* Update the display */
		void Update();

		/* Sets the clear colour of the screen */
		void SetClearColour(Colour colour);

		/* Returns if a key is down */
		bool IsKeyDown(unsigned int key);

		/* Returns the mouses position as a 2D vector */
		Vec2f GetMousePosition();

		/* Returns if the window should be closed */
		int ShouldWindowClose();

		/* Getters */
		unsigned int GetWidth() { return m_width; }
		unsigned int GetHeight() { return m_height; }

		/* Returns the resolution of the primary monitor */
		static Vec2i GetMonitorSize();

		void Close() { m_should_close = true; }
	private:
		GLFWwindow* m_window;
		string m_title;
		unsigned int m_width, m_height;
		bool m_should_close;
	};
}
