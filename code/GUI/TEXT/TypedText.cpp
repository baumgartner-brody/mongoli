#include "TypedText.h"

#include "../../ECS/Components.h"
#include "TextManager.h"

/* TODO */
// First add [LEFT_ARROW_KEY] and [RIGHT_ARROW_KEY] support for text cursor 
// Then add a hitbox to each letter entity
// Then use that to add support for selecting part of a text string

// Holding CTRL and pressing an arrow key should allow for seeking to the next "word"
// Holding SHIFT and pressing an arrow key should allow for selecting multiple letters

// It looks like this will need to get its own TextManager::addText overload

// You must account for the fact that spaces and newline characters can potentially take up space in _text but do not create entities
// With text highlighted by the mouse, it looks like clicking outside of the text input box resets the cursor back to the end (and unselects text)

TypedText::TypedText(const int &x, const int &y) {
    this->_x = x;
    this->_y = y;
    this->_init();
}

TypedText::TypedText(const int &x, const int &y, const bool &x_centered) {
    this->_x = x;
    this->_y = y;
    this->_x_centered = x_centered;
    this->_init();
}

TypedText::~TypedText() {
    this->clear();
}

// TODO - Centering capability
void TypedText::draw() {
    //TextManager::destroyFreeKeyboardEntry();
    //*TextManager::freeKeyboardDisplayText = TextManager::addText(x, y, assetNumber, TextManager::freeKeyboardSymbols);
    this->_clearDisplayText();
    *this->_displayText = TextManager::addTextWithHitboxes(this->_x, this->_y, 0x0e, *this->_text);
}

/* Deep deletes all TypedText memory */
void TypedText::clear() noexcept {
    this->_selection_min = this->_selection_max = -1;
    this->_clearDisplayText();
    if (this->_displayText != nullptr) delete this->_displayText;
    this->_displayText = nullptr;
    if (this->_text != nullptr) {
        this->_text->clear();
        delete this->_text;
    }
    this->_text = nullptr;
}

// There is more to account for here than what meets the eye
// Must account for pasting mutliple characters in the middle of a string

// Normal typing (append)
// Typing with one char selected (replace that char and insert typed text)
// Typing or pasting with multiple chars selected - eat that text and put the new text in

// testing
void TypedText::insert(const char *t) {
    /*
    unsigned int i = 0u;
    while (t[i] != '\0') {

        Uint8 c = t[i];

        if (c == Uint8(0xe2) || c == Uint8(0xc2) || c == Uint8(0xc3) || c == Uint8(0xce) || c == Uint8(0xcf))
            this->_text->emplace_back(TextManager::digestComplexCharacter(t, i, c));
        else 
            this->_text->emplace_back(c);

        ++i;
    }
    this->_selection_min = this->_text->size();
    */
    if (this->_selection_max != -1) {
        int min = std::min(this->_selection_min, this->_selection_max);
        int max = std::max(this->_selection_min, this->_selection_max);
        if (max < this->_text->size()) ++max;
        this->_text->erase(std::begin(*this->_text) + min, std::begin(*this->_text) + max);
        this->_selection_min = min;
        this->_selection_max = -1;
        std::cout << "After deletion, selection_min is " << std::dec << this->_selection_min << '\n';
        //this->draw();
        //if (this->_selection_min < this->_text->size())
            //this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color); 
    } 
    if (this->_selection_min == this->_text->size() || this->_selection_min == -1) {
        std::cout << "typing or pasting text at the end of the string\n";
        unsigned int i = 0u;
        while (t[i] != '\0') {

            Uint8 c = t[i];

            if (c == Uint8(0xe2) || c == Uint8(0xc2) || c == Uint8(0xc3) || c == Uint8(0xce) || c == Uint8(0xcf))
                this->_text->emplace_back(TextManager::digestComplexCharacter(t, i, c));
            else 
                this->_text->emplace_back(c);
            this->_selection_min = this->_text->size();

            ++i;
        }
        this->draw();
    } else {
        std::cout << "inserting text into the middle of a string\n";

        unsigned int i = 0u;
        while (t[i] != '\0') {

            Uint8 c = t[i];

            if (c == Uint8(0xe2) || c == Uint8(0xc2) || c == Uint8(0xc3) || c == Uint8(0xce) || c == Uint8(0xcf))
                this->_text->insert(this->_text->begin() + this->_selection_min, TextManager::digestComplexCharacter(t, i, c));
            else 
                this->_text->insert(this->_text->begin() + this->_selection_min, c);
            ++this->_selection_min;

            ++i;
        }
        std::cout << "selection min is now " << std::dec << this->_selection_min << '\n';
        std::cout << "text.size() " << this->_text->size() << '\n';
        this->draw();
        if (this->_selection_min < this->_text->size()) {
            std::cout << "recoloring selected\n";
            this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color); 
        }
    }
}

