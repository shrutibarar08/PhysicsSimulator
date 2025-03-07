#pragma once

#include "Core/Interface/ILightInterface.h"

class SpotLight final: public ILightInterface
{
public:
	SpotLight();
	~SpotLight() override = default;

	//~ Setters
	void SetAmbient(DirectX::XMFLOAT4 value);
	void SetDiffuse(DirectX::XMFLOAT4 value);
	void SetSpecular(DirectX::XMFLOAT4 value);
	void SetPosition(DirectX::XMFLOAT3 value);
	void SetDirection(DirectX::XMFLOAT3 value);
	void SetAttenuate(DirectX::XMFLOAT3 value);
	void SetRange(float value);
	void SetSpot(float value);

	//~ Getters
	const DirectX::XMFLOAT4& GetAmbient() const;
	const DirectX::XMFLOAT4& GetDiffuse() const;
	const DirectX::XMFLOAT4& GetSpecular() const;
	const DirectX::XMFLOAT3& GetPosition() const;
	const DirectX::XMFLOAT3& GetDirection() const;
	const DirectX::XMFLOAT3& GetAttenuate() const;

	float GetRange() const;
	float GetSpot() const;

	void SetupObject() override{}
	void RenderObject(ID3D11DeviceContext* deviceContext) override;
	void UpdateBuffer(const Simulation::WorldSpace& space) override;

	void InitGUI() override;
	void InitLightConfig(const Simulation::Light& lightConfig) override;

private:
	
	DirectX::XMFLOAT4 mAmbient;
	DirectX::XMFLOAT4 mDiffuse;
	DirectX::XMFLOAT4 mSpecular;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mDirection;
	DirectX::XMFLOAT3 mAttenuate;

	float mRange{ 1.f };
	float mSpot{ 1.f };
};
