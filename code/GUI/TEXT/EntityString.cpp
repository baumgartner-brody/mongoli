#include "EntityString.h"

#include "../../ECS/Components.h"
#include "TextManager.h"

EntityString::EntityString(const int &x, const int &y, const std::string &assetName, const std::string &text) {
    this->_entities = new std::vector<Entity*>(TextManager::addText(x, y, assetName, text));
}

EntityString::~EntityString() {
    this->clear();
}

void EntityString::recolor(const std::string &assetName) {
    for (auto & e : *this->_entities) e->getComponent<SpriteComponent>().swapAsset(assetName);
}

void EntityString::clear() noexcept {
    for (auto & e : *this->_entities) e->destroy();
    this->_entities->clear();
    delete this->_entities;
    this->_entities = nullptr;
}