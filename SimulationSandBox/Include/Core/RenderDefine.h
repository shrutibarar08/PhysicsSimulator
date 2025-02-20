#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>

namespace Render
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
		DirectX::XMFLOAT4 color;
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
			DirectX::XMFLOAT3 scl = { 1.0f, 1.0f, 1.0f })
			: Translation(pos), Rotation(rot), Scale(scl) {
		}
	};
}
