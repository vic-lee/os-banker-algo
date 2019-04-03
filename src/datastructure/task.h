#include <string>
#include <map>
#include <vector> 

#include <resource.h>
#include <activity.h>

namespace task
{
class Task
{
    std::string id;
    std::map<int, Resource> claims_table;
    std::vector<Activity> activities_table;
};
} // namespace Task