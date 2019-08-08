#pragma once

#include <string>
#include <time.h>

namespace ppittawat::boggle
{

class TimeInit
{
public:
    TimeInit()
    {
        srand( time( nullptr ) );
    }
};

//get a single random boggle letter
inline char random_letter()
{
    static TimeInit t;

    return 'A' + rand() % 26;
}

/*
// test case
void test_random_letter()
{
    // exclude this
    for( auto i = 0; i < 10; ++i )
    {
        std::cout << random_letter() << std::endl;
    }
}
*/

} //end namespace ppittawat::boggle
