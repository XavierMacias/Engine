#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"
#include "MathGeoLib/Math/float2.h"
#include "SDL/include/SDL_scancode.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{

public:

	ModuleInput();

	// Destructor
	virtual ~ModuleInput();

	// Called before render is available
	bool Init();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	update_status PreUpdate();

	// Called before quitting
	bool CleanUp();

	bool CheckKey(SDL_Scancode key);

public:
	int wheel;	
	bool Lpressed = false;
	bool Rpressed = false;	
	
private:	
	const Uint8* keyboard = NULL;	
};

#endif // __MODULEINPUT_H__