#pragma once
#include "glad.h"
#include <vector>
#include <string>
using namespace std;

/* Models */
namespace TinyEngine
{
	class TinyModel
	{
	public:
		/* Create full 3D model */
		TinyModel(vector<float> positions, vector<float> textures, vector<float> normals, vector<unsigned int> indices);

		/* Delete data from GPU */
		~TinyModel();

		/* Load a model from an obj file */
		static TinyModel* loadObjModel(string file_path);

		/* Returns the models OpenGL id */
		inline GLuint getModelID() { return m_model_id; }

		/* Returns the length of the model */
		inline unsigned int getModelLength() { return m_model_length; }
	private:
		GLuint m_model_id;
		unsigned int m_model_length;
		GLuint* m_vbo_list;
	};

	string GetRawFileData(string file_path);
	void InitAssets();
}

/* Textures */
namespace TinyEngine
{
	class TinyTexture
	{
	public:
		TinyTexture(unsigned int width, unsigned int height, unsigned char* image_data);

		/* Load a PNG image from a file */
		static TinyTexture* LoadPNGImage(string file_path);

		/* Bind this texture to be used */
		void Bind();

		/* Unbind all texture that are in use */
		static void UnbindAll() { glBindTexture(GL_TEXTURE_2D, 0); }

		~TinyTexture();
	private:
		GLuint m_texture_id;
	};
}
