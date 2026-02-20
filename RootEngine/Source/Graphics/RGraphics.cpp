#include "Graphics/RGraphics.h"

#include "Graphics/DX12/Graphics_DX12.h"

namespace Faia
{
	namespace Root
	{
        namespace Graphics
        {
            RGraphics* gGraphics;
            RGraphics* GetGraphics()
            {
                if (gGraphics == nullptr)
                {
                    gGraphics = new Graphics_DX12();
                }

                return gGraphics;
            }
        }
	}
}