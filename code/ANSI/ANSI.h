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
            const std::string& getColorName() const;

            /* Get the corresponding hex value for the current color */
            const Uint8& numeric() const { return this->_current; }

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

    /* Maps each color to its appropriate hex nibble */
    namespace NUMERIC {
        const Uint8 BLACK = Uint8(0);
        const Uint8 DARK_BLUE = Uint8(1);
        const Uint8 DARK_GREEN = Uint8(2);
        const Uint8 DARK_CYAN = Uint8(3);
        const Uint8 DARK_RED = Uint8(4);
        const Uint8 DARK_PURPLE = Uint8(5);
        const Uint8 DARK_YELLOW = Uint8(6);
        const Uint8 LIGHT_GREY = Uint8(7);
        const Uint8 DARK_GREY = Uint8(8);
        const Uint8 BRIGHT_BLUE = Uint8(9);
        const Uint8 BRIGHT_GREEN = Uint8(10);
        const Uint8 BRIGHT_CYAN = Uint8(11);
        const Uint8 BRIGHT_RED = Uint8(12);
        const Uint8 BRIGHT_PURPLE = Uint8(13);
        const Uint8 BRIGHT_YELLOW = Uint8(14);
        const Uint8 WHITE = Uint8(15);

        /* Returns the appropriate 8-bit color code for a foreground and background color of choice */
        const Uint8 createColorCode(const Uint8 &FG, const Uint8 &BG);
    };

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

    /* The names of all extended ascii characters */
    enum CHARACTERS : Uint8 {
        BLANK_SPACE = 0u, // ' ' (0) (NUL)
        SMILEY, // ☺
        SMILEY_FILLED_IN, // ☻
        HEART, // ♥
        DIAMOND, // ♦
        CLUB, // ♣
        SPADE, // ♠
        BULLET_POINT, // •
        BULLET_POINT_INVERSE, // ◘
        CIRCLE, // ○
        CIRCLE_INVERSE, // ◙
        MALE_SIGN, // ♂
        FEMALE_SIGN, // ♀
        MUSIC_NOTE_SINGLE, // ♪
        MUSIC_NOTE_DOUBLE, // ♫
        SUN, // ☼
        RIGHT_POINTER, // ►
        LEFT_POINTER, // ◄
        EXCLAMATION_POINT_DOUBLE, // ‼
        ARROW_UPDOWN, // ↕
        PARAGRAPH, // ¶
        SECTION_SIGN, // §
        RECTANGLE, // ▬
        ARROW_UPDOWN_UNDERLINED, // ↨
        ARROW_UP, // ↑
        ARROW_DOWN, // ↓
        ARROW_RIGHT, // →
        ARROW_LEFT, // ←
        RIGHT_ANGLE, // ∟ (WILTED PLANT)
        ARROW_LEFTRIGHT, // ↔
        SLOPE_UPWARD, // ▲
        SLOPE_DOWNWARD, // ▼
        BLANK_SPACE2, // ' ' (32)
        EXCLAMATION_POINT_SINGLE, // !
        QUOTE_DOUBLE, // "
        HASHTAG, // #
        DOLLAR_SIGN, // $
        PERCENT_SIGN, // %
        AMPERSAND, // &
        QUOTE_SINGLE, // ' (APOSTROPHE)
        OPEN_PARENTHESIS, // (
        CLOSE_PARENTHESIS, // )
        ASTERISK, // *
        PLUS_SIGN, // +
        COMMA, // ,
        MINUS_SIGN, // -
        PERIOD, // .
        FOREWARD_SLASH, // /
        ZERO, // 0
        ONE, // 1
        TWO, // 2
        THREE, // 3
        FOUR, // 4
        FIVE, // 5
        SIX, // 6
        SEVEN, // 7
        EIGHT, // 8
        NINE, // 9
        COLON, // :
        SEMICOLON, // ;
        LESS_THAN, // <
        EQUAL, // =
        MORE_THAN, // >
        QUESTION_MARK, // ?
        AT_SIGN, // @
        CAPITAL_A, // A
        CAPITAL_B, // B
        CAPITAL_C, // C
        CAPITAL_D, // D
        CAPITAL_E, // E
        CAPITAL_F, // F
        CAPITAL_G, // G
        CAPITAL_H, // H
        CAPITAL_I, // I
        CAPITAL_J, // J
        CAPITAL_K, // K
        CAPITAL_L, // L
        CAPITAL_M, // M
        CAPITAL_N, // N
        CAPITAL_O, // O
        CAPITAL_P, // P
        CAPITAL_Q, // Q
        CAPITAL_R, // R
        CAPITAL_S, // S
        CAPITAL_T, // T
        CAPITAL_U, // U
        CAPITAL_V, // V
        CAPITAL_W, // W
        CAPITAL_X, // X
        CAPITAL_Y, // Y
        CAPITAL_Z, // Z
        OPEN_SQUARE_BRACKET, // [
        BACKSLASH, /* \ */
        CLOSE_SQUARE_BRACKET, // ]
        CARET, // ^
        UNDERLINE, // _
        BACKTICK, // `
        LOWERCASE_A, // a
        LOWERCASE_B, // b
        LOWERCASE_C, // c
        LOWERCASE_D, // d
        LOWERCASE_E, // e
        LOWERCASE_F, // f
        LOWERCASE_G, // g
        LOWERCASE_H, // h
        LOWERCASE_I, // i
        LOWERCASE_J, // j
        LOWERCASE_K, // k
        LOWERCASE_L, // l
        LOWERCASE_M, // m
        LOWERCASE_N, // n
        LOWERCASE_O, // o
        LOWERCASE_P, // p
        LOWERCASE_Q, // q
        LOWERCASE_R, // r
        LOWERCASE_S, // s
        LOWERCASE_T, // t
        LOWERCASE_U, // u
        LOWERCASE_V, // v
        LOWERCASE_W, // w
        LOWERCASE_X, // x
        LOWERCASE_Y, // y
        LOWERCASE_Z, // z
        OPEN_CURLY_BRACE, // {
        VERTICAL_PIPE, // |
        CLOSE_CURLY_BRACE, // }
        TILDE, // ~
        GREEK_DELTA_CAPITAL, // ⌂
        CEDILLA_CAPITAL, // Ç (C WITH TAIL)
        U_WITH_DOTS_LOWERCASE, // ü
        E_WITH_RIGHT_ACCENT_LOWERCASE, // é
        A_WITH_HAT_LOWERCASE, // â
        A_WITH_DOTS_LOWERCASE, // ä
        A_WITH_LEFT_ACCENT_LOWERCASE, // à
        ANGSTROM_LOWERCASE, // å
        CEDILLA_LOWERCASE, // ç (C WITH TAIL)
        E_WITH_HAT_LOWERCASE, // ê
        E_WITH_DOTS_LOWERCASE, // ë
        E_WITH_LEFT_ACCENT, // è
        I_WITH_DOTS_LOWERCASE, // ï
        I_WITH_HAT_LOWERCASE, // î
        I_WITH_LEFT_ACCENT, // ì
        A_WITH_DOTS_CAPITAL, // Ä
        ANGSTROM_CAPITAL, // Å
        E_WITH_RIGHT_ACCENT_CAPITAL, // É
        AE_LOWERCASE, // æ 
        AE_CAPITAL, // Æ
        O_WITH_HAT_LOWERCASE, // ô
        O_WITH_DOTS_LOWERCASE, // ö
        O_WITH_LEFT_ACCENT, // ò
        U_WITH_HAT_LOWERCASE, // û
        U_WITH_LEFT_ACCENT_LOWERCASE, // ù
        Y_WITH_DOTS_LOWERCASE, // ÿ
        O_WITH_DOTS_CAPITAL, // Ö
        U_WITH_DOTS_CAPITAL, // Ü
        CENTS_SIGN, // ¢
        POUNDS_SIGN, // £
        YEN_SIGN, // ¥ (FUNNY LOBSTER)
        PETESA, // ₧ (PHARMACY)
        FLORIN, // ƒ (FANCY F)
        A_WITH_RIGHT_ACCENT_LOWERCASE, // á
        I_WITH_RIGHT_ACCENT_LOWERCASE, // í
        O_WITH_RIGHT_ACCENT_LOWERCASE, // ó
        U_WITH_RIGHT_ACCENT_LOWERCASE, // ú
        N_WITH_TILDE_LOWERCASE, // ñ
        N_WITH_TILDE_CAPITAL, // Ñ
        SMALL_LETTER_A, // ª (CLOTH?)
        SMALL_LETTER_O, // º (CLOTH?)
        QUESTION_MARK_UPSIDE_DOWN, // ¿
        REVERSE_NOT_SIGN, // ⌐
        NOT_SIGN, // ¬
        FRACTION_HALF, // ½
        FRACTION_QUARTER, // ¼
        EXCLAMATION_POINT_UPSIDE_DOWN, // ¡
        LESS_THAN_DOUBLE, // «
        GREATER_THAN_DOUBLE, // »
        SHADED_WALL_LIGHT, // ░
        SHADED_WALL_MEDIUM, // ▒
        SHADED_WALL_DARK, // ▓
        BOX_DRAWING_VERTICAL_SINGLE, // │
        BOX_DRAWING_T_LEFT_SINGLE, // ┤
        BOX_DRAWING_T_SINGLE_VERTICAL_DOUBLE_HORIZONTAL, // ╡
        BOX_DRAWING_T_DOUBLE_VERTICAL_SINGLE_HORIZONTAL, // ╢
        BOX_DRAWING_CORNER_TOP_RIGHT_DOUBLE_VERTICAL_SINGLE_HORIZONTAL, // ╖
        BOX_DRAWING_CORNER_TOP_RIGHT_SINGLE_VERTICAL_DOUBLE_HORIZONTAL, // ╕
        BOX_DRAWING_T_LEFT_DOUBLE, // ╣
        BOX_DRAWING_VERTICAL_DOUBLE, // ║
        BOX_DRAWING_CORNER_TOP_RIGHT_DOUBLE, // ╗ 
        BOX_DRAWING_CORNER_BOTTOM_RIGHT_DOUBLE, // ╝
        BOX_DRAWING_CORNER_BOTTOM_RIGHT_DOUBLE_VERTICAL_SINGLE_HORIZONTAL, // ╜
        BOX_DRAWING_CORNER_BOTTOM_RIGHT_SINGLE_VERTICAL_DOUBLE_HORIZONTAL, // ╛
        BOX_DRAWING_CORNER_TOP_RIGHT_SINGLE, // ┐
        BOX_DRAWING_CORNER_BOTTOM_LEFT_SINGLE, // └
        BOX_DRAWING_T_UPWARD_SINGLE, // ┴
        BOX_DRAWING_T_DOWNARD_SINGLE, // ┬
        BOX_DRAWING_T_RIGHT_SINGLE, // ├
        BOX_DRAWING_HORIZONTAL_SINGLE, // ─ (LONG DASH)
        BOX_DRAWING_CROSS_SINGLE, // ┼
        BOX_DRAWING_T_RIGHT_SINGLE_VERTICAL_DOUBLE_HORIZONTAL, // ╞
        BOX_DRAWING_T_RIGHT_DOUBLE_VERTICAL_SINGLE_HORIZONTAL, // ╟
        BOX_DRAWING_CORNER_BOTTOM_LEFT_DOUBLE, // ╚
        BOX_DRAWING_CORNER_TOP_LEFT_DOUBLE, // ╔
        BOX_DRAWING_T_UPWARD_DOUBLE, // ╩
        BOX_DRAWING_T_DOWNWARD_DOUBLE, // ╦
        BOX_DRAWING_T_RIGHT_DOUBLE, // ╠
        BOX_DRAWING_HORIZONTAL_DOUBLE, // ═
        BOX_DRAWING_CROSS_DOUBLE, // ╬
        BOX_DRAWING_T_UPWARD_SINGLE_VERTICAL_DOUBLE_HORIZONTAL, // ╧
        BOX_DRAWING_T_UPWARD_DOUBLE_VERTICAL_SINGLE_HORIZONTAL, // ╨
        BOX_DRAWING_T_DOWNWARD_SINGLE_VERTICAL_DOUBLE_HORIZONTAL, // ╤
        BOX_DRAWING_T_DOWNWARD_DOUBLE_VERTICAL_SINGLE_HORIZONTAL, // ╥
        BOX_DRAWING_CORNER_BOTTOM_LEFT_DOUBLE_VERTICAL_SINGLE_HORIZONTAL, // ╙
        BOX_DRAWING_CORNER_BOTTOM_LEFT_SINGLE_VERTICAL_DOUBLE_HORIZONTAL, // ╘
        BOX_DRAWING_CORNER_TOP_LEFT_SINGLE_VERTICAL_DOUBLE_HORIZONTAL, // ╒
        BOX_DRAWING_CORNER_TOP_LEFT_DOUBLE_VERTICAL_SINGLE_HORIZONTAL, // ╓
        BOX_DRAWING_CROSS_DOUBLE_VERTICAL_SINGLE_HORIZONTAL, // ╫
        BOX_DRAWING_CROSS_SINGLE_VERTICAL_DOUBLE_HORIZONTAL, // ╪
        BOX_DRAWING_CORNER_BOTTOM_RIGHT_SINGLE, // ┘
        BOX_DRAWING_CORNER_TOP_LEFT_SINGLE, // ┌
        SOLID, // █
        SOLID_BOTTOM_HALF, // ▄
        SOLID_LEFT_HALF, // ▌
        SOLID_RIGHT_HALF, // ▐
        SOLID_TOP_HALF, // ▀
        GREEK_ALPHA, // α
        GREEK_BETA, // ß
        GREEK_GAMMA, // Γ
        GREEK_PI, // π
        GREEK_SIGMA_CAPITAL, // Σ
        GREEK_SIGMA_LOWERCASE, // σ
        GREEK_MEW, // µ
        GREEK_TAU, // τ
        GREEK_PHI_CAPITAL, // Φ
        GREEK_THETA, // Θ
        GREEK_OMEGA, // Ω
        GREEK_DELTA_LOWERCASE, // δ
        INFINITY, // ∞
        GREEK_PHI_LOWERCASE, // φ
        GREEK_EPSILON_CAPITAL, // ε
        INTERSECTION, // ∩
        TRIPLE_BAR, // ≡
        PLUS_MINUS, // ±
        GREATER_THAN_OR_EQUAL_TO, // ≥
        LESS_THAN_OR_EQUAL_TO, // ≤
        INTEGRAL_TOP_HALF, // ⌠
        INTEGRAL_BOTTOM_HALF, // ⌡
        DIVISION_SIGN, // ÷
        TILDE_DOUBLE, // ≈ (APPROX. EQUALS)
        DEGREE, // °
        BULLET_OPERATOR, // ∙ (SMALL BULLET)
        INTERPUNCT_OPERATOR, // · (HALF SMALL BULLET)
        SQUARE_ROOT, // √ (RADICAL)
        SMALL_LETTER_N, // ⁿ
        SMALL_NUMBER_2, // ² (SQUARED)
        SQUARE_FILLED, // ■
        SQUARE_HOLLOW, // □
    };

};

/* Stream insertion overload for SDL_Color */
std::ostream& operator<<(std::ostream &s, const SDL_Color &c);

#endif /* _ANSI_H_ */