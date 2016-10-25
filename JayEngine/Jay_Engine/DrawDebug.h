#ifndef __DRAWDEBUG_H__
#define __DRAWDEBUG_H__

#include "Math.h"

void beginDebugDraw();
void endDebugDraw();

void drawBoxDebug(AABB& box);
void drawSphereDebug();
void drawFrustumDebug(Frustum& frustum);

#endif // !__DRAWDEBUG_H__