#ifndef HEADER_RESOURCE
#define HEADER_RESOURCE

namespace task
{

class Resource
{
    int id;
    int unit_count;

  public:
    int get_unit_count();
    void print();
    Resource(int id, int unit_count)
    {
        this->id = id;
        this->unit_count = unit_count;
    }
};

} // namespace task

#endif