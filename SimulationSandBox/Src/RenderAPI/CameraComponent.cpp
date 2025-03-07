#include "RenderAPI/CameraComponent.h"
#include "Core/SystemManager/SubsystemManager.h"
#include "WindowsAPI/WindowsManager.h"
#include "WindowsAPI/MouseComponent.h"

#include <iostream>

#include "WindowsAPI/KeyboardComponent.h"

using namespace DirectX;


CameraComponent::CameraComponent()
{
	mCameraSpace.World = XMMatrixIdentity();
	// Initialize the view matrix
	mCameraEyePosition = XMVectorSet(0.0f, 1.0f, -10.0f, 0.0f);
	mCameraLookingAt = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	mCameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	mCameraRotationQuaternion = XMQuaternionIdentity();

	// Initialize the projection matrix
	SetAspectRatio(SubsystemManager::Get<WindowsManager>()->GetWidth() /
		static_cast<float>(SubsystemManager::Get<WindowsManager>()->GetHeight()));

	Update(0.0f);
}

void CameraComponent::Update(float deltaTime)
{
	UpdateCameraView();
	UpdateProjection();
	UpdateFirstPersonMode(deltaTime);
}

void CameraComponent::SetTranslationX(float x)
{
	mCameraEyePosition = XMVectorSetX(mCameraEyePosition, x);
}

void CameraComponent::AddTranslationX(float x)
{
	mCameraEyePosition = DirectX::XMVectorSetX(mCameraEyePosition, x + GetTranslationX());
}

float CameraComponent::GetTranslationX() const
{
	return DirectX::XMVectorGetX(mCameraEyePosition);
}

void CameraComponent::SetTranslationY(float y)
{
	mCameraEyePosition = DirectX::XMVectorSetY(mCameraEyePosition, y);
}

void CameraComponent::AddTranslationY(float y)
{
	mCameraEyePosition = DirectX::XMVectorSetY(mCameraEyePosition, y + GetTranslationY());
}

float CameraComponent::GetTranslationY() const
{
	return DirectX::XMVectorGetY(mCameraEyePosition);
}

void CameraComponent::SetTranslationZ(float z)
{
	mCameraEyePosition = DirectX::XMVectorSetZ(mCameraEyePosition, z);
}

void CameraComponent::AddTranslationZ(float z)
{
	mCameraEyePosition = DirectX::XMVectorSetZ(mCameraEyePosition, z + GetTranslationZ());
}

float CameraComponent::GetTranslationZ() const
{
	return DirectX::XMVectorGetZ(mCameraEyePosition);
}

void CameraComponent::AddTranslation(int axis, float value)
{
	if (axis == 0) AddTranslationX(value);
	else if (axis == 1) AddTranslationY(value);
	else if (axis == 2) AddTranslationZ(value);
}

void CameraComponent::Rotate(int axis, float value)
{
	if (axis == 0) RotatePitch(value);
	else if (axis == 1) RotateYaw(value);
	else if (axis == 2) RotateRoll(value);
}

DirectX::XMFLOAT3 CameraComponent::GetRotationAngles() const
{
	XMVECTOR forward = XMVector3Normalize(XMVectorSubtract(mCameraLookingAt, mCameraEyePosition));

	// Extract Yaw (rotation around Y-axis)
	float yaw = atan2(XMVectorGetX(forward), XMVectorGetZ(forward));

	// Extract Pitch (rotation around X-axis)
	float pitch = asin(-XMVectorGetY(forward)); // Invert Y to align with pitch movement

	// Extract Roll (rotation around Z-axis)
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(mCameraUp, forward));
	float roll = atan2(
		XMVectorGetY(right),
		XMVectorGetX(right)
	);

	return XMFLOAT3(pitch, yaw, roll);
}

void CameraComponent::UpdateProjection()
{
	mCameraSpace.Projection = GetProjectionMatrix();
}

void CameraComponent::UpdateCameraView()
{
	mCameraSpace.View = GetViewMatrix();
}

void CameraComponent::SetMaxVisibleDistance(float farZ)
{
	mFarZ = min(1.f, farZ);
}

float CameraComponent::GetMaxVisibleDistance() const
{
	return mFarZ;
}

void CameraComponent::SetAspectRatio(float ratio)
{
	mAspectRatio = ratio;
}

float CameraComponent::GetAspectRatio() const
{
	return mAspectRatio;
}

void CameraComponent::MoveForward(float delta)
{
	XMVECTOR forward = GetForwardVector();
	mCameraEyePosition = XMVectorAdd(mCameraEyePosition, XMVectorScale(forward, delta * mSpeed));
}

void CameraComponent::MoveRight(float delta)
{
	XMVECTOR right = GetRightVector();
	mCameraEyePosition = XMVectorAdd(mCameraEyePosition, XMVectorScale(right, delta * mSpeed));
}

