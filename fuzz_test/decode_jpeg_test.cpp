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
    auto data = torch::empty({static_cast<long>(Size-1)}, torch::kU8);
    memcpy(data.data_ptr<uint8_t>(), Data+1, Size-1);
    std::vector<torch::ScalarType> dtypes {torch::kU8,torch::kI8,torch::kI16,torch::kI32,torch::kI64,torch::kF16,torch::kF32};
    data=data.to(dtypes[Data[0]%dtypes.size()]);
    auto img =
        vision::image::decode_jpeg(data, vision::image::IMAGE_READ_MODE_RGB);

  } catch (...) {
  }
  return 0; // Non-zero return values are reserved for future use.
}
