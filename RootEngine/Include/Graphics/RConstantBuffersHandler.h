#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Faia/HashUtils.h"


struct ID3D11Buffer;

namespace Faia
{
    namespace Root
    {
        namespace Graphics
        {
			struct CB_Param 
			{
				uint32_t mHashName;
				uint16_t mSize;
				std::vector<uint8_t> mDefaultParams;
			};
			
			struct CB_Info
			{
				uint32_t  mHashName;
				uint32_t mSlot;
				uint16_t mSize;
				std::vector<CB_Param> mParams;
			};

			// Shader resources
			//enum ConstantBuffer
			//{
			//	CB_Application, //ok _projectionMatrix
			//	CB_Frame, //ok viewmatrix
			//	CB_Object, //ok
			//	CB_Globals, //ok
			//	CB_Light, //ok
			//	NumConstantBuffers
			//};
			//CB Application 
			//XMMATRIX _projectionMatrix;

			//CB frame
			//XMMATRIX _viewMatrix;

		/*	struct CB_LightData
			{
				RColorRGB ambientLightColor;
				float ambientLightStrength;

				RColorRGB pointLightColor;
				float offset;

				RVector3D pointLightPosition;
				float pointLightStrength;

				CB_LightData()
				{
					ambientLightColor = RColorRGB(1.0f, 1.0f, 1.0f);
					ambientLightStrength = 1.0f;
					offset = 0.0f;
					pointLightColor = RColorRGB(1.0f, 1.0f, 1.0f);
					pointLightPosition = RVector3D(0.0f, 0.0f, 0.0f);
					pointLightStrength = 0.0f;
				}
			};

			struct GlobalBuffer
			{
				int hasTexture;
				int boneSelectedId;
			};

			struct PerObjectBufer
			{
				RMatrix4x4 worldMatrix;
				RMatrix4x4 animTransformMatrix[MAX_NUM_OF_ANIMATION_CHANNELS];
			};*/
			

			extern uint32_t gPerApplicationHash;
			extern uint32_t gProjectionMatrixHash;

			extern uint32_t gPerFrameHash;
			extern uint32_t gViewMatrixHash;

			extern uint32_t gPerObjectHash;
			extern uint32_t gIsSkinnedHash;
			extern uint32_t gHasTextureHash;
			extern uint32_t gWorldMatrixHash;
			extern uint32_t gAnimMatrixHash;

			extern uint32_t gLightBufferHash;
			extern uint32_t gLightData;

			class RConstantBuffersHandler
			{
            public:
				void Initialize();
				void RegisterData(CB_Param param);
				void SetParamData(uint32_t paramHashName, void* data);
				
				void UpdateSubresource(uint32_t bufferHashName);
				void PSSetConstantBuffers();
				void VSSetConstantBuffers();
			private:
				//I can do a tool in cmdTools to create a mock of this values before a make a shader builder
				std::map<uint32_t, CB_Info> mHashToCbInfo;
				std::vector<ID3D11Buffer*> mConstantBuffers;
				std::map<uint32_t, void*> mHashToParam;
				std::map<uint32_t, uint32_t> mHashParamToSize;
			};

			RConstantBuffersHandler* GetConstantBuffersHandler();
        }
    }
}