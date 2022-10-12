#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <string>
#include <vector>

/* Forward declaration of entity to store Entity* */
class Entity;
class EntityString;

/* TODO */
/* When the user decides to create a menu with a limited amount of displayed options, then  */
/*  their provided Y will be shifted down 10 to accomodate for the up arrow that may need to be drawn */
// Debating the above

// TODO - The options class has a lot of variables that describe up and down arrows
//   These are currently not implemented and need to be worked on
class Options {
public:
    Options(const int &x, const int &y, const std::initializer_list<std::string> &option_strings);
    Options(const int &x, const int &y, const unsigned int &num_options_to_display, const std::initializer_list<std::string> &option_strings);
    Options(const int &x, const int &y, const unsigned int &num_options_to_display, const std::initializer_list<std::string> &option_strings, const bool &x_centered);
    ~Options();

    /* Returns -1 if no option has been selected, or the numeric index of the selected option. */
    const int update();

    /* a newer smarter breed of du */
    const int update2() noexcept;

    /* Enable all options entities to be drawn */
    void draw() noexcept;

    /* Prevent all options entities from being drawn */
    void hide() noexcept;

    // unused
    const bool& isDrawn() const noexcept { return this->_drawn; }

    /* Delete of all options internal data */
    void clear() noexcept;

private:
    /* A flag to indicate whether the number of options should be limited when it is displayed */
    bool _limit_options_displayed = false;

    /* This number is only relevant if _limit_options_displayed is set to true. */
    /* (The number of options to display between up and down arrows) */
    unsigned int _num_options_to_display = 0u;

    /* The index of the top option displayed */
    /* This value ranges from 0 - ((options.size() - 1) - _num_options_to_display) */
    unsigned int _top_option_displayed = 0u;

    /* The current selected option (defaults to 0) */
    unsigned int _selected_option = 0u;

    std::vector<EntityString*> _options_entities;

    std::vector<std::string> _options_strings;

    /* Deep deletion for all Options data */
    void _clearOptionsEntities() noexcept;

    /* blah */
    void _redrawOptionsEntities() noexcept; /* a.k.a Text */
    void _adjustOptionsArrows() noexcept;
    
    /* Backend memory operations */
    void _shiftOptionsUp() noexcept;
    void _shiftOptionsDown() noexcept;

    Entity *_upArrow = nullptr;
    Entity *_downArrow = nullptr;

    void _createUpArrow();
    void _createDownArrow();

    inline void _drawArrow(Entity *arrow) noexcept;
    inline void _hideArrow(Entity *arrow) noexcept;
    void _deleteArrow(Entity *&arrow) noexcept;

    /* Used to store the x & y position of the options object upon its construction. */
    int _x, _y;

    /* A flag for determining if strings should be centered on X or drawn on it */
    bool _x_centered = false;

    /* Default color values */
    uint8_t _normal_option_color = 0x0f;
    uint8_t _selected_option_color = 0x0a;
    uint8_t _up_arrow_color = 0x0b;
    uint8_t _down_arrow_color = 0x0b;

    /* Keeps track of the drawn state */
    bool _drawn = true;
};

#endif /* _OPTIONS_H_ */