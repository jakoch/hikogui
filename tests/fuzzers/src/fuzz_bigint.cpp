#include "fuzzing_helper.hpp"

#include "hikogui/bigint.hpp"

using namespace hi;

bool fuzz_me_test(const uint8_t *data, size_t dataSize) {
  try {
    std::string d(reinterpret_cast<char *>(const_cast<uint8_t *>(data)), dataSize);
    ubig128 big_int{d};
    //std::cout << big_int.string();
  } catch (std::out_of_range const &) {
    // :(
  }
  return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  //fuzzing_helper::print_fuzzer_input(data, size);
  fuzz_me_test(data, size);
  return 0;
}
