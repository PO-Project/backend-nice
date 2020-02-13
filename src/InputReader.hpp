#ifndef INPUTREADER_H_
#define INPUTREADER_H_

#include <string>
#include <unordered_map>

#include <ncurses.h>

namespace backends
{

class InputReader
{

public:

    inline static const std::string key_enter = "<ENTER>";
    inline static const std::string key_backspace = "<DEL>";
    inline static const std::string key_esc = "<ESC>";
    inline static const std::string key_up = "<UARROW>";
    inline static const std::string key_down = "<DARROW>";
    inline static const std::string key_left = "<LARROW>";
    inline static const std::string key_right = "<RARROW>";


    static std::string read()
    {
        static const std::unordered_map<int, std::string> keys =
        {
            {27,            key_esc},
            {10,     key_enter},
            {KEY_BACKSPACE, key_backspace},
            {KEY_UP,        key_up},
            {KEY_DOWN,      key_down},
            {KEY_LEFT,      key_left},
            {KEY_RIGHT,     key_right},

        };

        int key = getch();

        auto it = keys.find(key);
        if (it != keys.end())
            return it->second;

        return keyname(key);
    }
};

} // namespace backends
#endif

