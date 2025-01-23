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
#include "../includes/vertex.h"
#include "../includes/triangle.h"
#include "../includes/render.h"
#include "../includes/app.h"
#include "../includes/object.h"
#include "../includes/keyboard.h"

//Variable globale
std::vector<Vertex> vertices;
std::vector<Triangle> triangles;
Vertex referencePoint = {0.0f, 0.0f, 0.0f}; // Point de référence pour les rotations
Vertex referencelightSource = {0.0f, 0.0f, 0.0f};    // Source de lumière


// std::vector<Vertex> generateCircleVertices(const Vertex& center, float radius, int segments) {
//     std::vector<Vertex> circleVertices;
//     for (int i = 0; i < segments; ++i) {
//         float angle = 2.0f * M_PI * i / segments;
//         Vertex vertex;
//         vertex.x = center.x + radius * cos(angle);
//         vertex.y = center.y + radius * sin(angle);
//         vertex.z = center.z;
//         vertex.r = 1.0f;
//         vertex.g = 1.0f;
//         vertex.b = 0.0f;
//         circleVertices.push_back(vertex);
//     }
//     return circleVertices;
// }


// std::vector<Vertex> generateSphereVertices(float radius, int stacks, int slices) {
//     std::vector<Vertex> sphereVertices;
//     for (int i = 0; i <= stacks; ++i) {
//         float phi = M_PI * i / stacks;
//         for (int j = 0; j <= slices; ++j) {
//             float theta = 2.0f * M_PI * j / slices;
//             Vertex vertex;
//             vertex.x = radius * sin(phi) * cos(theta);
//             vertex.y = radius * sin(phi) * sin(theta);
//             vertex.z = radius * cos(phi);
//             vertex.r = 1.0f;
//             vertex.g = 1.0f;
//             vertex.b = 1.0f;
//             sphereVertices.push_back(vertex);
//         }
//     }
//     return sphereVertices;
// }

// std::vector<Triangle> generateSphereTriangles(int stacks, int slices) {
//     std::vector<Triangle> sphereTriangles;
//     for (int i = 0; i < stacks; ++i) {
//         for (int j = 0; j < slices; ++j) {
//             int first = (i * (slices + 1)) + j;
//             int second = first + slices + 1;

//             Triangle triangle1;
//             triangle1.indices[0] = first;
//             triangle1.indices[1] = second;
//             triangle1.indices[2] = first + 1;
//             sphereTriangles.push_back(triangle1);

//             Triangle triangle2;
//             triangle2.indices[0] = second;
//             triangle2.indices[1] = second + 1;
//             triangle2.indices[2] = first + 1;
//             sphereTriangles.push_back(triangle2);
//         }
//     }
//     return sphereTriangles;
// }

int main() {

    // Initialiser Application
    if (AppInit() != 0) {
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

    // Créer des points 3D carrés
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

    // // Créer des points 3D pour une sphère
    // float radius = 1.0f;
    // int stacks = 20; // Réduire le nombre de stacks
    // int slices = 20; // Réduire le nombre de slices
    // vertices = generateSphereVertices(radius, stacks, slices);
    // triangles = generateSphereTriangles(stacks, slices);

    // Distance de projection
    float distance = 5.0f;

    // Définir la source de lumière
    Vertex lightSource = { 0.0f, 0.0f, 5.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };

    // Tableau pour suivre l'état des touches
    bool key[ALLEGRO_KEY_MAX] = { false };

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    // Boucle principale
  bool running = true;
while (running) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);

    if (ev.type == ALLEGRO_EVENT_TIMER) {
        // Effacer l'écran
        al_clear_to_color(al_map_rgb(0, 0, 0));

        // faire tourner la lumière
        rotateY(lightSource, 1.0f, referencelightSource);

        // // Générer les sommets du cercle représentant la source de lumière en 3D
        // std::vector<Vertex> circleVertices = generateCircleVertices(lightSource, 0.5f, 36); // rayon = 0.5, segments = 36
        // std::vector<Triangle> circleTriangles;

        // // Vérifier que circleVertices contient au moins deux sommets
        // if (circleVertices.size() > 1) {
        //     for (size_t i = 0; i < circleVertices.size() - 1; ++i) {
        //         Triangle triangle;
        //         triangle.indices[0] = static_cast<unsigned int>(i);
        //         triangle.indices[1] = static_cast<unsigned int>((i + 1) % (circleVertices.size() - 1));
        //         triangle.indices[2] = static_cast<unsigned int>(circleVertices.size() - 1); // Centre du cercle
        //         circleTriangles.push_back(triangle);
        //     }

        //     // Ajouter les triangles du cercle à la liste des triangles
        //     triangles.insert(triangles.end(), circleTriangles.begin(), circleTriangles.end());
        // }

        // Calculer la distance moyenne de chaque triangle par rapport à la caméra
        for (auto& triangle : triangles) {
            triangle.distance = calculateTriangleDistance(triangle, vertices);
        }

        // Trier les triangles par distance décroissante
        std::sort(triangles.begin(), triangles.end(), [](const Triangle& a, const Triangle& b) {
            return a.distance > b.distance;
        });
        
        Render(vertices, triangles, lightSource, 2.5f, distance);

        // Afficher le rendu
        al_flip_display();
    } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        running = false;
    } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        key[ev.keyboard.keycode] = true;
    } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        key[ev.keyboard.keycode] = false;
    }

    // Gérer les entrées
    processInput(key, vertices, referencePoint, running);

    

}

// Détruire la file d'événements
al_destroy_event_queue(event_queue);

// Détruire la police
al_destroy_font(font);

// Détruire le timer
al_destroy_timer(timer);

return 0;
}