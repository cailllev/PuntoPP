#include "console.h"

std::string RESET = "\033[0m";

std::string black(std::string t) {return "\033[30;1m" + t + RESET;}
std::string red(std::string t) {return "\033[31;1m" + t + RESET;}
std::string green(std::string t) {return "\033[32;1m" + t + RESET;}
std::string yellow(std::string t) {return "\033[33;1m" + t + RESET;}
std::string blue(std::string t) {return "\033[34;1m" + t + RESET;}
std::string magenta(std::string t) {return "\033[35;1m" + t + RESET;}
std::string cyan(std::string t) {return "\033[36;1m" + t + RESET;}
std::string white(std::string t) {return "\033[37;1m" + t + RESET;}

std::string clear_screen() {return "\033[2J\033[H";}
