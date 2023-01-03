#pragma once

namespace GraphicsEngineSpace
{
	/// 소연 누나의 InputLayout클래스를 참고해서 제작한 inputlayout 클래스.
	/// 내부적으로 버텍스 구조체가 다른 오브젝트들의 inputlayout을 가지고 있으며,
	///	필요할 때 전달하는 역할을 한다. => Effect 클래스와 비슷한 느낌..
	///
	///	기본적으로 이 때까지 object에 붙어있던 BuildVertexLayout함수 부분을 클래스로 뽑아준것.
	///	2022.07.06(수) Dev.BlackFoot

	/**
	 * \brief 버텍스 구조체를 바탕으로 각종 인풋레이아웃 Desc를 만들어준다.
	 */
	class InputLayoutDesc
	{
	public:
		// 가장 기본적인 InputLayout 위치와 컬러 정보.
		static D3D11_INPUT_ELEMENT_DESC PosColor[2];

		// 노말 맵 정보를 포함하는 레이아웃, 위치, 노말, 텍스쳐, 탄젠트
		static D3D11_INPUT_ELEMENT_DESC LegacyStatic[4];

		// 스키닝 정보를 담는 레이아웃 위치, 노말, 텍스쳐, 가중치, 본 index
		static D3D11_INPUT_ELEMENT_DESC LegacySkinned[5];

		// PBR
		static D3D11_INPUT_ELEMENT_DESC PBRStatic[5];

		static D3D11_INPUT_ELEMENT_DESC PBRSkinned[9];

		// 스카이 박스 DESC
		static D3D11_INPUT_ELEMENT_DESC Pos[1];
	};

	/**
	 * \brief 반복되는 인풋 레이아웃 설정을 한번에 해주기 위한 InputLayout 클래스
	 *
	 * 기본적으로 각상태에 맞는 인풋레이아웃을 설정해준다.
	 */
	class InputLayout
	{
	public:
		// Desc를 Inputlayout으로 가지고 있는 변수들
		static ID3D11InputLayout* PosColor;
		static ID3D11InputLayout* LegacyStatic;
		static ID3D11InputLayout* LegacySkinned;
	public:
		// 위의 Desc들을 init하고. InputLayout 형태로 가져올 수 있는 매니저격 클래스
			// 모두 init.
		//static void InitAll(ID3D11Device* pDevice);
		static void InitInputLayout(ID3D11Device* pDevice, ID3DBlob* shaderBlob);
		static void DestoryAll();

	};

}