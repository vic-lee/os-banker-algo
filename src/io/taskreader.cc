#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> 

#include "taskreader.h"
#include "../ds/tasktable.h"
#include "../ds/resourcetable.h"

namespace io
{

std::string remove_spaces(std::string str)  
{ 
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); 
    return str; 
} 

int TaskReader::is_fist_line(std::string line)
{
    return isdigit(line[0]);
}


task::TaskTable TaskReader::import_to_tasktable()
{
    task::TaskTable task_table;
    task::ResourceTable resource_table;

    std::fstream input_file(fpath);
    if (input_file.is_open())
    {
        std::string line;
        while (std::getline(input_file, line))
        {
            if (is_fist_line(line))
            {
                std::string buffer = remove_spaces(line);
                // int first_line_len = buffer.length();

                // int task_count = buffer[0];
                int resource_type_count = buffer[1] - 48;

                for (int id = 1; id < (resource_type_count + 1); id++)
                {
                    int unit_count = buffer[1+id];
                    task::Resource new_resource(id, unit_count);
                    resource_table.add(new_resource); 
                }

                resource_table.print();
            }
            else
            {

            }
        }

        input_file.close();
    }
    else 
    {
        std::cout << "Cannot open file." << std::endl;
    }
    return task_table;
}


task::TaskTable TaskReader::to_tasktable()
{
    return table;
}

}