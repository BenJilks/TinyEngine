#pragma once
#include "TinyMaths.h"
#include <vector>
using namespace std;

/* Handle and create data about each object in the world */
namespace TinyEngine
{
	/* Store and handle a point in the world */
	class TinyWorldTransform
	{
	public:
		/* Create a transform at the center of the world */
		TinyWorldTransform() :
			m_position(), m_rotation(), m_scale(1.0f) {};

		/* Create a transform at a position in the world */
		TinyWorldTransform(Vec3f position) :
			m_position(position), m_rotation(), m_scale(1.0f) {};

		/* Create a full transform with position, rotation and scale */
		TinyWorldTransform(Vec3f position, Vec3f rotation, Vec3f scale) :
			m_position(position), m_rotation(rotation), m_scale(scale) {};

		/* Getters */
		inline Vec3f GetPosition() { return m_position; }
		inline Vec3f GetRotation() { return m_rotation; }
		inline Vec3f GetScale() { return m_scale; }
		inline Mat4f GetTransformationMatrix() { return m_transformation_matrix; }

		/* Setters */
		inline void SetPosition(Vec3f vector) { m_position = vector; }
		inline void SetRotation(Vec3f vector) { m_rotation = vector; }
		inline void SetScale(Vec3f vector) { m_scale = vector; }

		/* Update the transformation cache with that values stored and parent */
		void UpdateTransformation(TinyWorldTransform* parent);

		/* Update the transformations for a camera */
		void UpdateCameraTransforms();
	private:
		/* Store raw transform data */
		Vec3f m_position;
		Vec3f m_rotation;
		Vec3f m_scale;

		/* Cache combined transform data in a matrix */
		Mat4f m_transformation_matrix;
	};
	
	/* Store the data about a single node in a tree */
	class TinyTranformNode
	{
	public:
		TinyTranformNode(TinyTranformNode* parent, TinyWorldTransform* transform) : 
			m_parent(parent), m_transform(transform) {}

		/* Adds a child node */
		void AddChild(TinyTranformNode* child);

		/* Gets a child node by id */
		TinyTranformNode* GetChild(unsigned int index);

		/* Gets a child node by transform */
		TinyTranformNode* GetChild(TinyWorldTransform* transform);

		/* Deep search for a child node by transform */
		TinyTranformNode* SearchForChild(TinyWorldTransform* transform);

		/* Update branches transformations */
		void UpdateBranchTransforms();

		/* Setters */
		void SetTransform(TinyWorldTransform* transform) { m_transform = transform; };

		/* Delete this branch */
		~TinyTranformNode();
	private:
		TinyTranformNode* m_parent;
		vector<TinyTranformNode*> m_children;
		TinyWorldTransform* m_transform;
	};
}
