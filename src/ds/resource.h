#ifndef HEADER_RESOURCE
#define HEADER_RESOURCE

#include "activity_request.h"

namespace task
{

class Resource
{
    int id;
    int total_unit_count;
    int remaining_unit_count;

  public:
    int get_unit_count();
    bool can_satisfy_request(Request *request);
    void handle_new_request(Request *request);
    void print();
    Resource(int id, int unit_count)
    {
        this->id = id;
        this->total_unit_count = unit_count;
        this->remaining_unit_count = unit_count;
    }
};

} // namespace task

#endif