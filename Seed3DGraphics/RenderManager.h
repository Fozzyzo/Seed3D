#pragma once

#ifdef RENDERMANAGER_EXPORTS
#define RENDERMANAGER_API __declspec(dllexport)
#else
#define RENDERMANAGER_API __declspec(dllimport)
#endif

namespace S3DGraphics
{
	class RenderManager
	{
	public:
		RenderManager* startUp();
		void shutDown();
	};
}
