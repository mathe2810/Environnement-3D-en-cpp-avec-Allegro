#ifndef VERTEX_H
#define VERTEX_H

// Structure pour les points 3D
struct Vertex {
    float x, y, z;
    float r, g, b; // Composantes de couleur
    float u, v;    // Coordonnées de texture
};

#endif // VERTEX_H