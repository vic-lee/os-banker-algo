#include <fstream>

#include <taskreader.h>
#include <../datastructure/tasktable.h>

namespace io
{

task::TaskTable TaskReader::import_to_tasktable()
{

}

task::TaskTable TaskReader::to_tasktable()
{
    return table;
}

}