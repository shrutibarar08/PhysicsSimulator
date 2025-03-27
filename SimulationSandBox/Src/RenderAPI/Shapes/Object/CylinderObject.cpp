#include "RenderAPI/Shapes/Object/CylinderObject.h"
#include <cmath>

#include "imgui/imgui.h"


void CylinderObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<WORD> CylinderObject::BuildIndices()
{
    std::vector<WORD> indices;

    // Top circle
    for (int i = 1; i <= mSegmentCount; i++)
    {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back((i % mSegmentCount) + 1);
    }

    // Bottom circle
    int bottomCenterIndex = mSegmentCount + 1;
    for (int i = 1; i <= mSegmentCount; i++)
    {
        indices.push_back(bottomCenterIndex);
        indices.push_back(bottomCenterIndex + (i % mSegmentCount) + 1);
        indices.push_back(bottomCenterIndex + i);
    }

    // Side faces
    for (int i = 1; i <= mSegmentCount; i++)
    {
        int next = (i % mSegmentCount) + 1;
        int top = i;
        int bottom = bottomCenterIndex + i;

        indices.push_back(top);
        indices.push_back(bottom);
        indices.push_back(next);

        indices.push_back(next);
        indices.push_back(bottom);
        indices.push_back(bottomCenterIndex + next);
    }

    return indices;
}

std::vector<DirectX::XMFLOAT3> CylinderObject::BuildPosition()
{
    std::vector<DirectX::XMFLOAT3> positions;

    // Top center
    positions.push_back({ 0.0f, mHeight * 0.5f, 0.0f });

    // Top circle
    for (int i = 0; i < mSegmentCount; i++)
    {
        float theta = (2.0f * DirectX::XM_PI * i) / mSegmentCount;
        positions.push_back({ mRadius * cosf(theta), mHeight * 0.5f, mRadius * sinf(theta) });
    }

    // Bottom center
    positions.push_back({ 0.0f, -mHeight * 0.5f, 0.0f });

    // Bottom circle
    for (int i = 0; i < mSegmentCount; i++)
    {
        float theta = (2.0f * DirectX::XM_PI * i) / mSegmentCount;
        positions.push_back({ mRadius * cosf(theta), -mHeight * 0.5f, mRadius * sinf(theta) });
    }

    return positions;
}

std::vector<DirectX::XMFLOAT3> CylinderObject::BuildNormal()
{
    std::vector<DirectX::XMFLOAT3> normals;

    // Top center normal
    normals.push_back({ 0.0f, 1.0f, 0.0f });

    // Top circle normals
    for (int i = 0; i < mSegmentCount; i++)
        normals.push_back({ 0.0f, 1.0f, 0.0f });

    // Bottom center normal
    normals.push_back({ 0.0f, -1.0f, 0.0f });

    // Bottom circle normals
    for (int i = 0; i < mSegmentCount; i++)
        normals.push_back({ 0.0f, -1.0f, 0.0f });

    // Side normals
    for (int i = 0; i < mSegmentCount; i++)
    {
        float theta = (2.0f * DirectX::XM_PI * i) / mSegmentCount;
        normals.push_back({ cosf(theta), 0.0f, sinf(theta) });
        normals.push_back({ cosf(theta), 0.0f, sinf(theta) });
    }

    return normals;
}

std::vector<DirectX::XMFLOAT2> CylinderObject::BuildTexCoords()
{
    std::vector<DirectX::XMFLOAT2> texCoords;

    // Top center
    texCoords.push_back({ 0.5f, 0.5f });

    // Top circle
    for (int i = 0; i < mSegmentCount; i++)
    {
        float theta = (2.0f * DirectX::XM_PI * i) / mSegmentCount;
        texCoords.push_back({ 0.5f + 0.5f * cosf(theta), 0.5f + 0.5f * sinf(theta) });
    }

    // Bottom center
    texCoords.push_back({ 0.5f, 0.5f });

    // Bottom circle
    for (int i = 0; i < mSegmentCount; i++)
    {
        float theta = (2.0f * DirectX::XM_PI * i) / mSegmentCount;
        texCoords.push_back({ 0.5f + 0.5f * cosf(theta), 0.5f + 0.5f * sinf(theta) });
    }

    // Side texture coordinates
    for (int i = 0; i < mSegmentCount; i++)
    {
        float u = static_cast<float>(i) / mSegmentCount;
        texCoords.push_back({ u, 0.0f });
        texCoords.push_back({ u, 1.0f });
    }

    return texCoords;
}

void CylinderObject::InitPrimitiveControlGUI()
{
    ImGui::Text("Cylinder Properties");

    ImGui::SliderInt("Segments", &mSegmentCount, 3, 100, "%d");
    ImGui::DragFloat("Radius", &mRadius, 0.01f, 0.1f, 10.0f, "%.2f");
    ImGui::DragFloat("Height", &mHeight, 0.01f, 0.1f, 10.0f, "%.2f");

    if (ImGui::Button("Rebuild Cylinder"))
    {
        SetupObject();
    }
}
