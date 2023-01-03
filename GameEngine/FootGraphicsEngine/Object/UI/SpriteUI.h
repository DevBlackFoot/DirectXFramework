#pragma once

#include "UIBase.h"

namespace GraphicsEngineSpace
{
	class Texture;
	class Canvas;

	// 2D SpriteUI를 화면에 띄우기 위한 클래스
	class SpriteUI : public UIBase
	{
		// 사용할 텍스쳐 정보..리소스 매니저에 생성된 텍스쳐를 사용한다.
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