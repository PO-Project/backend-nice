
#include "MyTool.hpp"
#include "BackendNice.h"

int main()
{
    tools::MyTool tool;
    backends::BackendNice backend;
    backend.setTool(tool);
    tool.setBackend(backend);
    backend();
}
