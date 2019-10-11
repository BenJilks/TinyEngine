#include "TinyWorld.h"
#include <iostream>
#include <memory.h>
#include <map>
using namespace std;
using namespace TinyEngine;

TinyWorld::TinyWorld(unsigned int width, unsigned int height)
{
	m_camera = new TinyWorldTransform();
	m_static_renderer = new TinyStaticRenderer();

	m_projection_matrix = CreatePerspectiveMatrix(90.0f, 0.1f, 1000.0f, (float)width / (float)height);
}

/* Updates the whole transform tree */
void TinyWorld::UpdateFullTranformTree()
{
	m_static_renderer->UpdateFullTranformTree();
}

/* Render the world to the screen */
void TinyWorld::RenderWorld()
{
	m_camera->UpdateCameraTransforms();
	m_static_renderer->RenderSceene(m_camera, m_projection_matrix);
}

/* Read a string from a binary file */
string ReadString(string data, unsigned int& i)
{
	/* Get the strings length */
	unsigned int length = data[i++];
	i += length;

	/* Get the string and return it */
	string out = data.substr(i - length, length);
	return out;
}
/* Read a float from a binary file */
float ReadFloat(string data, unsigned int& i)
{
	float f;
	char b[] = { data[i + 3], data[i + 2], data[i + 1], data[i] }; i += 4;
	memcpy(&f, &b, sizeof(f));
	return f;
}
/* Read a short from a binary file */
short ReadShort(string data, unsigned int& i)
{
	short* s_pointer = (short*)new char[2]{ data[i + 1], data[i] }; i += 2;
	short s = *s_pointer;

	delete s_pointer;
	return s;
}

/* Reads a vector from a binary file */
Vec3f ReadVector(string data, unsigned int& i)
{
	Vec3f vector;
	vector.x(ReadFloat(data, i));
	vector.y(ReadFloat(data, i));
	vector.z(ReadFloat(data, i));
	return vector;
}
/* Reads a tree index from a binary file */
vector<unsigned int> ReadTreeIndex(string data, unsigned int& i)
{
	vector<unsigned int> tree_index;
	unsigned int length = data[i++];
	for (unsigned int j = 0; j < length; j++)
		tree_index.push_back(ReadShort(data, i));
	return tree_index;
}
/* Load assets and render calls from a file */
void TinyWorld::LoadLVLFile(string file_path)
{
	TinyTexture* test_texture = TinyTexture::LoadPNGImage("uv.png");

	string data = GetRawFileData(file_path);
	unsigned int i = 0;

	/* Read header info */
	unsigned int version = data[i++];
	string level_name = ReadString(data, i);

	/* Read model map */
	map<unsigned int, TinyModel*> model_map;
	unsigned int model_length = data[i++];
	for (unsigned int j = 0; j < model_length; j++)
	{
		string name = ReadString(data, i);
		unsigned int id = data[i++];
		cout << id << ": " << name << endl;

		TinyModel* model = TinyModel::loadObjModel(name);
		m_models.push_back(model);
		model_map[id] = model;
	}
	cout << endl;

	/* Read render calls */
	unsigned int render_call_length = data[i++];
	TinyTranformNode* transform_tree = new TinyTranformNode(NULL, NULL);
	for (unsigned int j = 0; j < render_call_length; j++)
	{
		Vec3f position = ReadVector(data, i);
		Vec3f rotation = ReadVector(data, i);
		Vec3f scale = ReadVector(data, i);
		string name = ReadString(data, i);
		TinyWorldTransform* transform = new TinyWorldTransform(position, rotation, scale);

		unsigned int id = data[i++];
		vector<unsigned int> tree_index = ReadTreeIndex(data, i);
		unsigned int model_id = data[i++];
		unsigned int texture_id = data[i++];

		TinyTranformNode* current_node = transform_tree;
		for (int node_index : tree_index) {
			TinyTranformNode* next_node = current_node->GetChild(node_index);
			while (next_node == NULL) {
				next_node = new TinyTranformNode(current_node, NULL);
				current_node->AddChild(next_node);
				next_node = current_node->GetChild(node_index);
			}
			current_node = next_node;
		}
		current_node->SetTransform(transform);

		TinyModel* model = model_map[model_id];
		TinyRenderCall* render_call = new TinyRenderCall(model, test_texture, transform);
		AddStaticRenderCall(render_call, NULL);
	}
	m_static_renderer->SetTransformTree(transform_tree);
}

TinyWorld::~TinyWorld()
{
	for (TinyModel* model : m_models)
		delete model;
	delete m_static_renderer;
	delete m_camera;
}
