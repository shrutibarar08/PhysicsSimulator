#include "RenderAPI/Shapes/Object/ConeObject.h"
#include <cmath>


void ConeObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<WORD> ConeObject::BuildIndices()
{
    const int slices = 20;
    std::vector<WORD> indices;

    // Side triangles
    for (int i = 0; i < slices; i++)
    {
        indices.push_back(slices);          // Tip of the cone
        indices.push_back(i);
        indices.push_back((i + 1) % slices);
    }

    // Base triangles
    for (int i = 0; i < slices - 2; i++)
    {
        indices.push_back(slices + 1);  // Center of the base
        indices.push_back(i + 1);
        indices.push_back(i);
    }

    return indices;
}

std::vector<DirectX::XMFLOAT3> ConeObject::BuildPosition()
{
    const int slices = 20;
    const float radius = 1.0f;
    const float height = 2.0f;
    std::vector<DirectX::XMFLOAT3> positions;

    // Base circle vertices
    for (int i = 0; i < slices; i++)
    {
        float theta = (2.0f * 3.14159265f * i) / slices;
        positions.emplace_back(radius * cosf(theta), 0.0f, radius * sinf(theta));
    }

    // Cone tip
    positions.emplace_back(0.0f, height, 0.0f);

    // Base center
    positions.emplace_back(0.0f, 0.0f, 0.0f);

    return positions;
}

std::vector<DirectX::XMFLOAT3> ConeObject::BuildNormal()
{
    const int slices = 20;
    std::vector<DirectX::XMFLOAT3> normals;

    for (int i = 0; i < slices; i++)
    {
        float theta = (2.0f * 3.14159265f * i) / slices;
        normals.emplace_back(cosf(theta), 0.5f, sinf(theta));
    }

    // Tip normal (approximated as up)
    normals.emplace_back(0.0f, 1.0f, 0.0f);

    // Base normal (down)
    normals.emplace_back(0.0f, -1.0f, 0.0f);

    return normals;
}

std::vector<DirectX::XMFLOAT2> ConeObject::BuildTexCoords()
{
    const int slices = 20;
    std::vector<DirectX::XMFLOAT2> texCoords;

    for (int i = 0; i < slices; i++)
    {
        float u = static_cast<float>(i) / (slices - 1);
        texCoords.emplace_back(u, 1.0f);
    }

    // Tip UV
    texCoords.emplace_back(0.5f, 0.0f);

    // Base center UV
    texCoords.emplace_back(0.5f, 0.5f);

    return texCoords;
}
