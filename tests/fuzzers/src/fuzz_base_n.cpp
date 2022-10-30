#include "fuzzing_helper.hpp"

#include "hikogui/codec/base_n.hpp"

using namespace hi;

bool fuzz_base_16_test(hi::bstring data) {
  try {
    auto base16_value = base16::encode(data);
    base64::decode(base16_value);
  } catch (std::out_of_range const &) {
    // :(
  }
  return 0;
}

bool fuzz_base_64_test(hi::bstring data) {
  try {
    auto base64_value = base64::encode(data);
    base64::decode(base64_value);
  } catch (std::out_of_range const &) {
    // :(
  }
  return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  //fuzzing_helper::print_fuzzer_input(data, size);
  hi::bstring _data = fuzzing_helper::to_bstring(data, size);
  fuzz_base_16_test(_data);
  fuzz_base_64_test(_data);

  return 0;
}