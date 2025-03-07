#include "RenderAPI/Shapes/Object/TorusObject.h"
#include <cmath>


void TorusObject::SetupObject()
{
    IObjectInterface::SetupObject();
    ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<WORD> TorusObject::BuildIndices()
{
    std::vector<WORD> indices;

    for (int j = 0; j < tubularSegments; ++j)
    {
        for (int i = 0; i < radialSegments; ++i)
        {
            int nextI = (i + 1) % radialSegments;
            int nextJ = (j + 1) % tubularSegments;

            WORD a = j * radialSegments + i;
            WORD b = j * radialSegments + nextI;
            WORD c = nextJ * radialSegments + i;
            WORD d = nextJ * radialSegments + nextI;

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

    for (int j = 0; j < tubularSegments; ++j)
    {
        float v = (float)j / tubularSegments * DirectX::XM_2PI;
        float cosV = cos(v);
        float sinV = sin(v);

        for (int i = 0; i < radialSegments; ++i)
        {
            float u = (float)i / radialSegments * DirectX::XM_2PI;
            float cosU = cos(u);
            float sinU = sin(u);

            float x = (radius + tubeRadius * cosU) * cosV;
            float y = (radius + tubeRadius * cosU) * sinV;
            float z = tubeRadius * sinU;

            positions.emplace_back(x, y, z);
        }
    }

    return positions;
}

std::vector<DirectX::XMFLOAT3> TorusObject::BuildNormal()
{
    std::vector<DirectX::XMFLOAT3> normals;

    for (int j = 0; j < tubularSegments; ++j)
    {
        float v = (float)j / tubularSegments * DirectX::XM_2PI;
        float cosV = cos(v);
        float sinV = sin(v);

        for (int i = 0; i < radialSegments; ++i)
        {
            float u = (float)i / radialSegments * DirectX::XM_2PI;
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

    for (int j = 0; j < tubularSegments; ++j)
    {
        float v = (float)j / tubularSegments;
        for (int i = 0; i < radialSegments; ++i)
        {
            float u = (float)i / radialSegments;
            texCoords.emplace_back(u, v);
        }
    }

    return texCoords;
}
