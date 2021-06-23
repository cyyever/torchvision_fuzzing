/*!
 * \file load_torch_fuzz_test.cpp
 *
 */

#include <fstream>
#include <sstream>
#include <torch/csrc/api/include/torch/serialize.h>
#include <torchvision/io/image/cpu/read_write_file.h>
#include <torchvision/io/image/image.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  try {
    if (Size == 0) {
      return 0;
    }
    auto data = torch::empty({static_cast<long>(Size)}, torch::kU8);
    memcpy(data.data_ptr<uint8_t>(), Data, Size);
    auto img = vision::image::decode_jpeg_cuda(
        data, vision::image::IMAGE_READ_MODE_RGB, 0);

  } catch (...) {
  }
  return 0; // Non-zero return values are reserved for future use.
}
