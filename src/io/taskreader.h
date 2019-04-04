#ifndef HEADER_TASK_READER
#define HEADER_TASK_READER

#include <string>
#include "../ds/tasktable.h"

namespace io
{
class TaskReader
{
    std::string fpath;
    task::TaskTable table;

    std::string remove_spaces(std::string str);
    std::vector<std::string> parse_line(std::string line);
    int is_fist_line(std::string line);
    void read_in_new_activities(task::TaskTable& task_table, std::vector<std::string> parsed_line);
    task::ResourceTable read_in_resource_table(std::string line);
    task::TaskTable import_to_tasktable();

  public:
    task::TaskTable to_tasktable();

    TaskReader(std::string fp)
    {
        fpath = fp;
        table = import_to_tasktable();
    }
};
} // namespace io

#endif