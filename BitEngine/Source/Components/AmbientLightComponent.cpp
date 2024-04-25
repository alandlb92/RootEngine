#include "Components/AmbientLightComponent.h"
#include "Graphics/GraphicsMain.h"

void AmbientLightComponent::SetColor(float r, float g, float b)
{
     _cbLight.ambientLightColor.R = r;
     _cbLight.ambientLightColor.G = g;
     _cbLight.ambientLightColor.B = b;

     GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Light, &_cbLight);
}

void AmbientLightComponent::SetStrength(float s)
{
    _cbLight.ambientLightStrength = s;
    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Light, &_cbLight);
}
