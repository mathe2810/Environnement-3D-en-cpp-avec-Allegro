#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <array>

// Structure pour les triangles
struct Triangle {
    unsigned int indices[3];
    float distance; // Distance moyenne du triangle par rapport à la caméra
};

#endif // TRIANGLE_H