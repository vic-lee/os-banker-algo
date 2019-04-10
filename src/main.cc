#include <string>
#include <iostream>
#include <cctype>

#include "io/taskreader.h"
#include "ds/tasktable.h"
#include "manager/banker.h"
#include "manager/optimisticmanager.h"

namespace banker
{

std::tuple<std::string, bool> read_cmd_arg(int argc, char **argv)
{
    std::string input_fpath = "test/input-01";
    bool debug = false;

    if (argc > 1)
    {
        std::string input = argv[1];
        if (input.find("test") != std::string::npos)
        {
            input_fpath = input;
        }
        else
        {
            if (input.length() == 1)
                input = "0" + input;
            input_fpath = "test/input-" + input;
        }

        if (argc > 2)
        {
            std::string debug_mode = argv[2];
            if (debug_mode == "--debug")
                debug = true;
        }
    }

    return std::make_tuple(input_fpath, debug);
}

void do_optimistic_manager(int argc, char **argv)
{
    std::string input_fpath;
    bool debug = false;
    std::tie(input_fpath, debug) = banker::read_cmd_arg(argc, argv);

    io::TaskReader task_reader(input_fpath);
    task::TaskTable task_table = task_reader.to_tasktable();
    task::ResourceTable resource_table = task_reader.to_resourcetable();

    // manager::OptimisticManager optimistic_manager(task_table, resource_table);
    manager::OptimisticManager *optimistic_manager = new manager::OptimisticManager(task_table, resource_table, debug);
    optimistic_manager->do_tasks();
    optimistic_manager->print();

    delete optimistic_manager;
}

void do_banker(int argc, char **argv)
{
    std::string input_fpath;
    bool debug = false;
    std::tie(input_fpath, debug) = banker::read_cmd_arg(argc, argv);    
    io::TaskReader task_reader(input_fpath);
    task::TaskTable task_table = task_reader.to_tasktable();
    task::ResourceTable resource_table = task_reader.to_resourcetable();

    // manager::Banker banker(task_table, resource_table);
    manager::Banker *banker = new manager::Banker(task_table, resource_table, debug);
    banker->do_tasks();
    banker->print();

    delete banker;
}

} // namespace banker

int main(int argc, char **argv)
{
    banker::do_optimistic_manager(argc, argv);
    banker::do_banker(argc, argv);

    return 0;
}
