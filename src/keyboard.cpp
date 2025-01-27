#include "../includes/keyboard.h"

void processInput(bool key[], std::vector<Vertex>& vertices, Vertex& referencePoint, bool& running) {
    if (key[ALLEGRO_KEY_UP]) {
        for (auto& vertex : vertices) {
            translate(vertex, 0.0f, 0.0f, 0.1f);
        }
        translate(referencePoint, 0.0f, 0.0f, 0.1f);
    }
    if (key[ALLEGRO_KEY_DOWN]) {
        for (auto& vertex : vertices) {
            translate(vertex, 0.0f, 0.0f, -0.1f);
        }
        translate(referencePoint, 0.0f, 0.0f, -0.1f);
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
            translate(vertex, 0.0f, 0.1f, 0.0f);
        }
        translate(referencePoint, 0.0f, 0.1f, 0.0f);
    }
    if (key[ALLEGRO_KEY_S]) {
        for (auto& vertex : vertices) {
            translate(vertex, 0.0f, -0.1f, 0.0f);
        }
        translate(referencePoint, 0.0f, -0.1f, 0.0f);
    }

    // Gérer les touches
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
    if (key[ALLEGRO_KEY_I]) {
        for (auto& vertex : vertices) {
            rotateZ(vertex, 5.0f, referencePoint);
        }
    }
    if (key[ALLEGRO_KEY_O]) {
        for (auto& vertex : vertices) {
            rotateZ(vertex, -5.0f, referencePoint);
        }
    }
    if (key[ALLEGRO_KEY_K]) {
        for (auto& vertex : vertices) {
            rotateX(vertex, -5.0f, referencePoint);
        }
    }
    if (key[ALLEGRO_KEY_J]) {
        for (auto& vertex : vertices) {
            rotateX(vertex, 5.0f, referencePoint);
        }
    }
    if (key[ALLEGRO_KEY_ESCAPE]) {
        running = false;
    }
}