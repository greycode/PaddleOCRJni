// Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
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

#pragma once

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "paddle_api.h"
#include "paddle_inference_api.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <vector>

#include <cstring>
#include <fstream>
#include <numeric>

#include <include/ocr_cls.h>
#include <include/ocr_det.h>
#include <include/ocr_rec.h>
#include <include/preprocess_op.h>
#include <include/utility.h>

using namespace paddle_infer;

namespace PaddleOCR {

class PPOCR {
public:
  explicit PPOCR();
  explicit PPOCR(const bool &use_det, const bool &use_cls, const bool &use_angle_cls, const bool &use_rec,
                 const std::string &det_model_dir, const std::string &cls_model_dir, const std::string &rec_model_dir,
                 const std::string &rec_char_dict_path);

  explicit PPOCR(const bool &use_det, const bool &use_cls, const bool &use_angle_cls, const bool &use_rec,
                 const std::string &det_model_dir, const std::string &cls_model_dir, const std::string &rec_model_dir,
                 const std::string &image_dir,

                  const bool &use_gpu,
                  const int &gpu_id, const int &gpu_mem,
                  const int &cpu_math_library_num_threads,
                  const bool &use_mkldnn, 

                  const int &max_side_len,
                  const double &det_db_thresh,
                  const double &det_db_box_thresh,
                  const double &det_db_unclip_ratio,
                  const std::string &det_db_score_mode,
                  const bool &use_dilation,

                  const double &cls_thresh,
                  const int &cls_batch_num,

                  const string &label_path,
                  const int &rec_batch_num, const int &rec_img_h,
                  const int &rec_img_w,

                  const bool &use_tensorrt, const std::string &precision
                 );               
  ~PPOCR();
  std::vector<std::vector<OCRPredictResult>>
  ocr(std::vector<cv::String> cv_all_img_names, bool det = true,
      bool rec = true, bool cls = true);

private:
  DBDetector *detector_ = nullptr;
  Classifier *classifier_ = nullptr;
  CRNNRecognizer *recognizer_ = nullptr;

  void det(cv::Mat img, std::vector<OCRPredictResult> &ocr_results,
           std::vector<double> &times);
  void rec(std::vector<cv::Mat> img_list,
           std::vector<OCRPredictResult> &ocr_results,
           std::vector<double> &times);
  void cls(std::vector<cv::Mat> img_list,
           std::vector<OCRPredictResult> &ocr_results,
           std::vector<double> &times);
  void log(std::vector<double> &det_times, std::vector<double> &rec_times,
           std::vector<double> &cls_times, int img_num);
};

} // namespace PaddleOCR
