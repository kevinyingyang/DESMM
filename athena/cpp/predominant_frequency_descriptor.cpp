#include <iostream>

#include <ctime>
#include <cstdlib>
#include <sstream>
#include <string>

#include "predominant_frequency_descriptor.hpp"

namespace yalecg {

  //####################################################//
  //##### CO-DESTRUCTOR ################################//
  //####################################################//

  predominant_frequency_descriptor::predominant_frequency_descriptor() {
    image_      = 0;
    nacf_image_.release();
    yPP_vec_.clear();
    dft_vec_.clear();
    debug_flag_ = false;
  }

  predominant_frequency_descriptor::~predominant_frequency_descriptor() {
    image_      = 0;
    nacf_image_.release();
    yPP_vec_.clear();
    dft_vec_.clear();
    debug_flag_ = false;
  }

  //####################################################//
  //##### PUBLIC #######################################//
  //####################################################//

  void predominant_frequency_descriptor::compute_in(std::size_t&   coeff_idx, 
						    float&         coeff_amp, 
						    const image_t& image) {
    if (debug_flag_) { 
      std::cerr << std::endl;
      std::cerr << "##### predominant_frequency_descriptor::compute_in(...) ##########" << std::endl;
    }
    assert_8uc1_image(image);
    reset();
    initialize(image);
    normalized_autocorrelation_function();
    compute_yPP();
    discrete_fourier_transform();
    predominant_frequency_in(coeff_idx,coeff_amp);
    if (debug_flag_) { 
      std::cerr << "##################################################################" << std::endl;
      std::cerr << std::endl;
    }
  }

  //####################################################//
  //##### PROTECTED ####################################//
  //####################################################//

  void predominant_frequency_descriptor::reset() {
    image_      = 0;
    nacf_image_.release();
    yPP_vec_.clear();
    dft_vec_.clear();
  }

  void predominant_frequency_descriptor::initialize(const image_t& image) {
    image_  = &image;
    height_ = image_->size().height;
    width_  = image_->size().width;
  }

  void predominant_frequency_descriptor::normalized_autocorrelation_function() {
    acf_t* acf = new acf_t;
    acf->compute_in(nacf_image_,*image_,true);
    if (acf) { delete acf; acf = 0; }
    if (debug_flag_) { debug_normalized_autocorrelation_function(); }
  }

  void predominant_frequency_descriptor::compute_yPP() {
    yPP_vec_.resize(height_,0.0f);
    for (std::size_t i = 0; i < height_; ++i) {
      for (std::size_t j = 0; j < width_; ++j) {
	yPP_vec_[i] += nacf_image_.at<float>(i,j);
      }    
    }
    if (debug_flag_) { debug_compute_yPP(); }
  }

  void predominant_frequency_descriptor::discrete_fourier_transform() {
    cv::Mat* f = new cv::Mat(height_,1,CV_32FC1);
    for (std::size_t i = 0; i < height_; ++i) { f->at<float>(i,0) = yPP_vec_[i]; }
    cv::Mat* f_dft = new cv::Mat(height_,1,CV_32FC2);
    f_dft->setTo(cv::Vec2f(0.0f,0.0f));
    cv::dft(*f,*f_dft,cv::DFT_COMPLEX_OUTPUT);
    dft_vec_.resize(height_);
    for (std::size_t i = 0; i < height_; ++i) {
      dft_vec_[i].first  = f_dft->at<cv::Vec2f>(i,0)[0];
      dft_vec_[i].second = f_dft->at<cv::Vec2f>(i,0)[1];
    }
    if (debug_flag_) { debug_discrete_fourier_transform(); }
  }

  void predominant_frequency_descriptor::predominant_frequency_in(std::size_t& coeff_idx,
								  float&       coeff_amp) const {
    int max_0 = -1;
    int max_1 = -1;
    float max_0_value = -1e30f;
    float max_1_value = -1e30f;
    for (std::size_t i = 0; i < height_; ++i) {
      float real = dft_vec_[i].first;
      float imag = dft_vec_[i].second;
      float value = 2.0 * std::sqrt(real*real+imag*imag) / float(height_);
      if (max_0_value < value) {
	max_1_value = max_0_value;
	max_1       = max_0;
	max_0_value = value;
	max_0       = i;
      } else if (max_1_value < value) {
	max_1_value = value;
	max_1       = i;
      }
    }
    if (max_0 == 0) {
      coeff_idx = max_1;
      coeff_amp = max_1_value;
    } else {
      coeff_idx = max_0;
      coeff_amp = max_0_value;
    }
    if (debug_flag_) { debug_predominant_frequency_in(max_0,max_1,coeff_idx,coeff_amp); }
  }

  //####################################################//
  //##### PROTECTED - DEBUG ############################//
  //####################################################//

