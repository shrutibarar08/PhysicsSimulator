#pragma once

#include "Core/RegistryManager/RegistryLight.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


REGISTER_LIGHT(DirectionalLight);
REGISTER_LIGHT(SpotLight);
REGISTER_LIGHT(PointLight);
