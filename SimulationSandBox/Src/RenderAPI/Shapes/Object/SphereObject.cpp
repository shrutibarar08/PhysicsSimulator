#include "RenderAPI/Shapes/Object/SphereObject.h"

#include "imgui/imgui.h"


void SphereObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<WORD> SphereObject::BuildIndices()
{
    std::vector<WORD> indices;
    for (int lat = 0; lat < mLatitudeBands; ++lat)
    {
        for (int lon = 0; lon < mLongitudeBands; ++lon)
        {
            int first = (lat * (mLongitudeBands + 1)) + lon;
            int second = first + mLongitudeBands + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    return indices;
}

std::vector<DirectX::XMFLOAT3> SphereObject::BuildPosition()
{
    std::vector<DirectX::XMFLOAT3> positions;
    for (int lat = 0; lat <= mLatitudeBands; ++lat)
    {
        float theta = lat * DirectX::XM_PI / mLatitudeBands;
        float sinTheta = sinf(theta);
        float cosTheta = cosf(theta);

        for (int lon = 0; lon <= mLongitudeBands; ++lon)
        {
            float phi = lon * 2 * DirectX::XM_PI / mLongitudeBands;
            float sinPhi = sinf(phi);
            float cosPhi = cosf(phi);

            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            positions.emplace_back(mRadius * x, mRadius * y, mRadius * z);
        }
    }
    return positions;
}

std::vector<DirectX::XMFLOAT3> SphereObject::BuildNormal()
{
    std::vector<DirectX::XMFLOAT3> normals;
    auto positions = BuildPosition();

    for (const auto& pos : positions)
    {
        DirectX::XMFLOAT3 normal = pos;
        float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

        if (length > 0.0f)
        {
            normal.x /= length;
            normal.y /= length;
            normal.z /= length;
        }

        normals.push_back(normal);
    }
    return normals;
}

std::vector<DirectX::XMFLOAT2> SphereObject::BuildTexCoords()
{
    std::vector<DirectX::XMFLOAT2> texCoords;
    for (int lat = 0; lat <= mLatitudeBands; ++lat)
    {
        for (int lon = 0; lon <= mLongitudeBands; ++lon)
        {
            float u = static_cast<float>(lon) / mLongitudeBands;
            float v = static_cast<float>(lat) / mLatitudeBands;
            texCoords.emplace_back(u, 1.0f - v);
        }
    }
    return texCoords;
}

void SphereObject::InitPrimitiveControlGUI()
{
    ImGui::SliderInt("Latitude Bands", &mLatitudeBands, 3, 100);
    ImGui::SliderInt("Longitude Bands", &mLongitudeBands, 3, 100);
    ImGui::DragFloat("Radius", &mRadius, 0.1f, 0.1f, 10.0f, "%.2f");

    if (ImGui::Button("Build"))
    {
        SetupObject();
    }
}

void SphereObject::LoadParamFromJson(const nlohmann::json& json)
{
    if (json.contains("LatitudeBands") && json["LatitudeBands"].is_number_integer())
        mLatitudeBands = json["LatitudeBands"];

    if (json.contains("LongitudeBands") && json["LongitudeBands"].is_number_integer())
        mLongitudeBands = json["LongitudeBands"];

    if (json.contains("Radius") && json["Radius"].is_number_float())
        mRadius = json["Radius"];
}

nlohmann::json SphereObject::SaveParamToJson()
{
    return {
        {"LatitudeBands", mLatitudeBands},
        {"LongitudeBands", mLongitudeBands},
        {"Radius", mRadius}
    };
}
