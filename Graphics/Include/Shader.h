#pragma once

class ID3D11InputLayout;
class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11InputLayout;
class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11ShaderResourceView;
class ID3D11SamplerState;

class Shader
{
public:
    Shader();
    Shader(const wchar_t* shaderName);

    static Shader MakeSimpleShader();

    ID3D11InputLayout* GetInputLayout() { return _inputLayout; }
    ID3D11VertexShader* GetVertexShader() { return _vertexShader; }
    ID3D11PixelShader* GetPixelShader() { return _pixelShader; }
    ID3D11ShaderResourceView* GetTexture() { return _textureSRV; }
    ID3D11SamplerState* GetSamplerState() { return _samplerState; }

    void SetTextureResources(const wchar_t* texturePath);


private:
    const wchar_t* _shaderName;
    void Load();
    void LoadTexture(const wchar_t* textureRelativePath);

    ID3D11InputLayout* _inputLayout = nullptr;
    ID3D11VertexShader* _vertexShader = nullptr;
    ID3D11PixelShader* _pixelShader = nullptr;
    ID3D11ShaderResourceView* _textureSRV = nullptr;
    ID3D11SamplerState* _samplerState = nullptr;
};