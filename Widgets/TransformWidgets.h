#pragma once
#include "TransformModes.h"
#include "DisplayObject.h"
#include "ObjectLoader.h"
#include "DisplayObjectUtils.h"
#include <vector>
#include "TranslateWidget.h"
#include "RotateWidget.h"
#include "ScaleWidget.h"
#include "MousePicking.h"
class TransformWidgets
{
public:
	TransformWidgets();
	~TransformWidgets();
	//Sets the mode to the new mode passes in.
	void SetMode(TransformModes currentMode) { m_CurrentMode = currentMode; }
	//Gets the current mode.
	const TransformModes& GetMode() { return m_CurrentMode; }
	//std::vector<DisplayObject>* GetTranslationWidgets() { return &m_translateObjects; }
	void UpdateWidgets(SceneObject& selectedObject, Vector3 cameraPosition);
	void AddWidgets(ObjectLoader* objectLoader);
	void RenderWidgets(ID3D11DeviceContext* deviceContext, CommonStates& states, Matrix world, Matrix view, Matrix projection);
	void MousePicking(float mouseX, float mouseY, RECT screenDimensions, Matrix worldMatrix, Matrix projectionMatrix, Matrix viewMatrix, FLOAT minDepth, FLOAT maxDepth);
	void UpdateMousePosition(float mouseX, float mouseY, SceneObject& objectToTransform);
	void SetTransformMouseStart(float mouseX, float mouseY);
	void SetTolerance(float newTolerance) { m_transformTolerance = newTolerance; }
	const float GetTolerance() { return m_transformTolerance; }
	void SetRotationSpeed(float newSpeed);
	const float GetRotationSpeed();
	void SetTranslateDistance(float newDistance);
	const float GetTranslateDistance();
	void SetScaleAmount(float newAmount);
	const float GetScaleAmount();
private:
	TransformModes m_CurrentMode;
	std::vector<TranslateWidget>	m_translateObjects;
	std::vector<RotateWidget>		m_rotateObjects;
	std::vector<ScaleWidget>		m_scaleObjects;
	Widget*							m_selectedWidget;
	POINT							m_mouseTransformStart;
	float							m_transformTolerance;
private:
	void SetColours();
	double GetDistance(POINT pointOne, POINT pointTwo);
};

