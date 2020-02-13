#ifndef EXAMPLETOOL_H_
#define EXAMPLETOOL_H_

#include <map>
#include <cassert>
#include <ncurses.h>
#include <panel.h>

#include "ToolBase.hpp"
#include "BackendBase.hpp"

namespace tools
{

class MyTool : public ToolBase
{
public:
    MyTool()
    {
        setEntry("IS_SAVED", "NO");
    }
    ~MyTool()
    {
        if (window != nullptr)
        {
            del_panel(panel);
            delwin(window);
        }
    }
    void setEntry(const std::string &key, const std::string &value) noexcept override
    {
        entries[key] = value;
    }
    std::string getEntry(const std::string &key) noexcept override
    {
        return entries[key];
    }
    void setCoordinates(int width, int height, int startx, int starty) noexcept override
    {
        if (window != nullptr)
        {
            del_panel(panel);
            delwin(window);
        }

        window = newwin(width, height, startx, starty);
        panel = new_panel(window);
        printText();
    }
    void setBackend(backends::BackendBase &newBackend) override
    {
        ToolBase::setBackend(newBackend);

        newBackend.bind("<EDITION>", [this]() {
            std::string temp = getEntry("KEY");

            if (temp == "<DEL>")
            {
                if (!text.empty())
                    text.pop_back();
            }
            else if (temp.length() == 1)
                text.insert(text.end(), temp.begin(), temp.end());

            printText();
        },"");

        newBackend.bind("#other#.File.BAD", [this](){ text += "open"; printText(); }, "Open file");
        newBackend.bind(".Edit.Undo", [this](){text += "\nundo"; printText();},
                        "Recovers the state before last action");
        newBackend.bind("#nice#.File.Open${Provide a file name:|FILE_NAME}", [this](){ text += "\nOPENED: "  + getEntry("FILE_NAME"); printText(); }, "Open file");
    }


private:
    std::map<std::string, std::string> entries;
    WINDOW *window = nullptr;
    PANEL *panel = nullptr;
    std::string text;

    void printText()
    {
        werase(window);
        mvwprintw(window, 0, 0, text.c_str());
    }
};

} // namespace tools
#endif
