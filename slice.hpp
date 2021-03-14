#include <iostream>
#include <string>
#include <regex>
#include <limits>

class slice {
  static std::string _r_int, _r_colon_int;
  static std::regex re;
  
public:
  int start;
  int stop;
  int step;

  slice(int start_, int stop_, int step_)
    : start(start_), stop(stop_), step(step_) {}
  slice(int idx)
    : slice(idx, idx+1, 0) {}
  slice(std::string str) { // ":"とか"1:-1"とか
    std::smatch m;
    if (std::regex_match(str, m, re)) {
      start = (m[1].str().empty()) ? 0 : std::stoi(m[1]);
      stop = (m[3].str().empty()) ? std::numeric_limits<int>::max() : std::stoi(m[3]);
      step = (m[5].str().empty()) ? 1 : std::stoi(m[5]);
    }
  }
};

std::string slice::_r_int = "[+-]?[1-9]\\d*|[+-]?0";
std::string slice::_r_colon_int = ":(" + slice::_r_int + ")?";
std::regex slice::re("(" + slice::_r_int + ")?(" + slice::_r_colon_int + ")?(" + slice::_r_colon_int + ")?");
