#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
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

task::ResourceTable generate_resource_table_from_input(std::string buffer)
{
    task::ResourceTable resource_table;
    int resource_type_count = buffer[1] - 48;

    for (int id = 1; id < (resource_type_count + 1); id++)
    {
        int unit_count = buffer[1 + id];
        task::Resource new_resource(id, unit_count);
        resource_table.add(new_resource);
    }

    resource_table.print();
    return resource_table;
}

void TaskReader::read_in_new_activities(task::TaskTable& task_table, std::vector<std::string> parsed_line)
{
    if (parsed_line[0] == "initiate")
        task_table.create_task_from_input(parsed_line);

    else if (parsed_line[0] == "request")
        task_table.add_new_request_to_task(parsed_line);

    else if (parsed_line[0] == "release")
        task_table.add_new_release_to_task(parsed_line);

    else if (parsed_line[0] == "terminate")
        task_table.add_termination_to_task(parsed_line);

}

task::TaskTable TaskReader::import_to_tasktable()
{
    task::TaskTable task_table;
    task::ResourceTable resource_table;
    int task_count;
    int resource_type_count;

    std::fstream input_file(fpath);
    if (input_file.is_open())
    {
        std::string line;
        while (std::getline(input_file, line))
        {
            std::vector<std::string> parsed_line = parse_line(line);
            if (is_fist_line(line))
            {
                std::string buffer = remove_spaces(line);

                task_count = buffer[0] - 48;
                resource_type_count = buffer[1] - 48;

                resource_table = generate_resource_table_from_input(buffer);
            }
            else
            {
                read_in_new_activities(task_table, parsed_line);
            }
        }

        input_file.close();
    }
    else
    {
        std::cout << "Cannot open file." << std::endl;
    }
    task_table.print();
    return task_table;
}

task::TaskTable TaskReader::to_tasktable()
{
    return table;
}

} // namespace io