#ifndef _ENTITY_STRING_H_
#define _ENTITY_STRING_H_

/* The entities that compose a string of text displayed to the player */
/* This is mainly a wrapper to vector<Entity*> with some specializations for text strings */

#include <string>
#include <vector>
#include <stdint.h>

typedef uint8_t Uint8;

class Entity;

class EntityString {
public:
    EntityString(const int &x, const int &y, const Uint8 &assetNumber, const std::string &text);
    ~EntityString();

    /* Swaps the asset number of all the entities in this EntityString */
    void recolor(const Uint8 &assetNumber);

    /* Move all entities in the EntityString by the specified delta */
    void move(const int &x, const int &y) noexcept;

    /* Allows all EntityString entities to be shown */
    void draw() noexcept;

    /* Prevents all EntityString entities from being displayed */
    void hide() noexcept;

    /* Deletes the entities from this EntityString */
    void clear() noexcept; 

private:
    std::vector<Entity*> *_entities = nullptr;
};

#endif /* _ENTITY_STRING_H_ */