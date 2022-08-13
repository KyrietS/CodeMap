#pragma once

#include "raylib.h"


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
	bool Finished() { return m_Finished; }

private:
	float ExecuteEaseFuncion();

private:
	float m_CurrentTime = 0.0f;
	float m_Start = 0.0f;
	float m_CurrentValue = 0.0f;
	float m_Stop = 0.0f;
	float m_Duration = 0.0f;
	bool m_Finished = true;
	EasingType m_EasingType = EasingType::Linear;
};


class AnimationVec2
{
public:
	AnimationVec2() = default;
	AnimationVec2(Vector2 start, Vector2 stop, float duration, Animation::EasingType easing = Animation::EasingType::Linear)
		: xAnim(start.x, stop.x, duration, easing), yAnim(start.y, stop.y, duration, easing) {}

	Vector2 Step()
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
