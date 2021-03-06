// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "lite/kernels/arm/read_from_array_compute.h"
#include "lite/backends/arm/math/funcs.h"

namespace paddle {
namespace lite {
namespace kernels {
namespace arm {

void ReadFromArrayCompute::Run() {
  auto& ctx = this->ctx_->template As<ARMContext>();
  auto& param = this->Param<param_t>();

  CHECK_EQ(param.I->numel(), 1) << "I should have only one element";
  int id = param.I->data<int64_t>()[0];
  int in_num = param.X->size();
  CHECK_LE(id, in_num) << "id is not valid";

  param.Out->Resize((*param.X)[id].dims());
  param.Out->CopyDataFrom((*param.X)[id]);
}

}  // namespace arm
}  // namespace kernels
}  // namespace lite
}  // namespace paddle

REGISTER_LITE_KERNEL(read_from_array,
                     kARM,
                     kAny,
                     kNCHW,
                     paddle::lite::kernels::arm::ReadFromArrayCompute,
                     def)
    .BindInput("X", {LiteType::GetTensorListTy(TARGET(kARM), PRECISION(kAny))})
    .BindInput("I", {LiteType::GetTensorTy(TARGET(kARM), PRECISION(kInt64))})
    .BindOutput("Out", {LiteType::GetTensorTy(TARGET(kARM), PRECISION(kAny))})
    .Finalize();
