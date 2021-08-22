#include "TranslateWidget.h"

TranslateWidget::TranslateWidget() : Widget()
{
	
}

TranslateWidget::~TranslateWidget()
{
}

void TranslateWidget::TransformObject(SceneObject& objectToTransform, float scaling)
{
	switch (m_WidgetAxis)
	{
	case TransformAxis::X:
		objectToTransform.posX += m_TransformAmount * scaling;
		break;
	case TransformAxis::Y:
		objectToTransform.posY += m_TransformAmount * scaling;
		break;
	case TransformAxis::Z:
		objectToTransform.posZ += m_TransformAmount * scaling;
		break;
	default:
		break;
	}
}

void TranslateWidget::Update(SceneObject& selectedObject, Vector3 cameraPosition)
{
	Widget::Update(selectedObject, cameraPosition);

}

