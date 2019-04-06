#ifndef HEADER_TASK_READER
#define HEADER_TASK_READER

#include <string>
#include <tuple>
#include "../ds/tasktable.h"
#include "../ds/resourcetable.h"

namespace io
{
class TaskReader
{
    std::string fpath;
    task::TaskTable task_table;
    task::ResourceTable resource_table;

    std::string remove_spaces(std::string str);
    std::vector<std::string> parse_line(std::string line);
    int is_fist_line(std::string line);
    void read_in_new_activities(task::TaskTable& task_table, std::vector<std::string> parsed_line);
    task::ResourceTable read_in_resources(std::vector<std::string> parsed_line);
    std::tuple<task::TaskTable, task::ResourceTable> import();

  public:
    TaskReader(std::string fp)
    {
        fpath = fp;
        std::tie(task_table, resource_table) = import();
    }
    task::TaskTable to_tasktable();
    task::ResourceTable to_resourcetable();
};
} // namespace io

#endif