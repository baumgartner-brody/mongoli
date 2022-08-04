#ifndef _MAP_H_
#define _MAP_H_

class Entity;
class Manager;

namespace Map {

    /* This function runs on a separate thread, created and maintained by the game class */
    int updateMapThreadFunction(void *ptr);

    /* Initializes all map resources */
    int init();

    void handleEvents();

    void update();

    void render();

    void clean() noexcept;

    /* Map data & resources */
    extern Manager *_manager; 


};

#endif /* _MAP_H_ */