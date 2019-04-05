#include <iostream>
#include "task.h"
#include "activity.h"

namespace task
{


int Task::set_latest_activity()
{
    for (int i = 0; i < activities_table.size(); i++)
    {
        if (activities_table[i].is_active())
            return i;
    }
    return -1;
}

void Task::add_new_activity(Activity activity)
{
    activities_table.push_back(activity);
}

int Task::get_id()
{
    return id;
}

bool Task::is_terminated()
{
    return terminated;
}

void Task::print()
{
    std::cout << std::endl;

    std::cout << "Task ID: " << id << std::endl;

    std::cout << "---------------- Claims ---------------" << std::endl;
    std::map<int, Claim>::iterator it;
    for (it = claims_table.begin(); it != claims_table.end(); it++)
    {
        it->second.print();
    }

    std::cout << "------------- Activities --------------" << std::endl;
    for (int i = 0; i < activities_table.size(); i++)
    {
        activities_table[i].print();
    }
}

} // namespace task