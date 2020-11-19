#include "Game.h"
#include "BlackHole.h"


BlackHole::BlackHole(const int screen_width, const int screen_height, const float size_multiplier, const float screen_ratio)
{
	enlarge = false;
	deflate = false;

	s_w = screen_width;
	s_h = screen_height;

	debug = false;
	angle_modifier = 200;
	img = Game::game_images[Game::eImages::B_BLACK_HOLE];
	rect = Game::GetRect(img, 0, 0);

	rect->w *= screen_ratio;
	rect->h *= screen_ratio;
	pos_x = rect->x = (((float)rand() / (float)RAND_MAX)) * ((float)screen_width - rect->w);
	pos_y = rect->y = (((float)rand() / (float)RAND_MAX)) * ((float)screen_height - rect->h);
	orig_pos.first = pos_x;
	orig_pos.second = pos_y;
	rad_img = Game::game_images[Game::eImages::CIRCLE];
	radius_rect = Game::GetRect(rad_img, 0,0);

	radius = rect->w / 5.0;
	radius_rect->w = radius * 2.0;
	radius_rect->h = radius * 2.0;
	center.x = rect->w * .5;
	center.y = rect->h * .5;
	radius_rect->x = pos_x + center.x - radius_rect->w / 2.0;
	radius_rect->y = pos_y + center.y - radius_rect->h / 2.0;

	orig_size.first = rect->w;
	orig_size.second = rect->h;

	CalculateCentreOffset();
}

void BlackHole::CalculateCentreOffset()
{
	offset.first = s_w / 2.0 - ((float)pos_x + center.x);
	offset.second = s_h / 2.0 - ((float)pos_y + center.y);
}


void BlackHole::InterpCentre(float dt, float total_interp_secs)
{
	float total_dt = dt / total_interp_secs;
	if (enlarge)
	{
		// enlarge the blackhole by 3* its total size over total_dt of frame
		enlarge_offset.first = (orig_size.first * 9.0f) * total_dt;
		enlarge_offset.second = (orig_size.second * 9.0f) * total_dt;

		// amount of offset to add to the position values (x,y) based on total_dt of frame
		curr_offset.first = offset.first * total_dt;
		curr_offset.second = offset.second * total_dt;
	}
	if (deflate)
	{
		// do things
	}

	// alter pos values (add offset to interp to centre) but also re-center by taking into account half of the enlarge offset of the frame
	pos_x += curr_offset.first - (enlarge_offset.first / 2.0);
	pos_y += curr_offset.second - (enlarge_offset.second / 2.0);

	// update rect
	rect->x = pos_x;
	rect->y = pos_y;
	rect->w += enlarge_offset.first;
	rect->h += enlarge_offset.second;
	radius = rect->w / 5.0;

	// update SDL_Point center pos to ensure it rotate using correct center point
	center.x = rect->w * .5;
	center.y = rect->h * .5;
}


void BlackHole::NewPosition(const int screen_width, const int screen_height)
{
	pos_x = rect->x = (((float)rand() / (float)RAND_MAX)) * ((float)screen_width - rect->w);
	pos_y = rect->y = (((float)rand() / (float)RAND_MAX)) * ((float)screen_height - rect->h);

	radius_rect->x = pos_x + center.x - radius_rect->w / 2.0;
	radius_rect->y = pos_y + center.y - radius_rect->h / 2.0;
	orig_pos.first = pos_x;
	orig_pos.second = pos_y;
	CalculateCentreOffset();
}


void BlackHole::Update(const float dt)
{
	angle += angle_modifier * dt;
	angle = angle % 360;
}



void BlackHole::Draw() const
{
	SDL_RenderCopyEx(Game::gRenderer, img, NULL, rect, angle, &center, SDL_FLIP_NONE);
	if (debug)
	{
		SDL_RenderCopy(Game::gRenderer, rad_img, NULL, radius_rect);
	}

}


BlackHole::~BlackHole()
{
	//SDL_DestroyTexture(img);
	img = nullptr;
	delete rect;
	rect = nullptr;
	delete radius_rect;
	radius_rect = nullptr;
}