#ifndef _ENTITY_COMPONENT_SYSTEM_H_
#define _ENTITY_COMPONENT_SYSTEM_H_

#include <algorithm> /* remove_if */
#include <array>
#include <bitset>
#include <iostream> /* size_t */
#include <vector>

#define MAX_COMPONENTS 32u
#define MAX_GROUPS 32u

typedef uint8_t Group;

/* Forward declaration of Entity so Base can hold a pointer to it */
class Entity;

/* Forward declaration of Manager so Entity can hold a pointer to it */
class Manager;

/* We use a base class as a skeleton for our user defined classes */
class Component {
public:
    Entity *entity;

    virtual void init() {}

    virtual void update() {}

    virtual void draw() {}

    /* Calls (*this == e.getComponent<Derived>()) */
    virtual const bool equals(const Entity &e) { return false; }

    /* Calls dst.addComponent<Derived>(src.getComponent<Derived>()) */
    virtual void copy(Entity &dst, const Entity &src) {}

    /* Prints information unique to each derived class */
    virtual void print(std::ostream &out) {}
    
    virtual ~Component() {}
};

/* An Entity capable of holding a mixed amount of T's */
class Entity {
public:
    /* Default constructor for Entity - */
    Entity() = default;

    Entity(Manager *m) : _manager(m) {}

    /* Copy constructor for Entity (creates deep copy of old) */
    Entity(const Entity &old);

    /* Copy assignment for Entity (creates deep copy of old) */
    const Entity& operator=(const Entity &old);

    /* Frees all of this Entity's data */
    ~Entity();

    /* Clears all Components from this Entity */
    void clear() noexcept;

    /* Sets this entity's active flag to false */
    /* This is the proper way to delete a managed entity */
    void destroy() { this->_active = false; }

    void update() { for (auto & c : components) c->update(); }

    void draw() { for (auto & c : components) c->draw(); }

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
        c->init();

        this->componentArray[getClassTypeID<T>()] = c;
        this->componentBitset[getClassTypeID<T>()] = true;

        this->components.emplace_back(c);
        return *c;
    }

    /* Adds the entity to the specified group */
    void addGroup(const Group &g);

    /* Removes the entity from the specified group */
    void delGroup(const Group &g) {
        this->groupBitset[g] = false;
    }

    /* Determines whether or not this Entity contains an instance of T */
    template <typename T>
    const bool hasComponent() const {
        return this->componentBitset[getClassTypeID<T>()];
    }

    /* Determines whether or not this entity is in the group in question */
    const bool hasGroup(const Group &g) const {
        return this->groupBitset[g];
    }

    /* Determines whether or not this entity is managed */
    const bool managed() const { return this->_manager != nullptr; }

    /* Determines whether or not this entity is active */
    const bool& active() const { return this->_active; }

    /* Attempt to get this Entity's T UDC. */
    /*  Return a reference if valid or exit(-1) if invalid */
    template <typename T>
    T& getComponent() const {
        /* If the user attempts to get an invalid component print a warning then exit(-1); */
        if (!this->hasComponent<T>()) {
            std::cerr << "\33[91mError" << "\33[0m" << ": Tried to get a component that this Entity does not have\n";
            exit(-1);
        }

        return *static_cast<T*>(this->componentArray[getClassTypeID<T>()]);
    }

    /* Delete the Component of type T from this Entity */
    /*  Safe to call even if the Entity does not have a T */
    template <typename T>
    void delComponent() noexcept {
        /* Delete iff exists */
        if (this->hasComponent<T>()) {
            
            /* Remove the pointer to the element of type T within the Entity's classes vector */
            this->components.erase(std::remove_if(std::begin(components), std::end(components), 
                /* A lambda used to find the Component* of type T* */
                /*  Note that since Component is a skeleton, you could always */
                /*  add methods for determining if this component should be deleted. */
                [&](Component *mComponent) {
                    return typeid(*mComponent) == typeid(T);
                }),
                    std::end(components)
            ); 

            /* Do the actual deep delete for the Component */
            delete this->componentArray[getClassTypeID<T>()];
            
            /* Update the array and bitset so we dont accidentally try and access the deleted T* */
            this->componentArray[getClassTypeID<T>()] = nullptr;
            this->componentBitset[getClassTypeID<T>()] = false;
        }
    }

    /* Equality operators for template entity */
    friend const bool operator==(const Entity &e1, const Entity &e2) noexcept;
    friend const bool operator!=(const Entity &e1, const Entity &e2) noexcept;

    /* Ability to print a Entity for easy visualization */
    friend std::ostream& operator<<(std::ostream &out, const Entity &e);
private:

    /* A way of holding our classes in fixed positions */
    std::array<Component*, MAX_COMPONENTS> componentArray;

    /* A quick way of saying true/false we have this type of class */
    std::bitset<MAX_COMPONENTS> componentBitset;

    /* A bitset to keep track of groups this entity has */
    std::bitset<MAX_GROUPS> groupBitset;

    /* The raw Base* for when we need to iterate */
    std::vector<Component*> components;

    /* A ptr to the manager */
    Manager *_manager = nullptr;

    /* A bool indicating whether or not this entity is active */
    bool _active = true;

    /* We need a way of getting a numeric index for each (Derived*) */
    /*  This method is only accessible from inside a Entity,  */
    /*  but not unique to any one Entity object. */

    /*  It's easy to confuse these two functions, so I'm going to put a Penn & Teller */
    /*  label on each to explain what its role is. */

    /* This is the Raymond Teller function */
    /*  It creates, maintains and gives the numeric ID to the Penn function. */
    static unsigned int getClassID() {
        static unsigned int ID = 0u;
        return ++ID;
    }
    
    /* This is the Penn Gilette function. */
    /*  It asks Teller what numeric ID the type of T should have and reports it back to the */
    /*  Entity. This is exclusively the one that will "do the talking." */
    template <typename T> 
    static unsigned int getClassTypeID() noexcept {
        static unsigned int typeID = getClassID();
        return typeID;
    }

    /* Does the actual deep copy backend work */
    void copy(const Entity &old);    
};

class Manager {
public:
    Manager() = default;
    ~Manager() { this->clear(); }

    /* Adds a managed entity  */
    Entity& addEntity();

    /* This function is really not necessary as a static member of Manager */
    static Entity& addUnManagedEntity();

    /* Iterates thru managed entities and removes ones that are no longer active */
    void refresh();

    /* Iterates thru and updates all entities */
    void update();

    void addToGroup(Entity *e, const Group &g);

    void clear() noexcept;

    friend class Entity;

private:

    /* A way of organizing entities into their groups */
    std::array<std::vector<Entity*>, MAX_GROUPS> _groupedEntities;

    /* A vector to hold all managed entities */
    std::vector<Entity*> _entities;

    /* Starts to manage e */
    void startManagingEntity(Entity *e);

    /* Stops managing e */
    void stopManagingEntity(Entity *e);
};

#endif /* _TEMPLATE_ENTITY_H_ */