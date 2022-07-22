#ifndef _ANSI_H_
#define _ANSI_H_

/* ANSI.h - Defines an ANSI namespace/class that contains all ANSI colors */
/*  in the form of terminal colors, SDL_Colors and Uint8's */

#include "SDL2/SDL.h"

#include <iostream> /* std::size_t */

#include <stdint.h>
typedef uint8_t Uint8; /* ANSI Decimals range from 0x00 to 0xff (0-255) */

/* The number of ANSI Colors */
#define NUM_ANSI_COLORS 16

namespace ANSI {

    /* Defines the terminal escape code */
    class TERMINAL {
        public:
        class FOREGROUND {
            public:
                static const char * BLACK;
                static const char * DARK_RED;
                static const char * DARK_GREEN;
                static const char * DARK_YELLOW;
                static const char * DARK_BLUE;
                static const char * DARK_PURPLE;
                static const char * DARK_CYAN;
                static const char * LIGHT_GREY;
                static const char * DARK_GREY;
                static const char * BRIGHT_RED;
                static const char * BRIGHT_GREEN;
                static const char * BRIGHT_YELLOW;
                static const char * BRIGHT_BLUE;
                static const char * BRIGHT_PURPLE;
                static const char * BRIGHT_CYAN;
                static const char * WHITE;
            };
        class BACKGROUND {
            public:
                static const char * BLACK;
                static const char * DARK_RED;
                static const char * DARK_GREEN;
                static const char * DARK_YELLOW;
                static const char * DARK_BLUE;
                static const char * DARK_PURPLE;
                static const char * DARK_CYAN;
                static const char * LIGHT_GREY;
                static const char * DARK_GREY;
                static const char * BRIGHT_RED;
                static const char * BRIGHT_GREEN;
                static const char * BRIGHT_YELLOW;
                static const char * BRIGHT_BLUE;
                static const char * BRIGHT_PURPLE;
                static const char * BRIGHT_CYAN;
                static const char * WHITE;
            };

            static const char * RESET;

            /* Flags that are unique to terminal text */
            static const char * BOLD;
            static const char * UNDERLINE;
            static const char * INVERSE; 
            static const char * BOLD_OFF;
            static const char * UNDERLINE_OFF;
            static const char * INVERSE_OFF;
    };

    class SDLCOLOR_ITERATOR {
        public:
            SDLCOLOR_ITERATOR();
            SDLCOLOR_ITERATOR(const SDLCOLOR_ITERATOR &old);
            const SDLCOLOR_ITERATOR& operator=(const SDLCOLOR_ITERATOR &old);
            ~SDLCOLOR_ITERATOR();

            /* ++ Operator overloads for SDLCOLOR_ITERATOR ++ */
            const SDLCOLOR_ITERATOR operator++(int);
            const SDLCOLOR_ITERATOR& operator++();

            /* -- Operator overloads for SDLCOLOR_ITERATOR -- */
            const SDLCOLOR_ITERATOR operator--(int);
            const SDLCOLOR_ITERATOR& operator--();

            /* Get the corresponding SDL_Color */
            const SDL_Color& operator*();

            /* Get the corresponding std::string color name from SDLCOLOR_LOOKUP */
            const std::string& getColorName();

            /* + Standard arithmetic operators + */
            friend const SDLCOLOR_ITERATOR operator+(const SDLCOLOR_ITERATOR &i, const int &x);
            const SDLCOLOR_ITERATOR& operator+=(const int &x);

            /* - Standard arithmetic operators - */
            friend const SDLCOLOR_ITERATOR operator-(const SDLCOLOR_ITERATOR &i, const int &x);
            const SDLCOLOR_ITERATOR& operator-=(const int &x);


            /* Equality operators for SDLCOLOR_ITERATOR */
            friend const bool operator==(const SDLCOLOR_ITERATOR &i1, const SDLCOLOR_ITERATOR &i2) noexcept;
            friend const bool operator!=(const SDLCOLOR_ITERATOR &i1, const SDLCOLOR_ITERATOR &i2) noexcept;

            /* Give SDLCOLOR the ability to create the end() iterator */
            friend class SDLCOLOR;

        private:
            /* The current color being referenced by the iterator */
            Uint8 _current;

            /* The backend work behind all add/sub operations */
            /*  x - the amount of steps */
            /*  sub - true for subtraction */ 
            void add(const int &x, const bool &sub);

            /* A special constructor used to create the end() itr */
            SDLCOLOR_ITERATOR(const Uint8 &i);
    };

    /* Every friend function has to be defined outside of SDLCOLOR_ITERATOR but still within ANSI */
    const bool operator==(const SDLCOLOR_ITERATOR &i1, const SDLCOLOR_ITERATOR &i2) noexcept;
    const bool operator!=(const SDLCOLOR_ITERATOR &i1, const SDLCOLOR_ITERATOR &i2) noexcept;
    const SDLCOLOR_ITERATOR operator+(const SDLCOLOR_ITERATOR &i, const int &x);
    const SDLCOLOR_ITERATOR operator-(const SDLCOLOR_ITERATOR &i, const int &x);

    /* Defines the SDL_Color equivalent of all ANSI Colors */
    class SDLCOLOR {
        public:

            /* Returns an SDLCOLOR_ITERATOR pointing to ANSI::SDLCOLOR::BLACK */
            static const SDLCOLOR_ITERATOR begin();

            /* Returns an SDLCOLOR_ITERATOR that points nowhere. */
            static const SDLCOLOR_ITERATOR end();

            static const SDL_Color BLACK;
            static const SDL_Color DARK_RED;
            static const SDL_Color DARK_GREEN;
            static const SDL_Color DARK_YELLOW;
            static const SDL_Color DARK_BLUE;
            static const SDL_Color DARK_PURPLE;
            static const SDL_Color DARK_CYAN;
            static const SDL_Color LIGHT_GREY;
            static const SDL_Color DARK_GREY;
            static const SDL_Color BRIGHT_RED;
            static const SDL_Color BRIGHT_GREEN;
            static const SDL_Color BRIGHT_YELLOW;
            static const SDL_Color BRIGHT_BLUE;
            static const SDL_Color BRIGHT_PURPLE;
            static const SDL_Color BRIGHT_CYAN;
            static const SDL_Color WHITE;

            void testItrArithmetic();
    };

    /* The string names that correspond to each SDLCOLOR */
    class SDLCOLOR_LOOKUP {
        public:

            /* Attempts to lookup the name of a color by its numeric index */
            static const std::string NAME[NUM_ANSI_COLORS];
    };

    /* A function to replicate the iconic "win10colors.cmd" */
    void terminalTest(std::ostream &out);

};

/* Stream insertion overload for SDL_Color */
std::ostream& operator<<(std::ostream &s, const SDL_Color &c);

#endif /* _ANSI_H_ */