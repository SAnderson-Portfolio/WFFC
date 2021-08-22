#include "TransformWidgets.h"

TransformWidgets::TransformWidgets()
{
	m_CurrentMode = TransformModes::TRANSLATION;
	m_translateObjects.resize(3);
	m_rotateObjects.resize(3);
	m_scaleObjects.resize(3);
	m_selectedWidget = nullptr;
	m_transformTolerance = 5.0f;
}

TransformWidgets::~TransformWidgets()
{
	m_selectedWidget = nullptr;
}

void TransformWidgets::UpdateWidgets(SceneObject& selectedObject, Vector3 cameraPosition)
{
	//Calculate the centre point between all the selected objects.
	/*float totalX = 0.0f;
	float totalY = 0.0f;
	float totalZ = 0.0f;
	for (int i = 0; i < m_selectedList.size(); i++)
	{
		totalX += m_selectedList[i].m_position.x;
		totalY += m_selectedList[i].m_position.y;
		totalZ += m_selectedList[i].m_position.z;
	}
	totalX /= m_selectedList.size();
	totalY /= m_selectedList.size();
	totalZ /= m_selectedList.size();*/

	for (int i = 0; i < 3; i++)
	{
		switch (m_CurrentMode)
		{
		case TransformModes::TRANSLATION:
			m_translateObjects[i].Update(selectedObject, cameraPosition);
			break;
		case TransformModes::ROTATION:
			m_rotateObjects[i].Update(selectedObject, cameraPosition);
			break;
		case TransformModes::SCALING:
			m_scaleObjects[i].Update(selectedObject, cameraPosition);
			break;
		default://No current mode
			break;
		}
	}
	
}

void TransformWidgets::AddWidgets(ObjectLoader* objectLoader)
{
	//Create translation objects
	objectLoader->CreateDisplayObject(m_translateObjects[0], "database/data/TranslateArrow.cmo", "TranslateX", "database/data/TranslateArrow.dds", false, Vector3::Zero, Vector3(0, 0, 90.0), Vector3::One / 2.0f); 
	objectLoader->CreateDisplayObject(m_translateObjects[1], "database/data/TranslateArrow.cmo", "TranslateY", "database/data/TranslateArrow.dds", false, Vector3::Zero, Vector3::Zero, Vector3::One / 2.0f);
	objectLoader->CreateDisplayObject(m_translateObjects[2], "database/data/TranslateArrow.cmo", "TranslateZ", "database/data/TranslateArrow.dds", false, Vector3::Zero, Vector3(90.0, 0, 0), Vector3::One / 2.0f);
	//Create roation objects
	objectLoader->CreateDisplayObject(m_rotateObjects[0], "database/data/RotateObject.cmo", "RotateX", "database/data/RotateObject.dds", false, Vector3::Zero, Vector3(0, 0, 0));
	objectLoader->CreateDisplayObject(m_rotateObjects[1], "database/data/RotateObject.cmo", "RotateY", "database/data/RotateObject.dds", false, Vector3::Zero, Vector3(0, 0, 90.0));
	objectLoader->CreateDisplayObject(m_rotateObjects[2], "database/data/RotateObject.cmo", "RotateZ", "database/data/RotateObject.dds", false, Vector3::Zero, Vector3(90.0, 0, 90.0));
	//Create scaling objects
	objectLoader->CreateDisplayObject(m_scaleObjects[0], "database/data/ScaleObject.cmo", "ScaleX", "database/data/ScaleObject.dds", false, Vector3::Zero, Vector3(0, 0, 90.0));
	objectLoader->CreateDisplayObject(m_scaleObjects[1], "database/data/ScaleObject.cmo", "ScaleY", "database/data/ScaleObject.dds", false, Vector3::Zero);
	objectLoader->CreateDisplayObject(m_scaleObjects[2], "database/data/ScaleObject.cmo", "ScaleZ", "database/data/ScaleObject.dds", false, Vector3::Zero, Vector3(90.0, 0, 0));

	for (int i = 0; i < 3; i++)
	{
		m_translateObjects[i].SetAxis((TransformAxis)i);
		m_rotateObjects[i].SetAxis((TransformAxis)i);
		m_scaleObjects[i].SetAxis((TransformAxis)i);
	}
	SetColours();
}

void TransformWidgets::RenderWidgets(ID3D11DeviceContext* deviceContext, CommonStates& states, Matrix world, Matrix view, Matrix projection)
{
	for (int i = 0; i < 3; i++)
	{
		switch (m_CurrentMode)
		{
		case TransformModes::TRANSLATION:
			m_translateObjects[i].Render(deviceContext, states, world, view, projection);
			break;
		case TransformModes::ROTATION:
			m_rotateObjects[i].Render(deviceContext, states, world, view, projection);
			break;
		case TransformModes::SCALING:
			m_scaleObjects[i].Render(deviceContext, states, world, view, projection);
			break;
		default://No current mode
			//Render nothing
			break;
		}
		
	}
	

}

