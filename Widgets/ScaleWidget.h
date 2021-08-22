#pragma once
#include "Widget.h"
class ScaleWidget :
	public Widget
{
public:
	ScaleWidget();
	virtual ~ScaleWidget();
	void TransformObject(SceneObject& objectToTransform, float scaling = 1.0f) override;
	void Update(SceneObject& selectedObject, Vector3 cameraPosition) override;
};

