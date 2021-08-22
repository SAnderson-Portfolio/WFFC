#pragma once
#include "Widget.h"
class TranslateWidget :
	public Widget
{
public:
	TranslateWidget();
	virtual ~TranslateWidget();
	void TransformObject(SceneObject& objectToTransform, float scaling = 1.0f) override;
	void Update(SceneObject& selectedObject, Vector3 cameraPosition) override;
};

