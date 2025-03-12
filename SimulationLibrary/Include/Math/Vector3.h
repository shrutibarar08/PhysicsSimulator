#pragma once


namespace Physics
{
    class Vector3D
    {

    public:
        Vector3D();
        Vector3D(const float x, const float y, const float z);
        
        ~Vector3D() = default;

        
        
        void Invert();

    public:
        float x, y, z;

    private:
        float pad;
    };

}

