#pragma once

namespace Faia
{
	namespace Root
	{
		namespace Graphics
		{
			class RGraphics
			{
				public:
					virtual void SetupDevice() = 0;
			};

			RGraphics* GetGraphics();
		}
	}
}