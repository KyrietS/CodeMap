#include "pch.hpp"
#include "Time.hpp"


TimePoint Time::s_ProgramStart;
TimePoint Time::s_FrameStart;
PreciseDuration Time::s_RealFrameDuration;
PreciseDuration Time::s_FrameDuration;

TimePoint Time::s_PauseStart;
PreciseDuration Time::s_PauseDuration;

double Time::s_TargetFPS;
std::array<double, 60> Time::s_SamplesFPS;
size_t Time::s_SamplesFPSIndex;
