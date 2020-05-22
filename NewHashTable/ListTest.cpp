#include "List.hpp"

int TestPushBack ()
{
    List list;
    list.PushBack(nullptr);
    list.PushBack(nullptr);
    list.PushBack(nullptr);
    list.PushBack(nullptr);
    list.PushBack(nullptr);
    list.PushBack(nullptr);
    list.PushBack(nullptr);
    list.PushBack(nullptr);
    list.Dump_();
    return 0;
}
int TestMultipleLists ()
{
    List list1, list2;
    list1.PushBack(nullptr);
    list1.PushBack(nullptr);
    list1.PushBack(nullptr);
    list1.PushBack(nullptr);
    list1.PushBack(nullptr);
    list1.PushBack(nullptr);
    list1.PushBack(nullptr);
    list1.PushBack(nullptr);
    list1.Dump_();

    list2.PushBack(nullptr);
    list2.PushBack(nullptr);
    list2.PushBack(nullptr);
    list2.PushBack(nullptr);
    list2.PushBack(nullptr);
    list2.PushBack(nullptr);
    list2.PushBack(nullptr);
    list2.PushBack(nullptr);
    list2.Dump_();

    return 0;
}

int main ()
{
    TestPushBack();
    TestMultipleLists();
    return 0;
}