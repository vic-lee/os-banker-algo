#include <string>

#include <io/taskreader.h>
#include <datastructure/tasktable.h>

namespace banker
{
int main()
{
    std::string input_fpath = "../test/input-01";
    io::TaskReader task_reader(input_fpath);
    task::TaskTable task_table = task_reader.to_tasktable();

}   
}