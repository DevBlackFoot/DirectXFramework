#pragma once

#include <functional>
#include "UIBase.h"
#include "Resources/Texture.h"

namespace GraphicsEngineSpace
{
	class Canvas;

	enum class ButtonState
	{
		DEFAULT,	// �⺻ ����
		DOWN,		// ���� �� ����
		UP,			// �� �� ����
		PRESS,		// ������ �ִ� ����
		HOVER,		// �÷��ΰ� �ִ� ����
		ENTER		// ���� ó�� ������ ����(hover ����, �� ���� ���;��ϴ� �̺�Ʈ)
	};

	// �⺻������ Sprite UI�� ����������, ���� �̺�Ʈ���� functor�� ������ �ִ� ��ư UI
	class ButtonUI : public UIBase
	{
		std::shared_ptr<Texture> defaultTexture;
		std::shared_ptr<Texture> hoverTexture;
		std::shared_ptr<Texture> pressTexture;
		ButtonState state;

		// �ƹ� ���� ��ȯ���� �ʰ�, �Ű� ������ �ʿ� ���� �Լ� ������
		std::function<void()> clickEvent;	// ��ư�� ������ ��
		std::function<void()> pressEvent;	// ��ư�� ������ ���� ��
		std::function<void()> enterEvent;	// ��ư�� �������� ��
		std::function<void()> hoverEvent;	// ��ư�� ������ �ʰ� ���� ��� �� ��
		std::function<void()> outEvent;		// ��ư���� ����� �� (hover���� ��ư ���� ������ ���� ��)

	public:
		ButtonUI();
		~ButtonUI() override;

		virtual void SetDefaultTexture(std::shared_ptr<Texture> texture);
		virtual void SetHoverTexture(std::shared_ptr<Texture> texture);
		virtual void SetPressTexture(std::shared_ptr<Texture> texture);
		virtual void SetButtonState(ButtonState state);

		virtual void SetClickEvent(std::function<void()> clickEvent);
		virtual void SetPressEvent(std::function<void()> pressEvent);
		virtual void SetEnterEvent(std::function<void()> enterEvent);
		virtual void SetHoverEvent(std::function<void()> hoverEvent);
		virtual void SetOutEvent(std::function<void()> outEvent);

		virtual ButtonState GetButtonState();

	protected:
		void Render(float tick) override;

		friend Canvas;

	};
}