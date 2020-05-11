
#include "values.h"
#include "mathlib.h"
#include "objects.h"
#include "rendering.h"

#include <math.h>
#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>

Vect3d Shade(float distance, Vect3d* rayEnergy)
{
    if (distance < MAX_VIEW_DISTANCE + EPSILON)
    {
        Vect3d specular = (Vect3d){ 0.6f, 0.6f, 0.6f };

        *rayEnergy = mulVect3d(*rayEnergy, specular);
        return (Vect3d) { 0, 0, 0 };
    }
    else
    {
        *rayEnergy = (Vect3d) { 0.f, 0.f, 0.f };
        //Skycolor
        return (Vect3d) { 0.207f, 0.617f, 0.922f };
    }
}

void castRay(Vect3d ro, Vect3d rd, Color* color) 
{
    int i, k, bestI;
    float t, bestT;
    bestI = 0;
    bestT = MAX_VIEW_DISTANCE + EPSILON;
    bool reflect;
    
    Vect3d rayColor = (Vect3d){ 0.f, 0.f, 0.f };

    Vect3d rayEnergy = (Vect3d){ 1.f, 1.f, 1.f };

    Vect3d normal = (Vect3d){ -1.f, -1.f, -1.f };

    //Phong
    Vect3d K_a = (Vect3d){ 0.2f, 0.2f, 0.2f };
    Vect3d K_d = (Vect3d){ 0.7f, 0.2f, 0.2f };
    Vect3d K_s = (Vect3d){ 1.0f, 1.0f, 1.0f };
    float shininess = 3.0f;

    for (i = 0; i <= MAX_REFLECTIONS; i++)
    {
        bestT = MAX_VIEW_DISTANCE;
        reflect = false;
        for (k = 0; k < 2; k++)// For each reflectives objects
        {
            t = distanceToSphere(ro, rd, spheres[k]);
            if (t < 0) continue;
            if (t < bestT)
            {
                reflect = true;
                bestI = k;
                bestT = t;
            }
        }
        // For each non-reflectives objects
        t = distanceToGround(rd, ro);
        if (t > 0 && t < bestT)
        {
            reflect = false;
            bestT = t;
        }

        if (bestT >= MAX_VIEW_DISTANCE) break;
        ro = addVect3d(ro, scalefVect3d(rd, bestT));
        if (reflect == false)
        {
            normal = groundNormal();
            rayColor = addVect3d(rayColor, mulVect3d(rayEnergy, phongIllumination(K_a, K_d, K_s, shininess, ro, rd, normal))); // color of the ground
            break;
        }
        else {
            normal = sphereNormal(ro, spheres[bestI].pos);
            if (i != MAX_REFLECTIONS){
            rayColor = addVect3d(rayColor, mulVect3d(rayEnergy, Shade(bestT, &rayEnergy)));
            } else {
                rayColor = addVect3d(rayColor, mulVect3d(rayEnergy, phongIllumination(K_a, K_d, K_s, shininess, ro, rd, normal))); // color of the sphere
            }
        }
        if (normal.x == normal.z && normal.x == normal.y && normal.x == -1.f) printf("Error\n");
        rd = reflectedRay(rd, normal);
        ro = addVect3d(ro, scalefVect3d(normal, EPSILON));
    }
    if (bestT < MAX_VIEW_DISTANCE)
    {

        *color = (Color){ min((int)(floorf(rayColor.x * 255)),255),min((int)(floorf(rayColor.y * 255)),255) ,min((int)(floorf(rayColor.z * 255)),255), SDL_ALPHA_OPAQUE };
    }
    else {
        
        Color temp = skyboxColor(rd);
        rayColor = addVect3d(rayColor, mulVect3d(rayEnergy, (Vect3d) { (float)temp.R / 256, (float)temp.G / 256, (float)temp.B / 256 }));
        *color = (Color){ min((int)(floorf(rayColor.x * 255)),255),min((int)(floorf(rayColor.y * 255)),255) ,min((int)(floorf(rayColor.z * 255)),255), SDL_ALPHA_OPAQUE };
        
        //*color = (Color){ 0.f, 0.f, 0.f, SDL_ALPHA_OPAQUE};
    }
}

/*void raymarcheRay(Color* color, Vect3d ro, Vect3d rd)
{
    float distance = shortestDistanceToSurface(ro, rd, 0, MAX_VIEW_DISTANCE);
    //printf("x = %i , y = %i , distance = %f\n", k % WINDOW_WIDTH, WINDOW_LENGHT - k / WINDOW_WIDTH, distance);
    if (distance > MAX_VIEW_DISTANCE - EPSILON)
    {
        //Didn't hit aanything
        color->R = 0;
        color->G = 0;
        color->B = 0;
        return;
    }
    ro = (Vect3d){ ro.x + rd.x * distance, ro.y + rd.y * distance, ro.z + rd.z * distance };

    //float dif = getLight(ro);
    //float dif = 1.f;

    //*color = (Color) { 112 * dif, 168 * dif, 237 * dif, SDL_ALPHA_OPAQUE };
    //return;
    //Phong implementation
    
    Vect3d K_a = (Vect3d){ 0.2f, 0.2f, 0.2f };
    Vect3d K_d = (Vect3d){ 0.7f, 0.2f, 0.2f };
    Vect3d K_s = (Vect3d){ 1.0f, 1.0f, 1.0f };
    float shininess = 3.0f;

    Vect3d rayColor = phongIllumination(K_a, K_d, K_s, shininess, ro, rd);
    //printf("colorR = %f\n", rayColor.x * 255);
    *color = (Color){ min((int)(floorf(rayColor.x * 255)),255),min((int)(floorf(rayColor.y * 255)),255) ,min((int)(floorf(rayColor.z * 255)),255), SDL_ALPHA_OPAQUE};
    //*color = (Color){ (int)floorf(rayColor.x) * 4, (int)floorf(rayColor.y) * 4, (int)floorf(rayColor.z) * 4, SDL_ALPHA_OPAQUE };
    
    return;
}*/

