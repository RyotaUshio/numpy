#pragma once

#include <string>
#include <cstdio>

namespace numpy {
  template <typename Dtype>
  void savetxt(const char* fname, const ndarray<Dtype>& X,
	       std::string fmt="%.18e", std::string delimiter=" ", std::string newline="\n",
	       std::string header="", std::string footer="") {

    FILE* fp;
    fp = std::fopen(fname, "w");

    // if (fp == NULL) {
    //   例外処理...
    // }

    std::fprintf(fp, "%s", header.c_str());

    auto it = X.begin();
    for(int i=0; i<X.shape(0); i++) {
      for(int j=0; j<X.shape(1); j++) {
	std::fprintf(fp, (fmt + delimiter).c_str(), *it);
	++it;
      }
      std::fprintf(fp, "%s", newline.c_str());
    }
    std::fprintf(fp, "%s", footer.c_str());
    
    std::fclose(fp);
  }
}
