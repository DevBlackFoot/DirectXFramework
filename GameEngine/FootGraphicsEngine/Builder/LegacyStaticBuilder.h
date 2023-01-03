#pragma once
#include "Interface/IBuilder.h"

#include "FBXBinaryLayout.h"


namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief Basic32���̴��� ����Ͽ� �ؽ���, �������� �޴� ������Ʈ�� ����� �����Դϴ�.
	 *
	 * �ϵ� �ڵ��� �޾�����.. filePath�� �޾Ƽ� ASE�Ľ��� �Ҽ��� �ֵ��� ������� �մϴ�.
	 * 2022.07.07(��) Dev.BlackFoot
	 */
	class LegacyStaticBuilder : public IBuilder
	{
		// ���������� Device�� ������ �־�� �Ѵ�.
			// ��� �Լ��� �������� �Ǹ鼭 ���� �κп��� ����ϴ� ���� ��� ������ �ʿ�������.
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
		// ��ü ���˿��� ������ ���� Mesh�� Resources ������ �߰����ݴϴ�.
		void BuildMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData);
	};
}
