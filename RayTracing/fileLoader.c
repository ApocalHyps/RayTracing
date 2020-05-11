#include <stdio.h>
#include <stdlib.h>

#include "mathlib.h"
#include "objects.h"
#include "fileLoader.h"
#include "values.h"

//-----------------|SKYBOX|---------------------------------

FILE* loadSkyboxFile()
{
	FILE* f;

	fopen_s(&f, "ressources/Skybox.bmp", "rb");
	if (f == NULL)
	{
		printf("Unable to load Skybox.bmp");
		exit(0);
	}

	return f;
}

void unloadSkyboxFile(FILE* f)
{
	fclose(f);
}

//----------------------------------------------------------