#include <string>
#include <iostream>
#include <cctype>

#include "io/taskreader.h"
#include "ds/tasktable.h"

namespace banker
{

std::string read_cmd_arg(int argc, char **argv)
{
    std::string input_fpath = "test/input-01";

    if (argc > 1)
    {
        std::string input = argv[1];
        if (input.find("test") != std::string::npos)
            input_fpath = input;
        else
            input_fpath = "test/input-0" + input;
    }

    return input_fpath;
}

} // namespace banker

int main(int argc, char **argv)
{
    std::string input_fpath = banker::read_cmd_arg(argc, argv);
    io::TaskReader task_reader(input_fpath);
    task::TaskTable task_table = task_reader.to_tasktable();

    return 0;
}
