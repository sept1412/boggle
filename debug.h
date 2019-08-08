#pragma once

#undef DEBUG

#ifdef DEBUG
#include <iostream>
auto& dlog = std::cout;
#else
#include <ostream>
class DummyLog{};

template<typename T> DummyLog& operator << ( DummyLog& l, T )
{
    return l;
}

DummyLog& operator << ( DummyLog& l, std::ostream& (*)( std::ostream& ) )
{
    return l;
} 

template<class ManipData>
DummyLog& operator << ( DummyLog& osf , std::ostream& (*)(std::ostream&, ManipData )  )
 {
   return osf ;
 }

static DummyLog dlog;

#endif
