#include "fuzzing_helper.hpp"

using namespace hi;

bool fuzz_me_test(const uint8_t *data, size_t size) {
  if (size >= 3 &&
      data[0] == 'F' &&
      data[1] == 'U' &&
      data[2] == 'Z' &&
      data[3] == 'Z') {
    return 1;  // this code-path is taken, if input data is "FUZZ".
  }
  return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  fuzzing_helper::print_fuzzer_input(data, size);
  fuzz_me_test(data, size);
  return 0;
}
