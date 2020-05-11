#pragma once

typedef struct Color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
}Color;

typedef struct Object {
	//float (*signedDist)(Vect3d, Vect3d, float);
	Color color;
	Vect3d pos;
}Object;

typedef struct Sphere {
	Vect3d pos;
	float radius;
}Sphere;

typedef struct RayHit {
	Vect3d position;
	float distance;
	Vect3d normal;
}RayHit;

float distanceToGround(Vect3d rd, Vect3d ro);
Vect3d groundNormal();

Vect3d reflectedRay(Vect3d rd, Vect3d normal);
Vect3d sphereRayReflection(Vect3d rd, Vect3d contactPoint, Vect3d normal);
float distanceToSphere(Vect3d ro, Vect3d rd, Sphere sphere);
Vect3d sphereNormal(Vect3d contactPoint, Vect3d sphereCenter);

Color skyboxColor(Vect3d rd);
