#ifndef _ENTITY_STRING_H_
#define _ENTITY_STRING_H_

/* The entities that compose a string of text displayed to the player */
/* This is mainly a wrapper to vector<Entity*> with some specializations for text strings */

#include <string>
#include <vector>

class Entity;

class EntityString {
public:
    EntityString(const int &x, const int &y, const std::string &assetName, const std::string &text);
    ~EntityString();

    /* Swaps the asset name of all the entities in this EntityString */
    void recolor(const std::string &assetName);

    /* Deletes the entities from this EntityString */
    void clear() noexcept; 

private:
    std::vector<Entity*> *_entities = nullptr;
};

#endif /* _ENTITY_STRING_H_ */