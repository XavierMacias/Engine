#pragma once

class ModuleTimer
{
public:
	ModuleTimer();
	~ModuleTimer();

private:
	//Game clock
	int frameCountSinceStart;
	int timeSinceStart;
	int timeScale;
	float deltaTime;

	//Real time clock
	int Real_Time_Since_Startup;
	int Real_Time_Delta_time;
};

