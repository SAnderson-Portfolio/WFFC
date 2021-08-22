#pragma once
#include "Widget.h"
class RotateWidget :
	public Widget
{
public:
	RotateWidget();
	virtual ~RotateWidget();
	void TransformObject(SceneObject& objectToTransform, float scaling = 1.0f) override;
	void Update(SceneObject& selectedObject, Vector3 cameraPosition) override;
};

