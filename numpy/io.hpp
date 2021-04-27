#pragma once

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

namespace numpy {

  template <typename Dtype>
  void savetxt(const char* fname, const ndarray<Dtype>& X,
	       std::string delimiter=" ", std::string newline="\n",
	       std::string header="", std::string footer="") {

    std::ofstream ofs(fname);
    if (!ofs)
      throw std::runtime_error("cannot open file " + std::string(fname));

    ofs << header;
    ofs << std::fixed << std::setprecision(18);
    auto it = X.begin();
    for(int i=0; i<X.shape(0); i++) {
      for(int j=0; j<X.shape(1); j++) {
	ofs << *it << delimiter;
	++it;
      }
      ofs << newline;
    }
    ofs << footer;
  }

  template <typename Dtype=float_>
  ndarray<Dtype> loadtxt(const char* fname) {
    /* Load a 2d-array from a txt file. */
    
    std::ifstream ifs(fname);
    if (!ifs)
      throw std::runtime_error("OSError: " + std::string(fname) + " not found.");

    numpy::size_type height, width;
    ifs >> width >> height;

    std::istream_iterator<Dtype> first(ifs), last;
    std::vector<Dtype> buf(first, last);
    
    auto out = ndarray<Dtype>(buf, {height, width});
    return out;
  }
}
