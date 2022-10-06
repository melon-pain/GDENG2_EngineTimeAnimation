#include "EngineTime.h"

EngineTime* EngineTime::sharedInstance = nullptr;

EngineTime::EngineTime()
{
}

EngineTime::~EngineTime()
{
}

void EngineTime::initialize()
{
	sharedInstance = new EngineTime();
}

double EngineTime::getDeltaTime()
{
	return sharedInstance->deltaTime;
}

void EngineTime::LogFrameStart()
{
	sharedInstance->start = std::chrono::system_clock::now();
}

void EngineTime::LogFrameEnd()
{
	sharedInstance->end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = sharedInstance->end - sharedInstance->start;

	sharedInstance->deltaTime = elapsed_seconds.count();
}
