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

			extern uint32_t gPerApplicationHash;
			extern uint32_t gProjectionMatrixHash;

			extern uint32_t gPerFrameHash;
			extern uint32_t gViewMatrixHash;
			extern uint32_t gcameraPositionHash;

			extern uint32_t gPerObjectHash;
			extern uint32_t gIsSkinnedHash;
			extern uint32_t gHasTextureHash;
			extern uint32_t gWorldMatrixHash;
			extern uint32_t gAnimMatrixHash;

			extern uint32_t gLightBufferHash;
			extern uint32_t gLightData;

			class RConstantBuffersHandler_DX11
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

			RConstantBuffersHandler_DX11* GetConstantBuffersHandler();
        }
    }
}