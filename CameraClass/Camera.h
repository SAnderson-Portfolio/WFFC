#pragma once
#include "pch.h"
#include "StepTimer.h"
#include "SimpleMath.h"
#include "DisplayObject.h"
#include <DirectXMath.h>
#include "InputCommands.h"
#include "CameraModes.h"
class Camera
{
public:
	Camera();
	~Camera();
	//Updates the camera based off the step timer.
	void Update();
	//Get the view matrix from the camera.
	DirectX::SimpleMath::Matrix  GetViewMatrix() { return m_view; }
	//Get the current position of the camera.
	DirectX::SimpleMath::Vector3 GetCameraPosition() { return m_camPosition; }
	//Set the current camera position.
	void SetCameraPosition(DirectX::SimpleMath::Vector3 newPosition) { m_camPosition = newPosition; Update(); }
	/// <summary>
	/// Sets the object that the camera should focus on.
	/// </summary>
	/// <param name="newObject">Reference to the object you wish to focus.</param>
	void SetFocus(DisplayObject& newObject) { m_focusedObject = &newObject; Update(); }
	void RemoveFocus() { m_focusedObject = nullptr; }
	void HandleInput(InputCommands &Input);
	void SetWindowSizes(int width, int height) { m_windowWidth = width; m_windowHeight = height; }
	CameraMode GetCurrentCameraMode() { return m_currentMode; }
	void SetCameraMode(CameraMode newMode) { m_currentMode = newMode; }
	DirectX::SimpleMath::Vector3& GetLookDirection() { return m_camLookDirection; }
private:
	//The speed the camera moves at.
	float							m_movespeed;
	//The multiplier for movement speed.
	float							m_moveMultiplier;
	//The position of the camera.
	DirectX::SimpleMath::Vector3	m_camPosition;
	//The orientation of the camera.
	DirectX::SimpleMath::Vector3	m_camOrientation;
	//The camera's look at.
	DirectX::SimpleMath::Vector3	m_camLookAt;
	//The direction the camera is looking in.
	DirectX::SimpleMath::Vector3	m_camLookDirection;
	//The camera's right vector.
	DirectX::SimpleMath::Vector3	m_camRight;
	//The camera's up vector.
	DirectX::SimpleMath::Vector3	m_camUp;
	//Yaw, pitch, roll.
	float							m_yaw, m_pitch, m_roll;
	//How fast the camera turns.
	float							m_sensitivity;
	//The speed the camera rotates at.
	float							m_camRotRate;
	//The view matrix from the camera.
	DirectX::SimpleMath::Matrix		m_view;
	//The object being focused on.
	DisplayObject*					m_focusedObject;
	//Mouse
	int								m_windowWidth, m_windowHeight;
	CameraMode						m_currentMode;
private:
	//Rotates the camera to the left.
	void RotateLeft();
	//Rotates the camera to the right.
	void RotateRight();
	//Rotates the camera up
	void RotateUp();
	//Rotates the camera down
	void RotateDown();
	//Moves the camera forward.
	void MoveForward();
	//Moves the camera backwards.
	void MoveBackward();
	//Moves the camera to the right.
	void MoveRight();
	//Moves the camera to the left.
	void MoveLeft();
	//Moves the camera up
	void MoveUp();
	//Moves the camera down
	void MoveDown();
	//Turns the camera
	void Turn(int& x, int& y, bool& needsUpdate);
	void FreeCamControls(bool& needsUpdate, InputCommands &Input);
	void RotateCamControls(bool& needsUpdate, InputCommands &Input);
	void FocusCamControls(bool& needsUpdate, InputCommands &Input);
	void ResetOrientation();
};

