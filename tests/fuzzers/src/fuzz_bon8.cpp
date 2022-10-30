#include "fuzzing_helper.hpp"

#include "hikogui/byte_string.hpp"
#include "hikogui/codec/BON8.hpp"

using namespace hi;

/*
auto bon8_data = to_bstring(0x8c, 0x04, 0x08, 0x0f, 0x28);
auto data_read_back = decode_BON8(bon8_data);
ASSERT_EQ(datum{67637032}, data_read_back);
*/

bool fuzz_BON8(const uint8_t *data, size_t size) {
  auto bon8_data_str = std::string(reinterpret_cast<const char *>(data), size);
  auto bon8_data = to_bstring(bon8_data_str);
  try {
    auto data_read_back = decode_BON8(bon8_data);
    //printf("%s\n", std::string(data_read_back).c_str());
  } catch (hi::parse_error& e) {
    std::cout << e.what() << '\n';
  }
  return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  //fuzzing_helper::print_fuzzer_input(data, size);
  fuzz_BON8(data, size);
  return 0;
}
