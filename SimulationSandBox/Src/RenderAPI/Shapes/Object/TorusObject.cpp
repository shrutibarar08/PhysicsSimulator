#include "RenderAPI/Shapes/Object/TorusObject.h"
#include <cmath>

#include "imgui/imgui.h"


void TorusObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<WORD> TorusObject::BuildIndices()
{
    std::vector<WORD> indices;

    for (int j = 0; j < mTubularSegments; ++j)
    {
        for (int i = 0; i < mRadialSegments; ++i)
        {
            int nextI = (i + 1) % mRadialSegments;
            int nextJ = (j + 1) % mTubularSegments;

            WORD a = j * mRadialSegments + i;
            WORD b = j * mRadialSegments + nextI;
            WORD c = nextJ * mRadialSegments + i;
            WORD d = nextJ * mRadialSegments + nextI;

            indices.push_back(a);
            indices.push_back(c);
            indices.push_back(b);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    return indices;
}

std::vector<DirectX::XMFLOAT3> TorusObject::BuildPosition()
{
    std::vector<DirectX::XMFLOAT3> positions;

    for (int j = 0; j < mTubularSegments; ++j)
    {
        float v = (float)j / mTubularSegments * DirectX::XM_2PI;
        float cosV = cos(v);
        float sinV = sin(v);

        for (int i = 0; i < mRadialSegments; ++i)
        {
            float u = (float)i / mRadialSegments * DirectX::XM_2PI;
            float cosU = cos(u);
            float sinU = sin(u);

            float x = (mRadius + mTubeRadius * cosU) * cosV;
            float y = (mRadius + mTubeRadius * cosU) * sinV;
            float z = mTubeRadius * sinU;

            positions.emplace_back(x, y, z);
        }
    }

    return positions;
}

std::vector<DirectX::XMFLOAT3> TorusObject::BuildNormal()
{
    std::vector<DirectX::XMFLOAT3> normals;

    for (int j = 0; j < mTubularSegments; ++j)
    {
        float v = (float)j / mTubularSegments * DirectX::XM_2PI;
        float cosV = cos(v);
        float sinV = sin(v);

        for (int i = 0; i < mRadialSegments; ++i)
        {
            float u = (float)i / mRadialSegments * DirectX::XM_2PI;
            float cosU = cos(u);
            float sinU = sin(u);

            float x = cosU * cosV;
            float y = cosU * sinV;
            float z = sinU;

            normals.emplace_back(x, y, z);
        }
    }

    return normals;
}

std::vector<DirectX::XMFLOAT2> TorusObject::BuildTexCoords()
{
    std::vector<DirectX::XMFLOAT2> texCoords;

    for (int j = 0; j < mTubularSegments; ++j)
    {
        float v = (float)j / mTubularSegments;
        for (int i = 0; i < mRadialSegments; ++i)
        {
            float u = (float)i / mRadialSegments;
            texCoords.emplace_back(u, v);
        }
    }

    return texCoords;
}

void TorusObject::InitPrimitiveControlGUI()
{
    ImGui::SliderInt("Radial Segments", &mRadialSegments, 3, 100);
    ImGui::SliderInt("Tubular Segments", &mTubularSegments, 3, 100);
    ImGui::DragFloat("Radius", &mRadius, 0.1f, 0.1f, 10.0f, "%.2f");
    ImGui::DragFloat("Tube Radius", &mTubeRadius, 0.05f, 0.01f, 5.0f, "%.2f");

    if (ImGui::Button("Build"))
    {
        SetupObject();
    }
}

void TorusObject::LoadParamFromJson(const nlohmann::json& json)
{
    if (json.contains("RadialSegments") && json["RadialSegments"].is_number_integer())
        mRadialSegments = json["RadialSegments"];

    if (json.contains("TubularSegments") && json["TubularSegments"].is_number_integer())
        mTubularSegments = json["TubularSegments"];

    if (json.contains("Radius") && json["Radius"].is_number_float())
        mRadius = json["Radius"];

    if (json.contains("TubeRadius") && json["TubeRadius"].is_number_float())
        mTubeRadius = json["TubeRadius"];
}

nlohmann::json TorusObject::SaveParamToJson()
{
    return {
        {"RadialSegments", mRadialSegments},
        {"TubularSegments", mTubularSegments},
        {"Radius", mRadius},
        {"TubeRadius", mTubeRadius}
    };
}
