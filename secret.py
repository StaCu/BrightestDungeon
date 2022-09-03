#!/bin/python3
# To generate a secret morse code that can be integrated into the game write the text here and run the script.
# It will update the source code accordingly.

text = 'hello im glad you enjoy the brightest dungeon\nit was a really fun project to work on\nsadly there are only few bytes left\ngoodby'
text = 'thanks for playing brightest dungeon goodby'

# morse_code[character] = dits and dots
morse_code = {
    'A': '.-',
    'B': '-...',
    'C': '-.-.',
    'D': '-..',
    'E': '.',
    'F': '..-.',
    'G': '--.',
    'H': '....',
    'I': '..',
    'J': '.---',
    'K': '-.-',
    'L': '.-..',
    'M': '--',
    'N': '-.',
    'O': '---',
    'P': '.--.',
    'Q': '--.-',
    'R': '.-.',
    'S': '...',
    'T': '-',
    'U': '..-',
    'V': '...-',
    'W': '.--',
    'X': '-..-',
    'Y': '-.--',
    'Z': '--..',
    '0': '-----',
    '1': '.----',
    '2': '..---',
    '3': '...--',
    '4': '....-',
    '5': '.....',
    '6': '-....',
    '7': '--...',
    '8': '---..',
    '9': '----.',
    ' ': ' ',
    '\n': '\n'
}

def convert_text_to_morse_cpp(text):
    """
    Converts the text to morse code, then converts the 
    morse code to a signal encoding used by BrightestDungeon
    (00: '.', 01: '-', 11: pause between words, 10: pause between characters).
    Afterwards the signal is wrapped in c++ code to integrate it.

    @param text
        the text to integrate into the game as morse code
    @return
        the resulting c++ code
    """

    message = '' # the message that is actually transmitted (may differ from the input text)
    morse   = '' # the morse code
    signal  = '' # the encoded signal
    code    = '' # the c++ code

    # morse code only contains upper case letters
    for c in text.upper():
        if c not in morse_code:
            # unknown characters are replaced by ' '
            c = ' '
        message += c
        
        for d in morse_code[c]:
            if d == '.':
                morse  += d
                signal += '00'
            elif d == '-':
                morse  += d
                signal += '01'

        if c == ' ':
            morse  += '\n'
            signal += '11'
        elif c == '\n':
            morse  += '\n\n\n'
            signal += '111111'
        else:
            morse  += ' '
            signal += '10'
    
    # the signal length must be divisible by 8 to fit the uint8_t data type
    # => pad with 1s
    while len(signal) % 8 != 0:
        signal += '1'

    # put text, message and morse at the beginning of the code
    # so the user can verify it
    code += '/*\n'
    code += text
    code += '\n'
    code += '\n'
    code += message
    code += '\n'
    code += '\n'
    code += morse
    code += '\n'
    code += ' */\n'
    code += '\n'

    # definition of the signal encoding
    code += 'const uint8_t Sound::secret_code[] PROGMEM = {\n'
    i = 0
    while i < len(signal):
        # write each 8bit-chunk seperately
        # => because of uint8_t data type
        code += '    0b'
        for b in range(8):
            code += signal[i]
            i += 1
        code += ',\n'

    code += '};\n'
    return code

# convert the given text
code = convert_text_to_morse_cpp(text)
# integrate the code into the game
with open('src/Secret.h', 'w') as file:
    file.write(code)
