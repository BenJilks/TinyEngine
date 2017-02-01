#pragma once
#include "TinyRender.h"

/* Render and create a world of objects */
namespace TinyEngine
{
	class TinyWorld
	{
	public:
		TinyWorld(unsigned int width, unsigned int height);
		
		/* Render the world to the screen */
		void RenderWorld();

		/* Add a render call to the static renderer */
		void AddStaticRenderCall(TinyRenderCall* call, TinyRenderCall* parent) { m_static_renderer->AddRenderCall(call, parent); }

		/* Getters */
		TinyWorldTransform* getCamera() { return m_camera; };

		/* Load assets and render calls from a file */
		void LoadLVLFile(string file_path);

		/* Updates the whole transform tree */
		void UpdateFullTranformTree();

		~TinyWorld();
	private:
		TinyStaticRenderer* m_static_renderer;
		TinyWorldTransform* m_camera;
		vector<TinyModel*> m_models;

		Mat4f m_projection_matrix;
	};
}
