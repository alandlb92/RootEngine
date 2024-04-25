#pragma once
#include "Graphics/GraphicsMain.h"
#include "Core/BComponent.h"


class AmbientLightComponent : public BComponent
{
public:
	void SetColor(float r, float g, float b);
	void SetStrength(float s);

private:
	CB_LightData _cbLight;
};
