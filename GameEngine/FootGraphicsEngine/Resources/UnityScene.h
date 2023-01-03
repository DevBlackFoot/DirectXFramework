#pragma once
#include "Define/YAMLBinaryLayout.h"

namespace GraphicsEngineSpace
{
	class UnityScene
	{
	private:
		

	public:
		UnityScene()
		{}
		~UnityScene()
		{}

	public:
		std::shared_ptr<YAMLBinaryData::Scene> LoadUnitySceneBinary(std::string path);
	};
}
