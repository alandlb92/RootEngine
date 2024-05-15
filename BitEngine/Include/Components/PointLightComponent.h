#pragma once
#include "Graphics/GraphicsMain.h"
#include "Core/BComponent.h"


class PointLightComponent : public BComponent
{
	typedef BComponent Super;

public:
	void SetColor(float r, float g, float b);
	void SetStrength(float s);

protected:
	void OnOwnerObjectChangePosition(Vector3D newPosition);
	void Init() override;
};
