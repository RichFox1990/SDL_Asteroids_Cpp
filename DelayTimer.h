#pragma once
#include "SDL_Timer.h"

class DelayTimer
{
public:

	Uint32 current_check = SDL_GetTicks();
	Uint32 last_success = 0;
	float delay;

	DelayTimer(float delay_in_ms) 
	{
		this->delay = delay_in_ms;
	}

	bool Check()
	{
		current_check = SDL_GetTicks();
		if (current_check - last_success >= delay)
		{
			last_success = current_check;
			return true;
		}
		else
		{
			return false;
		}
	}

};