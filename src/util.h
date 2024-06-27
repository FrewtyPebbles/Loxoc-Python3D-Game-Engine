#pragma once
#include "Tup.h"
#include <string>
#include <cctype>
#include <vector>
#include <cmath>

static std::string MOD_PATH;

void c_set_mod_path(std::string path);

std::string get_mod_path();

using std::vector;
using std::string;
namespace gamemath {
  inline float lerp(float start, float end, float ratio) {return start*ratio + end*(1-ratio);}

  template <typename T>
  inline T clamp(const T& value, const T& low, const T& high) {
    return value < low ? low : (value > high ? high : value); 
  }
}


inline tup<float, 3> barycentric_coords(float x1, float y1, float x2, float y2, float x3, float y3,
float xp, float yp) {
    float exp1 = xp - x3;
    float exp2 = yp - y3;
    float det = fmaf((x1 - x3), (y2 - y3), -(x2 - x3)*(y1 - y3));
    float u1 = fmaf((y2 - y3), (exp1), (x3 - x2)*(exp2))/det;
    float u2 = fmaf((y3 - y1), (exp1), (x1 - x3)*(exp2))/det;
    float u3 = 1 - u1 - u2;
    return make_tup<float, 3>({u1, u2, u3});
}

template <typename T>
inline void vec_extend(vector<T>& v, vector<T>& v_prime) {
  v.reserve(v.size() + distance(v_prime.begin(),v_prime.end()));
  v.insert(v.end(),v_prime.begin(),v_prime.end());
}

namespace str_tool
{
    inline string to_lowercase(const std::string& str) {
      string result;
      for (char ch : str) {
        result += std::tolower(ch);
      }
      return result;
    }

    inline std::string rem_path_from_file(std::string const & path) {
      return path.substr(path.find_last_of("/\\") + 1);
    }

    inline std::string rem_file_from_path(std::string const & path) {
      return path.substr(0, path.find_last_of("\\/"));
    }
};