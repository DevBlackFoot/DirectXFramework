#include "GraphicsPch.h"
#include "UnityScene.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<YAMLBinaryData::Scene> UnityScene::LoadUnitySceneBinary(std::string path)
	{
		std::ifstream ifs(path, std::ios_base::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
		buffer.push(boost::iostreams::zlib_decompressor());
		buffer.push(ifs);
		boost::archive::binary_iarchive ia(buffer);

		std::shared_ptr<YAMLBinaryData::Scene> sceneData;

		ia >> sceneData;

		return sceneData;
	}
}