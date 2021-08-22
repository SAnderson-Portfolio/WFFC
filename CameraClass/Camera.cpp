#include "Camera.h"

Camera::Camera()
{
	//functional
	m_movespeed = 0.30;
	m_camRotRate = 3.0;

	//camera
	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0;
	m_camOrientation.y = 0;
	m_camOrientation.z = 0;

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	m_camUp.x = 0.0f;
	m_camUp.y = 1.0f;
	m_camUp.z = 0.0f;
	
	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;

	m_focusedObject = nullptr;

	m_yaw = -90.0f;
	m_pitch = 0.0f;
	m_roll = 0.0f;
	m_sensitivity = 0.3f;

	m_currentMode = CameraMode::ROTATE;
	m_moveMultiplier = 1.0f;
	Update();
}

Camera::~Camera()
{
}

void Camera::Update()
{
	

	float cosR, cosP, cosY;
	float sinR, sinP, sinY;
	// Only want to calculate these values once, when rotation changes, not every frame.
	cosY = cosf(DirectX::XMConvertToRadians(m_camOrientation.z));
	cosP = cosf(DirectX::XMConvertToRadians(m_camOrientation.y));
	cosR = cosf(DirectX::XMConvertToRadians(m_camOrientation.x));
	sinY = sinf(DirectX::XMConvertToRadians(m_camOrientation.z));
	sinP = sinf(DirectX::XMConvertToRadians(m_camOrientation.y));
	sinR = sinf(DirectX::XMConvertToRadians(m_camOrientation.x));

	if (m_currentMode == CameraMode::FOCUSED && m_focusedObject)
	{
		DirectX::SimpleMath::Vector3 objectDirection = m_focusedObject->m_position - m_camPosition;
		objectDirection.Normalize();
		m_camLookDirection = objectDirection;
	}
	else if (m_currentMode != CameraMode::FREE)
	{
		//create look direction from Euler angles in m_camOrientation
		m_camLookDirection.x = sinY * cosP;
		m_camLookDirection.y = sinP;
		m_camLookDirection.z = cosP * -cosY;
		m_camLookDirection.Normalize();
	}


	// Up Vector
	m_camUp.x = (-cosY * sinR - sinY * sinP * cosR);
	m_camUp.y = (cosP * cosR);
	m_camUp.z = (-sinY * sinR - sinP * cosR * -cosY);

	//create right vector from look Direction
	m_camLookDirection.Cross(m_camUp, m_camRight);

	//update lookat point
	if (m_focusedObject)
		m_camLookAt = m_focusedObject->m_position;


	m_camLookAt = m_camPosition + m_camLookDirection;

	
	//apply camera vectors
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_camPosition, m_camLookAt, m_camUp);
}

void Camera::RotateLeft()
{
	m_camOrientation.z -= m_camRotRate * m_moveMultiplier;
}

void Camera::RotateRight()
{
	m_camOrientation.z += m_camRotRate * m_moveMultiplier;
}

void Camera::RotateUp()
{
	m_camOrientation.y += m_camRotRate * m_moveMultiplier;
}

void Camera::RotateDown()
{
	m_camOrientation.y -= m_camRotRate * m_moveMultiplier;
}

void Camera::MoveForward()
{
	m_camPosition += m_camLookDirection * (m_movespeed * m_moveMultiplier);
}

void Camera::MoveBackward()
{
	m_camPosition -= m_camLookDirection * (m_movespeed * m_moveMultiplier);
}

void Camera::MoveRight()
{
	m_camPosition += m_camRight * (m_movespeed * m_moveMultiplier);
}

void Camera::MoveLeft()
{
	m_camPosition -= m_camRight * m_movespeed;
}

void Camera::MoveUp()
{
	m_camPosition.y += m_movespeed * m_moveMultiplier;
}

void Camera::MoveDown()
{
	m_camPosition.y -= m_movespeed * m_moveMultiplier;
}

