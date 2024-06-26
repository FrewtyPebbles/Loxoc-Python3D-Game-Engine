#include "util.h"

void c_set_mod_path(std::string path) {
  MOD_PATH = path;
}

std::string get_mod_path() {
  return MOD_PATH;
}