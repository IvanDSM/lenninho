#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

typedef SDL_RendererFlip sdlflip; // Original type name is too large

struct SObject
{
	struct SObject*	Prev;

	SDL_Texture*	sprites;
	SDL_Rect		spr_rect;
	SDL_Rect		rnd_rect;
	int				visible;
	sdlflip			flip;
	float			angle;
	struct SObject* parent;

	struct SObject*	Next;
};

struct SObjList
{
	SDL_Renderer*	renderer; // See docs/objlist_renderer.txt.
	struct SObject*	Last;
	int				Size;
};

int ObjList_CreateObjectList (struct SObjList* list, SDL_Renderer* renderer_pointer);
struct SObject* ObjList_Seek (struct SObjList* list, int index);
int ObjList_DestroyObjectList (struct SObjList* list);

int Obj_CreateObject (struct SObjList* list, const char *sprites_path, int spr_x, int spr_y, int spr_w, int spr_h, int rnd_x, int rnd_y, int rnd_w, int rnd_h, int visible, SDL_RendererFlip flip, float angle, struct SObject* parent);
int Obj_DestroyObject (struct SObjList* list, int index);

