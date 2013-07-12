#include "stdafx.h"
#include <iostream>
#include <ctime>

#include "probability_mass_function.hpp"

namespace yalecg {

  //####################################################//
  //##### CO-DESTRUCTOR ################################//
  //####################################################//

  probability_mass_function::probability_mass_function() { 
    reset();
    debug_flag_ = false;
  }

  probability_mass_function::~probability_mass_function() { 
    reset();
    debug_flag_ = false;
  }

  //####################################################//
  //##### PUBLIC #######################################//
  //####################################################//

  void probability_mass_function::compute_in(pmf_data_t&        pmf,
					     const image_t&     image,
					     const std::size_t& level) {
    if (debug_flag_) { 
      std::cerr << std::endl;
      std::cerr << "##### probability_mass_function::compute_in(...) ##########" << std::endl;
    }
    assert_8uc1_image(image);
    reset();
    initialize(image,level);
    utility_t::level_domain_subdivision_in(sbd_vec_,height_,width_,level,utility_t::LATTICE_SHIFTED);
    compute_dft_coefficient_histogram();
    compute_winner_attributes();
    compute_pmf_in(pmf);
    std::cerr << "pmf data [mu,sigma,weight] - [" << pmf.mu << "," << pmf.sigma << "," << pmf.weight << "]" << std::endl;
    if (debug_flag_) { 
      std::cerr << "##################################################################" << std::endl;
      std::cerr << std::endl;
    }
  }

  //####################################################//
  //##### PROTECTED ####################################//
  //####################################################//

  void probability_mass_function::reset() {
    image_                     = 0;
    height_                    = -1;
    width_                     = -1;
    level_                     = -1;
    sbd_vec_.clear();
    coeff_hist_map_.clear();
    winner_idx_                = 0;
    winner_multiplicity_       = 0;
    winner_amplitude_integral_ = 0.0f;
  }

  void probability_mass_function::initialize(const image_t& image,
					     const int&     level) {
    image_  = &image;
    height_ = image.size().height;
    width_  = image.size().width;
    level_  = level;
  }

  void probability_mass_function::compute_dft_coefficient_histogram() {
    std::size_t sbd_vec_size = sbd_vec_.size();
    pfd_t pfd;
    for (std::size_t i = 0; i < sbd_vec_size; ++i) {
      cv::Rect rect = sbd_vec_[i];
      image_t sbd_image(rect.height,rect.width,CV_8UC1);
      (*image_)(rect).copyTo(sbd_image);
      std::size_t coeff_idx;
      float       coeff_amp;
      pfd.compute_in(coeff_idx,coeff_amp,sbd_image);
      if (coeff_idx > 1) {
	update_coeff_hist(coeff_idx,coeff_amp);
      }
    }
    if (debug_flag_) { debug_compute_dft_coefficient_histogram(); }
  }

  void probability_mass_function::update_coeff_hist(const std::size_t& coeff_idx,
						    const float&       coeff_amp) {
    coeff_hist_map_t::iterator chm_it = coeff_hist_map_.find(coeff_idx);
    bool coeff_idx_found = (chm_it != coeff_hist_map_.end());
    if (coeff_idx_found) {
      ch_map_value_t value = chm_it->second;
      value.first  += 1;
      value.second += coeff_amp;
      chm_it->second = value;
    } else {
      ch_map_key_t   key   = coeff_idx;
      ch_map_value_t value = std::make_pair(1,coeff_amp);
      coeff_hist_map_.insert(std::make_pair(key,value));
    }
  }

  void probability_mass_function::compute_winner_attributes() {
    winner_amplitude_integral_ = -0.0f;
    for (coeff_hist_map_t::const_iterator chm_it = coeff_hist_map_.begin();
	 chm_it != coeff_hist_map_.end(); ++chm_it) {
      ch_map_key_t   key   = chm_it->first;
      ch_map_value_t value = chm_it->second;
      if (winner_amplitude_integral_ < value.second) {
	winner_idx_                = key;
	winner_multiplicity_       = value.first;
	winner_amplitude_integral_ = value.second;
      }
    }
  }

  void probability_mass_function::compute_pmf_in(pmf_data_t& pmf) const {
    if (winner_multiplicity_ == 0) {
      pmf.mu     = 0.0f;
      pmf.sigma  = 1.0f;
      pmf.weight = 0.0f;
      return;
    } 
    float ratio         = float(1<<level_);
    float sbd_height    = float(height_) / ratio;
    float sbd_width     = float(width_)  / ratio;
    float in            = float(winner_idx_);
    float th_min        = sbd_height / (in + 0.5f);
    float th_max        = sbd_height / (in - 0.5f);
    pmf.mu              = 0.5f * (th_min + th_max);
    pmf.sigma           = th_max - pmf.mu;
    float avg_amplitude = winner_amplitude_integral_ / float(winner_multiplicity_);
    pmf.weight          = 100 * avg_amplitude / sbd_width;
  }

  //####################################################//
  //##### PROTECTED - DEBUG ############################//
  //####################################################//

  void probability_mass_function::debug_compute_dft_coefficient_histogram() const {
    std::cerr << "      --> probability_mass_function::debug_compute_dft_coefficient_histogram()" << std::endl;
    //std::srand(time(NULL));
	std::srand ( (unsigned int)time(NULL) ); // Revised by Ying Yang
    std::stringstream ss;
    ss << "dft_hist_" << level_ << "_" << std::size_t(rand()) << ".png";
    std::string output_fname = ss.str();
    int output_width  = height_;
    int output_height = height_ * 3 / 4; 
    image_t output_image(output_height,output_width,CV_8UC3);
    draw_dft_hist(output_image);
    cv::imwrite(output_fname.c_str(),output_image);
    std::cerr << "          - writing dft hist image - " << output_fname << std::endl;
  }

  void probability_mass_function::draw_dft_hist(image_t& image) const {
    assert_8uc3_image(image);
    image.setTo(cv::Vec3b(0,0,0));
    int height = image.size().height;
    int width  = image.size().width;
    for (int i = 0; i < width-1; ++i) {
      coeff_hist_map_t::const_iterator chm_it = coeff_hist_map_.find(i);
      bool coeff_idx_found = (chm_it != coeff_hist_map_.end());
      cv::Point p0, p1;
      p0.x = i;
      p0.y = height;
      p1.x = i;
      p1.y = height;
      if (coeff_idx_found) {
	float ratio         = float(1<<level_);
	float sbd_width     = float(width_)  / ratio;
	float avg_amplitude = chm_it->second.second / float(chm_it->second.first);
	float value         = avg_amplitude / sbd_width;
	p1.y -= (value * height);
      } 
      cv::line(image,p0,p1,cv::Scalar(0,0,255),1);
    }
  }

  void probability_mass_function::assert_8uc1_image(const image_t& image) const {
    bool is_8uc1 = (image.type() == CV_8UC1);
    if (!is_8uc1) {
      std::cerr << "ASSERT - (image.type() == CV_8UC1) - failed" << std::endl;
      exit(2);
    }
  }

  void probability_mass_function::assert_8uc3_image(const image_t& image) const {
    bool is_8uc3 = (image.type() == CV_8UC3);
    if (!is_8uc3) {
      std::cerr << "ASSERT - (image.type() == CV_8UC3) - failed" << std::endl;
      exit(2);
    }
  }

} // namespace yalecg

