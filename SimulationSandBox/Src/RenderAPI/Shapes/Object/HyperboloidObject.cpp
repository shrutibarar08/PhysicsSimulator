#include "RenderAPI/Shapes/Object/HyperboloidObject.h"
#include <cmath>

#include "imgui/imgui.h"


void HyperboloidObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<DirectX::XMFLOAT3> HyperboloidObject::BuildPosition()
{
    std::vector<DirectX::XMFLOAT3> positions;

    for (int y = 0; y <= mHeightSegments; y++)
    {
        float t = static_cast<float>(y) / mHeightSegments;
        float currentHeight = -mHeight / 2.0f + t * mHeight;
        float radius = sqrtf((mRadiusBottom + t * (mRadiusTop - mRadiusBottom)) * (mRadiusBottom + t * (mRadiusTop - mRadiusBottom)) + currentHeight * currentHeight);

        for (int i = 0; i < mRadialSegments; i++)
        {
            float theta = (2.0f * DirectX::XM_PI * i) / mRadialSegments;
            positions.emplace_back(radius * cosf(theta), currentHeight, radius * sinf(theta));
        }
    }
    return positions;
}

std::vector<WORD> HyperboloidObject::BuildIndices()
{
    std::vector<WORD> indices;


    for (int y = 0; y < mHeightSegments; y++)
    {
        for (int i = 0; i < mRadialSegments; i++)
        {
            int next = (i + 1) % mRadialSegments;
            int currentRow = y * mRadialSegments;
            int nextRow = (y + 1) * mRadialSegments;

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

    for (int y = 0; y <= mHeightSegments; y++)
    {
        float v = static_cast<float>(y) / mHeightSegments;

        for (int i = 0; i < mRadialSegments; i++)
        {
            float u = static_cast<float>(i) / mRadialSegments;
            texCoords.emplace_back(u, v);
        }
    }

    return texCoords;
}

void HyperboloidObject::InitPrimitiveControlGUI()
{
    ImGui::SliderInt("Radial Segments", &mRadialSegments, 3, 100);
    ImGui::SliderInt("Height Segments", &mHeightSegments, 1, 50);
    ImGui::DragFloat("Height", &mHeight, 0.1f, 0.1f, 10.0f, "%.2f");
    ImGui::DragFloat("Bottom Radius", &mRadiusBottom, 0.1f, 0.1f, 10.0f, "%.2f");
    ImGui::DragFloat("Top Radius", &mRadiusTop, 0.1f, 0.1f, 10.0f, "%.2f");

    if (ImGui::Button("Build"))
    {
        SetupObject();
    }
}

void HyperboloidObject::LoadParamFromJson(const nlohmann::json& json)
{
    if (json.contains("RadialSegments") && json["RadialSegments"].is_number_integer())
        mRadialSegments = json["RadialSegments"];

    if (json.contains("HeightSegments") && json["HeightSegments"].is_number_integer())
        mHeightSegments = json["HeightSegments"];

    if (json.contains("Height") && json["Height"].is_number_float())
        mHeight = json["Height"];

    if (json.contains("BottomRadius") && json["BottomRadius"].is_number_float())
        mRadiusBottom = json["BottomRadius"];

    if (json.contains("TopRadius") && json["TopRadius"].is_number_float())
        mRadiusTop = json["TopRadius"];
}

nlohmann::json HyperboloidObject::SaveParamToJson()
{
    return {
        {"RadialSegments", mRadialSegments},
        {"HeightSegments", mHeightSegments},
        {"Height", mHeight},
        {"BottomRadius", mRadiusBottom},
        {"TopRadius", mRadiusTop}
    };
}
