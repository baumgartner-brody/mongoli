#include "ANSI.h"

/* Returns the appropriate 8-bit color code for a foreground and background color of choice */
const Uint8 ANSI::NUMERIC::createColorCode(const Uint8 &FG, const Uint8 &BG) {
    return (Uint8(16) * BG) + FG;
}