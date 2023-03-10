#include "GraphicsPch.h"
#include "LineBuilder.h"

#include "Interface/IDXObject.h"
#include "Object/LineObj.h"
#include "GraphicsCore/RasterizerState.h"
#include "Manager/ResourceManager.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<IDXObject> LineBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		// 들어온 오브젝트 확인
		if (std::dynamic_pointer_cast<LineObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		pDXObj->SetMeshResources(BuildGeometry(pDXObj, objectName));

		return pDXObj;
	}

	// 들어온 오브젝트의 이름에 따라 세팅을 다르게 해줍니다.
	std::shared_ptr<MeshResources> LineBuilder::BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath)
	{
		// 오브젝트 이름이 Grid, Axis인 경우..
		std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();
		// 또한 이름을 추가해준다.
		_tmpObjRes->ObjName = objectPath;

		if (objectPath == "Grid")
		{
			_tmpObjRes = BuildGridResources(_tmpObjRes);
			std::dynamic_pointer_cast<LineObj>(DXObject)->SetHasColor(true);
		}
		else if (objectPath == "Axis")
		{
			_tmpObjRes = BuildAxisResources(_tmpObjRes);
			std::dynamic_pointer_cast<LineObj>(DXObject)->SetHasColor(true);
		}
		else if (objectPath == "WireCube")
		{
			_tmpObjRes->meshID = WIRECUBE_MESH;
			_tmpObjRes->setMesh = true;
			std::dynamic_pointer_cast<LineObj>(DXObject)->SetHasColor(false);
		}
		else
		{
			_tmpObjRes.reset();
			return nullptr;
		}

		return _tmpObjRes;
	}

	void LineBuilder::InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// 리소스 매니저 바인딩
		resourceManager = ResourceManager::GetInstance();
	}

	// LineBuilder는 Texture를 사용하지 않습니다.
	std::shared_ptr<IDXObject> LineBuilder::AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
	                                                   std::string textureName, std::wstring path, TextureMapType mapType)
	{
		return nullptr;
	}

	std::shared_ptr<MeshResources> LineBuilder::BuildGridResources(std::shared_ptr<MeshResources> _objRes)
	{
		_objRes->meshID = GRID_MESH;

		_objRes->setMesh = true;

		return _objRes;
	}

	std::shared_ptr<MeshResources> LineBuilder::BuildAxisResources(std::shared_ptr<MeshResources> _objRes)
	{
		_objRes->meshID = AXIS_MESH;

		_objRes->setMesh = true;

		return _objRes;
	}

}