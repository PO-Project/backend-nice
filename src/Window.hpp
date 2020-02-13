#ifndef CURSESWINDOW_H_
#define CURSESWINDOW_H_

#include <string>
#include <ncurses.h>
#include <panel.h>

namespace backends
{

class Window
{


protected:

public:
    PANEL* panel;

    WINDOW* window;
    Window(int linesC, int colsC, int posY, int posX)
    {
        window = newwin(linesC, colsC, posY, posX);
        panel= new_panel(window);

    }
    Window() : Window(0, 0, 0, 0) {};
    ~Window()
    {
        del_panel(panel);
        delwin(window);
    }

    static void update()
    {
        update_panels();
        doupdate();
    }

    void print(const std::string &s)
    {
        top_panel(panel);
        wprintw(window, s.c_str());
    }
    void print(const std::string &s, int y, int x)
    {
        mvwprintw(window, y, x, s.c_str());
    }
    void moveCursor(int y, int x)
    {
        wmove(window, y, x);
    }
    void clear()
    {
        werase(window);
    }

    void resize(int x, int y)
    {
        wresize(window, y, x);
        replace_panel(panel, window);
    }

    void hide()
    {
        hide_panel(panel);
        clear();
    }

    void show(){
        show_panel(panel);
    }

    void top_window()
    {
        show();
        top_panel(panel);
    }

    void bottom_window()
    {
        show();
        bottom_panel(panel);
    }
};



} // namespace backends
#endif

