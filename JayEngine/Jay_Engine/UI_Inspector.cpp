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
	static float* rot = comp->getEulerRot();
	//Position
	if (ImGui::DragFloat3("Position:", pos)) comp->setPosition(pos);
	//Scale
	if (ImGui::DragFloat3("Scale:", scale)) comp->setScale(scale);
	//Rotation
	if (ImGui::DragFloat3("Rotation:", rot, 0.5f, -360.f, 360.f)) comp->setRotation(rot);

	ImGui::Separator();
}

void UI_Inspector::drawMesh()
{

}

void UI_Inspector::drawMaterial()
{

}