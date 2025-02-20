#include "Shapes/CubeObject.h"


void CubeObject::InitObject()
{
	IObjectInterface::InitObject();

	std::wstring vertexPath = L"Shaders/SimpleVertexShader.cso";
	std::wstring pixelPath = L"Shaders/SimplePixelShader.cso";

	D3D11_INPUT_ELEMENT_DESC inputDesc[]
	{
		{"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u,
			0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},

		{"COLOR", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u,
			12u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};
	UINT inputCounts = ARRAYSIZE(inputDesc);
	ConstructInputLayout(vertexPath, inputDesc, inputCounts);

	std::vector<WORD> indices
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};
	ConstructIndices(indices);

	std::vector<Render::Vertex> cube =
	{
		// Front face
		{DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)},
		{DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},

		// Bottom face
		{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)},
		{DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)}
	};
	ConstructVertices(cube);

	ConstructPixelShader(pixelPath);
	ConstructVertexShader(vertexPath);

	ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
