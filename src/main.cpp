#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Structure pour les points 3D
struct Vertex {
    float x, y, z;
    float r, g, b; // Composantes de couleur
    float u, v;    // Coordonnées de texture
};

// Structure pour les triangles
struct Triangle {
    unsigned int indices[3];
    float distance; // Distance moyenne du triangle par rapport à la caméra
};

std::vector<Vertex> vertices;
std::vector<Triangle> triangles;
Vertex referencePoint = {0.0f, 0.0f, 0.0f}; // Point de référence pour les rotations

// Fonction pour projeter un point 3D sur un plan 2D
void projectVertex(const Vertex& vertex, float& x2D, float& y2D, float distance) {
    x2D = vertex.x * distance / (distance + vertex.z);
    y2D = vertex.y * distance / (distance + vertex.z);
}

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

// Fonction pour calculer l'intensité de la lumière
float calculateLightIntensity(const Vertex& vertex, const Vertex& lightSource) {
    float dx = lightSource.x - vertex.x;
    float dy = lightSource.y - vertex.y;
    float dz = lightSource.z - vertex.z;
    float distance = sqrt(dx * dx + dy * dy + dz * dz);
    return 1.0f / (distance * distance); // Inverse square law
}

// Fonction pour calculer la distance moyenne d'un triangle par rapport à la caméra
float calculateTriangleDistance(const Triangle& triangle) {
    float z1 = vertices[triangle.indices[0]].z;
    float z2 = vertices[triangle.indices[1]].z;
    float z3 = vertices[triangle.indices[2]].z;
    return (z1 + z2 + z3) / 3.0f;
}

