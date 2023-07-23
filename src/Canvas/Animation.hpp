#pragma once

#include <glm/vec2.hpp>

class EventQueue;

class Animation
{
public:
	enum class EasingType
	{
		Linear = 0,

		SineIn, SineOut, SineInOut,
		CircIn, CircOut, CircInOut,
		CubicIn, CubicOut, CubicInOut,
		QuadIn, QuadOut, QuadInOut,
		ExpoIn, ExpoOut, ExpoInOut,
		BackIn, BackOut, BackInOut,
		BounceIn, BounceOut, BounceInOut,
		ElasticIn, ElasticOut, ElasticInOut
	};

	Animation() = default;
	Animation(float start, float stop, float duration, EasingType easing = EasingType::Linear)
		: m_CurrentTime(0.0f), 
		  m_Start(start),
		  m_CurrentValue(start),
		  m_Stop(stop),
		  m_Duration(duration),
		  m_Finished(false),
		  m_EasingType(easing)
	{
	}

	float Step();
	bool Finished() const { return m_Finished; }

	static void Init(EventQueue& eventQueue);
private:
	float ExecuteEaseFuncion();

	float m_CurrentTime = 0.0f;
	float m_Start = 0.0f;
	float m_CurrentValue = 0.0f;
	float m_Stop = 0.0f;
	float m_Duration = 0.0f;
	bool m_Finished = true;
	EasingType m_EasingType = EasingType::Linear;

	static EventQueue* s_EventQueue;
};


class AnimationVec2
{
public:
	AnimationVec2() = default;
	AnimationVec2(glm::vec2 start, glm::vec2 stop, float duration, Animation::EasingType easing = Animation::EasingType::Linear)
		: xAnim(start.x, stop.x, duration, easing), yAnim(start.y, stop.y, duration, easing) {}

	glm::vec2 Step()
	{
		return { xAnim.Step(), yAnim.Step() };
	}

	bool Finished()
	{
		return xAnim.Finished() && yAnim.Finished();
	}

private:
	Animation xAnim;
	Animation yAnim;
};
