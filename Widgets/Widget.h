#pragma once
#include "ObjectLoader.h"
#include "TransformAxis.h"
#include "DisplayObjectUtils.h"
class Widget : public DisplayObject
{
public:
	Widget();
	virtual ~Widget();
	//Transform a scene object.
	virtual void TransformObject(SceneObject& objectToTransform, float scaling = 1.0f);
	//Update the widget.
	virtual void Update(SceneObject& selectedObject, Vector3 cameraPosition);
	//Render the widget.
	void Render(ID3D11DeviceContext* deviceContext, CommonStates& states, Matrix world, Matrix view, Matrix projection);
	//Set the colour of the widget.
	void SetColour(XMVECTORF32 newColour);
	//Get the colour of the widget.
	const XMVECTORF32 GetColour() { return m_WidgetColour; }
	//Turn the highlight on.
	void HighlightOn();
	//Turn the highlight off.
	void HighlightOff();
	//Is this widget selected?
	const bool IsSelected() { return m_IsSelected; }
	//Sets the widget to be selected.
	void IsSelected(bool newState) { m_IsSelected = newState; }
	//Sets which axis this widget controls.
	void SetAxis(TransformAxis newAxis) { m_WidgetAxis = newAxis; }
	//Which axis does this widget control?
	const TransformAxis GetAxis() { return m_WidgetAxis; }
	//Set how much the widget transforms the object by.
	void SetTransformAmount(float newAmount) { m_TransformAmount = newAmount; }
	//Get how much the widget transform an object by.
	const float GetTransformAmount() { return m_TransformAmount; }
protected:
	XMMATRIX	  m_WidgetTransform;
	XMVECTORF32	  m_WidgetColour;
	bool		  m_IsSelected;
	TransformAxis m_WidgetAxis;
	float		  m_TransformAmount;
protected:
	void SetFog();
	void UpdateWidgetTransform();
};

