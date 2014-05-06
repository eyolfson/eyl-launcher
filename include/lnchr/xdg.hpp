// Copyright 2014 Jon Eyolfson
//
// This file is distributed under the GPLv3 license

#ifndef __LNCHR_XDG_HPP__
#define __LNCHR_XDG_HPP__

#include <map>
#include <string>

namespace lnchr {

class application_info {
public:
  std::string name;
  std::string icon_path;
  std::string exec_path;
};

class applications {
public:
    typedef std::map<std::string, application_info> map_t;
    applications();
    map_t const & data() const;
private:
    map_t map;
};

}

#endif
