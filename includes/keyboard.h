#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "object.h"
#include <vector>
#include "vertex.h"

// Déclaration des fonctions
void processInput(bool key[], std::vector<Vertex>& vertices, Vertex& referencePoint, bool running);

#endif // KEYBOARD_H