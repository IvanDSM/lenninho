#include "global.h"
#include "objects.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int ObjList_CreateObjectList (struct SObjList* list, SDL_Renderer* renderer_pointer)
{
	/*if (renderer_pointer == NULL)
	{
		printf("%sAttempted to create an Object List with no pointer to a renderer! Set the pointer to your Video Context's renderer!\n", LENERR);
	}*/
	list = malloc(sizeof(struct SObjList));
	if (list == NULL)
	{
		printf("%sObject List allocation failed!\n", LENERR);
		return -1;
	}
	list->Last = NULL;
	list->Size = 0;
	list->renderer = renderer_pointer;
	return 0;
}

struct SObject* ObjList_Seek (struct SObjList* list, int index)
{
	struct SObject* object;
	int i;
	object = list->Last;
	if (object == NULL)
	{
		printf("%sAttempted to seek on an empty list!\n", LENERR);
		return NULL;
	}
	for (i = list->Size; i > index; i--)
	{
		object = object->Prev;
	}
	return object;
}

int ObjList_DestroyObjectList (struct SObjList* list)
{
	int i, errcheck, reportnum;

	// Deletes all objects in the list, from last to first.
	for (i = list->Size; i > 0; i--)
	{
		// This is for Lenninho's nested error reporting mechanism.
		errcheck = Obj_DestroyObject(list, i);
		reportnum = ReportError(errcheck, "Object List destruction failed! See error above.");
		if (reportnum <= -1)
			return reportnum;
	}

	// Then we free the list.
	free(list);
	list = NULL;
}

int Obj_CreateObject (struct SObjList* list, const char *sprites_path, int spr_x, int spr_y, int spr_w, int spr_h, int rnd_x, int rnd_y, int rnd_w, int rnd_h, int visible, SDL_RendererFlip flip, float angle, struct SObject* parent)
{
	struct SObject* new_obj = malloc(sizeof(struct SObject));
	if (new_obj == NULL)
	{
		printf("%sObject allocation failed!\n", LENERR);
		return -1;
	}

	// Importing spritesheet into the object
	SDL_Surface* temp_spritesheet;
	temp_spritesheet = IMG_Load(sprites_path);
	if (temp_spritesheet == NULL)
	{
		printf("%sError when loading spritesheet! IMG_Load Error: %s\n", LENERR, IMG_GetError());
		return -1;
	}
	new_obj->sprites = SDL_CreateTextureFromSurface(list->renderer, temp_spritesheet);

	// Defining the source rectangle
	new_obj->spr_rect.x = spr_x;
	new_obj->spr_rect.y = spr_y;
	new_obj->spr_rect.w = spr_w;
	new_obj->spr_rect.h = spr_h;

	// Defining the target rectangle
	new_obj->rnd_rect.x = rnd_x;
	new_obj->rnd_rect.y = rnd_y;
	new_obj->rnd_rect.w = rnd_w;
	new_obj->rnd_rect.h = rnd_h;

	// Miscellaneous properties
	new_obj->visible = visible;
	new_obj->flip = flip;
	new_obj->angle = angle;
	new_obj->parent = parent;
	new_obj->Next = NULL;

	// Insertion into given Object List
	if (list->Last != NULL) // If our list isn't empty...
	{
		list->Last->Next = new_obj; // we must make the current last object point to our new object...
		new_obj->Prev = list->Last; // and our new object point to the current last object.
	}
	else // However, if the list isn empty...
	{
		new_obj->Prev = NULL; // we should make our new object not point to any object, to avoid bugs.
		new_obj->Next = NULL;
	}
	list->Last = new_obj;
	list->Size++;
	return list->Size-1; // When everything goes fine and dandy, we return the index of the newly inserted object.
}

int Obj_DestroyObject (struct SObjList* list, int index)
{
	struct SObject* object = ObjList_Seek(list, index);
	if (object == NULL)
	{
		return ReportError(-1, "Object destruction failed! See error above.");
	}
	object->visible = 0; // To avoid rendering artifacts since our render thread is asynchronous.

	// First we make it 'disappear' from the list.
	object->Prev->Next = object->Next;
	object->Next->Prev = object->Prev;
	list->Size--;

	// Then we free our texture
	SDL_DestroyTexture(object->sprites);

	// Finally, we free the object itself.
	free(object);

	// If we have just emptied the list, we should set Last to NULL.
	if (list->Size == 0)
		list->Last = NULL;

	return 0;
}
