#ifndef COMMAND_H_
#define COMMAND_H_

#include <functional>
#include <string>
#include <utility>
#include <algorithm>

namespace backends
{

    struct Command{
        Command(const std::string &command, std::function<void ()> callback, const std::string &helpMessage){
            this->function = std::move(callback);
            this->command = command;
            this->help = helpMessage;
        }

        std::function<void()> function;
        std::string command;
        std::string help;
    };

    class Commands
    {
    protected:
        std::vector<Command*> commands;

    public:
        void put_command(const std::string &command, std::function<void ()> callback, const std::string &helpMessage){
            commands.push_back(new Command(command, std::move(callback), helpMessage));
        }

        void run(const std::string &command_to_run){
            for(auto* command : commands){
                if(command->command == command_to_run){
                    command->function();
                }
            }
        }

        std::string get_commands(){
            std::string commands_string = "";

            for(auto* command: commands){
                if(command->command[0] != '<')
                    commands_string += command->command + " - " + command->help + "\n";
            }

            return commands_string;
        }
};



} // namespace
#endif
