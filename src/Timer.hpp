#pragma once

#include "Time.hpp"

class Timer
{
public:
	Timer()
	{
		Reset();
	}

	void Reset()
	{
		m_Start = Time::Now();
	}

	double Elapsed()
	{
		return Time::Diff(Time::Now(), m_Start);
	}

private:
	TimePoint m_Start;
};
