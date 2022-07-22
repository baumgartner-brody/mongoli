#include "ECS.h"

//#include "SDL2/SDL.h" /* SDL_Rect */ (included in ECS.h)

/* External definitions for Component, Entity and Manager */

const Entity& Entity::operator=(const Entity &old) {

    /* Guard self assignment */
    if (*this == old) return *this;

    /* Clear this */
    this->clearComponents();
    this->clearGroups();

    /* Do the copy work and then return (*this) */
    this->copy(old);
    return *this;

}

/* Deletes all components from this entity (the vector, the array and the bitset) */
void Entity::clearComponents() {
    /* Clear the component vector */
    for (auto & c : this->components) delete c; 
    this->components.clear();

    /* Clear the component array and bitset  */
    for (auto & c : this->componentArray) c = nullptr;
    this->componentBitset.reset();

}

/* Clears the groupBitset (sets everything to false) */
void Entity::clearGroups() {
    this->groupBitset.reset();
}

/* Equality operator for two Entities */
const bool operator==(const Entity &e1, const Entity &e2) noexcept {

    /* Check if all groups are equal (Cheaper than checking components first) */
    if (e1.groupBitset != e2.groupBitset) return false;

    std::cout << "made it here in entity == \n";

    /* Check if each entity entity has an equal number of components */
    if (e1.components.size() != e2.components.size()) return false;

    std::cout << "made it here in entity == \n";

    /* Check if each entity has the same type of components */
    for (unsigned int c = 0u; c < maxComponents; ++c)
        if (e1.componentBitset[c] != e2.componentBitset[c]) return false;

    std::cout << "made it here in entity == \n";
    
    /* Check if the components' themselves are equal */
    for (unsigned int c = 0u; c < maxComponents; ++c)
        /* Per the previous check, if e1 has this component, than e2 also has it */
        if (e1.componentBitset[c])
            if (!(e1.componentArray[c]->equals(e2))) return false;

    /* If you've made it here then the two entities are equal */
    return true; 
}

/* Iterate thru groupedEntities and Entities and remove any inactive entities/entities that no longer have that group */
void Manager::refresh() {

    for (unsigned int i = 0; i < maxGroups; ++i) {
        std::vector<Entity*>& v(this->groupedEntities[i]);
        v.erase(std::remove_if(v.begin(), v.end(), 
            [i](Entity *mEntity) {
                return !mEntity->active() || !mEntity->hasGroup(i);
            }),
                v.end()
        );
    }

    /* The big boy delete for actual memory */
    for (auto & e : entities) {
        if (!e->active()) {
            delete e;
            e = nullptr;
        }
    }

    /* The erase to make sure we don't use something we just deleted */
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [](Entity *mEntity) {
            return mEntity == nullptr;
        }),
            entities.end()
    );
}

/* Equality operators for SDL_Rects */
const bool operator==(const SDL_Rect &r1, const SDL_Rect &r2) noexcept {
    return (r1.x == r2.x) && (r1.y == r2.y) && (r1.w == r2.w) && (r1.h == r2.h);
}

const bool operator!=(const SDL_Rect &r1, const SDL_Rect &r2) noexcept { return !(r1 == r2); }