void TypedText::moveCursorLeft() noexcept {
    if (this->_selection_max != -1) {
        this->_unselectTextRangeAndResetCursorToMinimum();
        /* Seg fault prevention */
        if (this->_selection_min < this->_text->size())
            this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
        std::cout << "changed selection min(unselect event left) to: " << this->_selection_min << '\n';
    } else if (this->_selection_min > 0) {
        /* Seg fault prevention */
        if (this->_selection_min < this->_text->size())
            this->_recolorSymbolAtIndex(this->_selection_min, this->_normal_text_color);
        --this->_selection_min;
        std::cout << "changed selection min(--) to: " << this->_selection_min << '\n';
        this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
    }
}

void TypedText::moveCursorRight() noexcept {
    if (this->_selection_max != -1) {
        this->_unselectTextRangeAndResetCursorToMaximum();
        /* Seg fault prevention */
        if (this->_selection_min < this->_text->size())
            this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
        std::cout << "changed selection min(unselect event right) to: " << this->_selection_min << '\n';
    } else if (this->_selection_min < this->_text->size()) {
        this->_recolorSymbolAtIndex(this->_selection_min, this->_normal_text_color);
        ++this->_selection_min;
        std::cout << "changed selection min(++) to: " << this->_selection_min << '\n';
        /* Seg fault prevention */
        if (this->_selection_min < this->_text->size())
            this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
    }
}

/* Moves cursor left or right to the next whitespace character */
void TypedText::moveCursorLeftWord() noexcept {
    if (this->_selection_max != -1) 
        this->_unselectTextRangeAndResetCursorToMinimum();
    if (this->_selection_min > 0) {
        /* Seg fault prevention */
        if (this->_selection_min < this->_text->size())
            this->_recolorSymbolAtIndex(this->_selection_min, this->_normal_text_color);
        bool b = false; // observed at least one non-whitespace character
        while (true) {
            --this->_selection_min;
            if (this->_selection_min == 0 || (b && this->_text->at(this->_selection_min) == ' ')) {
                this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
                break;
            } else if (this->_text->at(this->_selection_min) != ' ')
                b = true;
        }
    }
}

void TypedText::moveCursorRightWord() noexcept {
    if (this->_selection_max != -1) 
        this->_unselectTextRangeAndResetCursorToMaximum();
    if (this->_selection_min < this->_text->size()) {
        this->_recolorSymbolAtIndex(this->_selection_min, this->_normal_text_color);
        bool b = false; // observed at least one non-whitespace character
        while (true) {
            ++this->_selection_min;
            if (this->_selection_min == this->_text->size() || (b && this->_text->at(this->_selection_min) == ' ')) {
                /* Seg fault prevention */
                if (this->_selection_min < this->_text->size())
                    this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
                break;
            } else if (this->_text->at(this->_selection_min) != ' ')
                b = true;
        }
    }
}

void TypedText::selectCharacterLeft() noexcept {
    if (this->_selection_max != -1) {
        /* The user started selecting text from right to left, so the left end of the selection will be modified */
        if (this->_selection_max > this->_selection_min) {
            if (this->_selection_min > 0) {
                --this->_selection_min;
                this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
            }
        /* The user started selecting text from left to right, so the right end of the selection will be modified */
        } else {
            if (this->_selection_min <= this->_text->size()) {
                /* Seg fault prevention */
                if (this->_selection_min < this->_text->size())
                    this->_recolorSymbolAtIndex(this->_selection_min, this->_normal_text_color);
                --this->_selection_min;
            }
        }
        if (this->_selection_min == this->_selection_max) {
            this->_selection_max = -1;
            /* Seg fault prevention */
            if (this->_selection_min < this->_text->size())
                this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
        }
    } else if (this->_selection_min > 0) {
        if (this->_selection_max == -1)
            this->_selection_max = this->_selection_min;
        --this->_selection_min;
        std::cout << "changed selection min(--) to: " << this->_selection_min << '\n';
        //this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
        this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
    }
}

void TypedText::selectCharacterRight() noexcept {
    if (this->_selection_max != -1) {
        /* The user started selecting text from right to left, so the left end of the selection will be modified */
        if (this->_selection_max > this->_selection_min) {
            if (this->_selection_min < this->_text->size()) {
                /* Seg fault prevention */
                if (this->_selection_min < this->_text->size())
                    this->_recolorSymbolAtIndex(this->_selection_min, this->_normal_text_color);
                ++this->_selection_min;
            }
        /* The user started selecting text from left to right, so the right end of the selection will be modified */
        } else {
            if (this->_selection_min < this->_text->size()) {
                ++this->_selection_min;
                /* Seg fault prevention */
                if (this->_selection_min < this->_text->size())
                    this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
            }
        }
        if (this->_selection_min == this->_selection_max) {
            this->_selection_max = -1;
            /* Seg fault prevention */
            if (this->_selection_min < this->_text->size())
                this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
        }
    } else if (this->_selection_min < this->_text->size()) {
        if (this->_selection_max == -1)
            this->_selection_max = this->_selection_min;
        ++this->_selection_min;
        std::cout << "changed selection min(++) to: " << this->_selection_min << '\n';
        /* Seg fault prevention */
        if (this->_selection_min < this->_text->size())
            this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
    }
}