void CameraComponent::MoveUp(float delta)
{
	XMVECTOR up = GetUpVector();
	mCameraEyePosition = XMVectorAdd(mCameraEyePosition, XMVectorScale(up, delta * mSpeed));
}

void CameraComponent::RotateYaw(float angle)
{
	XMVECTOR rotation = XMQuaternionRotationAxis(mCameraUp, angle);
	mCameraRotationQuaternion = XMQuaternionMultiply(mCameraRotationQuaternion, rotation);
}

void CameraComponent::RotatePitch(float angle)
{
	XMVECTOR rotation = XMQuaternionRotationAxis(GetRightVector(), angle);
	mCameraRotationQuaternion = XMQuaternionMultiply(mCameraRotationQuaternion, rotation);
}

void CameraComponent::RotateRoll(float angle)
{
	XMVECTOR forward = GetForwardVector();
	XMVECTOR rotation = XMQuaternionRotationAxis(forward, angle);

	mCameraRotationQuaternion = XMQuaternionMultiply(rotation, mCameraRotationQuaternion);
}

DirectX::XMMATRIX CameraComponent::GetViewMatrix() const
{
	DirectX::XMVECTOR forward = GetForwardVector();
	DirectX::XMVECTOR lookAtPosition = DirectX::XMVectorAdd(mCameraEyePosition, forward);

	return DirectX::XMMatrixLookAtLH(mCameraEyePosition, lookAtPosition, mCameraUp);
}

void CameraComponent::SetFieldOfView(float fov)
{
	mFOV = fov;
}

float CameraComponent::GetFieldOfView() const
{
	return mFOV;
}

void CameraComponent::SetMovementSpeed(float speed)
{
	mSpeed = speed;
}

float CameraComponent::GetMovementSpeed() const
{
	return mSpeed;
}

DirectX::XMVECTOR CameraComponent::GetForwardVector() const
{
	return XMVector3Rotate(XMVectorSet(0, 0, 1, 0), mCameraRotationQuaternion);
}

DirectX::XMVECTOR CameraComponent::GetRightVector() const
{
	return XMVector3Rotate(XMVectorSet(1, 0, 0, 0), mCameraRotationQuaternion);
}

DirectX::XMVECTOR CameraComponent::GetUpVector() const
{
	return XMVector3Rotate(XMVectorSet(0, 1, 0, 0), mCameraRotationQuaternion);
}

void CameraComponent::SetCameraMode(Simulation::CameraState state)
{
	mCameraState = state;
}

Simulation::CameraState CameraComponent::GetCameraMode() const
{
	return mCameraState;
}

Simulation::WorldSpace& CameraComponent::GetCameraSpace()
{
	return mCameraSpace;
}

void CameraComponent::UpdateFirstPersonMode(float deltaTime)
{
	MouseControl(deltaTime);
	KeyboardControl(deltaTime);
}

void CameraComponent::MouseControl(float deltaTime)
{
	float sensitivity = 0.002f; // Adjust sensitivity

	float deltaX = MouseComponent::GetDeltaX();
	float deltaY = MouseComponent::GetDeltaY();

	if (deltaX != 0.0f)
	{
		RotateYaw(deltaX * sensitivity);
	}
	if (deltaY != 0.0f)
	{
		RotatePitch(deltaY * sensitivity);
	}
	MouseComponent::ResetMouseDelta();
}

void CameraComponent::KeyboardControl(float deltaTime)
{
	float delta = mSpeed * deltaTime; // Scaled movement speed

	if (KeyboardComponent::IsKeyPressed('W'))
	{
		MoveForward(delta);
	}
	if (KeyboardComponent::IsKeyPressed('S'))
	{
		MoveForward(-delta);
	}
	if (KeyboardComponent::IsKeyPressed('A'))
	{
		MoveRight(-delta);
	}
	if (KeyboardComponent::IsKeyPressed('D'))
	{
		MoveRight(delta);
	}
	if (KeyboardComponent::IsActive(VK_ESCAPE))
	{
		MouseComponent::ToggleFreelock(true);
	}else
	{
		MouseComponent::ToggleFreelock(false);
	}
}

DirectX::XMMATRIX CameraComponent::GetProjectionMatrix() const
{
	if (mAspectRatio <= 0.0f)
		return XMMatrixIdentity();

	float nearZ = 0.1f;
	float farZ = (mFarZ > nearZ) ? mFarZ : nearZ + 10.0f;

	if (mCameraState == Simulation::CameraState::Perspective)
	{
		return XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, nearZ, farZ);
	}
	else  // Orthographic projection
	{
		float orthoHeight = 10.0f;  // Change this based on zoom level
		return XMMatrixOrthographicLH(mAspectRatio * orthoHeight, orthoHeight, nearZ, farZ);
	}
}
