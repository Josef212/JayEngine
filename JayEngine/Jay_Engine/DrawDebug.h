#ifndef __DRAWDEBUG__
#define __DRAWDEBUG__

#include "Math.h"
#include "Color.h"

void BeginDebugDraw();
void EndDebugDraw();

void DrawBoxDebug(AABB& box, Color color = White);
void DrawBoxDebug(OBB& box, Color color = White);
void DrawSphereDebug();
void DrawFrustumDebug(Frustum& frustum, Color color = Yellow);

#endif // !__DRAWDEBUG__