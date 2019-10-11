#include "TinyShader.h"
#include "TinyAssets.h"
using namespace TinyEngine;

/* Load a shader from a file */
template<GLuint shader_type>
GLuint loadShader(string file_path)
{
	/* Read the shader source from a file */
	string source_code = GetRawFileData(file_path);
	GLuint shader_id = glCreateShader(shader_type);

	/* Link the source to the shader */
	char* raw_code = (char*)source_code.c_str();
	glShaderSource(shader_id, 1, &raw_code, NULL);
	glCompileShader(shader_id);

	/* Return the shader id */
	return shader_id;
}

TinyShader::TinyShader(string vertex_path, string fragment_path)
{
	/* Create the vertex and the fragment shader and link to the shader program */
	GLuint vertex_shader_id = loadShader<GL_VERTEX_SHADER>(vertex_path);
	GLuint fragment_shader_id = loadShader<GL_FRAGMENT_SHADER>(fragment_path);
	GLuint shader_program_id = glCreateProgram();
	glAttachShader(shader_program_id, vertex_shader_id);
	glAttachShader(shader_program_id, fragment_shader_id);
	glLinkProgram(shader_program_id);
	glValidateProgram(shader_program_id);

	m_shader_vertex_id = vertex_shader_id;
	m_shader_fragment_id = fragment_shader_id;
	m_shader_id = shader_program_id;
}

/* Bind an attribute to a location in the shader */
void TinyShader::BindAttribute(string name, unsigned int id)
{
	glBindAttribLocation(m_shader_id, id, name.c_str());
}

/* Allocate new uniform location */
void TinyShader::SetLocation(string name, unsigned int location)
{
	m_uniforms[location] = glGetUniformLocation(m_shader_id, name.c_str());
}

/* Starts using the shader to render models to the screen */
void TinyShader::BindShader()
{
	glUseProgram(m_shader_id);
}

TinyShader::~TinyShader()
{
	glDeleteShader(m_shader_vertex_id);
	glDeleteShader(m_shader_fragment_id);
	glDeleteProgram(m_shader_id);
}