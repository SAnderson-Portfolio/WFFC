#include "RotateWidget.h"

RotateWidget::RotateWidget() : Widget()
{
}

RotateWidget::~RotateWidget()
{
}

void RotateWidget::TransformObject(SceneObject& objectToTransform, float scaling)
{
	switch (m_WidgetAxis)
	{
	case TransformAxis::X:
		objectToTransform.rotX += m_TransformAmount * scaling;
		break;
	case TransformAxis::Y:
		objectToTransform.rotY += m_TransformAmount * scaling;
		break;
	case TransformAxis::Z:
		objectToTransform.rotZ += m_TransformAmount * scaling;
		break;
	default:
		break;
	}
}

void RotateWidget::Update(SceneObject& selectedObject, Vector3 cameraPosition)
{
	Widget::Update(selectedObject, cameraPosition);
}

