#include <string>
#include <iostream>

#include "io/taskreader.h"
#include "ds/tasktable.h"

namespace banker
{

}

int main(int argc, char **argv)
{
    std::string input_fpath = "test/input-01";
    io::TaskReader task_reader(input_fpath);
    task::TaskTable task_table = task_reader.to_tasktable();

    return 0;
}   
