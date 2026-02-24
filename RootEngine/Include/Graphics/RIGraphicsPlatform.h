#pragma once

namespace Faia
{
	namespace Root
	{
		namespace Graphics
		{
			class RIGraphicsPlatform
			{
			public:
				virtual void Setup() = 0;
				virtual void BeginFrame() = 0;
				virtual void EndFrame() = 0;
			};
		}
	}
}