/* If 0 or more characters but less than all of them are selected, select the rest. */
/*  Otherwise, deselect all. */
void TypedText::selectAll() noexcept {
    /* If there is no text, do nothing */
    if (this->_text->empty()) return;

    /* If less than all characters are selected, select all of them */
    /* This is treated as a left to right selection event */
    int min = std::min(this->_selection_min, this->_selection_max);
    int max = std::max(this->_selection_min, this->_selection_max);

    /* All characters are selected, deselect all */
    if (min == 0 && max == this->_text->size()) {
        std::cout << "Deselecting all\n";
        this->_recolorSymbolBetweenRange(min, max - 1, this->_normal_text_color);
        this->_selection_max = -1;
        this->_selection_min = this->_text->size();
    /* Less than all characters are selected, select all */
    } else {
        std::cout << "Selecting all\n";
        this->_selection_max = this->_text->size();
        this->_selection_min = 0;
        this->_recolorSymbolBetweenRange(this->_selection_min, this->_selection_max - 1, this->_selected_text_color);
    }
}

/* FALSE - backspace */
/* TRUE  - delete */
void TypedText::backspaceOrDeleteEvent(const bool &backspace_or_delete) {
    /* If multiple characters are selected, backspace and delete function the same. */
    /* They remove the selected characters and reset the cursor to the minimum position */
    if (this->_selection_max != - 1) {

        std::cout << "Delete letter group event\n";
        
        int min = std::min(this->_selection_min, this->_selection_max);
        int max = std::max(this->_selection_min, this->_selection_max);
        if (max < this->_text->size()) ++max;
        this->_text->erase(std::begin(*this->_text) + min, std::begin(*this->_text) + max);
        if (min > 0) --min;
        this->_selection_min = min;
        this->_selection_max = -1;
        this->draw();
        if (this->_selection_min < this->_text->size())
            this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color); 

    /* Backspace event */
    } else if (!backspace_or_delete) {
        std::cout << "backspace event\n";
        if (this->_selection_min == this->_text->size()) {
            if (!this->_text->empty()) {
                this->_text->pop_back();
                --this->_selection_min;
                this->draw();
            }
        } else {
            this->_text->erase(this->_text->begin() + this->_selection_min);
            if (this->_selection_min > 0) --this->_selection_min;
            this->draw();
            if (!this->_text->empty())
                this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
        }
    /* Delete event */
    } else {
        if (this->_selection_min < (int)this->_text->size() - 1) {
            this->_text->erase(this->_text->begin() + this->_selection_min + 1);
            //if (this->_selection_min > 0) --this->_selection_min;
            this->draw();
            if (!this->_text->empty())
                this->_recolorSymbolAtIndex(this->_selection_min, this->_selected_text_color);
        }
    }
}

void TypedText::handleMouseSelectionEvents() {
    
}

void TypedText::_init() {
    this->_text = new std::vector<Uint8>;
    this->_displayText = new std::vector<Entity*>;
}

void TypedText::_clearDisplayText() noexcept {
    if (this->_displayText != nullptr) {
        for (auto & e : *this->_displayText) e->destroy();
        this->_displayText->clear();
    }
}

/* Recolor the symbols between a speicified range */
void TypedText::_recolorSymbolBetweenRange(const unsigned int &l, const unsigned int &h, const Uint8 &c) {

    if (l >= this->_displayText->size()) {
        std::cerr << "Provided low-end of recolor range (" << l << ") is out of bounds.\n";
        exit(-1);
    }
    if (h >= this->_displayText->size()) {
        std::cerr << "Provided high-end of recolor range (" << h << ") is out of bounds.\n";
        exit(-1);
    }

    for (int i = 0; i < std::min(h + 1u, static_cast<unsigned int>(this->_displayText->size())); ++i) 
        if (l <= i && i <= h) this->_displayText->at(i)->getComponent<SpriteComponent>().swapAsset(c);
}

/* Unselects a range of text and resets the cursor position to the minimum */
void TypedText::_unselectTextRangeAndResetCursorToMinimum() noexcept {
    if (this->_selection_max != -1) {
        int min = std::min(this->_selection_min, this->_selection_max);
        int r = std::max(this->_selection_min, this->_selection_max) - min;
        if (min + r == this->_text->size()) --r;
        this->_recolorSymbolBetweenRange(min, min + r, this->_normal_text_color);
        this->_selection_min = min;
        this->_selection_max = -1;
    }
}

/* Unselects a range of text and resets the cursor position to the maximum */
void TypedText::_unselectTextRangeAndResetCursorToMaximum() noexcept {
    if (this->_selection_max != -1) {
        int min = std::min(this->_selection_min, this->_selection_max);
        int max = std::max(this->_selection_min, this->_selection_max);
        int r = max - min;
        if (min + r == this->_text->size()) r -= 1;
        this->_recolorSymbolBetweenRange(min, min + r, this->_normal_text_color);
        this->_selection_min = max;
        this->_selection_max = -1;
    }
}

/* Remove the text selection from the entry */
void TypedText::_removeSelectedLetters() noexcept {

}