/*float getLight(Vect3d p) // Useless since Phong
{
    Vect3d lightPos = (Vect3d){ 4.0f*sinf((float)INTERN_TICKS * 0.1f), 5, 4.0f*cosf((float)INTERN_TICKS*0.1f) };
    Vect3d L = normalize3d((Vect3d) { lightPos.x - p.x, lightPos.y - p.y, lightPos.z - p.z });
    Vect3d N = sphereNormal(p, spheres[0].pos);

    float dif = dot(L, N);
    dif = dif > 1.f ? 1.f : dif;
    dif = dif < 0.f ? 0.f : dif;
    float d = shortestDistanceToSurface((Vect3d) { p.x + N.x * 0.02f, p.y + N.y * 0.02f, p.y + N.y * 0.02f }, L, 0, MAX_VIEW_DISTANCE);
    if (d<norme((Vect3d) { lightPos.x - p.x, lightPos.y - p.y, lightPos.z - p.z })) dif *= .1f;

    return dif;
}*/

/**
 * Lighting contribution of a single point light source via Phong illumination.
 *
 * The Vect3d returned is the RGB color of the light's contribution.
 *
 * k_a: Ambient color
 * k_d: Diffuse color
 * k_s: Specular color
 * alpha: Shininess coefficient
 * p: position of point being lit
 * eye: the position of the camera
 * lightPos: the position of the light
 * lightIntensity: color/intensity of the light
 *
 * See https://en.wikipedia.org/wiki/Phong_reflection_model#Description
 */
Vect3d phongContribForLight(Vect3d k_d, Vect3d k_s, float alpha, Vect3d p, Vect3d eye,
    Vect3d lightPos, Vect3d lightIntensity, Vect3d normal) {
    Vect3d N = normal;
    Vect3d L = normalize3d((Vect3d) { lightPos.x - p.x, lightPos.y - p.y, lightPos.z - p.z });
    Vect3d V = normalize3d((Vect3d) { eye.x - p.x, eye.y - p.y, eye.z - p.z });
    float dotLN = dot(L, N);
    Vect3d R = normalize3d(subVect3d(scalefVect3d(N, 2*dotLN), L));
    
    float dotRV = dot(R, V);
    float coef;

    if (dotLN < 0.0f) {
        // Light not visible from this point on the surface
        return (Vect3d) { 0.0f, 0.0f, 0.0f };
    }

    if (dotRV < 0.0f) {
        // Light reflection in opposite direction as viewer, apply only diffuse
        // component
        //return lightIntensity * (k_d * dotLN);
        return (Vect3d) { lightIntensity.x* k_d.x* dotLN, lightIntensity.y* k_d.y* dotLN, lightIntensity.z* k_d.z* dotLN };
    }
    coef = powf(dotRV, alpha);
    return (Vect3d) {
        lightIntensity.x * (k_d.x * dotLN +  k_s.x * coef),
        lightIntensity.y * (k_d.y * dotLN + k_s.y * coef),
        lightIntensity.z * (k_d.z * dotLN + k_s.z * coef)
    };
    //return lightIntensity * (k_d * dotLN + k_s * pow(dotRV, alpha));
}

/**
 * Lighting via Phong illumination.
 *
 * The Vect3d returned is the RGB color of that point after lighting is applied.
 * k_a: Ambient color
 * k_d: Diffuse color
 * k_s: Specular color
 * alpha: Shininess coefficient
 * p: position of point being lit
 * eye: the position of the camera
 *
 * See https://en.wikipedia.org/wiki/Phong_reflection_model#Description
 */
Vect3d phongIllumination(Vect3d k_a, Vect3d k_d, Vect3d k_s, float alpha, Vect3d p, Vect3d eye, Vect3d normal) {
    const Vect3d ambientLight = (Vect3d) { 0.5f, 0.5f, 0.5f };
    Vect3d color = (Vect3d){ ambientLight.x * k_a.x, ambientLight.y * k_a.y, ambientLight.z * k_a.z };//ambientLight * k_a;

    Vect3d light1Pos = (Vect3d){ -5.0f * sinf(INTERN_TICKS * 0.1f),
        3.0f,
        5.0f * cosf(INTERN_TICKS * 0.1f) };
    Vect3d light1Intensity = (Vect3d){ 0.4f, 0.4f, 0.4f };

    Vect3d contrib;
    contrib = phongContribForLight(k_d, k_s, alpha, p, eye,
        light1Pos,
        light1Intensity, normal);
    color = (Vect3d){ color.x + contrib.x,color.y + contrib.y, color.z + contrib.z };

    /*
    Vect3d light2Pos = Vect3d(2.0 * sin(0.37 * iTime),
        2.0 * cos(0.37 * iTime),
        2.0);
    Vect3d light2Intensity = Vect3d(0.4, 0.4, 0.4);

    color += phongContribForLight(k_d, k_s, alpha, p, eye,
        light2Pos,
        light2Intensity);
    */
    return color;
}


