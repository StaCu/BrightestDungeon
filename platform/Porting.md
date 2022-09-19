# Porting Brightest Dungeon

- Put required libraries in platform/\<platform\>/lib
- Put platform specific code in platform/\<platform\>/lib/Platform/src
- Put information about the platform in platform/\<platform\>/Platform.md (minimum: pin usage and required libraries)
- Add new environment to platformio.ini

## Platform Specific Code

The platform specific code must implement all functions from platform/Platform.h
The files library.json and library.properties are also required for the build tool to find the code.
Folder structure:

folder/file                              | content
-----------------------------------------|-------------
platform/                                |
\|-- lib/                                | common libraries
\|-- \<platform\>/                       |
\|-- \|-- lib/                           | platform specific libraries
\|-- \|-- \|-- lib1/                     | required libraries
\|-- \|-- \|-- Platform/                 | 
\|-- \|-- \|-- \|-- src/                 | platform specific code
\|-- \|-- \|-- \|-- \|-- Platform.h      |
\|-- \|-- \|-- \|-- \|-- Platform.cpp    |
\|-- \|-- \|-- \|-- \|-- file1           |
\|-- \|-- \|-- \|-- library.json         | required by PlatformIO
\|-- \|-- \|-- \|-- library.properties   | required by ArduinoIDE
\|-- \|-- pcb/                           | PCB files
\|-- \|-- Platform.md                    | information about the platform
\|-- Platform.h                          | template for platform specific code
\|-- Porting.md                          | how to port to a new platform
platformio.ini                           | build tool instructions

See platform/simulator/ or platform/atmega328p for a simple example.
