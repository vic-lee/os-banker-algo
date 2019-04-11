#ifndef HEADER_RESOURCE
#define HEADER_RESOURCE

#include "activity_request.h"
#include "activity_release.h"

namespace task
{

class Resource
{
    /**
     * This class encapsulates a resource's ID, total number of units, 
     * number of units currently available, and units that will be available
     * next cycle. 
     */ 

public:
    Resource(int id, int unit_count);
    int get_unit_count();
    int remaining_unit_count();

    bool can_satisfy_request(Request *request);
    bool can_satisfy_request_next_cycle(Request *request);

    bool handle_new_request(Request *request);
    void reverse_request(Request *request);
    void reverse_request(int unit_count);

    void handle_new_release(Release *release);
    void add_release_next_cycle(int unit_count);

    void clear_to_be_added_units();
    void print();

private:
    int id_;
    int total_unit_count_;
    int remaining_unit_count_;
    int units_to_be_added_next_cycle_; /* Units released but not yet available (until next cycle) */
};

} // namespace task

#endif