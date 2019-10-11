#pragma once
#include "TinyDisplay.h"
#include "TinyWorld.h"
#include <map>

namespace TinyEngine
{
	/* Updates a free movement camera for debugging */
	void UpdateDebugCamera(TinyDisplay* display, TinyWorld* render);
}