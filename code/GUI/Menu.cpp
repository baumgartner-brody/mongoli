#include "Menu.h"

#include "../render/TextureManager.h"
#include "../ECS/Components.h"
#include "../PHYSICS/Collision.h"

Menu::Menu(Entity *mMouse) {
    this->_mouse = mMouse;
    this->_menu_rect = new HitboxComponent(100, 500, 20, 20);
}

Menu::~Menu() {
    this->_mouse = nullptr;
    delete this->_menu_rect;
    this->_menu_rect = nullptr;
}

void Menu::update() {
    if (PHYSICS::CollisionAABB(this->_mouse->getComponent<HitboxComponent>(), *this->_menu_rect)) {
        this->rect_color = { 0, 127, 0, 255 };
    } else {
        this->rect_color = { 127, 0, 0, 255 };
    }
}

void Menu::draw() {
    TextureManager::drawRect(this->_menu_rect->_r, rect_color);
}