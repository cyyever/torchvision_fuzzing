/*!
 * \file load_torch_fuzz_test.cpp
 *
 */

#include <sstream>
#include <torch/csrc/api/include/torch/serialize.h>
#include <torchvision/io/image/cpu/read_write_file.h>
#include <torchvision/io/image/image.h>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  try {
    if (Size <= 2) {
      return 0;
    }
    std::vector<torch::ScalarType> dtypes{torch::kU8,  torch::kI8,  torch::kI16,
                                          torch::kI32, torch::kI64, torch::kF16,
                                          torch::kF32};
    auto data = torch::empty({static_cast<long>(Size - 2)},
                             dtypes[Data[0] % dtypes.size()]);
    memcpy(data.data_ptr<uint8_t>(), Data + 2, Size - 2);
    auto image = vision::image::encode_png(data, Data[1]);
    vision::image::write_file("/tmp/__vision_fuzz_png.png",image);

  } catch (...) {
  }
  return 0; // Non-zero return values are reserved for future use.
}
