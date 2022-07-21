#include "ECS.h"

/* Note: Entity::addGroup() is defined at the bottom */

/* Copy constructor for Entity (creates deep copy of old) */
Entity::Entity(const Entity &old) {
    this->copy(old);
}

/* Copy assignment for Entity (creates deep copy of old) */
const Entity& Entity::operator=(const Entity &old) {

    /* Guard against self assignment */
    if (*this == old) return *this;

    /* Clear and then copy */
    this->clear();
    this->copy(old);
    return *this;
}

/* Frees all of this Entity's data */
Entity::~Entity() { this->clear(); }

/* Clears all Components from this Entity */
void Entity::clear() noexcept {

    /* Do the actual deep deletion */
    for (auto & c : this->components) delete c;
    this->components.clear();

    /* NULL out the internal array used to access Components */
    for (auto & c : this->componentArray) c = nullptr;

    /* Effectively sets every bit to false */
    this->componentBitset.reset();
    this->groupBitset.reset();
}

/* Turns *this into a deep copy of old */
void Entity::copy(const Entity &old) {
    /* Make a deep copy of every one of old's Components in *this */
    for (auto & c : old.components) c->copy(*this, old);

    /* Equalize the groups */
    for (unsigned int i = 0u; i < MAX_GROUPS; ++i)
        if (old.hasGroup(i)) this->groupBitset[i] = true;

    /* Perform the appropriate manager job */
    if (this->managed() && !old.managed()) {
        this->_manager->stopManagingEntity(this);
        this->_manager = nullptr;
    } 
    if (!this->managed() && old.managed()) {
        this->_manager = old._manager;
        this->_manager->startManagingEntity(this);
    }
}

/* Equality operators for template entity */
const bool operator==(const Entity &e1, const Entity &e2) noexcept {

    /* Check if both e1 and e2 are managed */
    if (e1._manager != e2._manager) return false;

    /* Check if the active flags match */
    if (e1._active != e2._active) return false;

    /* Check if e1 and e2 have an equal number of components */
    if (e1.components.size() != e2.components.size()) return false;

    /* Check if e1 and e2 have the same Groups */
    for (unsigned int i = 0u; i < MAX_GROUPS; ++i)
        if (e1.groupBitset[i] != e2.groupBitset[i]) return false;

    /* Check if e1 and e2 have the same Components types */
    for (unsigned int i = 0u; i < MAX_COMPONENTS; ++i) 
        if (e1.componentBitset[i] != e2.componentBitset[i]) return false;
    
    /* If the have the same types now you have to check all types */
    for (unsigned int i = 0u; i < MAX_COMPONENTS; ++i) 
        /* As per the above, if e1 has it, e2 also has it */
        if (e1.componentBitset[i])
            if (!(e1.componentArray[i]->equals(e2))) return false;
    
    /* If you've made it here, e1 == e2 */
    return true;
}

const bool operator!=(const Entity &e1, const Entity &e2) noexcept {
    return !(e1 == e2);
}

/* Prints the components of this Entity in order of the bitset */
std::ostream& operator<<(std::ostream &out, const Entity &e) {
    if (e.components.empty()) out << "is empty";
    else { 
        out << " has " << e.components.size() << " Components\n";

        /* Don't print a \n after the last derived */
        unsigned int num_printed = 0;

        /* Print each derived class in order of the bitset */
        for (unsigned int i = 0u; i < MAX_COMPONENTS; ++i)
            if (e.componentBitset[i]) {
                e.componentArray[i]->print(out);

                ++num_printed;

                if (num_printed < e.components.size())
                    out << '\n';
            }
    }
    
    return out;
}

void Manager::addToGroup(Entity *e, const Group &g) {
    this->_groupedEntities[g].emplace_back(e);
}

Entity& Manager::addEntity() {
    this->_entities.emplace_back(new Entity(this));
    return *this->_entities.back();
}

Entity& Manager::addUnManagedEntity() {
    Entity *e = new Entity;
    return *e;
}

void Manager::refresh() {

    /* Removes inactive/ entities that no longer have groups from _groupedEntities */
    for (unsigned int i = 0u; i < MAX_GROUPS; ++i) {
        _groupedEntities[i].erase(std::remove_if(std::begin(_groupedEntities[i]), std::end(_groupedEntities[i]), 
            [&](Entity *e) {
                return !e->active() || !e->hasGroup(i);
            }),
                std::end(_groupedEntities[i])
        );
    }

    _entities.erase(std::remove_if(std::begin(_entities), std::end(_entities),
        [&](Entity *&e) {
            if (!e->active()) {
                delete e;
                return true;
            }
            return false;
        }),
            std::end(_entities)
    );

}

void Manager::update() {
    for (auto & e : _entities) e->update();
}

void Manager::clear() noexcept {
    std::cout << "Sizeof entities before clear(): " << this->_entities.size() << '\n';
    /* Clear all the entities from their group vectors */
    for (auto & v : _groupedEntities) v.clear();

    /* Deep delete all managed entities */
    for (auto & e : _entities) delete e;
    _entities.clear();
}

void Manager::startManagingEntity(Entity *e) {
    
    /* Add e to _entities */
    this->_entities.emplace_back(e);

    /* Add e to its appropriate groups in _groupedEntities */
    for (unsigned int i = 0u; i < MAX_GROUPS; ++i)
        if (e->hasGroup(i)) this->_groupedEntities[i].emplace_back(e);
}

void Manager::stopManagingEntity(Entity *e) {
    
    /* Remove e from _entities */
    this->_entities.erase(std::remove_if(std::begin(_entities), std::end(_entities),
        [&](Entity *&_e) {
            return _e == e;
        }), std::end(_entities)
    );

    /* Remove e from its groups in _groupedEntities */
    for (unsigned int i = 0u; i < MAX_GROUPS; ++i)
        _groupedEntities[i].erase(std::remove_if(std::begin(_groupedEntities[i]), std::end(_groupedEntities[i]),
            [&](Entity *&_e) {
                return _e == e;
            }), std::end(_groupedEntities[i])
        );
}

void Entity::addGroup(const Group &g) {
    this->groupBitset[g] = true;

    if (this->managed())
        this->_manager->addToGroup(this, g);
}