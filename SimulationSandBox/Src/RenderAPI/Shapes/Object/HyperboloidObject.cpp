#include "RenderAPI/Shapes/Object/HyperboloidObject.h"
#include <cmath>


void HyperboloidObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<DirectX::XMFLOAT3> HyperboloidObject::BuildPosition()
{
    std::vector<DirectX::XMFLOAT3> positions;

    for (int y = 0; y <= heightSegments; y++)
    {
        float t = static_cast<float>(y) / heightSegments;
        float currentHeight = -height / 2.0f + t * height;
        float radius = sqrtf((radiusBottom + t * (radiusTop - radiusBottom)) * (radiusBottom + t * (radiusTop - radiusBottom)) + currentHeight * currentHeight);

        for (int i = 0; i < radialSegments; i++)
        {
            float theta = (2.0f * DirectX::XM_PI * i) / radialSegments;
            positions.emplace_back(radius * cosf(theta), currentHeight, radius * sinf(theta));
        }
    }
    return positions;
}

std::vector<WORD> HyperboloidObject::BuildIndices()
{
    std::vector<WORD> indices;


    for (int y = 0; y < heightSegments; y++)
    {
        for (int i = 0; i < radialSegments; i++)
        {
            int next = (i + 1) % radialSegments;
            int currentRow = y * radialSegments;
            int nextRow = (y + 1) * radialSegments;

            indices.push_back(currentRow + i);
            indices.push_back(nextRow + i);
            indices.push_back(nextRow + next);

            indices.push_back(currentRow + i);
            indices.push_back(nextRow + next);
            indices.push_back(currentRow + next);
        }
    }

    return indices;
}

std::vector<DirectX::XMFLOAT3> HyperboloidObject::BuildNormal()
{
    std::vector<DirectX::XMFLOAT3> normals(BuildPosition().size(), { 0.0f, 0.0f, 0.0f });
    auto positions = BuildPosition();
    auto indices = BuildIndices();

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        DirectX::XMFLOAT3 u = { positions[indices[i + 1]].x - positions[indices[i]].x,
                                 positions[indices[i + 1]].y - positions[indices[i]].y,
                                 positions[indices[i + 1]].z - positions[indices[i]].z };

        DirectX::XMFLOAT3 v = { positions[indices[i + 2]].x - positions[indices[i]].x,
                                 positions[indices[i + 2]].y - positions[indices[i]].y,
                                 positions[indices[i + 2]].z - positions[indices[i]].z };

        DirectX::XMFLOAT3 normal = { u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x };
        float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        normal = { normal.x / length, normal.y / length, normal.z / length };

        normals[indices[i]] = normal;
        normals[indices[i + 1]] = normal;
        normals[indices[i + 2]] = normal;
    }

    return normals;
}

std::vector<DirectX::XMFLOAT2> HyperboloidObject::BuildTexCoords()
{
    std::vector<DirectX::XMFLOAT2> texCoords;

    for (int y = 0; y <= heightSegments; y++)
    {
        float v = static_cast<float>(y) / heightSegments;

        for (int i = 0; i < radialSegments; i++)
        {
            float u = static_cast<float>(i) / radialSegments;
            texCoords.emplace_back(u, v);
        }
    }

    return texCoords;
}
