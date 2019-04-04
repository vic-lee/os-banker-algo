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

    int is_fist_line(std::string line);
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