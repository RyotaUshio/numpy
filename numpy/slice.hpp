#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <limits>
#include <stdexcept>

namespace python {

  class slice
  {
    
    static std::string _r_int, _r_colon_int;
    static std::regex re;
    
  public:
    int start;
    int stop;
    int step;

  private:
    bool stop_is_None;

  public:
    constexpr slice(int start, int stop, int step=1)
      : start(start), stop(stop), step(step), stop_is_None(false) {}
    
    constexpr explicit slice(int stop)
      : slice(0, stop, 1) {}
    
    slice(const char* str) { // ":"とか"1:-1"とか
      std::cmatch m;
      if (std::regex_match(str, m, re)) {
	bool start_empty = m[1].str().empty();
	bool stop_empty = m[3].str().empty();
	bool step_empty = m[5].str().empty();

	if (stop_empty and step_empty and (not start_empty))
	  throw std::invalid_argument("It is forbidden to express a non-slice index (i.e. a mere integer) by a string in order to eliminate ambiguity; use an integer type instead.");
	
	// check step first!!
	step = (step_empty) ? 1 : std::stoi(m[5]);
	start = (start_empty) ? ( (step < 0) ? -1: 0) : std::stoi(m[1]);
	if (stop_empty) {
	  stop_is_None = true;
	  stop = std::numeric_limits<int>::max();
	} else {
	  stop_is_None = false;
	  stop = std::stoi(m[3]);
	}

      }
      else
	throw std::invalid_argument("SyntaxError: invalid syntax while initializing slice: \"" + std::string(str) + "\"");
    }
    
    slice(const std::string& str)
      : slice(str.c_str()) {}

    int size(int length) const {
      int ret =  (abs_stop(length) - abs_start(length)) / step;
      return (ret > 0) ? ret : 0;
    }

    inline int abs_start(int length) const {
      return (start < 0) ? length + start : start;
    }

    static int abs_index(int length, int index)  {
      return (index < 0) ? length + index : index;
    }

    inline int abs_stop(int length) const {
      if (stop_is_None)
	return (step > 0) ? length : -1;
      return (stop < 0) ? length + stop : stop;
    }
 
    std::string __repr__() const {
      std::stringstream ss;
      ss << "slice(" << start << ", ";
      if (stop_is_None)
	ss << "None, ";
      else
	ss << stop << ", ";
      ss << step << ")";
      return ss.str();
    }

    std::string __str__() const {
      return __repr__();
    }

  };

  std::string slice::_r_int = "[+-]?[1-9]\\d*|[+-]?0*";
  std::string slice::_r_colon_int = ":(" + slice::_r_int + ")?";
  std::regex slice::re("(" + slice::_r_int + ")?(" + slice::_r_colon_int + ")?(" + slice::_r_colon_int + ")?");

}
