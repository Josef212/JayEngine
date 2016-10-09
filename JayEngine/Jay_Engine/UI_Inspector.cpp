#include "UI_Inspector.h"

#include "GameObject.h"
#include "Tranform.h"
#include "Mesh.h"
#include "Material.h"


UI_Inspector::UI_Inspector() : UI_Comp()
{
}


UI_Inspector::~UI_Inspector()
{
}

void UI_Inspector::selectGameObject(GameObject* gameObj)
{
	if(gameObj)
		selectedGameObject = gameObj;
}

void UI_Inspector::draw()
{
	if (ImGui::Begin("Inspector"), &active)
	{
		if (selectedGameObject)
		{
			for (uint i = 0; i < selectedGameObject->components.size(); ++i)
			{
				switch (selectedGameObject->components[i]->type)
				{
				case TRANSFORMATION:
					drawTransformation();
					break;
				case MESH:
					drawMesh();
					break;
				case MATERIAL:
					drawMaterial();
					break;
				}
			}
		}
		ImGui::End();
	}
}

void UI_Inspector::drawTransformation()
{
	float pX, pY, pZ, sX, sY, sZ, rX, rY, rZ, rW;
	
	Tranform* comp = (Tranform*)selectedGameObject->findComponent(TRANSFORMATION);
	static float* pos = comp->getPosition();
	static float* scale = comp->getScale();
	static float* rot = comp->getRotation();
	//Position
	if (ImGui::InputFloat3("Position:", pos)) comp->setPosition(pos);
	//Scale
	if (ImGui::InputFloat3("Scale:", scale)) comp->setScale(scale);
	//Rotation
}

void UI_Inspector::drawMesh()
{

}

void UI_Inspector::drawMaterial()
{

}