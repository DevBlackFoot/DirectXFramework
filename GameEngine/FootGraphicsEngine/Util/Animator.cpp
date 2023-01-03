#include "GraphicsPch.h"
#include "Animator.h"

#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	Animator::Animator(int boneSize)
		: nowAnimClip(nullptr)
		, nowKeyFrameIdx(0)
		, isLoop(true)
		, isPlay(true)
		, nowFrameTime(0.f)
	{
		// �ش� obj�� ������ �ִ� �� ũ�� ��ŭ�� Matrix vector�� �������ش�.
		modifiedBoneTM.assign(boneSize, SimpleMath::Matrix::Identity);
	}

	Animator::~Animator()
	{
	}

	std::vector<SimpleMath::Matrix> Animator::UpdateBoneTM(float tick)
	{
		// TODO �ִϸ��̼��� ������ ���ؼ��� �ش� �κ��� �����ؾ��մϴ�.
		// ��Ʈ�� �Ʒ��� �ּ��� �������ּ���

		// ���������� ���͸� ������ �־�� �ش� ������ ���� ������� �ʰ� ���� �ȴ�.
		// ���, ���� �� Matrix ������ �߰����ش�.

		// �ϴ� ���� ������ Animation�� �����غ���.
			// Frame Rate�� TickPerFrame ���� ���� �������� �ʴ´�.

		// �⺻������ �ִϸ��̼���
			// ���� FrameTime�� delta Tick�� ��� �����ָ鼭
			// FrameTime ���� ���� prev KeyFrame��
			// FrameTime ���� ū next KeyFrame�� �����ؼ� ���� TM�� ������.

		// �ִϸ��̼� Ŭ���� ��� Ű�������� ���鼭..
			// cf)AnimationClip�� ����
			// ���ο� KeyFrames�� �ٽ�
			// n������ m��° Frame�� �������.

		// ���������� �ش� TMVector�� �������ش�.
		return modifiedBoneTM;
	}

	void Animator::SetAnimationClip(std::shared_ptr<AnimationClipResource> animClip, bool isLoop)
	{
		nowAnimClip = animClip;
		// ù Ű ������ ���.
		nowKeyFrameIdx = nowAnimClip->startKeyFrame;

		this->isLoop = isLoop;
	}
}
