#pragma once


Vect3d phongIllumination(Vect3d k_a, Vect3d k_d, Vect3d k_s, float alpha, Vect3d p, Vect3d eye, Vect3d normal);
Vect3d phongContribForLight(Vect3d k_d, Vect3d k_s, float alpha, Vect3d p, Vect3d eye, Vect3d lightPos, Vect3d lightIntensity, Vect3d normal);
//float getLight(Vect3d p);

//void raymarcheRay(Color* color, Vect3d ro, Vect3d rd);
void castRay(Vect3d ro, Vect3d rd, Color* color);
