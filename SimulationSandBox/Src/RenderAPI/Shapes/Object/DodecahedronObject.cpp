#include "RenderAPI/Shapes/Object/DodecahedronObject.h"

void DodecahedronObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// Helper function to calculate normal for a face
DirectX::XMFLOAT3 ComputeNormal(const DirectX::XMFLOAT3& v0, const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    DirectX::XMFLOAT3 u = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
    DirectX::XMFLOAT3 v = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };

    DirectX::XMFLOAT3 normal = {
        (u.y * v.z - u.z * v.y),
        (u.z * v.x - u.x * v.z),
        (u.x * v.y - u.y * v.x)
    };

    float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    return { normal.x / length, normal.y / length, normal.z / length };
}

std::vector<WORD> DodecahedronObject::BuildIndices()
{
    return {
        0, 1, 2,  0, 2, 3,  0, 3, 4,  0, 4, 1,
        1, 5, 6,  1, 6, 2,  2, 6, 7,  2, 7, 3,
        3, 7, 8,  3, 8, 4,  4, 8, 9,  4, 9, 1,
        5, 10, 11, 5, 11, 6, 6, 11, 12, 6, 12, 7,
        7, 12, 13, 7, 13, 8, 8, 13, 14, 8, 14, 9,
        9, 14, 10, 9, 10, 5, 10, 15, 11, 11, 15, 12,
        12, 15, 13, 13, 15, 14, 14, 15, 10
    };
}

std::vector<DirectX::XMFLOAT3> DodecahedronObject::BuildPosition()
{
    float phi = (1.0f + sqrtf(5.0f)) / 2.0f;  // Golden ratio

    return {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1},
        {0, -1 / phi, -phi}, {0, 1 / phi, -phi}, {0, -1 / phi, phi}, {0, 1 / phi, phi},
        {-1 / phi, -phi, 0}, {1 / phi, -phi, 0}, {-1 / phi, phi, 0}, {1 / phi, phi, 0}
    };
}

std::vector<DirectX::XMFLOAT3> DodecahedronObject::BuildNormal()
{
    std::vector<DirectX::XMFLOAT3> positions = BuildPosition();
    std::vector<WORD> indices = BuildIndices();
    std::vector<DirectX::XMFLOAT3> normals(positions.size());

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        DirectX::XMFLOAT3 normal = ComputeNormal(positions[indices[i]], positions[indices[i + 1]], positions[indices[i + 2]]);
        normals[indices[i]] = normal;
        normals[indices[i + 1]] = normal;
        normals[indices[i + 2]] = normal;
    }

    return normals;
}

std::vector<DirectX::XMFLOAT2> DodecahedronObject::BuildTexCoords()
{
    return std::vector<DirectX::XMFLOAT2>(16, { 0.5f, 0.5f });
}

void DodecahedronObject::InitPrimitiveControlGUI()
{
}

void DodecahedronObject::LoadParamFromJson(const nlohmann::json& json)
{
}

nlohmann::json DodecahedronObject::SaveParamToJson()
{
    return {};
}
