#include "EntityString.h"

#include "../../ECS/Components.h"
#include "TextManager.h"

EntityString::EntityString(const int &x, const int &y, const Uint8 &assetNumber, const std::string &text) {
    this->_entities = new std::vector<Entity*>(TextManager::addText(x, y, assetNumber, text));
}

EntityString::~EntityString() {
    this->clear();
}

/* Move all entities in the EntityString by the specified delta */
void EntityString::move(const int &x, const int &y) noexcept {
    for (auto & e : *this->_entities) {
        e->getComponent<TransformComponent>().xpos += x;
        e->getComponent<TransformComponent>().ypos += y;
    }
}

void EntityString::recolor(const Uint8 &assetNumber) {
    TextManager::recolorText(*this->_entities, assetNumber);
}

void EntityString::draw() noexcept {
    for (auto & e : *this->_entities) e->addGroup(groupLabels::groupPlayers);
}

void EntityString::hide() noexcept {
    for (auto & e : *this->_entities) e->delGroup(groupLabels::groupPlayers);
}

void EntityString::clear() noexcept {
    for (auto & e : *this->_entities) e->destroy();
    this->_entities->clear();
    delete this->_entities;
    this->_entities = nullptr;
}