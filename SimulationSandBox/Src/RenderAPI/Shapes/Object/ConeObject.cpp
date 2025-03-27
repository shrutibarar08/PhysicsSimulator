#include "RenderAPI/Shapes/Object/ConeObject.h"
#include <cmath>

#include "imgui/imgui.h"


void ConeObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<WORD> ConeObject::BuildIndices()
{
    std::vector<WORD> indices;

    // Side triangles
    for (int i = 0; i < mSlices; i++)
    {
        indices.push_back(mSlices);          // Tip of the cone
        indices.push_back(i);
        indices.push_back((i + 1) % mSlices);
    }

    // Base triangles
    for (int i = 0; i < mSlices - 2; i++)
    {
        indices.push_back(mSlices + 1);  // Center of the base
        indices.push_back(i + 1);
        indices.push_back(i);
    }

    return indices;
}

std::vector<DirectX::XMFLOAT3> ConeObject::BuildPosition()
{
    std::vector<DirectX::XMFLOAT3> positions;

    // Base circle vertices
    for (int i = 0; i < mSlices; i++)
    {
        float theta = (2.0f * 3.14159265f * i) / mSlices;
        positions.emplace_back(mRadius * cosf(theta), 0.0f, 
            mRadius * sinf(theta));
    }

    // Cone tip
    positions.emplace_back(0.0f, mHeight, 0.0f);

    // Base center
    positions.emplace_back(0.0f, 0.0f, 0.0f);

    return positions;
}

std::vector<DirectX::XMFLOAT3> ConeObject::BuildNormal()
{
    std::vector<DirectX::XMFLOAT3> normals;

    for (int i = 0; i < mSlices; i++)
    {
        float theta = (2.0f * 3.14159265f * i) / static_cast<float>(mSlices);
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
    std::vector<DirectX::XMFLOAT2> texCoords;

    for (int i = 0; i < mSlices; i++)
    {
        float u = static_cast<float>(i) / (mSlices - 1);
        texCoords.emplace_back(u, 1.0f);
    }

    // Tip UV
    texCoords.emplace_back(0.5f, 0.0f);

    // Base center UV
    texCoords.emplace_back(0.5f, 0.5f);

    return texCoords;
}

void ConeObject::InitPrimitiveControlGUI()
{
    ImGui::SliderInt("Slices", &mSlices, 3, 100);
    ImGui::DragFloat("Radius", &mRadius, 0.1f, 0.1f, 100.0f, "%.2f");
    ImGui::DragFloat("Height", &mHeight, 0.1f, 0.1f, 100.0f, "%.2f");

    if (ImGui::Button("Build"))
    {
        SetupObject();
    }
}
