#include "TinyRender.h"
#include <iostream>
using namespace std;
using namespace TinyEngine;

#define MVP_LOCATION 0
TinyStaticRenderer::TinyStaticRenderer()
{
	m_shader = new TinyShader("static.vs", "static.fs");
	m_shader->BindAttribute("position", 0);
	m_shader->BindAttribute("textureCoord", 1);
	m_shader->BindAttribute("normal", 2);

	m_shader->AllocateLocations(1);
	m_shader->SetLocation("mvp", MVP_LOCATION);
}

/* Render the world to the screen */
void TinyStaticRenderer::RenderSceeneImp(TinyWorldTransform* camera, Mat4f projection_matrix)
{
	m_shader->BindShader();
	for (TinyRenderCall* call : m_render_calls)
	{
		TinyModel* model = call->m_model;
		TinyTexture* texture = call->m_texture;
		TinyWorldTransform* transform = call->m_tranform;
		glBindVertexArray(model->getModelID());
		texture->Bind();

		Mat4f tranformation_matrix = transform->GetTransformationMatrix();
		Mat4f view_matrix = camera->GetTransformationMatrix();
		Mat4f mvp = tranformation_matrix * view_matrix * projection_matrix;
		m_shader->LoadMatrix(MVP_LOCATION, mvp);

		glDrawElements(GL_TRIANGLES, model->getModelLength(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
	TinyShader::UnbindAll();
}