void TransformWidgets::MousePicking(float mouseX, float mouseY, RECT screenDimensions, Matrix worldMatrix, Matrix projectionMatrix, Matrix viewMatrix, FLOAT minDepth, FLOAT maxDepth)
{
	Widget* objectPicked = nullptr;
	float distance = INFINITY;

	bool objectWasPicked = false;
	switch (m_CurrentMode)
	{
	case TransformModes::TRANSLATION:
		//MousePicking::PickWidgets(objectPicked, m_translateObjects, mouseX, mouseY, worldMatrix, projectionMatrix, viewMatrix, screenDimensions, minDepth, maxDepth);
		for (int i = 0; i < m_translateObjects.size(); i++)
		{
			if (MousePicking::PickSingleObject(m_translateObjects[i], mouseX, mouseY, worldMatrix, projectionMatrix, viewMatrix, screenDimensions, minDepth, maxDepth, distance))
			{
				objectPicked = &m_translateObjects[i];
			}
		}
		break;
	case TransformModes::ROTATION:
		for (int i = 0; i < m_rotateObjects.size(); i++)
		{
			if (MousePicking::PickSingleObject(m_rotateObjects[i], mouseX, mouseY, worldMatrix, projectionMatrix, viewMatrix, screenDimensions, minDepth, maxDepth, distance))
			{
				objectPicked = &m_rotateObjects[i];
			}
		}
		break;
	case TransformModes::SCALING:
		for (int i = 0; i < m_scaleObjects.size(); i++)
		{
			if (MousePicking::PickSingleObject(m_scaleObjects[i], mouseX, mouseY, worldMatrix, projectionMatrix, viewMatrix, screenDimensions, minDepth, maxDepth, distance))
			{
				objectPicked = &m_scaleObjects[i];
			}
		}
		break;
	default://No current mode
		break;
	}
	

	if (objectPicked != m_selectedWidget)
	{
		if (m_selectedWidget)
			m_selectedWidget->HighlightOff();
	}

	m_selectedWidget = objectPicked;
	if(m_selectedWidget)
		m_selectedWidget->HighlightOn();
	
}

void TransformWidgets::UpdateMousePosition(float mouseX, float mouseY, SceneObject& objectToTransform)
{
	if (m_selectedWidget)
	{
		//XMVECTORF32 colours[] = { Colors::Red, Colors::Green, Colors::Blue};
		POINT mPos;
		mPos.x = mouseX;
		mPos.y = mouseY;
		ClientToScreen(GetActiveWindow(), &mPos);
		POINT direction;
		direction.x = m_mouseTransformStart.x - mPos.x;
		direction.y = m_mouseTransformStart.y - mPos.y;
		switch (m_selectedWidget->GetAxis())
		{
			case TransformAxis::X:
				if (direction.x >= m_transformTolerance)
					m_selectedWidget->TransformObject(objectToTransform);
				if (direction.x <= -m_transformTolerance)
					m_selectedWidget->TransformObject(objectToTransform, -1.0f);
				break;
			case TransformAxis::Y:
				if (direction.y >= m_transformTolerance)
					m_selectedWidget->TransformObject(objectToTransform);
				if (direction.y <= -m_transformTolerance)
					m_selectedWidget->TransformObject(objectToTransform, -1.0f);
				break;
			case TransformAxis::Z:
				if (direction.x <= -m_transformTolerance)
					m_selectedWidget->TransformObject(objectToTransform);
				if (direction.x >= m_transformTolerance)
					m_selectedWidget->TransformObject(objectToTransform, -1.0f);
				break;
			default://Selected widget has no axis assigned.
				break;
		}
		
		if(m_CurrentMode != TransformModes::ROTATION)
			m_mouseTransformStart = mPos;
	}
}

void TransformWidgets::SetTransformMouseStart(float mouseX, float mouseY)
{
	m_mouseTransformStart.x = mouseX;
	m_mouseTransformStart.y = mouseY;
	ClientToScreen(GetActiveWindow(), &m_mouseTransformStart);
}

void TransformWidgets::SetRotationSpeed(float newSpeed)
{
	for (int i = 0; i < m_rotateObjects.size(); i++)
	{
		m_rotateObjects[i].SetTransformAmount(newSpeed);
	}
}

const float TransformWidgets::GetRotationSpeed()
{
	if (m_rotateObjects.empty())
		return 0.0f;

	return m_rotateObjects.front().GetTransformAmount();
}

void TransformWidgets::SetTranslateDistance(float newDistance)
{
	for (int i = 0; i < m_translateObjects.size(); i++)
	{
		m_translateObjects[i].SetTransformAmount(newDistance);
	}
}

const float TransformWidgets::GetTranslateDistance()
{
	if (m_translateObjects.empty())
		return 0.0f;

	return m_translateObjects.front().GetTransformAmount();
}

void TransformWidgets::SetScaleAmount(float newAmount)
{
	for (int i = 0; i < m_scaleObjects.size(); i++)
	{
		m_scaleObjects[i].SetTransformAmount(newAmount);
	}
}

const float TransformWidgets::GetScaleAmount()
{
	if (m_scaleObjects.empty())
		return 0.0f;

	return m_scaleObjects.front().GetTransformAmount();
}


double TransformWidgets::GetDistance(POINT pointOne, POINT pointTwo)
{
	return sqrt(pow((pointOne.x - pointTwo.x), 2) + pow(pointOne.y - pointTwo.y, 2));
}

void TransformWidgets::SetColours()
{
	XMVECTORF32 colours[] = { Colors::Red, Colors::Green, Colors::Blue};
	for (int i = 0; i < 3; i++)
	{
		m_translateObjects[i].SetColour(colours[i]);
		m_rotateObjects[i].SetColour(colours[i]);
		m_scaleObjects[i].SetColour(colours[i]);
	}
	
}

