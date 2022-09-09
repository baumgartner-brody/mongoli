#include "Announcements.h"

#include "../ECS/Components.h"
#include "TEXT/TextManager.h"
#include "TEXT/EntityString.h"

Announcements::~Announcements() {
    this->clear();
}

void Announcements::init() {
    this->_announcement_border_entities = new std::vector<Entity*>;

    int _x = 0, _y = 600;

    this->_announcement_border_entities->emplace_back(TextManager::addText(_x, _y, 0x0f, ANSI::BOX_DRAWING_CORNER_TOP_LEFT_DOUBLE_VERTICAL_SINGLE_HORIZONTAL));

    for (int i = 0; i < 79; ++i) {
        _x += 10;
        this->_announcement_border_entities->emplace_back(TextManager::addText(_x, _y, 0x0f, ANSI::BOX_DRAWING_HORIZONTAL_SINGLE));
    }

    this->_announcement_border_entities->emplace_back(TextManager::addText(_x, _y, 0x0f, ANSI::BOX_DRAWING_CORNER_TOP_RIGHT_DOUBLE_VERTICAL_SINGLE_HORIZONTAL));

    for (int i = 0; i < 2; ++i) {
        _x = 0;
        _y += 10;
        this->_announcement_border_entities->emplace_back(TextManager::addText(_x, _y, 0x0f, ANSI::BOX_DRAWING_VERTICAL_DOUBLE));    

        _x = 790;
        this->_announcement_border_entities->emplace_back(TextManager::addText(_x, _y, 0x0f, ANSI::BOX_DRAWING_VERTICAL_DOUBLE));  
    }

    _x = 0;
    _y += 10;

    this->_announcement_border_entities->emplace_back(TextManager::addText(_x, _y, 0x0f, ANSI::BOX_DRAWING_CORNER_BOTTOM_LEFT_DOUBLE_VERTICAL_SINGLE_HORIZONTAL));

    for (int i = 0; i < 79; ++i) {
        _x += 10;
        this->_announcement_border_entities->emplace_back(TextManager::addText(_x, _y, 0x0f, ANSI::BOX_DRAWING_HORIZONTAL_SINGLE));
    }

    this->_announcement_border_entities->emplace_back(TextManager::addText(_x, _y, 0x0f, ANSI::BOX_DRAWING_CORNER_BOTTOM_RIGHT_DOUBLE_VERTICAL_SINGLE_HORIZONTAL));

    this->_current_announcement = new EntityString(20, 610, 0x0a, "Sample Announcement");

}

void Announcements::draw() {
    if (!this->_shown) return;

    for (auto & e : *this->_announcement_border_entities) e->draw(); 
}

void Announcements::clear() noexcept {
    if (this->_announcement_border_entities != nullptr) {
        for (auto & e : *this->_announcement_border_entities) e->destroy();
        this->_announcement_border_entities->clear();
        delete this->_announcement_border_entities;
        this->_announcement_border_entities = nullptr;
    }

    if (this->_current_announcement != nullptr) {
        delete this->_current_announcement;
        this->_current_announcement = nullptr;
    }
}