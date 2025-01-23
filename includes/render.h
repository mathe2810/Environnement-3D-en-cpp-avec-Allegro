#ifndef RENDER_H
#define RENDER_H

#include <vector>
#include "vertex.h"
#include "triangle.h"

//Fonction de calcul:

// Fonction pour calculer l'intensité de la lumière
float calculateLightIntensity(const Vertex& vertex, const Vertex& lightSource, float intensity);
// Fonction pour calculer la distance moyenne d'un triangle par rapport à la caméra
float calculateTriangleDistance(const Triangle& triangle, const std::vector<Vertex>& vertices);
// Fonction pour calculer le centre d'un triangle
Vertex calculCentreTriangle(const Triangle& triangle, const std::vector<Vertex>& vertices);

//Fonction de renderisme:

// Fonction pour projeter un point 3D sur un plan 2D
void projectVertex(const Vertex& vertex, float& x2D, float& y2D, float distance);
// Fonction pour dessiner les objets 3D
void Render(const std::vector<Vertex>& vertices, const std::vector<Triangle>& triangles, const Vertex& lightSource, float intensity, float distance);

#endif // RENDER_H