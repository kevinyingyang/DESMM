#include "stdafx.h"
#include <iostream>

#include "text_height_estimation.hpp"

namespace yalecg {

  //####################################################//
  //##### CO-DESTRUCTOR ################################//
  //####################################################//

  text_height_estimation::text_height_estimation() {
    reset();
    level_min_ = 2;
    level_max_ = 5;
  }

  text_height_estimation::~text_height_estimation() {
    reset();
    level_min_ = 2;
    level_max_ = 5;
  }

  //####################################################//
  //##### PUBLIC #######################################//
  //####################################################//

  void text_height_estimation::compute_in(std::size_t&   text_height,
					  const image_t& image) {
    reset();
    initialize(image);
    compute_pmf_vec();
    compute_voting_function_vec();
    compute_text_height(text_height);
  }

  //####################################################//
  //##### PROTECTED ####################################//
  //####################################################//

  void text_height_estimation::reset() {
    image_       = 0;
    pmf_data_vec_.clear();
    voting_function_vec_.clear();
  }

  void text_height_estimation::initialize(const image_t& image) {
    image_  = &image;
    height_ = image_->size().height;
  }

  void text_height_estimation::compute_pmf_vec() {
    pmf_t pmf;
    for (std::size_t l = level_min_; l < level_max_; ++l) {
      pmf_data_t pmf_data;
      pmf.compute_in(pmf_data,*image_,l);
      pmf_data_vec_.push_back(pmf_data);
    }
  }

  void text_height_estimation::compute_voting_function_vec() {
    std::size_t pmf_data_vec_size = pmf_data_vec_.size();
    for (int i = 0; i < height_; ++i) {
      float sum = 0.0f;
      for (std::size_t l = 0; l < pmf_data_vec_size; ++l) {
	pmf_data_t pmf_data = pmf_data_vec_[l];
	float exp_arg = (float(i) - pmf_data.mu) / pmf_data.sigma;
	float value  = pmf_data.weight * std::exp(- 0.5f * exp_arg * exp_arg);
	sum += value;
      }
      voting_function_vec_.push_back(sum);
    }
  }

  void text_height_estimation::compute_text_height(std::size_t& text_height) {
    text_height = 0;
    float max_vote = -0.0f;
    for (int i = 0; i < height_; ++i) {
      if (max_vote < voting_function_vec_[i]) {
	max_vote = voting_function_vec_[i];
	text_height = i;
      }
    }
  }

} // namespace yalecg
