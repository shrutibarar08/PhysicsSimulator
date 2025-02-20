#include "Scenarios/PlaneScenario.h"

#include "Shapes/CubeObject.h"
#include <random>


PlaneScenario::PlaneScenario()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> posDist(-5.0f, 5.0f);
	std::uniform_real_distribution<float> rotDist(0.0f, DirectX::XM_2PI);

	for (int i = 0; i < 10; i++)
	{
		Render::Transform transform
		{
			{ posDist(gen), posDist(gen), posDist(gen) },
			{ rotDist(gen), rotDist(gen), rotDist(gen) },
			{ 1.f, 1.f, 1.f }
		};

		AddObjectOnScene<CubeObject>(transform);
	}
}
