#include "pch.hpp"
#include "Animation.hpp"
#include "reasings.h"
#include "Time.hpp"
#include "App.hpp"
#include "Events/EventQueue.hpp"

EventQueue* Animation::s_EventQueue = nullptr;

float Animation::ExecuteEaseFuncion()
{
	const float change = m_Stop - m_Start;
	switch (m_EasingType)
	{
	case EasingType::Linear: return EaseLinearNone(m_CurrentTime, m_Start, change, m_Duration);
	// Sine
	case EasingType::SineIn: return EaseSineIn(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::SineOut: return EaseSineOut(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::SineInOut: return EaseSineInOut(m_CurrentTime, m_Start, change, m_Duration);
	// Circular
	case EasingType::CircIn: return EaseCircIn(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::CircOut: return EaseCircOut(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::CircInOut: return EaseCircInOut(m_CurrentTime, m_Start, change, m_Duration);
	// Cubic
	case EasingType::CubicIn: return EaseCircIn(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::CubicOut: return EaseCircOut(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::CubicInOut: return EaseCircInOut(m_CurrentTime, m_Start, change, m_Duration);
	// Quadratic
	case EasingType::QuadIn: return EaseQuadIn(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::QuadOut: return EaseQuadOut(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::QuadInOut: return EaseQuadInOut(m_CurrentTime, m_Start, change, m_Duration);
	// Exponential
	case EasingType::ExpoIn: return EaseExpoIn(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::ExpoOut: return EaseExpoOut(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::ExpoInOut: return EaseExpoInOut(m_CurrentTime, m_Start, change, m_Duration);
	// Back
	case EasingType::BackIn: return EaseBackIn(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::BackOut: return EaseBackOut(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::BackInOut: return EaseBackInOut(m_CurrentTime, m_Start, change, m_Duration);
	// Bounce
	case EasingType::BounceIn: return EaseBounceIn(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::BounceOut: return EaseBounceOut(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::BounceInOut: return EaseBounceInOut(m_CurrentTime, m_Start, change, m_Duration);
	// Elastic
	case EasingType::ElasticIn: return EaseElasticIn(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::ElasticOut: return EaseElasticOut(m_CurrentTime, m_Start, change, m_Duration);
	case EasingType::ElasticInOut: return EaseElasticInOut(m_CurrentTime, m_Start, change, m_Duration);
	// Should never occur
	default: 
		LOG_ERROR("Unknown EasingType. Using Linear instead");
		return EaseLinearNone(m_CurrentTime, m_Start, change, m_Duration);
	}
}

float Animation::Step()
{
	double deltaTime = Time::GetFrameTime();

	if (m_CurrentTime + deltaTime > m_Duration)
		m_Finished = true;

	if (m_Finished)
	{
		m_CurrentValue = m_Stop;
	}
	else
	{
		m_CurrentValue = ExecuteEaseFuncion();
		s_EventQueue->Push(EmptyEvent{}); // Redraw the canvas
	}
	m_CurrentTime += (float)deltaTime;
	return m_CurrentValue;
}

void Animation::Init(EventQueue& eventQueue)
{
	s_EventQueue = &eventQueue;
}
