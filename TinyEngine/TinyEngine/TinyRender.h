#pragma once
#include "TinyShader.h"
#include "TinyAssets.h"
#include "TinyWorldTransform.h"

/* Render all the different types of surfaces in the world */
namespace TinyEngine
{
	/* Store information about a render call */
	struct TinyRenderCall
	{
		TinyRenderCall(TinyModel* model, TinyTexture* texture, TinyWorldTransform* transform) :
			m_model(model), m_texture(texture), m_tranform(transform) {}
		TinyModel* m_model;
		TinyTexture* m_texture;
		TinyWorldTransform* m_tranform;

		TinyTranformNode* m_node;
	};

	template<class Derived>
	class TinyRender
	{
	public:
		TinyRender() { m_transform_tree = new TinyTranformNode(NULL, NULL); };

		/* Add a new render call to be rendered to the screen every frame */
		void AddRenderCall(TinyRenderCall* call, TinyRenderCall* parent)
		{
			/* Get the parent node */
			TinyTranformNode* parent_node = m_transform_tree;
			if (parent != NULL) parent_node = parent_node->SearchForChild(parent->m_tranform);

			/* Add the new node to the parent */
			TinyTranformNode* node = new TinyTranformNode(parent_node, call->m_tranform);
			parent_node->AddChild(node);

			/* Add the call to the rendering */
			call->m_node = node;
			m_render_calls.push_back(call);
		}

		/* Render the world to the screen */
		void RenderSceene(TinyWorldTransform* camera, Mat4f projection_matrix) 
		{ 
			static_cast<Derived*>(this)->RenderSceeneImp(camera, projection_matrix); 
		}

		/* Update initial transform tree */
		inline void UpdateFullTranformTree() { m_transform_tree->UpdateBranchTransforms(); };

		/* Sets the transform tree to a different layout */
		inline void SetTransformTree(TinyTranformNode* tree) { m_transform_tree = tree; }

		~TinyRender() { delete m_shader; delete m_transform_tree; };
	protected:
		TinyShader* m_shader;
		vector<TinyRenderCall*> m_render_calls;
		TinyTranformNode* m_transform_tree;
	};

	class TinyStaticRenderer : public TinyRender<TinyStaticRenderer>
	{
	public:
		TinyStaticRenderer();
		
		/* Render the world to the screen */
		void RenderSceeneImp(TinyWorldTransform* camera, Mat4f projection_matrix);
	};

}
