// Copyright 2014 Jon Eyolfson
//
// This file is distributed under the GPLv3 license

#include "lnchr/xdg.hpp"

#include <iostream>

using namespace lnchr;

int main(int argc, char const * const * argv)
{
    applications apps;
    for (auto i : apps.data()) {
        std::cout << i.first << '\n';
    }
    return 0;
}
