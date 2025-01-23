#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "vertex.h"
#include "triangle.h"



// Fonction pour faire tourner un point autour de l'axe X
void rotateX(Vertex& vertex, float angle, const Vertex& ref);

// Fonction pour faire tourner un point autour de l'axe Y
void rotateY(Vertex& vertex, float angle, const Vertex& ref);

// Fonction pour faire tourner un point autour de l'axe Z
void rotateZ(Vertex& vertex, float angle, const Vertex& ref);

// Fonction pour déplacer un point
void translate(Vertex& vertex, float dx, float dy, float dz);

#endif // OBJECT_H