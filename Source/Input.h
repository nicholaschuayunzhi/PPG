#pragma once
#include "stdafx.h"

using Input = unsigned char;

namespace Key
{
    const unsigned char NONE  = 0b0000'0000;
    const unsigned char UP    = 0b0000'0001;
    const unsigned char DOWN  = 0b0000'0010;
    const unsigned char LEFT  = 0b0000'0100;
    const unsigned char RIGHT = 0b0000'1000;
};
