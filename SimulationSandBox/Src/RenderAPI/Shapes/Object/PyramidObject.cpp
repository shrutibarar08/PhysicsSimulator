#include "RenderAPI/Shapes/Object/PyramidObject.h"

#include "imgui/imgui.h"


void PyramidObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<WORD> PyramidObject::BuildIndices()
{
    return {
        // Base (two triangles)
        0, 1, 2,
        0, 2, 3,

        // Side Faces
        0, 1, 4,  // Front-left
        1, 2, 4,  // Front-right
        2, 3, 4,  // Back-right
        3, 0, 4   // Back-left
    };
}

std::vector<DirectX::XMFLOAT3> PyramidObject::BuildPosition()
{
    float halfBase = mBaseSize * 0.5f;

    return {
        {-halfBase, 0.0f, -halfBase}, // 0 - Bottom-left
        { halfBase, 0.0f, -halfBase}, // 1 - Bottom-right
        { halfBase, 0.0f,  halfBase}, // 2 - Top-right
        {-halfBase, 0.0f,  halfBase}, // 3 - Top-left
        { 0.0f,    mHeight, 0.0f}    // 4 - Peak
    };
}

std::vector<DirectX::XMFLOAT3> PyramidObject::BuildNormal()
{
    return {
        { 0.0f, -1.0f, 0.0f },  // Base
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },

        { 0.0f, 1.0f, 0.0f }   // Approximate normal for peak
    };
}

std::vector<DirectX::XMFLOAT2> PyramidObject::BuildTexCoords()
{
    return {
        {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, // Base
        {0.5f, 0.0f}  // Peak
    };
}

void PyramidObject::InitPrimitiveControlGUI()
{
    ImGui::DragFloat("Base Size", &mBaseSize, 0.1f, 0.1f, 10.0f, "%.2f");
    ImGui::DragFloat("Height", &mHeight, 0.1f, 0.1f, 10.0f, "%.2f");

    if (ImGui::Button("Build"))
    {
        SetupObject();
    }
}
