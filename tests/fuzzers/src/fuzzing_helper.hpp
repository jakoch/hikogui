#pragma once

#include <stddef.h>
#include <stdint.h> // for uint8_t

#include <stdexcept> // for overflow_error, underflow_error, range_error, out_of_range

#include <iostream>
#include <sstream>

#include "hikogui/byte_string.hpp"

namespace hi::inline v1 {

class fuzzing_helper {
public:
  static void print_fuzzer_input(const uint8_t *data, size_t size);

  static std::string to_string(const uint8_t *data, size_t size);
  static hi::bstring to_bstring(const uint8_t *data, size_t size);
};

} // namespace hi::inline v1
