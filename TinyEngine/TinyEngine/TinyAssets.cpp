#include "TinyAssets.h"
#include "TinyMaths.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <streambuf>

#define ASSET_LOCATION "assets.pak"
#ifndef ASSET_LOCATION

/* Load a file as raw data */
string TinyEngine::GetRawFileData(string file_path)
{
	ifstream file;
	file.open("Assets/" + file_path, ios_base::out | ios_base::binary);

	stringstream str_stream;
	str_stream << file.rdbuf();
	return str_stream.str();
}

void TinyEngine::InitAssets() {}

#else

/* Store assets a one large string */
string asset_buffer;

/* Load a file as raw data */
string TinyEngine::GetRawFileData(string file_path)
{
	unsigned int pos = 0;
	for (unsigned int i = 0; i < 10; i++)
	{
		unsigned int name_length = asset_buffer[pos++] & 0xFF;
		string name = asset_buffer.substr(pos, name_length);
		pos += name_length;

		if (name == file_path)
		{
			unsigned int file_pointer = ((asset_buffer[pos] & 0xFF) << 24) | ((asset_buffer[pos+1] & 0xFF) << 16) | ((asset_buffer[pos+2] & 0xFF) << 8) | (asset_buffer[pos+3] & 0xFF);
			unsigned int file_length = ((asset_buffer[pos+4] & 0xFF) << 24) | ((asset_buffer[pos+5] & 0xFF) << 16) | ((asset_buffer[pos+6] & 0xFF) << 8) | (asset_buffer[pos+7] & 0xFF);
			return asset_buffer.substr(file_pointer, file_length);
		}
		pos += 8;
	}
	return "";
}

/* Load asset file if release assets are being loaded */
void TinyEngine::InitAssets()
{
	ifstream file;
	file.open(ASSET_LOCATION, ios_base::out | ios_base::binary);

	stringstream str_stream;
	str_stream << file.rdbuf();
	asset_buffer = str_stream.str();
}

#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;
using namespace TinyEngine;

