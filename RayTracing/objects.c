#include <math.h>
#include <stdio.h>
#include <SDL_surface.h>

#include "mathlib.h"
#include "objects.h"
#include "rendering.h"
#include "values.h"




Vect3d reflectedRay(Vect3d rd, Vect3d normal)
{
	return subVect3d(rd, scalefVect3d(normal, 2 * dot(normal, rd)));
}

Vect3d groundRayReflection(Vect3d rd)
{
	return (Vect3d) { rd.x, rd.y, -rd.z };
}

Vect3d groundNormal()
{
	return (Vect3d) { 0.f, 0.f, 1.f };
}

float distanceToGround(Vect3d rd, Vect3d ro)
{
	return -ro.z / rd.z;
}

Vect3d sphereNormal(Vect3d contactPoint, Vect3d sphereCenter)
{
	return normalize3d(subVect3d(contactPoint, sphereCenter));
}

Vect3d sphereRayReflection(Vect3d rd, Vect3d contactPoint, Vect3d normal)
{
	return subVect3d(rd, scalefVect3d(normal, 2 * dot(normal, rd)));
}

float distanceToSphere(Vect3d ro, Vect3d rd, Sphere sphere)
{
	Vect3d v = subVect3d(ro, sphere.pos);
	float vDotd = dot(v, rd);
	float vSquare = dot(v, v);
	float t1 = -vDotd;
	float t2Square = vDotd * vDotd - (vSquare - sphere.radius * sphere.radius);
	if (t2Square < 0.f)
	{
		return -1.f;
	}
	float t2 = sqrtf(t2Square);
	float t1Square = t1 * t1;
	if (t1Square - t2Square > 0) // racines de même signe
	{
		return t1 + t2 > t1 - t2 ? t1 - t2 : t1 + t2; //return min
	}
	else // une racine pos et une négative (seule la positive nous interesse)
	{
		return t1 + t2 < t1 - t2 ? t1 - t2 : t1 + t2; //return max
	}
}

Color skyboxColor(Vect3d rd)
{
	Color returned;
	float cp = norme2d((Vect2d) { rd.x, rd.y });
	float st = rd.z;
	//printf("cp = %f , st = %f", cp, st);
	int y = ((int)floorf((1 + st) * skybox->h / 2))% skybox->h;
	int x = ((int)floorf((1 + cp) * skybox->w / 2))% skybox->w;
	//printf("in direction %f,%f,%f color at map : x = %i, y = %i", rd.x, rd.y, rd.z, x, y);
	Uint32* pixels = (Uint32*)skybox->pixels;
	Uint32 pixel = pixels[(y * skybox->h) + x];
	SDL_GetRGBA(pixel, skybox->format, &(returned.R), &(returned.G), &(returned.B), &(returned.A));
	//printf("in direction %f,%f,%f color = R:%i,G:%i,B:%i", rd.x, rd.y, rd.z, returned.R, returned.G, returned.B);
	return returned;
}