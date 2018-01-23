//#include "objects.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "video.h"

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	struct SVideoContext* context = Vid_CreateVideoContext("teste", 640, 480, SDL_WINDOW_OPENGL, SDL_RENDERER_ACCELERATED, 2);
	Obj_CreateObject(&context->objlists[0], "aliendude.png", 120, 120, 16, 32, 64, 64, 32, 64, 1, SDL_FLIP_NONE, 0.0, NULL);
	
	printf("The Video Context renderer address is %d\n", context->render);
	printf("The ObjList renderer address is %d\n", context->objlists[0].renderer);
	
	while(1)
	{
	Vid_UpdateScreen(context, 1);
	SDL_Delay(64);
	}
}
