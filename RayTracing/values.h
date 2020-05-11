#pragma once
#include <SDL_surface.h>

extern int const WINDOW_WIDTH;
extern int const WINDOW_LENGHT;

extern float const FOV; // 70°
extern int const MAX_REFLECTIONS;
extern float const MAX_VIEW_DISTANCE;
extern float const COLIDE_PRECISION;
extern float const EPSILON;

extern int INTERN_TICKS;

extern struct Sphere* spheres;

extern SDL_Surface* skybox;