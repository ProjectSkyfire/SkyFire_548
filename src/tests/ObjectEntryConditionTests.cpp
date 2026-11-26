#include "ObjectEntryCondition.h"
#include <iostream>

int main()
{
    using Skyfire::MatchesObjectEntry;
    if (!MatchesObjectEntry(100, 10, 100, 0) ||
        !MatchesObjectEntry(100, 11, 100, 0) ||
        MatchesObjectEntry(101, 10, 100, 0) ||
        !MatchesObjectEntry(100, 10, 100, 10) ||
        MatchesObjectEntry(100, 11, 100, 10) ||
        MatchesObjectEntry(101, 10, 100, 10) ||
        !MatchesObjectEntry(100, 10, 0, 10) ||
        MatchesObjectEntry(100, 0, 0, 10) ||
        !MatchesObjectEntry(100, 0, 0, 0))
    {
        std::cerr << "Object entry/spawn condition matching failed\n";
        return 1;
    }
    return 0;
}
