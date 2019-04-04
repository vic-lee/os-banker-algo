#ifndef HEADER_ACTIVITY
#define HEADER_ACTIVITY

namespace task
{
class Activity
{
    std::string type;
    int target_id;
    int delay;

  public:
    Activity(std::string type, int target_id, int delay)
    {
        this->target_id = target_id;
        this->type = type;
        this->delay = delay;
    }
    
    int get_target_id();
};
} // namespace task

#endif