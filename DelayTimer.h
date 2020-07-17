#pragma once
#include "SDL_Timer.h"

class DelayTimer
{
public:

	Uint32 current_check = SDL_GetTicks();
	Uint32 last_success = 0;
	float delay;

	DelayTimer(float delay_in_ms, bool allow_first_check) 
	{
		this->delay = delay_in_ms;
		if (!allow_first_check)
		{
			last_success = current_check;
		}
	}

	void Reset()
	{
		current_check = last_success = SDL_GetTicks();
	}

	bool DelayComplete(bool reset_on_true)
	{
		current_check = SDL_GetTicks();
		if (current_check - last_success >= delay)
		{
			if (reset_on_true)
			{
				last_success = current_check;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

};