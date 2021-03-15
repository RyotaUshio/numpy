#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <limits>
#include <stdexcept>

namespace numpy {
  
  class slice {
    static std::string _r_int, _r_colon_int;
    static std::regex re;

    bool stop_is_None;
  public:
    int start;
    int stop;
    int step;

    slice(int start, int stop, int step=1)
      : start(start), stop(stop), step(step) {}
    explicit slice(int stop)
      : slice(0, stop, 1) {}
    slice(std::string str) { // ":"とか"1:-1"とか
      std::smatch m;
      if (std::regex_match(str, m, re)) {
	start = (m[1].str().empty()) ? 0 : std::stoi(m[1]);
	if (m[3].str().empty()) {
	  stop_is_None = true;
	  stop = std::numeric_limits<int>::max();
	} else {
	  stop_is_None = false;
	  stop = std::stoi(m[3]);
	}
	step = (m[5].str().empty()) ? 1 : std::stoi(m[5]);
      }
      else
	throw std::invalid_argument("Broken slice \"" + str + "\"");
    }
    slice(const char* str)
      : slice(std::string(str)) {}

    int size(int length) const {
      int ret =  (abs_stop(length) - abs_start(length)) / step;
      return (ret > 0) ? ret : 0;
    }

    int abs_start(int length) const {
      return (start < 0) ? length + start : start;
    }

    int abs_stop(int length) const {
      if (stop_is_None)
	return length;
      return (stop < 0) ? length + stop : stop;
    }

    // for debug
    std::string str() const {
      std::stringstream ss;
      ss << start << ":";
      if (stop_is_None)
	ss << "None:";
      else
	ss << stop << ":";
      ss << step << std::endl;
      return ss.str();
    }
    void print() const {
      std::cout << str() << std::endl;
    }
  };

  std::string slice::_r_int = "[+-]?[1-9]\\d*|[+-]?0";
  std::string slice::_r_colon_int = ":(" + slice::_r_int + ")?";
  std::regex slice::re("(" + slice::_r_int + ")?(" + slice::_r_colon_int + ")?(" + slice::_r_colon_int + ")?");
  
}
