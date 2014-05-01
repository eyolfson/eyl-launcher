/*
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __LNCHR_VERSION_HPP_
#define __LNCHR_VERSION_HPP_

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
