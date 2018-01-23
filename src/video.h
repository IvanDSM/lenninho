#include "global.h"
#include "objects.h"
#include <SDL2/SDL.h>
#include <stdio.h>

struct SVideoContext {
	SDL_Window*			window;
	SDL_Renderer*		render;
	int					layers;
	struct SObjList*	objlists;
};

struct SVideoContext* Vid_CreateVideoContext(char* window_title, int window_w, int window_h, SDL_WindowFlags window_flags, SDL_RendererFlags render_flags, int obj_layers);
int Vid_DestroyVideoContext(struct SVideoContext* context, int* render_switch);

int Vid_UpdateScreen(struct SVideoContext* context, int render_switch);
