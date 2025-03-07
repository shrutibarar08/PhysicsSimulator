#include "RenderAPI/Shapes/Object/PrismObject.h"


void PrismObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<WORD> PrismObject::BuildIndices()
{
    return {
        // Bottom Face
        0, 1, 2,

        // Front Face
        0, 1, 3,
        1, 4, 3,
        3, 4, 5,

        // Back Face
        2, 0, 3,
        2, 3, 5,

        // Left Face
        1, 2, 4,
        2, 5, 4,

        // Right Face
        0, 3, 5,
        0, 5, 2
    };
}

std::vector<DirectX::XMFLOAT3> PrismObject::BuildPosition()
{
    float halfWidth = mBaseWidth * 0.5f;
    float halfDepth = mDepth * 0.5f;

    return {
        // Base Triangle (Bottom)
        {-halfWidth, 0.0f, -halfDepth},  // 0
        { halfWidth, 0.0f, -halfDepth},  // 1
        { 0.0f, 0.0f,  halfDepth},       // 2

        // Top Triangle
        {-halfWidth, mHeight, -halfDepth},  // 3
        { halfWidth, mHeight, -halfDepth},  // 4
        { 0.0f, mHeight,  halfDepth}        // 5
    };
}

std::vector<DirectX::XMFLOAT3> PrismObject::BuildNormal()
{
    return {
        { 0.0f, -1.0f, 0.0f },  // Bottom
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },

        { 0.0f, 1.0f, 0.0f },   // Top
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f }
    };
}

std::vector<DirectX::XMFLOAT2> PrismObject::BuildTexCoords()
{
    return {
        {0.0f, 0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f},
        {0.0f, 0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f}
    };
}
