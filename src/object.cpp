#include "../includes/object.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Fonction pour faire tourner un point autour de l'axe X
void rotateX(Vertex& vertex, float angle, const Vertex& ref) {
    float rad = angle * M_PI / 180.0f;
    float y = ref.y + (vertex.y - ref.y) * cos(rad) - (vertex.z - ref.z) * sin(rad);
    float z = ref.z + (vertex.y - ref.y) * sin(rad) + (vertex.z - ref.z) * cos(rad);
    vertex.y = y;
    vertex.z = z;
}

// Fonction pour faire tourner un point autour de l'axe Y
void rotateY(Vertex& vertex, float angle, const Vertex& ref) {
    float rad = angle * M_PI / 180.0f;
    float x = ref.x + (vertex.x - ref.x) * cos(rad) + (vertex.z - ref.z) * sin(rad);
    float z = ref.z - (vertex.x - ref.x) * sin(rad) + (vertex.z - ref.z) * cos(rad);
    vertex.x = x;
    vertex.z = z;
}

// Fonction pour faire tourner un point autour de l'axe Z
void rotateZ(Vertex& vertex, float angle, const Vertex& ref) {
    float rad = angle * M_PI / 180.0f;
    float x = ref.x + (vertex.x - ref.x) * cos(rad) - (vertex.y - ref.y) * sin(rad);
    float y = ref.y + (vertex.x - ref.x) * sin(rad) + (vertex.y - ref.y) * cos(rad);
    vertex.x = x;
    vertex.y = y;
}

// Fonction pour déplacer un point
void translate(Vertex& vertex, float dx, float dy, float dz) {
    vertex.x += dx;
    vertex.y += dy;
    vertex.z += dz;
}