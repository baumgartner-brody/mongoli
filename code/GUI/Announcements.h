#ifndef _ANNOUNCEMENTS_H_
#define _ANNOUNCEMENTS_H_

#include <vector>

class Entity;
class EntityString;

class Announcements {
public:
    Announcements() = default;
    ~Announcements();

    void init();

    /* Show the announcements bar */
    void show() { this->_shown = true; }

    /* Hide the announcements bar */
    void hide() { this->_shown = false; }

    /* Displays the announcement bar in its current state */
    void draw();

    void clear() noexcept;

private:

    std::vector<Entity*> *_announcement_border_entities = nullptr;

    /* TODO - This is going to be some sort of queue */
    EntityString *_current_announcement = nullptr;

    bool _shown = true; /* TODO - default this to false */

};

#endif /* _ANNOUNCEMENTS_H_ */