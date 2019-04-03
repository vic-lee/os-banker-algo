#ifndef HEADER_TASK_READER
#define HEADER_TASK_READER

#include <string>
#include <tasktable.h>

namespace io
{
class TaskReader
{
    std::string fpath;
    task::TaskTable table;

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