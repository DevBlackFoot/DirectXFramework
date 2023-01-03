#pragma once
#include "Interface/IBuilder.h"

#include "FBXBinaryLayout.h"


namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief Basic32쉐이더를 사용하여 텍스쳐, 라이팅을 받는 오브젝트를 만드는 빌더입니다.
	 *
	 * 하드 코딩을 받았지만.. filePath를 받아서 ASE파싱을 할수도 있도록 만들고자 합니다.
	 * 2022.07.07(목) Dev.BlackFoot
	 */
	class LegacyStaticBuilder : public IBuilder
	{
		// 내부적으로 Device를 가지고 있어야 한다.
			// 멤버 함수가 여러개가 되면서 여러 부분에서 사용하는 각종 멤버 변수가 필요해졌다.
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;


		std::shared_ptr<ResourceManager> resourceManager;

	public:
		LegacyStaticBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
			, resourceManager(nullptr)
		{}
		~LegacyStaticBuilder() override;

		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) override;
		virtual std::shared_ptr<MeshResources> BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath) override;
		virtual std::shared_ptr<IDXObject> AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
		                                              std::string textureName, std::wstring path, TextureMapType mapType) override;

		virtual void InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC) override;

	private:
		// 자체 포맷에서 가져온 정적 Mesh의 Resources 정보를 추가해줍니다.
		void BuildMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData);
	};
}