int main() {
    std::cout << "Starting application..." << std::endl;

    // Initialiser Allegro
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro!" << std::endl;
        return -1;
    }

    // Initialiser les addons
    if (!al_init_primitives_addon()) {
        std::cerr << "Failed to initialize primitives addon!" << std::endl;
        return -1;
    }

    if (!al_init_font_addon()) {
        std::cerr << "Failed to initialize font addon!" << std::endl;
        return -1;
    }

    if (!al_init_ttf_addon()) {
        std::cerr << "Failed to initialize ttf addon!" << std::endl;
        return -1;
    }

    // Initialiser le clavier
    if (!al_install_keyboard()) {
        std::cerr << "Failed to initialize keyboard!" << std::endl;
        return -1;
    }

    // Créer une fenêtre
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    if (!display) {
        std::cerr << "Failed to create display!" << std::endl;
        return -1;
    }

    // Définir le titre de la fenêtre
    al_set_window_title(display, "Calculateur de points 3D avec Allegro");

    // Charger une police
    ALLEGRO_FONT* font = al_load_ttf_font("arial.ttf", 24, 0);
    if (!font) {
        std::cerr << "Failed to load font!" << std::endl;
        al_destroy_display(display);
        return -1;
    }
    std::cout << "Font loaded successfully." << std::endl;

    // Créer une file d'événements
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!event_queue) {
        std::cerr << "Failed to create event queue!" << std::endl;
        al_destroy_font(font);
        al_destroy_display(display);
        return -1;
    }
    std::cout << "Event queue created successfully." << std::endl;

    // Enregistrer les sources d'événements
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // Définir la couleur de fond
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    // Créer des points 3D
    vertices = {
        { -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
        {  1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
        {  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
        { -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
        { -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
        {  1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f },
        {  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
        { -1.0f,  1.0f, -1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f }
    };

    // Définir les triangles
    triangles = {
        {{0, 1, 2}, 0.0f}, {{2, 3, 0}, 0.0f},  // Face avant
        {{4, 5, 6}, 0.0f}, {{6, 7, 4}, 0.0f},  // Face arrière
        {{0, 1, 5}, 0.0f}, {{5, 4, 0}, 0.0f},  // Face inférieure
        {{2, 3, 7}, 0.0f}, {{7, 6, 2}, 0.0f},  // Face supérieure
        {{0, 3, 7}, 0.0f}, {{7, 4, 0}, 0.0f},  // Face gauche
        {{1, 2, 6}, 0.0f}, {{6, 5, 1}, 0.0f}   // Face droite
    };

    // Distance de projection
    float distance = 5.0f;

    // Définir la source de lumière
    Vertex lightSource = { 0.0f, 0.0f, 5.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };

    // Tableau pour suivre l'état des touches
    bool key[ALLEGRO_KEY_MAX] = { false };

    // Boucle principale
    bool running = true;
    while (running) {
        // Effacer l'écran
        al_clear_to_color(al_map_rgb(0, 0, 0));

        // Calculer la distance moyenne de chaque triangle par rapport à la caméra
        for (auto& triangle : triangles) {
            triangle.distance = calculateTriangleDistance(triangle);
        }

        // Trier les triangles par distance décroissante
        std::sort(triangles.begin(), triangles.end(), [](const Triangle& a, const Triangle& b) {
            return a.distance > b.distance;
        });

        // Dessiner les triangles projetés
        for (const auto& triangle : triangles) {
            float x2D1, y2D1, x2D2, y2D2, x2D3, y2D3;
            projectVertex(vertices[triangle.indices[0]], x2D1, y2D1, distance);
            projectVertex(vertices[triangle.indices[1]], x2D2, y2D2, distance);
            projectVertex(vertices[triangle.indices[2]], x2D3, y2D3, distance);

            // Calculer l'intensité de la lumière pour chaque sommet
            float intensity1 = calculateLightIntensity(vertices[triangle.indices[0]], lightSource);
            float intensity2 = calculateLightIntensity(vertices[triangle.indices[1]], lightSource);
            float intensity3 = calculateLightIntensity(vertices[triangle.indices[2]], lightSource);

            // Limiter l'intensité de la lumière à 1.0
            intensity1 = std::min(intensity1, 1.0f);
            intensity2 = std::min(intensity2, 1.0f);
            intensity3 = std::min(intensity3, 1.0f);

            // Couleurs des sommets en fonction de l'intensité de la lumière
            ALLEGRO_COLOR color1 = al_map_rgb(vertices[triangle.indices[0]].r * 255 * intensity1, vertices[triangle.indices[0]].g * 255 * intensity1, vertices[triangle.indices[0]].b * 255 * intensity1);
            ALLEGRO_COLOR color2 = al_map_rgb(vertices[triangle.indices[1]].r * 255 * intensity2, vertices[triangle.indices[1]].g * 255 * intensity2, vertices[triangle.indices[1]].b * 255 * intensity2);
            ALLEGRO_COLOR color3 = al_map_rgb(vertices[triangle.indices[2]].r * 255 * intensity3, vertices[triangle.indices[2]].g * 255 * intensity3, vertices[triangle.indices[2]].b * 255 * intensity3);

            // Dessiner le triangle avec les couleurs calculées
            al_draw_filled_triangle(
                400 + x2D1 * 100, 300 - y2D1 * 100,
                400 + x2D2 * 100, 300 - y2D2 * 100,
                400 + x2D3 * 100, 300 - y2D3 * 100,
                color1
            );
            al_draw_filled_triangle(
                400 + x2D1 * 100, 300 - y2D1 * 100,
                400 + x2D2 * 100, 300 - y2D2 * 100,
                400 + x2D3 * 100, 300 - y2D3 * 100,
                color2
            );
            al_draw_filled_triangle(
                400 + x2D1 * 100, 300 - y2D1 * 100,
                400 + x2D2 * 100, 300 - y2D2 * 100,
                400 + x2D3 * 100, 300 - y2D3 * 100,
                color3
            );
        }

        // Afficher le rendu
        al_flip_display();

        // Gérer les événements
        ALLEGRO_EVENT ev;
        while (al_get_next_event(event_queue, &ev)) {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                key[ev.keyboard.keycode] = true;
            } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                key[ev.keyboard.keycode] = false;
            }
        }

        // Appliquer les transformations en fonction des touches enfoncées
        if (key[ALLEGRO_KEY_UP]) {
            for (auto& vertex : vertices) {
                translate(vertex, 0.0f, 0.1f, 0.0f);
            }
            translate(referencePoint, 0.0f, 0.1f, 0.0f);
        }
        if (key[ALLEGRO_KEY_DOWN]) {
            for (auto& vertex : vertices) {
                translate(vertex, 0.0f, -0.1f, 0.0f);
            }
            translate(referencePoint, 0.0f, -0.1f, 0.0f);
        }
        if (key[ALLEGRO_KEY_LEFT]) {
            for (auto& vertex : vertices) {
                translate(vertex, -0.1f, 0.0f, 0.0f);
            }
            translate(referencePoint, -0.1f, 0.0f, 0.0f);
        }
        if (key[ALLEGRO_KEY_RIGHT]) {
            for (auto& vertex : vertices) {
                translate(vertex, 0.1f, 0.0f, 0.0f);
            }
            translate(referencePoint, 0.1f, 0.0f, 0.0f);
        }
        if (key[ALLEGRO_KEY_Z]) {
            for (auto& vertex : vertices) {
                translate(vertex, 0.0f, 0.0f, 0.1f);
            }
            translate(referencePoint, 0.0f, 0.0f, 0.1f);
        }
        if (key[ALLEGRO_KEY_S]) {
            for (auto& vertex : vertices) {
                translate(vertex, 0.0f, 0.0f, -0.1f);
            }
            translate(referencePoint, 0.0f, 0.0f, -0.1f);
        }
        if (key[ALLEGRO_KEY_X]) {
            for (auto& vertex : vertices) {
                rotateX(vertex, 5.0f, referencePoint);
            }
        }
        if (key[ALLEGRO_KEY_C]) {
            for (auto& vertex : vertices) {
                rotateX(vertex, -5.0f, referencePoint);
            }
        }
        if (key[ALLEGRO_KEY_U]) {
            for (auto& vertex : vertices) {
                rotateY(vertex, -5.0f, referencePoint);
            }
        }
        if (key[ALLEGRO_KEY_Y]) {
            for (auto& vertex : vertices) {
                rotateY(vertex, 5.0f, referencePoint);
            }
        }
        if (key[ALLEGRO_KEY_Z]) {
            for (auto& vertex : vertices) {
                rotateZ(vertex, 5.0f, referencePoint);
            }
        }
        if (key[ALLEGRO_KEY_X]) {
            for (auto& vertex : vertices) {
                rotateZ(vertex, -5.0f, referencePoint);
            }
        }
        if (key[ALLEGRO_KEY_ESCAPE]) {
            running = false;
        }

        // Attendre un peu
        al_rest(0.01);
    }

    // Détruire la file d'événements
    al_destroy_event_queue(event_queue);

    // Détruire la police
    al_destroy_font(font);

    // Détruire la fenêtre
    al_destroy_display(display);

    return 0;
}