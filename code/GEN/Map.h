#ifndef _MAP_H_
#define _MAP_H_

namespace Map {

    /* This function runs on a separate thread, created and maintained by the game class */
    int updateMapThreadFunction(void *ptr);

    void handleEvents();

    void update();

    void render();

    void clean() noexcept;

    /* Map data */

};

#endif /* _MAP_H_ */