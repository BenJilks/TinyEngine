#include "TinyWorldTransform.h"
#include <iostream>
using namespace std;
using namespace TinyEngine;

/* Update the transformation cache with that values stored */
void TinyWorldTransform::UpdateTransformation(TinyWorldTransform* parent)
{
	Mat4f tranformation_matrix = Mat4f(1.0f);
	tranformation_matrix = tranformation_matrix * ScaleMatrix(Mat4f(1.0f), m_scale);
	tranformation_matrix = tranformation_matrix * RotateMatrix(Mat4f(1.0f), m_rotation);
	tranformation_matrix = tranformation_matrix * TranslateMatrix(Mat4f(1.0f), m_position);
	
	if (parent != NULL)
		tranformation_matrix = parent->m_transformation_matrix * tranformation_matrix;
	m_transformation_matrix = tranformation_matrix;
}

/* Update the transformations for a camera */
void TinyWorldTransform::UpdateCameraTransforms()
{
	Mat4f tranformation_matrix = TranslateMatrix(Mat4f(1.0f), m_position);
	tranformation_matrix = tranformation_matrix * RotateMatrix(Mat4f(1.0f), m_rotation);
	m_transformation_matrix = tranformation_matrix;
}


/* Adds a child node */
void TinyTranformNode::AddChild(TinyTranformNode* child)
{
	m_children.push_back(child);
}

/* Gets a child node by id */
TinyTranformNode* TinyTranformNode::GetChild(unsigned int index)
{
	if (index >= m_children.size() || index < 0)
		return NULL;
	return m_children[index];
}

/* Gets a child node by transform */
TinyTranformNode* TinyTranformNode::GetChild(TinyWorldTransform* transform)
{
	for (TinyTranformNode* child : m_children)
		if (child->m_transform == transform)
			return child;
	return NULL;
}

/* Deep search for a child node by transform */
TinyTranformNode* TinyTranformNode::SearchForChild(TinyWorldTransform* transform)
{
	for (TinyTranformNode* child : m_children)
	{
		/* If this node is the one it is searching for, then return it */
		if (child->m_transform == transform)
			return child;
		
		/* Other wise search the next branch */
		TinyTranformNode* node = child->SearchForChild(transform);
		if (node != NULL)
			return node;
	}

	/* If the transform could not be found, then return null */
	return NULL;
}

/* Update branches transformations */
void TinyTranformNode::UpdateBranchTransforms()
{
	if (m_transform != NULL)
	{
		TinyWorldTransform* parent_transform = NULL;
		if (m_parent != NULL)
			parent_transform = m_parent->m_transform;

		m_transform->UpdateTransformation(parent_transform);
	}

	for (TinyTranformNode* child : m_children)
		child->UpdateBranchTransforms();
}

/* Delete this branch */
TinyTranformNode::~TinyTranformNode()
{
	for (TinyTranformNode* child : m_children)
		delete child;
}