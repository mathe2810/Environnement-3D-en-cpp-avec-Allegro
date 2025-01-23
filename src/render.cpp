#include "../includes/render.h"

#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// Fonction pour calculer l'intensité de la lumière
float calculateLightIntensity(const Vertex& vertex, const Vertex& lightSource, float intensity) {
    float dx = lightSource.x - vertex.x;
    float dy = lightSource.y - vertex.y;
    float dz = lightSource.z - vertex.z;
    float distance = sqrt(dx * dx + dy * dy + dz * dz);
    return  intensity / (distance * distance); // Inverse square law
}

// Fonction pour calculer la distance moyenne d'un triangle par rapport à la caméra
float calculateTriangleDistance(const Triangle& triangle, const std::vector<Vertex>& vertices) {
    float z1 = vertices[triangle.indices[0]].z;
    float z2 = vertices[triangle.indices[1]].z;
    float z3 = vertices[triangle.indices[2]].z;
    return (z1 + z2 + z3) / 3.0f;
}

Vertex calculCentreTriangle(const Triangle& triangle, const std::vector<Vertex>& vertices) {
    Vertex centre;
    centre.x = (vertices[triangle.indices[0]].x + vertices[triangle.indices[1]].x + vertices[triangle.indices[2]].x) / 3.0f;
    centre.y = (vertices[triangle.indices[0]].y + vertices[triangle.indices[1]].y + vertices[triangle.indices[2]].y) / 3.0f;
    centre.z = (vertices[triangle.indices[0]].z + vertices[triangle.indices[1]].z + vertices[triangle.indices[2]].z) / 3.0f;
    centre.r = (vertices[triangle.indices[0]].r + vertices[triangle.indices[1]].r + vertices[triangle.indices[2]].r) / 3.0f;
    centre.g = (vertices[triangle.indices[0]].g + vertices[triangle.indices[1]].g + vertices[triangle.indices[2]].g) / 3.0f;
    centre.b = (vertices[triangle.indices[0]].b + vertices[triangle.indices[1]].b + vertices[triangle.indices[2]].b) / 3.0f;
    return centre;
}

// Fonction pour projeter un point 3D sur un plan 2D
void projectVertex(const Vertex& vertex, float& x2D, float& y2D, float distance) {
    x2D = vertex.x * distance / (distance + vertex.z);
    y2D = vertex.y * distance / (distance + vertex.z);
}

//Fonction pour dessiner les objets 3D
void Render(const std::vector<Vertex>& vertices, const std::vector<Triangle>& triangles, const Vertex& lightSource, float intensity, float distance) {
    // Dessiner les triangles projetés
        for (const auto& triangle : triangles) {
            if (vertices[triangle.indices[0]].z < -1.0f && vertices[triangle.indices[1]].z < -1.0f && vertices[triangle.indices[2]].z < -1.0f) {
                continue; // Ne pas dessiner les triangles derrière la caméra
            }
            float x2D1, y2D1, x2D2, y2D2, x2D3, y2D3;
            projectVertex(vertices[triangle.indices[0]], x2D1, y2D1, distance);
            projectVertex(vertices[triangle.indices[1]], x2D2, y2D2, distance);
            projectVertex(vertices[triangle.indices[2]], x2D3, y2D3, distance);

            Vertex moy = calculCentreTriangle(triangle, vertices);

            // Calculer l'intensité de la lumière pour chaque sommet
            float intensity = calculateLightIntensity(moy, lightSource, 2.5f);

            // Limiter l'intensité de la lumière à 1.0
            intensity = std::min(intensity, 1.0f);

            // Couleurs des sommets en fonction de l'intensité de la lumière
            ALLEGRO_COLOR color = al_map_rgb(moy.r * 255 * intensity, moy.g * 255 * intensity, moy.b * 255 * intensity);

            // Dessiner le triangle avec les couleurs calculées
            al_draw_filled_triangle(
                400 + x2D1 * 100, 300 - y2D1 * 100,
                400 + x2D2 * 100, 300 - y2D2 * 100,
                400 + x2D3 * 100, 300 - y2D3 * 100,
                color
            );
        }

}