void Camera::Turn(int& x, int& y, bool& needsUpdate)
{
	//ShowCursor(false);
	int xOffset = (x + 2) - (m_windowWidth / 2);
	int yOffset = (m_windowHeight / 2) - (y + 26);

	if (!xOffset && !yOffset)
	{
		return;
	}

	xOffset *= m_sensitivity;
	yOffset *= m_sensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	DirectX::SimpleMath::Vector3 forward;
	
	forward.x = (cos(DirectX::XMConvertToRadians(m_yaw)) * cos(DirectX::XMConvertToRadians(m_pitch)));
	forward.y = (sin(DirectX::XMConvertToRadians(m_pitch)));
	forward.z = (sin(DirectX::XMConvertToRadians(m_yaw)) * cos(DirectX::XMConvertToRadians(m_pitch)));
	forward.Normalize();
	m_camLookDirection = forward;

	POINT cursor;
	cursor.x = m_windowWidth / 2;
	cursor.y = m_windowHeight / 2;
	ClientToScreen(GetActiveWindow(), &cursor);
	SetCursorPos(cursor.x, cursor.y);

	x = m_windowWidth / 2;
	y = m_windowHeight / 2;

	needsUpdate = true;
}

void Camera::FreeCamControls(bool& needsUpdate, InputCommands &Input)
{
	if(Input.mouseRBDown)
		Turn(Input.mouseX, Input.mouseY, needsUpdate);
	/*else
		ShowCursor(true);*/

	//Right rotation
	if (Input.rotateRight)
	{
		MoveUp();
		needsUpdate = true;
	}
	//Left rotation
	if (Input.rotateLeft)
	{
		MoveDown();
		needsUpdate = true;
	}
	//Forward movement
	if (Input.forward)
	{
		MoveForward();
		needsUpdate = true;
	}
	//Backward movement
	if (Input.back)
	{
		MoveBackward();
		needsUpdate = true;
	}
	//Right movement
	if (Input.right)
	{
		MoveRight();
		needsUpdate = true;
	}
	//Left movement
	if (Input.left)
	{
		MoveLeft();
		needsUpdate = true;
	}

	
}

void Camera::RotateCamControls(bool& needsUpdate, InputCommands &Input)
{
	if (Input.forward)
	{
		RotateUp();
		needsUpdate = true;
	}
	if (Input.back)
	{
		RotateDown();
		needsUpdate = true;
	}
	if (Input.left)
	{
		RotateLeft();
		needsUpdate = true;
	}
	if (Input.right)
	{
		RotateRight();
		needsUpdate = true;
	}
}

void Camera::FocusCamControls(bool& needsUpdate, InputCommands &Input)
{
	if (!m_focusedObject)
		return;

	//Implement proper rotation around the focused object.
	//Forward movement
	if (Input.forward)
	{
		if (DirectX::SimpleMath::Vector3::Distance(m_camPosition, m_focusedObject->m_position) > 2.0f)
		{
			MoveForward();
			needsUpdate = true;
		}
	}
	//Backward movement
	if (Input.back)
	{
		MoveBackward();
		needsUpdate = true;
	}
	//Right movement
	if (Input.right)
	{
		MoveRight();
		needsUpdate = true;
	}
	//Left movement
	if (Input.left)
	{
		MoveLeft();
		needsUpdate = true;
	}

	//Right rotation
	if (Input.rotateRight)
	{
		MoveUp();
		needsUpdate = true;
	}
	//Left rotation
	if (Input.rotateLeft)
	{
		MoveDown();
		needsUpdate = true;
	}
}

void Camera::ResetOrientation()
{
	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;
}

void Camera::HandleInput(InputCommands &Input)
{
	bool camUpdated = false;
	if (Input.numOne)
	{
		m_currentMode = CameraMode::FREE;
		ResetOrientation();
	}
	if (Input.numTwo)
	{
		m_currentMode = CameraMode::ROTATE;
		ResetOrientation();
	}

	if (Input.numThree)
	{
		m_currentMode = CameraMode::FOCUSED;
		ResetOrientation();
	}

	if (Input.ctrl)
	{
		m_moveMultiplier = 1.5f;
	}
	else
		m_moveMultiplier = 1.0f;


	switch (m_currentMode)
	{
	case CameraMode::FOCUSED:
		FocusCamControls(camUpdated, Input);
		break;
	case CameraMode::FREE:
		FreeCamControls(camUpdated, Input);
		break;
	case CameraMode::ROTATE:
		RotateCamControls(camUpdated, Input);
		break;
	default:
		break;
	}

	if (camUpdated)
		Update();
}




