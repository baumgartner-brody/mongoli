#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <string>
#include <vector>

/* Forward declaration of entity to store Entity* */
class Entity;

class Options {
public:
    Options() = default;
    ~Options() = default;

    /* Handles any events that would change options */
    void update();

    /* Clear all displayed options entities (but retain internal options data) */
    void clear() noexcept;
private:
    /* A flag to indicate whether the number of options should be limited when it is displayed */
    bool _limit_options_displayed = false;

    /* This number is only relevant if _limit_options_displayed is set to true. */
    /* (The number of options to display between up and down arrows) */
    unsigned int _num_options_to_display = 0u;

    /* The current selected option (defaults to 0) */
    unsigned int _selected_option = 0u;

    Entity *_upArrow = nullptr;
    Entity *_downArrow = nullptr;
};

#endif /* _OPTIONS_H_ */