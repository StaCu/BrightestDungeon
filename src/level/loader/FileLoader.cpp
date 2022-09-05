#include "FileLoader.h"

#if ENABLE_FILE_LOADER

#include "../Floor.h"
#include "../Room.h"
#include "../Options.h"
#include "../../peripheral/sound/Sound.h"

#include "../entity/Entity.h"
#include "../entity/EntityUnion.h"

// max buffer size is 254, because 255 is used to identify errors
#define FILEIO_BUFFER_SIZE 64

// the directory and file names are hardcoded
// the N is replaced by a digit from 0-f
#define FILEIO_DUNGEON_DIRECTORY "dng/N/"
// the FFF by a number from 001-255
#define FILEIO_FLOOR_DIRECTORY "FFF/"
// the RRR by a number from 000-255
#define FILEIO_ROOM_FILE "RRR.txt"

char buffer[FILEIO_BUFFER_SIZE+1];
char filename[] = {'d', 'n', 'g', '/', '\0', '/', '\0', '\0', '\0', '/', '\0', '\0', '\0', '.', 't', 'x', 't', '\0'};

bool FileLoader::load(uint8_t floor, uint8_t room) {
  // File system
  if (file_system_init()) {
    LOG_LN(F("SD card detected"));
  } else {
    LOG_LN(F("No SD card detected"));
    file_system_deinit();
    return false;
  }

  // Directory
  char dungeon = (char) Options::dungeon;
  if (Options::dungeon >= 10) {
    dungeon += 'a';
  } else {
    dungeon += '0';
  }
  filename[4] = dungeon;
  intToDec(floor, &filename[6]);
  intToDec(room, &filename[10]);

  LOG(F("open file \""));
  LOG(filename);
  LOG_LN(F("\""));

  if (!file_open(filename)) {
    LOG_LN(F("file not found"));
    // no such file
    // => previous level was the last one
    // => victory :)
    file_system_deinit();
    return false;
  }

  // Text
  LOG_LN(F("reading..."));
  uint8_t pos = 0;
  while (file_has_next_char()) {
    // add the next char to the buffer
    char c = file_get_next_char();
    buffer[pos++] = c;

    if (pos == FILEIO_BUFFER_SIZE+1 &&
        (c != '\n' && c != '\r' && c != '\0')) {
      // buffer is full
      // => enforce end of command
      c = '\0';
      // find the real end of the line
      while (file_has_next_char()) {
        char eol = file_get_next_char();
        if (eol == '\n' || eol == '\r' || eol == '\0') {
          break;
        }
      }
    }

    if (c == '\n' || c == '\r' || c == '\0') {
      // end of command
      // => process the content
      buffer[pos] = '\0';
      pos = 0;
      bool end = process(pos, &buffer[0]);
      if (end) {
        break;
      }
      pos = 0;
    }
  }

  file_close();
  file_system_deinit();
  return true;
}

bool FileLoader::process(uint8_t &p, char* b) {
  if (match(p,b,(const char*) F("RESET"))) {
    Entity::resetMaskPartially(readInt(p,b));
    return false;
  }
  if (match(p,b,(const char*) F("NEXT"))) {
    Hero::setGoal(true);
    return true;
  }  
  if (match(p,b,(const char*) F("END"))) {
    return true;
  }  
  if (match(p,b,(const char*) F("SECRET"))) {
    // this part is not mentioned in the README on purpose,
    // but it is still possible to play the secret morse code
    // from within a dungeon defined on the sdcard.
    Sound::play(Sound::SECRET);
    return false;
  }
  
  uint8_t type = ENTITY_TYPE_COUNT;
  if (match(p,b,(const char*) F("HERO"))) {
    type = HERO_ID;
  } else if (match(p,b,(const char*) F("BOSS"))) {
    type = BOSS_ID;
  } else if (match(p,b,(const char*) F("MONSTER"))) {
    type = MONSTER_ID;
  } else if (match(p,b,(const char*) F("WORM"))) {
    type = WORM_ID;
  } else if (match(p,b,(const char*) F("SPAWNER"))) {
    type = SPAWNER_ID;
  } else if (match(p,b,(const char*) F("BUTTON"))) {
    type = BUTTON_ID;
  } else if (match(p,b,(const char*) F("DOOR"))) {
    type = DOOR_ID;
  } else if (match(p,b,(const char*) F("FOG"))) {
    type = FOG_ID;
  } else if (match(p,b,(const char*) F("FORCEFIELD"))) {
    type = FORCEFIELD_ID;
  } else if (match(p,b,(const char*) F("ITEM"))) {
    type = ITEM_ID;
  } else if (match(p,b,(const char*) F("LAVA"))) {
    type = LAVA_ID;
  } else if (match(p,b,(const char*) F("MINE"))) {
    type = MINE_ID;
  } else if (match(p,b,(const char*) F("PORTAL"))) {
    type = PORTAL_ID;
  } else if (match(p,b,(const char*) F("SWITCH"))) {
    type = SWITCH_ID;
  } else if (match(p,b,(const char*) F("WALL"))) {
    type = WALL_ID;
  } else if (match(p,b,(const char*) F("WATER"))) {
    type = WATER_ID;
  }

  if (type == ENTITY_TYPE_COUNT) {
    // unknown type
    return false;
  }

  uint8_t arg_count = Entity::getArgCount(type);
  uint8_t args[MAX_PARAMETER_PER_ENTITY];
  for (uint8_t arg = 0; arg < arg_count; arg++) {
      args[arg] = readInt(p,b);
  }

  Entity::spawn(type, &args[0]);
  return false;
}

bool FileLoader::match(uint8_t &pos, char* buffer, const char *token) {
  uint32_t i = 0;
  while (platform_read_byte_progmem(&token[i]) != '\0') {
    if (platform_read_byte_progmem(&token[i]) != buffer[pos+i]) {
      return false;
    }
    if (buffer[pos+i] == '\0') {
      return false;
    }
    i++;
  }
  pos += i;
  return true;
}

uint8_t FileLoader::readInt(uint8_t &pos, char* buffer) {
  return decToInt(pos, buffer);
}

void FileLoader::intToDec(uint8_t value, char* text) {
  for (uint8_t i = 0; i < 3; i++) {
    uint8_t div = value / 10;
    uint8_t mod = value % 10;
    value = div;
    text[2-i] = mod + '0';
  }
}

int32_t FileLoader::decToInt(uint8_t &pos, char* buffer) {
  if (buffer[pos] == '\0') {
    return 0;
  }

  int32_t value = 0;
  while (buffer[pos] == ' ' || buffer[pos] == '(' || buffer[pos] == ',' || buffer[pos] == '\t') {
    pos++;
    if (buffer[pos] == '\0') {
      return 0;
    }
  }
  bool invert = false;
  int8_t multiplier = 10;
  if (buffer[pos] == '-') {
    invert = true;
    pos++;
  }
  if (buffer[pos] == '0') {
    pos++;
  }
  if (buffer[pos] == 'b') {
    multiplier = 2;
    pos++;
  }

  while (true) {
    char c = buffer[pos];
    if ('0' <= c && c <= '9') {
      value *= multiplier;
      // 0 .. 9
      value += c - '0';
    } else {
      // the loop will stop at the first
      // non-digit
      break;
    }
    pos++;
  }
  if (invert) {
    value = 0 - value;
  }

  return value;
}

#else

bool FileLoader::load(uint8_t floor, uint8_t room) {
  return false;
}

#endif