  void predominant_frequency_descriptor::debug_normalized_autocorrelation_function() const {
    std::cerr << "      --> predominant_frequency_descriptor::normalized_autocorrelation_function()" << std::endl;
    std::srand(time(NULL));
    std::stringstream ss;
    ss << "nacf_" << std::size_t(rand()) << ".png";
    std::string output_fname = ss.str();
    image_t output_image(height_,width_,CV_8UC3);
    draw_nacf(output_image);
    cv::imwrite(output_fname.c_str(),output_image);
    std::cerr << "          - writing nacf image - " << output_fname << std::endl;
  }

  void predominant_frequency_descriptor::debug_compute_yPP() const {
    std::cerr << "      --> predominant_frequency_descriptor::compute_yPP()" << std::endl;
    std::srand(time(NULL));
    std::stringstream ss;
    ss << "yPP_" << std::size_t(rand()) << ".png";
    std::string output_fname = ss.str();
    image_t output_image(height_,width_,CV_8UC3);
    draw_nacf(output_image);
    draw_yPP(output_image);
    cv::imwrite(output_fname.c_str(),output_image);
    std::cerr << "          - writing yPP image - " << output_fname << std::endl;
  }

  void predominant_frequency_descriptor::debug_discrete_fourier_transform() const {
    std::cerr << "      --> predominant_frequency_descriptor::discrete_fourier_transform()" << std::endl;
    std::srand(time(NULL));
    std::stringstream ss;
    ss << "dft_" << std::size_t(rand()) << ".png";
    std::string output_fname = ss.str();
    image_t output_image(height_,width_,CV_8UC3);
    draw_nacf(output_image);
    draw_yPP(output_image);
    draw_dft(output_image);
    cv::imwrite(output_fname.c_str(),output_image);
    std::cerr << "          - writing dft image - " << output_fname << std::endl;
  }

  void predominant_frequency_descriptor::debug_predominant_frequency_in(int         max_0,
									int         max_1,
									std::size_t coeff_idx,
									float       coeff_amp) const {
    std::cerr << "      --> predominant_frequency_descriptor::predominant_frequency_in(...)" << std::endl;
    if (max_0 != 0) { 
      std::cerr << "          - WARNING - constant index != max index" << std::endl;
      std::cerr << "            [max_0,max_1] - [" << max_0 << "," << max_1 << "]" << std::endl;
      std::cerr << "            Assigning max_0 to coeff_idx and max_0_value to coeff_amp" << std::endl;
    }
    std::cerr << "          - [coeff_idx,coeff_amp] - [" << coeff_idx << "," << coeff_amp << "]" << std::endl;
  }

  void predominant_frequency_descriptor::draw_nacf(image_t& image) const {
    assert_valid_image(image);
    for (std::size_t i = 0; i < height_; ++i) {
      for (std::size_t j = 0; j < width_; ++j) {
	uchar value = uchar(nacf_image_.at<float>(i,j) * 255.0f);
	image.at<cv::Vec3b>(i,j) = cv::Vec3b(value,value,value);
      }
    }
  }

  void predominant_frequency_descriptor::draw_yPP(image_t& image) const {
    assert_valid_image(image);
    for (std::size_t i = 0; i < height_-1; ++i) {
      float value0 = yPP_vec_[i];
      float value1 = yPP_vec_[i+1];
      cv::Point p0, p1;
      p0.x = width_ - int(value0);
      p1.x = width_ - int(value1);
      p0.y = i;
      p1.y = i+1;
      cv::line(image,p0,p1,cv::Scalar(0,255,0),std::max(std::size_t(2),height_/200));
    }
  }

  void predominant_frequency_descriptor::draw_dft(image_t& image) const {
    assert_valid_image(image);
    for (std::size_t i = 0; i < height_; ++i) {
      float real = dft_vec_[i].first;
      float imag = dft_vec_[i].second;
      cv::Point p0, p1;
      p0.x = int(2.0 * std::sqrt(real*real+imag*imag) / float(height_));
      p0.y = i;
      p1.x = 0;
      p1.y = i;// they are bins
      cv::line(image,p0,p1,cv::Scalar(0,0,255),1);
    }
  }

  void predominant_frequency_descriptor::assert_valid_image(const image_t& image) const {
    bool is_valid_type   = (image.type()                     == CV_8UC3);
    bool is_valid_height = (std::size_t(image.size().height) == height_);
    bool is_valid_width  = (std::size_t(image.size().width)  == width_ );
    bool is_valid = (is_valid_type && is_valid_height && is_valid_width);
    if (!is_valid) {
      std::cerr << "ASSERT - (image.type() == CV_8UC3) && (image.size().height == height_) && (image.size().width == width_) - failed" << std::endl;
      exit(2);
    }
  }

  void predominant_frequency_descriptor::assert_8uc1_image(const image_t& image) const {
    bool is_8uc1 = (image.type() == CV_8UC1);
    if (!is_8uc1) {
      std::cerr << "ASSERT - (image.type() == CV_8UC1) - failed" << std::endl;
      exit(2);
    }
  }

} // namespace yalecg
