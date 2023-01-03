#pragma once

using namespace DirectX;

#define DIRECTION_LIGHT_COUNT 1
#define POINT_LIGHT_COUNT 30
#define SPOT_LIGHT_COUNT 30

namespace GraphicsEngineSpace
{
	// ���� ����
	// �ش� ������ �� ���� �κ��� cb���۷� ����..
	struct Material
	{
		Material()
			: Ambient()
			, Diffuse()
			, Specular()
		{
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;
	};

	// �� ���� ���� ����ü.
	struct DirectionalLight
	{
		DirectionalLight()
			: Direction()
			, Diffuse()
			, Ambient()
			, Specular(0.0f)
		{
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT3 Direction;
		float Specular;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Ambient;
	};

	// Ư�� ��ġ���� ������� ��
	struct PointLight
	{
		PointLight()
			: Position()
			, Diffuse()
			, Specular(0.f)
			, Intensity(0.f)
		{
			ZeroMemory(this, sizeof(this));
		}

		// ������, Range���·� 4���� ���Ϳ� �ִ´�.
		XMFLOAT4 Position;
		XMFLOAT4 Diffuse;
		
		float Specular;
		// �������� �ȿ��� ���.
		float Intensity;
	};

	struct SpotLight
	{
		SpotLight()
			: Ambient()
			, Diffuse()
			, Specular()
			, Position()
			, Range(0.0f)
			, Direction()
			, Spot(0.0f)
			, Att()
			, Pad(0.0f)
		{
			ZeroMemory(this, sizeof(this));
		}

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;

		// ������, Range���·� 4���� ���Ϳ� �ִ´�.
		XMFLOAT3 Position;
		float Range;

		// ����� spot ���·� 4���� ���Ϳ� �ִ´�.
		XMFLOAT3 Direction;
		float Spot;

		// Att, Pad. ���·� �ϳ��� 4���� ����.
		XMFLOAT3 Att;
		float Pad;
	};

	// PBR�� �� �߰�
	namespace PBR
	{
		struct DirectionalLight
		{
			DirectionalLight()
				: color(SimpleMath::Vector3::Zero)
				, power(0.0f)
				, direction(SimpleMath::Vector3::Zero)
				, pad(0.0f)
				, lightViewProj(SimpleMath::Matrix::Identity)
			{
				ZeroMemory(this, sizeof(this));
			}

			SimpleMath::Vector3 color;
			float power;

			SimpleMath::Vector3 direction;
			float pad;

			SimpleMath::Matrix lightViewProj;
		};

		struct PointLight
		{
			PointLight()
				: color(SimpleMath::Vector3::Zero)
				, power(0.0f)
				, position(SimpleMath::Vector3::Zero)
				, range(0.0f)
				, lightViewProj(SimpleMath::Matrix::Identity)
			{
				ZeroMemory(this, sizeof(this));
			}

			SimpleMath::Vector3 color;
			float power;

			SimpleMath::Vector3 position;
			float range;

			SimpleMath::Matrix lightViewProj;
		};

		struct SpotLight
		{
			SpotLight()
				: color(SimpleMath::Vector3::Zero)
				, power(0.0f)
				, direction(SimpleMath::Vector3::Zero)
				, halfAngle(0.0f)
				, position(SimpleMath::Vector3::Zero)
				, range(0.0f)
				, lightViewProj(SimpleMath::Matrix::Identity)
			{
				ZeroMemory(this, sizeof(this));
			}

			SimpleMath::Vector3 color;
			float power;

			SimpleMath::Vector3 direction;
			float halfAngle;

			SimpleMath::Vector3 position;
			float range;

			SimpleMath::Matrix lightViewProj;
		};
	}

	// ���ο��� ������� constant Buffer.
	__declspec(align(16)) struct cbLight
	{
		PBR::DirectionalLight DirLights[DIRECTION_LIGHT_COUNT];
		PBR::PointLight PointLights[POINT_LIGHT_COUNT];
		PBR::SpotLight SpotLights[SPOT_LIGHT_COUNT];

		SimpleMath::Vector3 EyePosW;

		int DirLightCnt;
		int PointLightCnt;
		int SpotLightCnt;
	};
}
