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
#include <thread>
#include <mutex>
#include "../includes/vertex.h"
#include "../includes/triangle.h"
#include "../includes/render.h"
#include "../includes/app.h"
#include "../includes/object.h"
#include "../includes/keyboard.h"

// Variables globales
std::vector<Vertex> vertices;
std::vector<Triangle> triangles;
Vertex referencePoint = {0.0f, 0.0f, 0.0f}; // Point de référence pour les rotations
Vertex referencelightSource = {0.0f, 0.0f, 0.0f}; // Source de lumière
std::mutex mtx;

void renderLoop(std::vector<Triangle>& triangles, const std::vector<Vertex>& vertices, const Vertex& lightSource, float& distance, ALLEGRO_DISPLAY* display, bool& running) {
    while (running) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Limiter à environ 60 FPS
        // Calculer la distance moyenne de chaque triangle par rapport à la caméra
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& triangle : triangles) {
            triangle.distance = calculateTriangleDistance(triangle, vertices);
        }

        // Trier les triangles par distance décroissante
        std::sort(triangles.begin(), triangles.end(), [](const Triangle& a, const Triangle& b) {
            return a.distance > b.distance;
        });
    }
}

void generateGeodesicSphere(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles, int subdivisions) {
    // Vertices of an icosahedron
    const float X = 0.525731112119133606f;
    const float Z = 0.850650808352039932f;
    const float N = 0.0f;

    std::vector<Vertex> baseVertices = {
        {-X, N, Z}, {X, N, Z}, {-X, N, -Z}, {X, N, -Z},
        {N, Z, X}, {N, Z, -X}, {N, -Z, X}, {N, -Z, -X},
        {Z, X, N}, {-Z, X, N}, {Z, -X, N}, {-Z, -X, N}
    };

    std::vector<Triangle> baseTriangles = {
        {{0, 4, 1}}, {{0, 9, 4}}, {{9, 5, 4}}, {{4, 5, 8}}, {{4, 8, 1}},
        {{8, 10, 1}}, {{8, 3, 10}}, {{5, 3, 8}}, {{5, 2, 3}}, {{2, 7, 3}},
        {{7, 10, 3}}, {{7, 6, 10}}, {{7, 11, 6}}, {{11, 0, 6}}, {{0, 1, 6}},
        {{6, 1, 10}}, {{9, 0, 11}}, {{9, 11, 2}}, {{9, 2, 5}}, {{7, 2, 11}}
    };

    vertices = baseVertices;
    triangles = baseTriangles;

    // Subdivide each triangle
    for (int i = 0; i < subdivisions; ++i) {
        std::vector<Triangle> newTriangles;
        std::unordered_map<uint64_t, int> midPointCache;

        auto getMidPoint = [&](int v1, int v2) -> int {
            uint64_t key = (static_cast<uint64_t>(std::min(v1, v2)) << 32) | std::max(v1, v2);
            auto it = midPointCache.find(key);
            if (it != midPointCache.end()) {
                return it->second;
            }

            Vertex mid = {
                (vertices[v1].x + vertices[v2].x) / 2.0f,
                (vertices[v1].y + vertices[v2].y) / 2.0f,
                (vertices[v1].z + vertices[v2].z) / 2.0f,
                1.0f, 1.0f, 1.0f, 0.0f, 0.0f
            };

            float length = std::sqrt(mid.x * mid.x + mid.y * mid.y + mid.z * mid.z);
            mid.x /= length;
            mid.y /= length;
            mid.z /= length;

            vertices.push_back(mid);
            int index = vertices.size() - 1;
            midPointCache[key] = index;
            return index;
        };
        for (const auto& tri : triangles) {
            int a = getMidPoint(tri.indices[0], tri.indices[1]);
            int b = getMidPoint(tri.indices[1], tri.indices[2]);
            int c = getMidPoint(tri.indices[2], tri.indices[0]);
            
            Triangle t1 = {{tri.indices[0], a, c}};
            Triangle t2 = {{tri.indices[1], b, a}};
            Triangle t3 = {{tri.indices[2], c, b}};
            Triangle t4 = {{a, b, c}};
            

            newTriangles.push_back(t1);
            newTriangles.push_back(t2);
            newTriangles.push_back(t3);
            newTriangles.push_back(t4);
        }

        triangles = newTriangles;
    }
}

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

    // // Créer des points 3D carrés
    // vertices = {
    //     { -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    //     {  1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
    //     {  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
    //     { -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
    //     { -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
    //     {  1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    //     {  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
    //     { -1.0f,  1.0f, -1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f }
    // };

    // // Définir les triangles
    // triangles = {
    //     {{0, 1, 2}, 0.0f}, {{2, 3, 0}, 0.0f},  // Face avant
    //     {{4, 5, 6}, 0.0f}, {{6, 7, 4}, 0.0f},  // Face arrière
    //     {{0, 1, 5}, 0.0f}, {{5, 4, 0}, 0.0f},  // Face inférieure
    //     {{2, 3, 7}, 0.0f}, {{7, 6, 2}, 0.0f},  // Face supérieure
    //     {{0, 3, 7}, 0.0f}, {{7, 4, 0}, 0.0f},  // Face gauche
    //     {{1, 2, 6}, 0.0f}, {{6, 5, 1}, 0.0f}   // Face droite
    // };

    generateGeodesicSphere(vertices, triangles, 5);

    // Distance de projection
    float distance = 5.0f;

    // Définir la source de lumière
    Vertex lightSource = { 0.0f, 0.0f, 5.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };

    // variables pour les fps
    int frames = 0;
    double fps = 0.0;
    double lastTime = al_get_time();

    // Tableau pour suivre l'état des touches
    bool key[ALLEGRO_KEY_MAX] = { false };
    // Boucle principale
    bool running = true;
    // Créer un thread pour le rendu
    std::thread renderThread(renderLoop, std::ref(triangles), std::ref(vertices), std::ref(lightSource), std::ref(distance), display, std::ref(running));

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    
    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            // Faire tourner la lumière
            rotateY(lightSource, 1.0f, referencelightSource);


            // Rendu des triangles
            al_clear_to_color(al_map_rgb(0, 0, 0));
            Render(vertices, triangles, lightSource, 2.5f, distance);
            // Calculer les FPS
            frames++;
            double currentTime = al_get_time();
            if (currentTime - lastTime >= 1.0) {
                fps = frames / (currentTime - lastTime);
                frames = 0;
                lastTime = currentTime;
            }

            // Afficher les FPS
            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "FPS: %.1f", fps);
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

    renderThread.join();
    // Détruire la file d'événements
    al_destroy_event_queue(event_queue);

    // Détruire la police
    al_destroy_font(font);

    // Détruire le timer
    al_destroy_timer(timer);

    // Détruire l'affichage
    al_destroy_display(display);

    return 0;
}