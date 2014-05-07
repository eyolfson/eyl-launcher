// Copyright 2014 Jon Eyolfson
//
// This file is distributed under the GPLv3 license
//
// http://standards.freedesktop.org/desktop-entry-spec/latest/

#include "lnchr/xdg.hpp"

#include <dirent.h>
#include <fstream>
#include <map>
#include <sstream>

namespace {

class dir {
public:
    dir(std::string const & path)
    {
        ptr = opendir(path.c_str());
    }
    ~dir()
    {
        if (ptr) {
	    closedir(ptr);
	}
    }
    struct dirent * read() {
        return readdir(ptr);
    }
private:
    DIR * ptr;
};

std::string application_key(char const * name)
{
    char const * cur = name;
    while (*cur != '\0') {
        ++cur;
    }
    if ((*(--cur) == 'p') &&
        (*(--cur) == 'o') &&
	(*(--cur) == 't') &&
	(*(--cur) == 'k') &&
	(*(--cur) == 's') &&
	(*(--cur) == 'e') &&
	(*(--cur) == 'd') &&
	(*(--cur) == '.')) {
        return std::string(name, cur - name);
    }
    return std::string();
}

}

lnchr::applications::applications()
{
    const std::string xdg_data_dirs = "/usr/share";
    std::stringstream ss;
    ss << xdg_data_dirs << "/applications";
    const std::string applications_path = ss.str();
    dir applications_dir(applications_path);
    struct dirent * de;
    while ((de = applications_dir.read())) {
        ss.str("");
        ss << applications_path << '/' << de->d_name;
	const std::string path = ss.str();
	std::string key = application_key(de->d_name);
	if (key.empty()) {
	    continue;
	}
	std::string line;
	std::ifstream file(path);
	std::string name;
	if (file.is_open()) {
            bool is_desktop_entry = false;
	    std::string name_key = "Name=";
	    while (getline(file, line)) {
	        if (line.empty()) {
		    continue;
		}
		if (line[0] == '[') {
		    if (line == "[Desktop Entry]") {
		        is_desktop_entry = true;
		    }
		    else {
		        is_desktop_entry = false;
		    }
		}
		else if (line[0] != '#' && is_desktop_entry) {
		    if (line.substr(0, name_key.length()) == name_key) {
		        name = line.substr(name_key.length());
		    }
		}
	    }
	}
	if (!name.empty()) {
	    map.insert(std::make_pair(key, application_info()));
	}
    }
}

lnchr::applications::map_t const & lnchr::applications::data() const
{
    return map;
}
