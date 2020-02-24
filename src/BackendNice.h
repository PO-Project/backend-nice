//
// Created by bartek on 18.01.2020.
//

#ifndef BACKEND_NICE_BACKENDNICE_H
#define BACKEND_NICE_BACKENDNICE_H

#include <utility>

#include "BackendBase.hpp"
#include "MenuWindow.hpp"
#include "Commands.hpp"

namespace backends {

class BackendNice : public BackendBase
{
    enum ActualWindow { Frontend, Menu, Help, Popup };

    Commands commands;
    Window *window_help;
    MenuWindow* window_menu;
    ActualWindow actual_window = Frontend;
    int quit = 0;
public:
    BackendNice();
    ~BackendNice();
    void bind(const std::string &command, std::function<void ()> callback, const std::string &helpMessage) noexcept override;
    void operator()() override;
    void run();


    void render();


};

}

#endif //BACKEND_NICE_BACKENDNICE_H
