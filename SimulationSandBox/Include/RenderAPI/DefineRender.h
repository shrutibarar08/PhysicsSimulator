#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>
#include <string>

namespace Simulation
{
	struct WorldSpace
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texCoordinate;
	};

	struct Transform
	{
		DirectX::XMFLOAT3 Translation;
		DirectX::XMFLOAT3 Rotation;
		DirectX::XMFLOAT3 Scale;

		// Constructor with default values
		Transform(
			DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f },
			DirectX::XMFLOAT3 rot = { 0.0f, 0.0f, 0.0f },
			DirectX::XMFLOAT3 scl = { 1.0f, 1.0f, 1.0f },
			std::string path = "")
			: Translation(pos), Rotation(rot), Scale(scl)
		{}
	};

	enum class CameraState : uint8_t
	{
		Perspective,
		Orthographic
	};

	enum class LightType : int
	{
		DirectionalLight,
		PointLight,
		Spotlight
	};

	struct DirectionalLight
	{
		DirectionalLight() { ZeroMemory(this, sizeof(this)); }

		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;
		DirectX::XMFLOAT3 Direction;
		float Pad;
	};

	struct PointLight
	{
		PointLight() { ZeroMemory(this, sizeof(this)); }
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;
		// Packed into 4D vector: (Position, Range)
		DirectX::XMFLOAT3 Position;
		float Range;
		// Packed into 4D vector: (A0, A1, A2, Pad)
		DirectX::XMFLOAT3 Att;
		float Pad;
	};

	struct SpotLight
	{
		SpotLight() { ZeroMemory(this, sizeof(this)); }
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;
		// Packed into 4D vector: (Position, Range)
		DirectX::XMFLOAT3 Position;
		float Range;
		// Packed into 4D vector: (Direction, Spot)
		DirectX::XMFLOAT3 Direction;
		float Spot;
		// Packed into 4D vector: (Att, Pad)
		DirectX::XMFLOAT3 Att;
		float Pad;
	};

	struct Light
	{
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;
		DirectX::XMFLOAT3 Position;
		float Range;
		DirectX::XMFLOAT3 Direction;
		float Spot;
		DirectX::XMFLOAT3 Att;
		int Id;
	};

}
