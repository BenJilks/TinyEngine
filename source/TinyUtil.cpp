#include "TinyUtil.h"
#include <iostream>
using namespace std;
using namespace TinyEngine;

Vec2f pre_mouse_position;

/* Updates a free movement camera for debugging */
void TinyEngine::UpdateDebugCamera(TinyDisplay* display, TinyWorld* render)
{
	float speed = 0.1f;

	TinyWorldTransform* camera = render->getCamera();
	Vec3f position = camera->GetPosition();
	Vec3f rotation = camera->GetRotation();

	/* Calculate the rotation of the camera using the mouse's position */
	Vec2f mouse_pos = display->GetMousePosition();
	Vec2f mouse_pos_diffrence = (mouse_pos - pre_mouse_position) / 2.5f;
	rotation += Vec3f(mouse_pos_diffrence.y(), mouse_pos_diffrence.x(), 0.0f);
	rotation.x(max(min(rotation.x(), 90.0f), -90.0f));
	
	camera->SetRotation(rotation);
	pre_mouse_position = mouse_pos;

	/* Calculate the forward movement */
	float factor_x = -(float)sin(TO_RADIANS(rotation.y())) * speed;
	float factor_z = (float)cos(TO_RADIANS(rotation.y())) * speed;
	if (display->IsKeyDown(GLFW_KEY_W)) position += Vec3f(factor_x, 0.0f, factor_z);
	if (display->IsKeyDown(GLFW_KEY_S)) position -= Vec3f(factor_x, 0.0f, factor_z);

	/* Calculate the sidewards movement */
	factor_x = (float)sin(TO_RADIANS(rotation.y() + 90.0f)) * speed;
	factor_z = -(float)cos(TO_RADIANS(rotation.y() + 90.0f)) * speed;
	if (display->IsKeyDown(GLFW_KEY_A)) position += Vec3f(factor_x, 0.0f, factor_z);
	if (display->IsKeyDown(GLFW_KEY_D)) position -= Vec3f(factor_x, 0.0f, factor_z);

	/* Calculate the vertical movement */
	if (display->IsKeyDown(GLFW_KEY_SPACE)) position -= Vec3f(0.0f, speed, 0.0f);
	if (display->IsKeyDown(GLFW_KEY_LEFT_SHIFT)) position += Vec3f(0.0f, speed, 0.0f);
	camera->SetPosition(position);
}
