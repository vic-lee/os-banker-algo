#include "task.h"
#include "activity.h"

namespace task
{

void Task::add_new_activity(Activity activity)
{
    activities_table.push_back(activity);
}

int Task::get_id()
{
    return id;
}

}