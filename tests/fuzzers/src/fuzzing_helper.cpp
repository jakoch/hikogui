#include "fuzzing_helper.hpp"

namespace hi::inline v1 {

void fuzzing_helper::print_fuzzer_input(const uint8_t *data, size_t size) {
    std::stringstream ss;
    ss << to_string(data, size);
    std::cout << ss.str();
}

std::string fuzzing_helper::to_string(const uint8_t *data, size_t size)
{
  std::string _data(reinterpret_cast<char *>(const_cast<uint8_t *>(data)), size);
  return _data;
}

hi::bstring fuzzing_helper::to_bstring(const uint8_t *data, size_t size)
{
  std::string _data(reinterpret_cast<char *>(const_cast<uint8_t *>(data)), size);
  return hi::to_bstring(_data);
}

} // namespace hi::inline v1
