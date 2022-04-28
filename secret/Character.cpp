#include "Character.h"

#include <bits/stdc++.h>

Character Character::A("A", ".-");
Character Character::B("B", "-...");
Character Character::C("C", "-.-.");
Character Character::D("D", "-..");
Character Character::E("E", ".");
Character Character::F("F", "..-.");
Character Character::G("G", "--.");
Character Character::H("H", "....");
Character Character::I("I", "..");
Character Character::J("J", ".---");
Character Character::K("K", "-.-");
Character Character::L("L", ".-..");
Character Character::M("M", "--");
Character Character::N("N", "-.");
Character Character::O("O", "---");
Character Character::P("P", ".--.");
Character Character::Q("Q", "--.-");
Character Character::R("R", ".-.");
Character Character::S("S", "...");
Character Character::T("T", "-");
Character Character::U("U", "..-");
Character Character::V("V", "...-");
Character Character::W("W", ".--");
Character Character::X("X", "-..-");
Character Character::Y("Y", "-.--");
Character Character::Z("Z", "--..");
Character Character::SPACE(" ", " ");
Character Character::NEWLINE("\n", "   ");

Character Character::characters[] = {
    A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z
};

Character::Character(std::string character, std::string signal) 
: character(character), signal(signal) {

}

bool Character::operator==(const Character &other) const {
    return signal.compare(other.signal) == 0;
}

Character Character::get(char c) {
    if ('A' <= c && c <= 'Z') {
        return characters[c-'A'];
    }
    if ('a' <= c && c <= 'z') {
        return characters[c-'a'];
    }
    if (c == '\n') {
        return Character::NEWLINE;
    }
    return Character::SPACE;
}

std::string Character::convert(const std::string &text) {
    std::string code = "";
    std::string message = "";
    std::string morse = "";
    std::string signal = "";

    for (size_t i = 0; i < text.length(); i++) {
        Character c = Character::get(text[i]);
        message.append(c.character);
        for (int symbol_idx = 0; symbol_idx < c.signal.length(); symbol_idx++) {
            char symbol = c.signal[symbol_idx];
            if (symbol == '.') {
                morse.append(".");
                signal.append("00");
            } else if (symbol == '-') {
                morse.append("-");
                signal.append("01");
            }
        }
        if (c == Character::SPACE) {
            morse.append("\n");
            signal.append("11");
        } else if (c == Character::NEWLINE) {
            morse.append("\n\n\n");
            signal.append("111111");
        } else {
            morse.append(" ");
            signal.append("10");
        }
    }
    
    while (signal.size() % 8 != 0) {
        signal.push_back('1');
    }

    code.append("/*\n");
    code.append(text);
    code.append("\n");
    code.append("\n");
    code.append(message);
    code.append("\n");
    code.append("\n");
    code.append(morse);
    code.append("\n");
    code.append(" */\n");
    code.append("\n");

    code.append("const uint8_t Sound::secret_code[] PROGMEM = {\n");
    for (size_t i = 0; i < signal.size();) {
        code.append("  0b");
        for (int b = 0; b < 8; b++, i++) {
            code.push_back(signal[i]);
        }
        code.append(",\n");
    }
    code.append("};");
    return code;
}
