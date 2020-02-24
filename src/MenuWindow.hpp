#ifndef STATUSWINDOW_H_
#define STATUSWINDOW_H_

#include <string>
#include <ncurses.h>
#include <panel.h>
#include <map>
#include <utility>

#include "Menu.hpp"
#include "InputReader.hpp"

namespace backends{

    class MenuWindow : public Window{
    Menu menu;
    bool is_menu_choose = false;
public:
    bool menu_open = false;

    MenuWindow(): Window(){};
    ~MenuWindow(){
        del_panel(panel);
        delwin(window);
    }

    void toggle_menu(){
        menu_open = !menu_open;
    }

    void render(){
        int width = (int) menu.get_menu_fields().size() * 15;
        if (menu_open){
            show_menu();
            resize(width, 15);
        }
        else{
            hide_menu();
            resize(width, 1);
            bottom_window();
        }
    }

    void show_menu(){
        clear();
        top_window();
        menu_open = true;
        const VectorString &fields = menu.get_menu_fields();
        for(int i = 0,j = 0; i < fields.size(); i++, j+=12){
            print(fields[i], 0, j);
            if(i == menu.getSelectedMenu()){
                const VectorString &items = menu.get_menu_items(i);
                for(int iy = 0; iy < items.size(); iy++){
                    print(items[iy], iy+1, j);
                }
            }
        }

    }

    void hide_menu(){
        clear();
        is_menu_choose = false;
        menu.clear();
        menu_open = false;
        const VectorString &fields = menu.get_menu_fields();
        for(int i = 0,j = 0; i < fields.size(); i++){
            print(fields[i], 0, i*12);
        }
    }

    void navigate_menu(const std::string& key){
        if(!is_menu_choose){
            if(key == InputReader::key_left){
                menu.navigate(0);
            } else if (key == InputReader::key_right){
                menu.navigate(1);
            }
            else if (key == InputReader::key_enter || key == InputReader::key_down){
                is_menu_choose = true;
            }
            show_menu();
        }
        else{
            if(key == InputReader::key_up){
                if(menu.getSelectedItem() == 0)
                    is_menu_choose = false;
                else
                    menu.navigate_menu(0);
            } else if (key == InputReader::key_down){
                menu.navigate_menu(1);
            } else if (key == InputReader::key_enter){
                menu.exec();
                hide_menu();
            } else if (key == InputReader::key_backspace){
                is_menu_choose = false;
            }
        }
        moveCursor(is_menu_choose ? menu.getSelectedItem() + 1 : 0, menu.getSelectedMenu()*12);
    }

    void add_option(std::string command, const std::function<void()>& func){
        menu.add_option(std::move(command), func);
    }

    void add_field(std::string field) {
        menu.add_field(field);

    }
};



} // namespace backends
#endif

