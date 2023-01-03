#pragma once

#include "UIBase.h"

namespace GraphicsEngineSpace
{
	class Texture;
	class Canvas;

	// 2D SpriteUI�� ȭ�鿡 ���� ���� Ŭ����
	class SpriteUI : public UIBase
	{
		// ����� �ؽ��� ����..���ҽ� �Ŵ����� ������ �ؽ��ĸ� ����Ѵ�.
		std::shared_ptr<Texture> texture;

	public:
		SpriteUI();
		~SpriteUI() override;

		virtual void SetTexture(std::shared_ptr<Texture> texture);
		virtual std::shared_ptr<Texture> GetTexture();

	protected:
		void Render(float tick) override;

		friend Canvas;

	};

}