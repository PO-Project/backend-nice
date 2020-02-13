//
// Created by bartek on 18.01.2020.
//

#include <functional>
#include <ncurses.h>
#include "BackendNice.h"
#include "ToolBase.hpp"
#include "Window.hpp"
#include "InputReader.hpp"
#include "MenuWindow.hpp"

backends::BackendNice::BackendNice(){
    actual_window = Frontend;
    commands = Commands();
    initscr();
    cbreak();
    keypad(stdscr, true);
    noecho();
    set_escdelay(10);
    window_menu = new MenuWindow();
    window_help = new Window();
    window_menu->add_field("File");
    window_help->hide();
    window_menu->render();
};

void backends::BackendNice::bind(const std::string &s_command, std::function<void()> callback,
                                 const std::string &helpMessage) noexcept {
    std::string command = s_command;
    if(command[0] == '#'){
        std::string prefix = "#nice#";
        unsigned long prefix_n = command.find(prefix);
        if (prefix_n == std::string::npos)
            return;

        command = command.substr(prefix_n + prefix.size());
    }
    
    if(command[0] == '.') {
        command = command.substr(1);
    }
    
//    "#nice#.File.Open${Provide a file name:|FILE_NAME}"

    unsigned long dollar_pos = command.find("${");
    callback = [this, callback](){
        actual_window = Frontend;
        callback();
    };
    if(dollar_pos != std::string::npos){

        std::string text = command.substr(dollar_pos + 2, command.find('|') - (dollar_pos + 2));
        std::string key = command.substr(command.find('|') + 1, command.find('}') - (command.find('|') + 1));


        command = command.substr(0, dollar_pos);
        callback = [this, callback, key, text](){
            window_help->print(text);
            window_help->top_window();
            window_help->show();
            Window::update();
            std::string input;

            std::string input_key;
            while(true) {
                input_key = InputReader::read();
                if(input_key == "<BACKSPACE>"){
                    if(input.length())
                        input.pop_back();
                }else if (input_key.length() == 1){
                    input += input_key;
                }else{
                    actual_window = Frontend;
                    render();
                    if (input_key == InputReader::key_enter)
                        break;
                    else
                        return;
                }
                window_help->clear();
//                window_help->print();
                window_help->print(text + " " +input);
                Window::update();
            }

            tool->setEntry(key, input);
            callback();
        };
    }

    this->commands.put_command(command, callback, helpMessage);
    if(command.find('.') != std::string::npos)
        window_menu->add_option(command, callback);

}

void backends::BackendNice::operator()() {
    this->run();
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void backends::BackendNice::run() {

    tool->setCoordinates(0,0,1,0);
    window_menu->hide_menu();

    window_menu->add_option("File.Help", [this](){
        actual_window = Help;
    });
    window_menu->add_option("File.Quit", [this](){
        if(tool->getEntry("IS_SAVED") == "NO"){
            window_help->clear();
            window_help->print("File unsaved!");
            window_help->top_window();
            Window::update();
            InputReader::read();
            actual_window = Frontend;
            render();
        }
        else
            exit(0);
    });

    while (true)
    {
        render();
        Window::update();

        std::string key = InputReader::read();

        if(actual_window == Help){
            actual_window = Frontend;
            continue;
        }

        if(key == "<ESC>"){
            window_menu->toggle_menu();
            window_menu->render();
        }
        if(window_menu->menu_open) {
            actual_window = Menu;
        } else
            actual_window = Frontend;
        if(window_menu->menu_open){
            window_menu->navigate_menu(key);
        }else{
            tool->setEntry("KEY", key);
            commands.run("<EDITION>");
        }



    }
}
void backends::BackendNice::render() {

    if(actual_window == Menu){
        window_menu->top_window();
    }
    else if (actual_window == Frontend){
        window_menu->bottom_window();
        window_help->hide();
        window_help->bottom_window();
    }
    else if (actual_window == Help){
        window_help->print("Press any key to close help...");

        window_help->print(commands.get_commands(), 1, 0);

        window_help->top_window();
    }
}
