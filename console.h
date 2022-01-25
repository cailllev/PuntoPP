#ifndef CONSOLE_H
#define CONSOLE_H

#include <functional>
#include <string>

std::string black(std::string t);
std::string red(std::string t);
std::string green(std::string t);
std::string yellow(std::string t);
std::string blue(std::string t);
std::string magenta(std::string t);
std::string cyan(std::string t);
std::string white(std::string t);

std::string clear_screen();

const std::array<std::function<std::string(std::string)>, 5> to_colour = {
    &black, &red, &green, &blue, &yellow
};

#endif
