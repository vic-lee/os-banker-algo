#ifndef HEADER_ACTIVITY
#define HEADER_ACTIVITY

#include <string>

namespace task
{
class Activity
{
  protected:
    std::string type;
    int target_id;
    int delay;

  public:
    Activity(std::string type, int target_id, int delay)
    {
        this->type = type;
        this->target_id = target_id;
        this->delay = delay;
    }
    int get_target_id();
    void print();
};
} // namespace task

#endif