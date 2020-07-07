#include "TextureManager.h"
#include "Game.h"
#include "SDL.h"
#include <SDL_image.h>

using namespace std;


ImageData* TextureManager::load_image_data(string path, bool& allMediaLoaded)
// ( Called in the loadMedia() method )
// This loads the texture of the given path, then converts to the texture and returns the pointer to it.
{
	ImageData* newImData = new ImageData;

	SDL_Texture* newTexture = nullptr;
	cout << path << endl;

	// load initial surface image
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (tempSurface == NULL)
	{
		cout << path << " failed to load: SDL ERROR: " << IMG_GetError();
		allMediaLoaded = false;
	}
	else
	{
		cout << "converting";
		//Convert surface to a texture
		newTexture = SDL_CreateTextureFromSurface(Game::gRenderer, tempSurface);
		if (newTexture == NULL)
		{
			cout << "Unable to create texture " << path << " SDL Error: " << SDL_GetError() << endl;
			allMediaLoaded = false;
		}
		//Get rid of old temp loaded surface - VITAL!

		SDL_FreeSurface(tempSurface);

	}

	newImData->texture = newTexture;

	return newImData;
}