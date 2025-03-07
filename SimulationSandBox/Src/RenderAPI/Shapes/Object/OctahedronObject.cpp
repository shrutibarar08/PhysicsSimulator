#include "RenderAPI/Shapes/Object/OctahedronObject.h"

void OctahedronObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<WORD> OctahedronObject::BuildIndices()
{
    return {
        0, 1, 2,  // Top pyramid
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        5, 2, 1,  // Bottom pyramid
        5, 3, 2,
        5, 4, 3,
        5, 1, 4
    };
}

std::vector<DirectX::XMFLOAT3> OctahedronObject::BuildPosition()
{
    float size = 1.0f;
    return {
        {  0.0f,  size,  0.0f }, // Top vertex (0)
        { -size,  0.0f,  0.0f }, // Left-front (1)
        {  0.0f,  0.0f,  size }, // Front-right (2)
        {  size,  0.0f,  0.0f }, // Right-back (3)
        {  0.0f,  0.0f, -size }, // Back-left (4)
        {  0.0f, -size,  0.0f }  // Bottom vertex (5)
    };
}

std::vector<DirectX::XMFLOAT3> OctahedronObject::BuildNormal()
{
    return {
        {  0.0f,  1.0f,  0.0f },  // Top
        { -1.0f,  0.0f,  0.0f },  // Left
        {  0.0f,  0.0f,  1.0f },  // Front
        {  1.0f,  0.0f,  0.0f },  // Right
        {  0.0f,  0.0f, -1.0f },  // Back
        {  0.0f, -1.0f,  0.0f }   // Bottom
    };
}

std::vector<DirectX::XMFLOAT2> OctahedronObject::BuildTexCoords()
{
    return {
        { 0.5f, 1.0f },  // Top
        { 0.0f, 0.5f },  // Left
        { 0.5f, 0.5f },  // Front
        { 1.0f, 0.5f },  // Right
        { 0.5f, 0.0f },  // Back
        { 0.5f, 0.0f }   // Bottom
    };
}
