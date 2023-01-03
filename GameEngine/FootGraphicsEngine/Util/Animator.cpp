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
		// 해당 obj가 가지고 있는 본 크기 만큼의 Matrix vector를 생성해준다.
		modifiedBoneTM.assign(boneSize, SimpleMath::Matrix::Identity);
	}

	Animator::~Animator()
	{
	}

	std::vector<SimpleMath::Matrix> Animator::UpdateBoneTM(float tick)
	{
		// TODO 애니메이션을 돌리기 위해서는 해당 부분을 구현해야합니다.
		// 힌트는 아래의 주석을 참고해주세요

		// 내부적으로 벡터를 가지고 있어야 해당 벡터의 값이 사라지지 않고 전달 된다.
		// 계산, 보간 및 Matrix 생성후 추가해준다.

		// 일단 가장 간단한 Animation을 구현해본다.
			// Frame Rate나 TickPerFrame 등은 아직 구현하지 않는다.

		// 기본적으로 애니메이션은
			// 현재 FrameTime에 delta Tick을 계속 더해주면서
			// FrameTime 보다 작은 prev KeyFrame과
			// FrameTime 보다 큰 next KeyFrame을 보간해서 현재 TM을 만들어낸다.

		// 애니메이션 클립의 모든 키프레임을 돌면서..
			// cf)AnimationClip의 구조
			// 내부에 KeyFrames가 핵심
			// n번본의 m번째 Frame이 담겨있음.

		// 마지막으로 해당 TMVector를 리턴해준다.
		return modifiedBoneTM;
	}

	void Animator::SetAnimationClip(std::shared_ptr<AnimationClipResource> animClip, bool isLoop)
	{
		nowAnimClip = animClip;
		// 첫 키 프레임 기록.
		nowKeyFrameIdx = nowAnimClip->startKeyFrame;

		this->isLoop = isLoop;
	}
}
