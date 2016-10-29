#ifndef __DRAWDEBUG_H__
#define __DRAWDEBUG_H__

#include "Math.h"
#include "Color.h"

void beginDebugDraw();
void endDebugDraw();

void drawBoxDebug(AABB& box, Color color = White);
void drawBoxDebug(OBB& box, Color color = White);
void drawSphereDebug();
void drawFrustumDebug(Frustum& frustum, Color color = Yellow);

#endif // !__DRAWDEBUG_H__