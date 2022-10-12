#ifndef _TYPED_TEXT_H_
#define _TYPED_TEXT_H_

#include <stdint.h>
#include <vector>

typedef uint8_t Uint8;

class Entity;

// TODO
// the sheer and utter horror of documentation awaits

// TODO needs a way of centering the entered text
// this will be interfaced via TextManager::freeKeyboardInput2()
// this will handle advanced deletion 

class TypedText {
public:
    TypedText(const int &x, const int &y);
    TypedText(const int &x, const int &y, const bool &x_centered);
    ~TypedText();

    void draw();

    /* Deep deletes all TypedText memory */
    void clear() noexcept;

    /* Insert a string into the current cursor position */
    void insert(const char *t);

    /* Move text cursor left or right by one character */
    void moveCursorLeft() noexcept;
    void moveCursorRight() noexcept;

    /* Moves cursor left or right to the next whitespace character after any non-whitespace character */
    void moveCursorLeftWord() noexcept;
    void moveCursorRightWord() noexcept;

    /* Add the character left or right of the cursor to the selection */
    void selectCharacterLeft() noexcept;
    void selectCharacterRight() noexcept;

    /* If 0 or more characters but less than all of them are selected, select the rest. */
    /*  Otherwise, deselect all. */
    void selectAll() noexcept;

    /* FALSE - backspace */
    /* TRUE  - delete */
    void backspaceOrDeleteEvent(const bool &backspace_or_delete);

    /* */
    void handleMouseSelectionEvents();

private:

    void _init();

    int _x, _y;

    bool _x_centered = false;

    std::vector<Uint8> *_text = nullptr;

    std::vector<Entity*> *_displayText = nullptr;

    int _selection_min = 0, _selection_max = -1;

    Uint8 _normal_text_color = 0x0e;
    Uint8 _selected_text_color = 0xf4;

    /* Clears the enities that make up the frontend of the text display */
    /* Note that this does NOT deep delete the vector */
    void _clearDisplayText() noexcept;

    /* Recolor a symbol at the specified index */
    void _recolorSymbolAtIndex(const unsigned int &i, const Uint8 &c) { this->_recolorSymbolBetweenRange(i, i, c); }

    /* Recolor the symbols between a specified range */
    void _recolorSymbolBetweenRange(const unsigned int &l, const unsigned int &h, const Uint8 &c);

    /* Unselects a range of text and resets the cursor position to the minimum */
    void _unselectTextRangeAndResetCursorToMinimum() noexcept;

    /* Unselects a range of text and resets the cursor position to the maximum */
    void _unselectTextRangeAndResetCursorToMaximum() noexcept;

    /* Remove the text selection from the entry */
    void _removeSelectedLetters() noexcept;

};

#endif /* _TYPED_TEXT_H_ */