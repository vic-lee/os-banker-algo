#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <tuple>
#include <vector>
#include <algorithm>

#include "taskreader.h"
#include "../ds/claim.h"
#include "../ds/tasktable.h"
#include "../ds/resourcetable.h"
#include "../ds/activity_request.h"
#include "../ds/activity_release.h"
#include "../ds/activity_terminate.h"

namespace io
{

int str_to_int(std::string s)
{
    return atoi(s.c_str());
}

std::string TaskReader::remove_spaces(std::string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

std::vector<std::string> TaskReader::parse_line(std::string line)
{
    std::vector<std::string> parsed_line;
    std::stringstream stream(line);
    std::string word;

    while (stream >> word)
    {
        parsed_line.push_back(word);
    }

    return parsed_line;
}

int TaskReader::is_fist_line(std::string line)
{
    return isdigit(line[0]);
}

task::ResourceTable TaskReader::read_in_resources(std::vector<std::string> parsed_line)
{
    task::ResourceTable resource_table;
    int resource_type_count = str_to_int(parsed_line[1]);

    for (int id = 1; id < (resource_type_count + 1); id++)
    {
        int unit_count = str_to_int(parsed_line[1 + id]);
        task::Resource* resource = new task::Resource(id, unit_count);
        resource_table.add(resource);
    }

    resource_table.print();
    return resource_table;
}

void TaskReader::read_in_new_activities(task::TaskTable &task_table, std::vector<std::string> parsed_line)
{
    if (parsed_line[0] == "initiate")
        task_table.handle_new_initiate(parsed_line);

    else if (parsed_line[0] == "request")
        task_table.add_new_request_to_task(parsed_line);

    else if (parsed_line[0] == "release")
        task_table.add_new_release_to_task(parsed_line);

    else if (parsed_line[0] == "terminate")
        task_table.add_termination_to_task(parsed_line);
}

std::tuple<task::TaskTable, task::ResourceTable> TaskReader::import()
{
    task::TaskTable task_table;
    task::ResourceTable resource_table;

    std::fstream input_file(fpath);
    if (input_file.is_open())
    {
        std::string line;

        while (std::getline(input_file, line))
        {
            std::vector<std::string> parsed_line = parse_line(line);

            if (parsed_line.size() == 0)
                continue;

            if (is_fist_line(line))
                resource_table = read_in_resources(parsed_line);
            else
                read_in_new_activities(task_table, parsed_line);
        }

        input_file.close();
    }
    else
    {
        std::cout << "Cannot open file." << std::endl;
    }
    task_table.print();
    return std::make_tuple(task_table, resource_table);
}

task::TaskTable TaskReader::to_tasktable()
{
    return task_table;
}

task::ResourceTable TaskReader::to_resourcetable()
{
    return resource_table;
}

} // namespace io