#include <SDL2/SDL.h>
#include <stdio.h>
#include "video.h"

struct SVideoContext* Vid_CreateVideoContext(char* window_title, int window_w, int window_h, SDL_WindowFlags window_flags, SDL_RendererFlags render_flags, int obj_layers)
{
	
	struct SVideoContext* context;
	
	// Allocating the Video Context
	context = malloc(sizeof(struct SVideoContext));
	if (context == NULL)
	{
		printf("%sVideo Context allocation failed!\n", LENERR);
		return -1;
	}

	// Creating the window
	context->window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_w, window_h, window_flags);
	if (context->window == NULL)
	{
		printf("%sError when creating window! SDL_CreateWindow Error: %s\n", LENERR, SDL_GetError());
		return -1;
	}

	// Creating the renderer (use this renderer when creating Object Lists!)
	context->render = SDL_CreateRenderer(context->window, -1, render_flags);
	// We ALWAYS use SDL_BLENDMODE_BLEND. If you change this, stuff will most likely go haywire.
	SDL_SetRenderDrawBlendMode(context->render, SDL_BLENDMODE_BLEND);
	if (context->render == NULL)
	{
		printf("%sError when creating renderer! SDL_CreateRenderer Error: %s\n", LENERR, SDL_GetError());
		return -1;
	}

	// Creating the layers (Object Lists)
	context->layers = obj_layers;
	context->objlists = calloc(obj_layers, sizeof(struct SObjList));
	if (context->objlists == NULL)
	{
		printf("%sVideo Context Layers allocation failed!", LENERR);
		return -1;
	}
	int i;
	for (i = 0; i < obj_layers; i++)
	{
		ObjList_CreateObjectList(&context->objlists[i], context->render);
	}

	
	
	return context;
	
}

int Vid_DestroyVideoContext(struct SVideoContext* context, int* render_switch)
{
	int i, errcheck, reportnum;
	*render_switch = 0; // If we don't this, our rendering thread will go kaboom!

	// Destroys all the object lists
	for (i = context->layers; i > 0; i--)
	{
		errcheck = ObjList_DestroyObjectList(&context->objlists[i]);
		reportnum = ReportError(errcheck, "Video Context destruction failed! See error above.");
		if (reportnum <= -1)
			return reportnum;
	}

	// Destroy the renderer
	SDL_DestroyRenderer(context->render);

	// Destroy the window
	SDL_DestroyWindow(context->window);

	free(context);
	context = NULL;

	return 0;
}

int Vid_UpdateScreen(struct SVideoContext* context, int render_switch)
{
	SDL_SetRenderDrawColor(context->render, 0, 0, 0, 255);
	//SDL_RenderClear(context->render);

	int i, objcount = 0;
	struct SObject* curobj;
	if (render_switch == 1)
	{
		for (i = 0; i < context->layers; i++)
		{
			if (context->objlists[i].Size > 0)
			{
				curobj = context->objlists[i].Last;
				while (objcount < context->objlists[i].Size)
				{
					if (curobj->visible)
					{
						if (curobj->flip == SDL_FLIP_NONE && curobj->angle == 0.0)
							SDL_RenderCopy(context->render, curobj->sprites, &curobj->spr_rect, &curobj->rnd_rect);
						else
							SDL_RenderCopyEx(context->render, curobj->sprites, &curobj->spr_rect, &curobj->rnd_rect, curobj->angle, NULL, curobj->flip);
					}
					curobj = curobj->Next;
					objcount++;
				}
			}
		}
		SDL_RenderPresent(context->render);
		return 1;
	}
	else
		return 0;
}
