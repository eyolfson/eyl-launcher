// Copyright 2014 Jon Eyolfson
//
// This file is distributed under the GPLv3 license

#ifndef __LNCHR_VERSION_HPP__
#define __LNCHR_VERSION_HPP__

#include <cstdint>
#include <ostream>
#include <string>

namespace lnchr {

class version {
public:
    std::uint8_t const major;
    std::uint8_t const minor;
    std::uint16_t const patch;
    std::string const extra;
};

extern const version VERSION;

std::ostream & operator<<(std::ostream & os, version const & v);

}

#endif
