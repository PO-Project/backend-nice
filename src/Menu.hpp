#ifndef MENU_H_
#define MENU_H_

#include <string>
#include <ncurses.h>
#include <panel.h>
#include <map>
#include <utility>
#include <vector>
#include <functional>
#include <iostream>

#include "Window.hpp"
#define QUIT "Quit"
#define HELP "Help"
namespace backends
{

    typedef std::pair<std::string, std::function<void()>> MenuItem;
    typedef std::vector<MenuItem> MenuItems;
    typedef std::pair<std::string, MenuItems> MenuField;

    typedef std::vector<MenuField> MenuFields;

    typedef std::vector<std::string> VectorString;

    class Menu
{
    int selected_item;
    MenuFields menu;
    
    int selected_menu = 0;
    public:
        int getSelectedItem() {
            return selected_item;
        }

        int getSelectedMenu() {
            return selected_menu;
        }

    Menu(){
        selected_item = 0;
        selected_menu = 0;
    };

    void add_option(std::string command, const std::function<void()>& func){
        std::string s = std::move(command);
        std::string delimiter = ".";

        size_t pos = 0;
        pos=s.find(delimiter);
        std::string menu_name = s.substr(0, pos);

        s.erase(0, pos + delimiter.length());

        std::string menu_item = s;


        auto* new_item = new MenuItem;
        new_item->first = menu_item;
        new_item->second = func;

        for( auto& menu_field : menu){
            if(menu_field.first == menu_name){

                menu_field.second.push_back(*new_item);
                return;
            }
        }
        auto* field = new MenuField;

        field->first = menu_name;
        field->second.push_back(*new_item);
        menu.push_back(*field);
    }

    void clear(){
        selected_item = 0;
        selected_menu = 0;
    }

    void update_selecteds() {
        int menu_size = get_menu_fields().size();
        if(selected_menu < 0) selected_menu = menu_size-1;
        else if (selected_menu >= menu_size) selected_menu = 0;

        int items_size = get_menu_items().size();

        if(selected_item < 0) selected_item = items_size-1;
        else if (selected_item >= items_size) selected_item = 0;
    }

    void navigate(int direction){
        if(direction == 0){//left
            selected_menu--;
        }else{//rigth
            selected_menu++;
        }
        update_selecteds();
    }

    void navigate_menu(int direction){
        if(direction == 0) selected_item--;//up
        else selected_item++;//down

        update_selecteds();
    }

    VectorString get_menu_items(int n_menu=-1){
        VectorString vector;
        for(auto& el: menu[n_menu > -1 ? n_menu : selected_menu].second){
            vector.push_back(el.first);
        }
        return vector;
    }

    VectorString get_menu_fields(){
        VectorString vector;
        for(auto& menu_field : menu){
            vector.push_back(menu_field.first);
        }
        return vector;
    }

    void exec(){
        menu[selected_menu].second[selected_item].second();
    }

    void add_field(std::string field) {
        auto* menu_field = new MenuField;
        menu_field->first = field;
        menu.push_back(*menu_field);
    }
};



} // namespace backends
#endif

