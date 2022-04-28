#include <iostream>
#include <fstream>

#include "Character.h"

std::string text0 = "hello im glad you enjoy the brightest dungeon\nit was a really fun project to work on\nsadly there are only few bytes left\ngoodby";
std::string text1 = "thanks for playing brightest dungeon goodby";

int main(int argc, const char **argv) {
    std::string code = Character::convert(text1);
    std::cout << code << std::endl;

    std::ofstream output("Secret.h");
    output << code << std::endl;

    return 0;
}
