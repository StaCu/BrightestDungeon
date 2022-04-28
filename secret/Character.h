#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct Character {

    const std::string signal;
    const std::string character;

    static Character A;
    static Character B;
    static Character C;
    static Character D;
    static Character E;
    static Character F;
    static Character G;
    static Character H;
    static Character I;
    static Character J;
    static Character K;
    static Character L;
    static Character M;
    static Character N;
    static Character O;
    static Character P;
    static Character Q;
    static Character R;
    static Character S;
    static Character T;
    static Character U;
    static Character V;
    static Character W;
    static Character X;
    static Character Y;
    static Character Z;

    static Character SPACE;
    static Character NEWLINE;

    static Character characters[];

private:
    Character(std::string character, std::string signal);

public:
    bool operator==(const Character &other) const;

    static Character get(char c);

    static std::string convert(const std::string &text);

};
