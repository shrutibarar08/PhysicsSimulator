#pragma once

#include "DefineRender.h"


class CameraComponent
{
public:
	CameraComponent();
	~CameraComponent() = default;

	CameraComponent(const CameraComponent&) = default;
	CameraComponent& operator=(const CameraComponent&) = default;
	CameraComponent(CameraComponent&&) = default;
	CameraComponent& operator=(CameraComponent&&) = default;

	void Update(float deltaTime);

	void  SetTranslationX(float x);
	void  AddTranslationX(float x);
	float GetTranslationX() const;

	void  SetTranslationY(float y);
	void  AddTranslationY(float y);
	float GetTranslationY() const;

	void  SetTranslationZ(float z);
	void  AddTranslationZ(float z);
	float GetTranslationZ() const;

	void AddTranslation(int axis, float value);
	void Rotate(int axis, float value);
	DirectX::XMFLOAT3 GetRotationAngles() const;

	void UpdateProjection();
	DirectX::XMMATRIX GetProjectionMatrix() const;

	void UpdateCameraView();

	void  SetMaxVisibleDistance(float farZ);
	float GetMaxVisibleDistance() const;

	void  SetAspectRatio(float ratio);
	float GetAspectRatio() const;

	void MoveForward(float delta);
	void MoveRight(float delta);
	void MoveUp(float delta);

	void RotateYaw(float angle);
	void RotatePitch(float angle);
	void RotateRoll(float angle);

	DirectX::XMMATRIX GetViewMatrix() const;

	void SetFieldOfView(float fov);
	float GetFieldOfView() const;

	void SetMovementSpeed(float speed);
	float GetMovementSpeed() const;

	DirectX::XMVECTOR GetForwardVector() const;
	DirectX::XMVECTOR GetRightVector() const;
	DirectX::XMVECTOR GetUpVector() const;

	void SetCameraMode(Simulation::CameraState state);
	Simulation::CameraState GetCameraMode() const;

	Simulation::WorldSpace& GetCameraSpace();

private:
	void UpdateFirstPersonMode(float deltaTime);
	void MouseControl(float deltaTime);
	void KeyboardControl(float deltaTime);

private:
	Simulation::WorldSpace  mCameraSpace{};
	DirectX::XMVECTOR   mCameraEyePosition{};
	DirectX::XMVECTOR   mCameraLookingAt{};
	DirectX::XMVECTOR   mCameraUp{};
	DirectX::XMVECTOR   mCameraRotationQuaternion;

	Simulation::CameraState mCameraState = Simulation::CameraState::Perspective;

	float mFarZ{ 500.f };
	float mAspectRatio;
	float mSpeed{ 5.f };
	float mFOV = DirectX::XMConvertToRadians(45.0f);
	bool mFreelockActivated{ false };
};
