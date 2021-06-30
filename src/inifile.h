#pragma once

class IniFile {
public:
    static void load(const char *filename);
    static void save(const char *filename);
};
