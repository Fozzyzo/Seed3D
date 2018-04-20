#pragma once

#ifdef SEED3DGRAPHICS_EXPORTS
#define SEED3DGRAPHICS_API __declspec(dllexport)
#else
#define SEED3DGRAPHICS_API __declspec(dllimport)
#endif

namespace S3DGraphics
{
	class RenderManager
	{
	public:
		void startUp();
		void shutDown();
	};
}
