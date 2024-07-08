#pragma once
#include <wrl/client.h>
#include "d3d11.h"
#include <string>

class Shader
{
public:
    Shader() = default;

    ID3D11InputLayout* GetInputLayout() { return _inputLayout.Get(); }
    ID3D11VertexShader* GetVertexShader() { return _vertexShader.Get(); }
    ID3D11PixelShader* GetPixelShader() { return _pixelShader.Get(); }
    std::string GetShaderName() { return _shaderName; }

    void Load(const char* shaderName);


private:
    std::string _shaderName;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader;
};