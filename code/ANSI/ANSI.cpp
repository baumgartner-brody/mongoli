#include "ANSI.h"

/* Definitions for ANSI::functions */

/* Replicates the iconic "win10colors.cmd" (prints all ANSI colors to the terminal) */
void ANSI::terminalTest(std::ostream &out) {

    /* Test style specifiers */
    out << ANSI::TERMINAL::BOLD << ANSI::TERMINAL::FOREGROUND::BRIGHT_YELLOW << ANSI::TERMINAL::BACKGROUND::BRIGHT_RED << "STYLES" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[0m\"" << ANSI::TERMINAL::RESET << " Reset\n";
    out << "\"\\33[1m\"" << ANSI::TERMINAL::BOLD << " Bold" << ANSI::TERMINAL::BOLD_OFF << '\n';
    out << "\"\\33[4m\"" << ANSI::TERMINAL::UNDERLINE << " Underline" << ANSI::TERMINAL::UNDERLINE_OFF << '\n';
    out << "\"\\33[7m\" " << ANSI::TERMINAL::INVERSE << "Inverse" << ANSI::TERMINAL::INVERSE_OFF << '\n';

    /* Test normal foreground colors */
    out << '\n';
    out << ANSI::TERMINAL::BOLD << ANSI::TERMINAL::FOREGROUND::BRIGHT_YELLOW << ANSI::TERMINAL::BACKGROUND::BRIGHT_RED << "NORMAL FOREGROUND COLORS" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[30m\"" << ANSI::TERMINAL::FOREGROUND::BLACK << " Black " << ANSI::TERMINAL::RESET << "(Black)\n";
    out << "\"\\33[31m\"" << ANSI::TERMINAL::FOREGROUND::DARK_RED << " Red" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[32m\"" << ANSI::TERMINAL::FOREGROUND::DARK_GREEN << " Green" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[33m\"" << ANSI::TERMINAL::FOREGROUND::DARK_YELLOW << " Yellow" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[34m\"" << ANSI::TERMINAL::FOREGROUND::DARK_BLUE << " Blue" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[35m\"" << ANSI::TERMINAL::FOREGROUND::DARK_PURPLE << " Magenta" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[36m\"" << ANSI::TERMINAL::FOREGROUND::DARK_CYAN << " Cyan" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[37m\"" << ANSI::TERMINAL::FOREGROUND::LIGHT_GREY << " White" << ANSI::TERMINAL::RESET << '\n';

    /* Test normal background colors */
    out << '\n';
    out << ANSI::TERMINAL::BOLD << ANSI::TERMINAL::FOREGROUND::BRIGHT_YELLOW << ANSI::TERMINAL::BACKGROUND::BRIGHT_RED << "NORMAL BACKGROUND COLORS" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[40m\" " << ANSI::TERMINAL::BACKGROUND::BLACK << "Black" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[41m\" " << ANSI::TERMINAL::BACKGROUND::DARK_RED << "Red" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[42m\" " << ANSI::TERMINAL::BACKGROUND::DARK_GREEN << "Green" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[43m\" " << ANSI::TERMINAL::BACKGROUND::DARK_YELLOW << "Yellow" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[44m\" " << ANSI::TERMINAL::BACKGROUND::DARK_BLUE << "Blue" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[45m\" " << ANSI::TERMINAL::BACKGROUND::DARK_PURPLE << "Magenta" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[46m\" " << ANSI::TERMINAL::BACKGROUND::DARK_CYAN << "Cyan" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[47m\" " << ANSI::TERMINAL::BACKGROUND::LIGHT_GREY << "White " << ANSI::TERMINAL::RESET << " (White)\n";

    /* Test strong foreground colors */
    out << '\n';
    out << ANSI::TERMINAL::BOLD << ANSI::TERMINAL::FOREGROUND::BRIGHT_YELLOW << ANSI::TERMINAL::BACKGROUND::BRIGHT_RED << "STRONG FOREGROUND COLORS" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[90m\"" << ANSI::TERMINAL::FOREGROUND::DARK_GREY << " Black " << ANSI::TERMINAL::RESET << "(Black)\n";
    out << "\"\\33[91m\"" << ANSI::TERMINAL::FOREGROUND::BRIGHT_RED << " Red" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[92m\"" << ANSI::TERMINAL::FOREGROUND::BRIGHT_GREEN << " Green" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[93m\"" << ANSI::TERMINAL::FOREGROUND::BRIGHT_YELLOW << " Yellow" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[94m\"" << ANSI::TERMINAL::FOREGROUND::BRIGHT_BLUE << " Blue" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[95m\"" << ANSI::TERMINAL::FOREGROUND::BRIGHT_PURPLE << " Magenta" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[96m\"" << ANSI::TERMINAL::FOREGROUND::BRIGHT_CYAN << " Cyan" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[97m\"" << ANSI::TERMINAL::FOREGROUND::WHITE << " White" << ANSI::TERMINAL::RESET << '\n';

    /* Test strong background colors */
    out << '\n';
    out << ANSI::TERMINAL::BOLD << ANSI::TERMINAL::FOREGROUND::BRIGHT_YELLOW << ANSI::TERMINAL::BACKGROUND::BRIGHT_RED << "STRONG BACKGROUND COLORS" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[100m\" " << ANSI::TERMINAL::BACKGROUND::DARK_GREY << "Black" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[101m\" " << ANSI::TERMINAL::BACKGROUND::BRIGHT_RED << "Red" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[102m\" " << ANSI::TERMINAL::BACKGROUND::BRIGHT_GREEN << "Green" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[103m\" " << ANSI::TERMINAL::BACKGROUND::BRIGHT_YELLOW << "Yellow" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[104m\" " << ANSI::TERMINAL::BACKGROUND::BRIGHT_BLUE << "Blue" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[105m\" " << ANSI::TERMINAL::BACKGROUND::BRIGHT_PURPLE << "Magenta" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[106m\" " << ANSI::TERMINAL::BACKGROUND::BRIGHT_CYAN << "Cyan" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[107m\" " << ANSI::TERMINAL::BACKGROUND::WHITE << "White " << ANSI::TERMINAL::RESET << " (White)\n";

    /* Test combinations */
    out << '\n';
    out << ANSI::TERMINAL::BOLD << ANSI::TERMINAL::FOREGROUND::BRIGHT_YELLOW << ANSI::TERMINAL::BACKGROUND::BRIGHT_RED << "COMBINATIONS" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[31m\"" << ANSI::TERMINAL::FOREGROUND::DARK_RED << " Dark Red Foreground" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[7m\" " << ANSI::TERMINAL::INVERSE << "Inverse" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[7;31m\" " <<  ANSI::TERMINAL::INVERSE << ANSI::TERMINAL::FOREGROUND::DARK_RED << "Dark Red Foreground Inversed" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[7m\" and then \"\\33[31m\" " << ANSI::TERMINAL::INVERSE << "Normal Inverse" << ANSI::TERMINAL::FOREGROUND::DARK_RED << "Dark Red Foreground (retaining inverse)" << ANSI::TERMINAL::RESET << '\n';
    out << "\"\\33[31m\" and then \"\\33[7m\" " << ANSI::TERMINAL::FOREGROUND::DARK_RED << "Dark Red Foreground" << ANSI::TERMINAL::INVERSE << "Inverse (retaining dark red foreground)" << ANSI::TERMINAL::RESET << '\n';

}

void ANSI::SDLCOLOR::testItrArithmetic() {
    std::cout << "Creating SDLCOLOR_ITERATOR begin() and doing -5\n";
    ANSI::SDLCOLOR_ITERATOR i = ANSI::SDLCOLOR::begin() - 5;
    i -= 5;
    i -= -5;
    i -= 10;
    i += 2;
}

