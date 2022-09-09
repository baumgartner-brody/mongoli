#include "EntityString.h"

#include "../../ECS/Components.h"
#include "TextManager.h"

EntityString::EntityString(const int &x, const int &y, const Uint8 &assetNumber, const std::string &text) {
    this->_entities = new std::vector<Entity*>(TextManager::addText(x, y, assetNumber, text));
}

EntityString::~EntityString() {
    this->clear();
}

void EntityString::recolor(const Uint8 &assetNumber) {
    for (auto & e : *this->_entities) e->getComponent<SpriteComponent>().swapAsset(assetNumber);
}

void EntityString::clear() noexcept {
    for (auto & e : *this->_entities) e->destroy();
    this->_entities->clear();
    delete this->_entities;
    this->_entities = nullptr;
}