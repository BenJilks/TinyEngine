#pragma once
#include <glad/glad.h>
#include <string>
#include "TinyMaths.h"
using namespace std;

/* Create and handle OpenGL shaders */
namespace TinyEngine
{
	class TinyShader
	{
	public:
		TinyShader() {};
		TinyShader(string vertex_path, string fragment_path);

		/* Bind an attribute to a location in the shader */
		void BindAttribute(string name, unsigned int id);

		/* Allocate an amount of data for string locations */
		void AllocateLocations(unsigned int size) { m_uniforms = new GLuint[size]; };

		/* Allocate new uniform location */
		void SetLocation(string name, unsigned int location);

		/* Load data to shader */
		void LoadMatrix(unsigned int location, Mat4f matrix) { glUniformMatrix4fv(m_uniforms[location], 1, GL_TRUE, matrix.m); };
		void LoadVector(unsigned int location, Vec3f vector) { glUniform3f(m_uniforms[location], vector.x(), vector.y(), vector.z()); }

		/* Starts using the shader to render models to the screen */
		void BindShader();

		~TinyShader();

		/* Stop using all shaders */
		static void UnbindAll() { glUseProgram(0); };
	private:
		GLuint m_shader_id;
		GLuint m_shader_vertex_id;
		GLuint m_shader_fragment_id;
		GLuint* m_uniforms;
	};
}
