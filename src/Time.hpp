#pragma once


using Duration = std::chrono::duration<double>;
using PreciseDuration = std::chrono::nanoseconds;
using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

class Time
{
public:
	static void Init()
	{
		s_ProgramStart = Time::Now();
		s_FrameStart = Time::Now();
	}

	static TimePoint Now()
	{
		return std::chrono::high_resolution_clock::now();
	}

	static double Diff(TimePoint a, TimePoint b)
	{
		Duration duration = a > b ? a - b : b - a;
		return duration.count();
	}

	static void Pause()
	{
		s_PauseStart = Time::Now();
	}

	static void Resume()
	{
		s_PauseDuration = Time::Now() - s_PauseStart;
	}

	static void EndFrame()
	{
		if (s_TargetFPS > 0.0)
		{
			s_FrameDuration = Time::Now() - s_FrameStart - s_PauseDuration;
			auto targetFrameDuration = Duration(1.0 / s_TargetFPS);
			if (targetFrameDuration > s_FrameDuration)
			{
				auto sleepDuration = targetFrameDuration - s_FrameDuration;
				Time::Sleep(sleepDuration);
			}
		}

		s_RealFrameDuration = Time::Now() - s_FrameStart;
		s_FrameDuration = s_RealFrameDuration - s_PauseDuration;

		// Add FrameDuration to array of samples to calculate FPS
		s_SamplesFPS[s_SamplesFPSIndex] = Time::NanosecondsToSeconds(s_FrameDuration);
		s_SamplesFPSIndex = (s_SamplesFPSIndex + 1) % s_SamplesFPS.size();

		s_FrameStart = Time::Now();
		s_PauseDuration = PreciseDuration::zero();
	}

	static double GetFrameTime()
	{
		return NanosecondsToSeconds(s_FrameDuration);
	}

	static double GetRealFrameTime()
	{
		return NanosecondsToSeconds(s_RealFrameDuration);
	}

	static double GetFPS()
	{
		double sum = std::accumulate(s_SamplesFPS.begin(), s_SamplesFPS.end(), 0.0);
		return s_SamplesFPS.size() / sum;
	}

	static double GetRealFPS()
	{
		return 1.0 / GetRealFrameTime();
	}

	static void Sleep(Duration seconds)
	{
		// Standard sleep implementation is never accurate enough.
		// This is implementation of partial busy-loop. 95 percent
		// of the seconds are slept, the rest is looped.
		auto destinationTime = Time::Now() + Duration(seconds);
		auto sleepSeconds = Duration(0.95 * seconds);
		std::this_thread::sleep_for(sleepSeconds);

		while(Time::Now() < destinationTime) { /* nothing */ }		
	}

	static void Sleep(double seconds)
	{
		Time::Sleep(Duration(seconds));
	}

	static void LockFPS(double fps)
	{
		s_TargetFPS = fps;
	}

	static double NanosecondsToSeconds(std::chrono::nanoseconds nanoseconds)
	{
		return nanoseconds.count() / 1'000'000'000.0;
	}

private:
	static TimePoint s_ProgramStart;
	static TimePoint s_FrameStart;
	static PreciseDuration s_RealFrameDuration;
	static PreciseDuration s_FrameDuration;

	static TimePoint s_PauseStart;
	static PreciseDuration s_PauseDuration;

	static double s_TargetFPS;
	static std::array<double, 60> s_SamplesFPS;
	static size_t s_SamplesFPSIndex;
};