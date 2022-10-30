#include "fuzzing_helper.hpp"

#include "hikogui/required.hpp"
#include "hikogui/url_parser.hpp"

using namespace hi;

bool fuzz_url_parser_test(std::string data) {
  try {
    hilet parts = url_decode(data);
  } catch (std::out_of_range const &) { // hi_no_default
    // :(
  }
  return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  //fuzzing_helper::print_fuzzer_input(data, size);
  std::string _data = fuzzing_helper::to_string(data, size);
  fuzz_url_parser_test(_data);
  return 0;
}