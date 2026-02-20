#pragma once
#include "Graphics/RGraphics.h"

namespace Faia
{
	namespace Root
	{
		namespace Graphics
		{
			class Graphics_DX12 : public RGraphics
			{
				public:
					virtual void SetupDevice() override;
			};
		}
	}
}