#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <string>
#include <vector>

/* Forward declaration of entity to store Entity* */
class Entity;
class EntityString;

class Options {
public:
    Options(const int &x, const int &y, const std::initializer_list<std::string> &option_strings);
    Options(const int &x, const int &y, const unsigned int &num_options_to_display, const std::initializer_list<std::string> &option_strings);
    ~Options();

    /* Returns -1 if no option has been selected, or the numeric index of the selected option. */
    const int update();

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

    std::vector<EntityString*> _options_entities;

    Entity *_upArrow = nullptr;
    Entity *_downArrow = nullptr;

    /* Used to store the x & y position of the options object upon its construction. */
    int _x, _y; 
};

#endif /* _OPTIONS_H_ */