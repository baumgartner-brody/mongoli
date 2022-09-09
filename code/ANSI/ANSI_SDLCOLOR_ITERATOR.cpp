#include "ANSI.h"

/* Defines ANSI::SDLCOLOR_ITERATOR and its related functions */

/* Default constructor for SDLCOLOR_ITERATOR type, returns begin() by default */
ANSI::SDLCOLOR_ITERATOR::SDLCOLOR_ITERATOR() {
    this->_current = 0u;
}

/* Copy constructor for SDLCOLOR_ITERATOR */
ANSI::SDLCOLOR_ITERATOR::SDLCOLOR_ITERATOR(const ANSI::SDLCOLOR_ITERATOR &old) {
    this->_current = old._current;
}

/* Assignment operator for SDLCOLOR_ITERATOR */
const ANSI::SDLCOLOR_ITERATOR& ANSI::SDLCOLOR_ITERATOR::operator=(const ANSI::SDLCOLOR_ITERATOR &old) {
    /* Guard against self assignment */
    if (*this == old) return *this;

    this->_current = old._current;
    return *this;
}

/* Private value constructor for SDLCOLOR_ITERATOR */
ANSI::SDLCOLOR_ITERATOR::SDLCOLOR_ITERATOR(const Uint8 &i) {
    this->_current = i;
}

/* Deconstructor for SDLCOLOR_ITERATOR */
ANSI::SDLCOLOR_ITERATOR::~SDLCOLOR_ITERATOR() {}

/* ++ Operator overloads for SDLCOLOR_ITERATOR ++ */
const ANSI::SDLCOLOR_ITERATOR ANSI::SDLCOLOR_ITERATOR::operator++(int) {
    ANSI::SDLCOLOR_ITERATOR tmp(*this);
    ++(*this);
    return tmp;
} 

const ANSI::SDLCOLOR_ITERATOR& ANSI::SDLCOLOR_ITERATOR::operator++() {
    /* If you do ++ on end() then loop back around to begin() */
    if (this->_current == NUM_ANSI_COLORS)
        this->_current = 0;
    else 
        ++this->_current;
    return *this;
}

/* -- Operator overloads for SDLCOLOR_ITERATOR -- */
const ANSI::SDLCOLOR_ITERATOR ANSI::SDLCOLOR_ITERATOR::operator--(int) {
    ANSI::SDLCOLOR_ITERATOR tmp(*this);
    --(*this);
    return tmp;
}

const ANSI::SDLCOLOR_ITERATOR& ANSI::SDLCOLOR_ITERATOR::operator--() {
    /* If you do -- on the begin() loop back around to the last valid (not end) */
    if (this->_current == 0)
        this->_current = NUM_ANSI_COLORS - 1;
    else 
        --this->_current;
    return *this;
}

/* Attempt to get the SDL_Color that corresponds to "where" this iterator is. */
/*  Exits program if the iterator is out of bounds */
const SDL_Color& ANSI::SDLCOLOR_ITERATOR::operator*() {
    try {

        if (this->_current == 0) return ANSI::SDLCOLOR::BLACK;
        else if (this->_current == 1) return ANSI::SDLCOLOR::DARK_BLUE;
        else if (this->_current == 2) return ANSI::SDLCOLOR::DARK_GREEN;
        else if (this->_current == 3) return ANSI::SDLCOLOR::DARK_CYAN;
        else if (this->_current == 4) return ANSI::SDLCOLOR::DARK_RED;
        else if (this->_current == 5) return ANSI::SDLCOLOR::DARK_PURPLE;
        else if (this->_current == 6) return ANSI::SDLCOLOR::DARK_YELLOW;
        else if (this->_current == 7) return ANSI::SDLCOLOR::LIGHT_GREY;
        else if (this->_current == 8) return ANSI::SDLCOLOR::DARK_GREY;
        else if (this->_current == 9) return ANSI::SDLCOLOR::BRIGHT_BLUE;
        else if (this->_current == 10) return ANSI::SDLCOLOR::BRIGHT_GREEN;
        else if (this->_current == 11) return ANSI::SDLCOLOR::BRIGHT_CYAN;
        else if (this->_current == 12) return ANSI::SDLCOLOR::BRIGHT_BLUE;
        else if (this->_current == 13) return ANSI::SDLCOLOR::BRIGHT_PURPLE;
        else if (this->_current == 14) return ANSI::SDLCOLOR::BRIGHT_YELLOW;
        else if (this->_current == 15) return ANSI::SDLCOLOR::WHITE;

        /* The iterator is out of range */
        else 
            throw "bad ANSI::SDLCOLOR_ITERATOR";
    } catch (const char *e) {
        std::cerr << ANSI::TERMINAL::BOLD << ANSI::TERMINAL::FOREGROUND::BRIGHT_YELLOW 
            << "[WARNING]: " << ANSI::TERMINAL::RESET << e << '\n';
        exit(-1);
    }
}

