#include "ScaleWidget.h"

ScaleWidget::ScaleWidget() : Widget()
{

}

ScaleWidget::~ScaleWidget()
{
}

void ScaleWidget::TransformObject(SceneObject& objectToTransform, float scaling)
{
	switch (m_WidgetAxis)
	{
	case TransformAxis::X:
		objectToTransform.scaX += m_TransformAmount * scaling;
		break;
	case TransformAxis::Y:
		objectToTransform.scaY += m_TransformAmount * scaling;
		break;
	case TransformAxis::Z:
		objectToTransform.scaZ += m_TransformAmount * scaling;
		break;
	default:
		break;
	}
}

void ScaleWidget::Update(SceneObject & selectedObject, Vector3 cameraPosition)
{
	Widget::Update(selectedObject, cameraPosition);
}
