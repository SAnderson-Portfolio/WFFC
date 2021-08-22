#include "Widget.h"

Widget::Widget()
{
	m_WidgetTransform = XMMatrixTransformation(g_XMZero, Quaternion::Identity, DAUtils::GetScale(*this), g_XMZero, DAUtils::GetRotation(*this), DAUtils::GetTranslation(*this));
	m_TransformAmount = 1.0f;
}

Widget::~Widget()
{
}

void Widget::TransformObject(SceneObject& objectToTransform, float scaling)
{
}

void Widget::Update(SceneObject& selectedObject, Vector3 cameraPosition)
{
	m_position = Vector3(selectedObject.posX, selectedObject.posY, selectedObject.posZ);
	m_scale = Vector3(Vector3::Distance(cameraPosition, m_position), Vector3::Distance(cameraPosition, m_position), Vector3::Distance(cameraPosition, m_position)) / 4.0f;
	UpdateWidgetTransform();
}


void Widget::Render(ID3D11DeviceContext* deviceContext, CommonStates& states, Matrix world, Matrix view, Matrix projection)
{
	m_model->Draw(deviceContext, states, world * m_WidgetTransform, view, projection, false);
}

void Widget::SetColour(XMVECTORF32 newColour)
{
	m_WidgetColour = newColour;
	SetFog();
}

void Widget::HighlightOn()
{
	m_model->UpdateEffects([&](IEffect* effect)
	{

		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogColor(Colors::Yellow);
		}
	});
}

void Widget::HighlightOff()
{
	m_model->UpdateEffects([&](IEffect* effect)
	{

		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogColor(m_WidgetColour);
		}
	});
}

void Widget::SetFog()
{
	m_model->UpdateEffects([&](IEffect* effect)
	{

		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogEnabled(true);
			fog->SetFogStart(0);
			fog->SetFogEnd(1);
			fog->SetFogColor(m_WidgetColour);
		}
	});
}

void Widget::UpdateWidgetTransform()
{
	m_WidgetTransform = XMMatrixTransformation(g_XMZero, Quaternion::Identity, DAUtils::GetScale(*this), g_XMZero, DAUtils::GetRotation(*this), DAUtils::GetTranslation(*this));
}