/* Attempt to get the string that corresponds to the color that this iterator is "on". */
/*  Exits program if the iterator is out of bounds */
const std::string& ANSI::SDLCOLOR_ITERATOR::getColorName() const { 
    try {

        /* Return the string name of this color (if possible). */
        if (this->_current < NUM_ANSI_COLORS)
            return ANSI::SDLCOLOR_LOOKUP::NAME[this->_current];

        /* The iterator is out of range */
        else 
            throw "bad ANSI::SDLCOLOR_ITERATOR";
    } catch (const char *e) {
        std::cerr << ANSI::TERMINAL::BOLD << ANSI::TERMINAL::FOREGROUND::BRIGHT_YELLOW 
            << "[WARNING]: " << ANSI::TERMINAL::RESET << e << '\n';
        exit(-1);
    }
}

/* + Standard arithmetic operators + */
const ANSI::SDLCOLOR_ITERATOR ANSI::operator+(const ANSI::SDLCOLOR_ITERATOR &i, const int &x) {
    SDLCOLOR_ITERATOR tmp(i);
    tmp.add(x, false);
    return tmp;
}

const ANSI::SDLCOLOR_ITERATOR& ANSI::SDLCOLOR_ITERATOR::operator+=(const int &x) {
    this->add(x, false);
    return *this;
}

/* - Standard arithmetic operators - */
const ANSI::SDLCOLOR_ITERATOR ANSI::operator-(const ANSI::SDLCOLOR_ITERATOR &i, const int &x) {
    SDLCOLOR_ITERATOR tmp(i);
    tmp.add(x, true);
    return tmp;
}

const ANSI::SDLCOLOR_ITERATOR& ANSI::SDLCOLOR_ITERATOR::operator-=(const int &x) {
    this->add(x, true);
    return *this;
}

/* Stream insertion overload for SDL_Color */
std::ostream& operator<<(std::ostream &s, const SDL_Color &c) {
    s << "r: " << (int)c.r << " g: " << (int)c.g << " b: " << (int)c.b << " a: " << (int)c.a;
    return s;
}

/* Return a default SDLCOLOR_ITERATOR object (_current = 0 / SDLCOLOR::BLACK) */
const ANSI::SDLCOLOR_ITERATOR ANSI::SDLCOLOR::begin() {
    return ANSI::SDLCOLOR_ITERATOR();
}

/* Return a SDLCOLOR_ITERATOR with an internal value of (NUM_ANSI_COLORS + 1) */
const ANSI::SDLCOLOR_ITERATOR ANSI::SDLCOLOR::end() {
    return ANSI::SDLCOLOR_ITERATOR(NUM_ANSI_COLORS);
}

const bool ANSI::operator==(const ANSI::SDLCOLOR_ITERATOR &i1, const ANSI::SDLCOLOR_ITERATOR &i2) noexcept {
    return i1._current == i2._current;
}

const bool ANSI::operator!=(const ANSI::SDLCOLOR_ITERATOR &i1, const ANSI::SDLCOLOR_ITERATOR &i2) noexcept {
    return !(i1 == i2);
}

void ANSI::SDLCOLOR_ITERATOR::add(const int &x, const bool &sub) {

    /* Epic truth table moment */
    /* x < 0 && sub  | add */
    /* x > 0 && sub  | sub */
    /* x < 0 && !sub | sub */
    /* x > 0 && !sub | add */
    for (unsigned int i = 0; i < std::abs(x); ++i)
        (x > 0 != sub) ? ++(*this) : --(*this);
    
    /* Un-note this is problems arrise with iterator arithmetic */
    /* std::cout << "this->_current " << (short int)this->_current << '\n'; */
}