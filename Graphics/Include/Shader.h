#pragma once
#include <wrl/client.h>
#include "d3d11.h"

class Shader
{
public:
    Shader() = default;

    ID3D11InputLayout* GetInputLayout() { return _inputLayout.Get(); }
    ID3D11VertexShader* GetVertexShader() { return _vertexShader.Get(); }
    ID3D11PixelShader* GetPixelShader() { return _pixelShader.Get(); }
    void Load(const char* shaderName);


private:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader;
};