/* Load data to buffer */
void LoadToBuffer(unsigned int index, vector<float> data, unsigned int unit_size, GLuint vbo_list[1])
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_list[index]);
	glEnableVertexAttribArray(index);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(index, unit_size, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* Load indices to buffer */
void LoadIndices(unsigned int index, vector<unsigned int> indices, GLuint* vbo_list)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_list[index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

/* Create full 3D model */
TinyModel::TinyModel(vector<float> positions, vector<float> textures, vector<float> normals, vector<unsigned int> indices)
{
	GLuint vbo_list[4];
	GLuint model_id;
	glGenVertexArrays(1, &model_id);
	glBindVertexArray(model_id);

	glGenBuffers(4, vbo_list);
	LoadToBuffer(0, positions, 3, vbo_list);
	LoadToBuffer(1, textures, 2, vbo_list);
	LoadToBuffer(2, normals, 3, vbo_list);
	LoadIndices(3, indices, vbo_list);
	glBindVertexArray(0);

	m_model_id = model_id;
	m_model_length = (unsigned int)indices.size();
	m_vbo_list = vbo_list;
}

/* Delete data from GPU */
TinyModel::~TinyModel()
{
	glDeleteBuffers(4, m_vbo_list);
	glDeleteVertexArrays(1, &m_model_id);
}



/* Returns a vector from a string */
template<unsigned int v, typename T>
TinyVector<v, T> GetVectorFromString(string str, const char split, TinyVector<v, T> out_vector)
{
	unsigned int current_pos = 0;
	string current_value = "";
	for (unsigned int i = 0; i < v; i++)
	{
		while (str[current_pos] != split && current_pos <= str.length())
			current_value += str[current_pos++];
		current_pos += 1;
		out_vector.Set(i, stof(current_value));
		current_value = "";
	}

	return out_vector;
}

/* Splits a string by a character */
vector<string> Split(string str, const char split)
{
	vector<string> out_vector;

	unsigned int current_pos = 0;
	string current_value = "";
	while (current_pos <= str.length())
	{
		if (str[current_pos] == split)
		{
			out_vector.push_back(current_value);
			current_value = "";
			current_pos += 1;
			continue;
		}
		current_value += str[current_pos++];
	}
	out_vector.push_back(current_value);

	return out_vector;
}

/* Tests if a string starts with another */
int StartsWith(char* line, const char* with, unsigned int length)
{
	/* Create substring */
	char* substring = (char*)malloc(length + 1);
	memcpy((void*)substring, line, length);
	substring[length] = '\0';

	/* Compare strings */
	int resault = strcmp(substring, with);
	delete[] substring;
	return !resault;
}

/* Store data for a vertex */
struct Vertex
{
	Vertex() {}
	Vertex(Vec3f position) : m_position(position), 
		m_texture_index(-1), m_normal_index(-1) {};
	Vec3f m_position;
	int m_texture_index;
	int m_normal_index;
};

inline TinyModel* FinalizeModel(vector<Vertex> vertices, vector<Vec2f> textures, vector<Vec3f> normals, vector<unsigned int> indices)
{
	/* Convert the data into arrays compatible with OpenGL */
	vector<float> positionsArray = vector<float>(vertices.size() * 3);
	vector<float> texturesArray = vector<float>(vertices.size() * 2);
	vector<float> normalsArray = vector<float>(vertices.size() * 3);
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		Vertex vertex = vertices[i];
		Vec3f position = vertex.m_position;
		Vec2f texture = textures[vertex.m_texture_index];
		Vec3f normal = normals[vertex.m_normal_index];
		positionsArray[(i * 3) + 0] = position.x();
		positionsArray[(i * 3) + 1] = position.y();
		positionsArray[(i * 3) + 2] = position.z();
		texturesArray[(i * 2) + 0] = texture.x();
		texturesArray[(i * 2) + 1] = texture.y();
		normalsArray[(i * 3) + 0] = normal.x();
		normalsArray[(i * 3) + 1] = normal.y();
		normalsArray[(i * 3) + 2] = normal.z();
	}

	return new TinyModel(positionsArray, texturesArray, normalsArray, indices);
}

/* Load a model from an obj file */
TinyModel* TinyModel::loadObjModel(string file_path)
{
	string raw_data = GetRawFileData(file_path);
	vector<TinyModel*> out_models;

	vector<Vertex> vertices;
	vector<Vec2f> textures;
	vector<Vec3f> normals;
	vector<unsigned int> indices;

	/* Process every line in the file */
	char* next_token = NULL;
	char* delims = "\n";
	char* line = strtok((char*)raw_data.c_str(), delims);
	char p1[50], p2[50], p3[50];

	while (line != NULL)
	{
		if (StartsWith(line, "vt", 2))
		{
			sscanf(line, "vt %s %s", p1, p2);
			textures.push_back(Vec2f(atof(p1), atof(p2)));
		}
		else if (StartsWith(line, "vn", 2))
		{
			sscanf(line, "vn %s %s %s", p1, p2, p3);
			normals.push_back(Vec3f(atof(p1), atof(p2), atof(p3)));
		}
		else if (StartsWith(line, "v", 1))
		{
			sscanf(line, "v %s %s %s", p1, p2, p3);
			vertices.push_back(Vertex(Vec3f(atof(p1), atof(p2), atof(p3))));
		}
		else if (StartsWith(line, "f", 1))
		{
			/* Split the face into three vertices */
			sscanf(line, "f %s %s %s", p1, p2, p3);
			for (unsigned int i = 0; i < 3; i++)
			{
				char face[50];
				if (i == 0) memcpy(face, p1, 50);
				else if (i == 1) memcpy(face, p2, 50);
				else if (i == 2) memcpy(face, p3, 50);

				Vec3f values = GetVectorFromString(face, '/', Vec3f());
				Vertex vertex = vertices[(unsigned int)values.x() - 1];
				if (vertex.m_texture_index != -1 && vertex.m_normal_index != -1)
				{
					vertex.m_texture_index = (unsigned int)values.y() - 1;
					vertex.m_normal_index = (unsigned int)values.z() - 1;
					vertices[(unsigned int)values.x() - 1] = vertex;
				
					/* Add this vertex to be rendered */
					indices.push_back((unsigned int)values.x() - 1);
				}
				else
				{
					/* Create new vertex */
					Vertex vertex(vertex.m_position);
					vertex.m_texture_index = (unsigned int)values.y() - 1;
					vertex.m_normal_index = (unsigned int)values.z() - 1;
					vertices.push_back(vertex);

					/* Add this vertex to be rendered */
					indices.push_back((unsigned int)vertices.size() - 1);
				}
			}
		}

		line = strtok(NULL, delims);
	}

	/* Create and return a new model with the data calculated */
	return FinalizeModel(vertices, textures, normals, indices);
}

/* Converts a c array into a c++ vector */
template<typename T>
vector<T> ToVector(T* c_array, unsigned int size)
{
	vector<T> out_vector(size);
	for (unsigned int i = 0; i < size; i++)
		out_vector[i] = c_array[i];

	delete[] c_array;
	return out_vector;
}

TinyTexture::TinyTexture(unsigned int width, unsigned int height, unsigned char* image_data)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] image_data;
	m_texture_id = texture_id;
}

/* Bind this texture to be used */
void TinyTexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
}

/* Load a PNG image from a file */
TinyTexture* TinyTexture::LoadPNGImage(string file_path)
{
	int width, height, channels;
	string raw_data = GetRawFileData(file_path);
	unsigned char* image_data = stbi_load_from_memory((const stbi_uc*)raw_data.c_str(), 
		(int)raw_data.length(), &width, &height, &channels, 4);

	return new TinyTexture(width, height, image_data);
}

TinyTexture::~TinyTexture()
{
	glDeleteTextures(1, &m_texture_id);
}
