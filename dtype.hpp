#pragma once
#include <vector>

namespace numpy {
  
  using int32 = int;
  using int64 = long;
  using uint32 = unsigned int;
  using uint64 = unsigned long;
  using float32 = float;
  using float64 = double;

  using size_type = int;
  using shape_type = std::vector<size_type>;
  using dim_type = int;
  using stride_type = std::vector<int>;
  using offset_type = int;
  
}
