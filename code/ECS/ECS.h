#ifndef _ENTITY_COMPONENT_SYSTEM_H_
#define _ENTITY_COMPONENT_SYSTEM_H_

#include "SDL2/SDL.h" /* SDL_Rect */

#include <algorithm>
#include <array>
#include <bitset>
#include <memory>
#include <iostream>
#include <vector>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

constexpr std::size_t maxComponents = 32u;
constexpr std::size_t maxGroups = 32u;

using ComponentBitset = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

using ComponentArray = std::array<Component*, maxComponents>; 

/* The base class that all components are derived from. */
class Component {
public:

    /* A ptr to the entity that has this component */
    Entity *entity;

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}
    virtual void copy(Entity &dst, const Entity &old) {}

    /* Returns whether or not the calling component is equal to e->getComponent<T>() */
    virtual const bool equals(const Entity &e) { return false; }

    virtual ~Component() {}

};

class Entity {
public:
    Entity(Manager *mManager) {
        this->_manager = mManager;
    }

    /* Creates a deep copy of all old's data (components & groups) into *this. */
    /*  The manager is responsible for adding the copied entity to its internal */
    /*  groupedEntities vector. */
    Entity(const Entity &old) { this->copy(old); }

    /* Deletes all data from this Entity and turns it into a deep copy of old */
    const Entity& operator=(const Entity &old);

    /* Deletes all components and groups from this entity */
    ~Entity() {
        this->clearComponents();
        this->clearGroups();
    }

    /* Deletes all components from this entity */
    void clearComponents();

    /* Sets all groups to false for this entity */
    void clearGroups();

    /* The entity calls update() for all of its components */
    void update() { for (auto & c : this->components) c->update(); }

    /* The entity calls draw() for all of its components */
    void draw() { for (auto & c : this->components) c->draw(); }

    /* Returns whether or not the given entity is active */
    const bool& active() const { return this->_active; }

    /* Sets active to false on this entity. */
    void destroy() { this->_active = false; }

    void addGroup(const Group &group);

    void delGroup(const Group &group) {
        this->groupBitset[group] = false;
    }
    const bool hasGroup(const Group &group) const { return this->groupBitset[group]; }

    template <typename T>
    const bool hasComponent() const {
        return componentBitset[getComponentTypeID<T>()];
    }

    /* Attempts to invoke a constructor for T that takes in TArgs */
    /*  Allows you to call any valid constructor for the T you are creating */
    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) {

        /* If the user attempts to add an existing component print a warning then exit(-1); */
        if (this->hasComponent<T>()) {
            std::cerr << "\33[91mError" << "\33[0m" << ": Tried to add a component that this Entity already has\n";
            exit(-1);
        }

        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        
        components.emplace_back(c);

        componentArray[getComponentTypeID<T>()] = c;
        componentBitset[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    /* Attempts to get the "T" Component from the calling Entity. */
    /*  Prints a warning and exits if unsuccessful */
    template <typename T>
    T& getComponent() const {

        /* If the user attempts to get an invalid component print a warning then exit(-1); */
        if (!this->hasComponent<T>()) {
            std::cerr << "\33[91mError" << "\33[0m" << ": Tried to get a component that this Entity does not have\n";
            exit(-1);
        }

        return *static_cast<T*>(this->componentArray[getComponentTypeID<T>()]);
    }

    /* Delete the Component of type T from this Entity */
    /*  Safe to call even if the Entity does not have a T */
    template <typename T>
    void delComponent() noexcept {
        /* Delete iff exists */
        if (this->hasComponent<T>()) {
            
            /* Remove the pointer to the element of type T within the TemplateEntity's classes vector */
            this->components.erase(std::remove_if(components.begin(), components.end(), 
                /* A lambda used to find the Base* of type T* */
                /*  Note that since Base is a skeleton, you could always */
                /*  add methods for determining if this component should be deleted. */
                [](Component *mComponent) {
                    return typeid(*mComponent) == typeid(T);
                }),
                    components.end()
            ); 

            /* Do the actual deep delete for the Component<T> */
            delete this->componentArray[getComponentTypeID<T>()];
            
            /* Update the array and bitset so we dont accidentally try and access the deleted T* */
            this->componentArray[getComponentTypeID<T>()] = nullptr;
            this->componentBitset[getComponentTypeID<T>()] = false;
        }
    }

    friend const bool operator==(const Entity &e1, const Entity &e2) noexcept;

private:
    /* */
    std::vector<Component*> components;

    /* An array of components and a corresponding bitset for this entity */
    ComponentArray componentArray;
    ComponentBitset componentBitset;

    /* A bitset to keep track of groups this entity has */
    GroupBitset groupBitset;

    Manager *_manager;

    bool _active = true;

    /* Backend copy work behind copy ctor and assignment */
    void copy(const Entity &old) {
        /* Copy the Manager* from old */
        this->_manager = old._manager;

        /* Copies all components that old has into *this */
        /*  Components do this by overriding a function that invokes their copy constructor */
        for (auto & c : old.components) {
            c->copy(*this, old);
        }

        /* Copies all group information from the old entity. */
        for (Group i = 0u; i < maxGroups; ++i) {
            if (old.hasGroup(i)) this->addGroup(i);
        }

        this->_active = true;
    }

    /* This is the Raymond Teller function */
    /*  It creates, maintains and gives the numeric ID to the Penn function. */
    static ComponentID getComponentID() {
        static ComponentID lastID = 0u;
        return ++lastID;
    }

    /* This is the Penn Gilette function. */
    /*  It asks Teller what numeric ID the type of T should have and reports it back to the */
    /*  TemplateEntity. This is exclusively the one that will "do the talking." */
    template <typename T>
    static ComponentID getComponentTypeID() noexcept {
        static ComponentID typeID = getComponentID();
        return typeID;
    }
};

class Manager {
public:

    ~Manager() { this->clear(); }

    void update() {
        for (auto & e : entities) e->update();
    }

    void refresh();

    void addToGroup(Entity *e, const Group &group) {
        this->groupedEntities[group].emplace_back(e);
    }

    /* Returns a vector containing the entities in the requested group */
    std::vector<Entity*>& getGroup(const Group &group) { return this->groupedEntities[group]; }

    /* Deletes all entities from memory */
    void clear() {
        for (auto & e : entities) delete e;
        entities.clear();
    }

    /* Creates a new managed entity on the heap and returns a pointer to it */
    Entity& addEntity() {
        entities.emplace_back(new Entity(this));
        return *entities.back();
    }

    /* Creates a deep copy of the param entity */
    Entity& copyEntity(const Entity &old) {
        entities.emplace_back(new Entity(old));
        return *entities.back();
    }

private:
    std::vector<Entity*> entities;

    std::array<std::vector<Entity*>, maxGroups> groupedEntities;
};

inline void Entity::addGroup(const Group &group) { 
    this->groupBitset[group] = true;
    this->_manager->addToGroup(this, group);
}

/* Equality operators for SDL_Rects */
const bool operator==(const SDL_Rect &r1, const SDL_Rect &r2) noexcept;
const bool operator!=(const SDL_Rect &r1, const SDL_Rect &r2) noexcept;

#endif /* _ENTITY_COMPONENT_SYSTEM_H_ */