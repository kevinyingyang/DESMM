#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <cstdlib>
//#include <unistd.h>

#include <opencv2/highgui/highgui.hpp>

#include "utility.hpp"

namespace yalecg {

  //####################################################//
  //##### PUBLIC #######################################//
  //####################################################//

  void utility::level_domain_subdivision_in(subdivision_vec_t& sbd_vec,
					    const int&         height,
					    const int&         width,
					    const int&         level,
					    const int&         lattices,
					    const bool&        debug_flag) {
    sbd_vec.clear();
    int ratio = (1<<level);
    int sbd_height = height / ratio;
    int sbd_width  = width  / ratio;
    int i_count, j_count;
    bool regular_flag = (lattices == LATTICE_REGULAR) || (lattices == LATTICE_ALL);
    bool shifted_flag = (lattices == LATTICE_SHIFTED) || (lattices == LATTICE_ALL);
    if (regular_flag) {
      //----- regular lattice -----//
      i_count = ratio;
      j_count = ratio;
      for (int i = 0; i < i_count; ++i) {
	for (int j = 0; j < j_count; ++j) {
	  int x           = sbd_width  * j;
	  int y           = sbd_height * i;
	  int rect_width  = (j == ratio-1) ? std::max(sbd_width ,width-x ) : sbd_width; 
	  int rect_height = (i == ratio-1) ? std::max(sbd_height,height-y) : sbd_height;
	  cv::Rect rect(x,y,rect_width,rect_height);
	  sbd_vec.push_back(rect);
	}
      }
    }
    if (shifted_flag) {
      //----- shifted lattice -----//
      i_count = ratio-1;
      j_count = ratio-1;
      for (int i = 0; i < i_count; ++i) {
	for (int j = 0; j < j_count; ++j) {
	  int x = sbd_width  * j + sbd_width  / 2;
	  int y = sbd_height * i + sbd_height / 2;
	  int rect_width  = sbd_width; 
	  int rect_height = sbd_height;
	  cv::Rect rect(x,y,rect_width,rect_height);
	  sbd_vec.push_back(rect);
	}
      }
    }
    assert_subdivision_vec_size(level,sbd_vec.size(),lattices);
    if (debug_flag) { debug_level_domain_subdivision_in(sbd_vec,height,width,level,lattices); }
  }

  //####################################################//
  //##### PROTECTED ####################################//
  //####################################################//

  //####################################################//
  //##### PROTECTED - DEBUG ############################//
  //####################################################//

  void utility::assert_subdivision_vec_size(const int&         level,
					    const std::size_t& size,
					    const int&         lattices) {
    std::size_t ratio = (1<<level);
    std::size_t expected_size = 0;    
    if ((lattices == LATTICE_REGULAR) || (lattices == LATTICE_ALL)) { expected_size += ratio * ratio; }
    if ((lattices == LATTICE_SHIFTED) || (lattices == LATTICE_ALL)) { expected_size += ((ratio - 1) * (ratio - 1)); }
    if (expected_size != size) {
      std::cerr << "ASSERT - (sbd_vec_expected_size != sbd_vec_size) - failed" << std::endl;
      exit(2);
    }
  }

  void utility::debug_level_domain_subdivision_in(const subdivision_vec_t& sbd_vec,
						  const int&               height,
						  const int&               width,
						  const int&               level,
						  const int&               lattices) {
    std::cerr << std::endl;
    std::cerr << "--> utility::debug_level_domain_subdivision_in(...)" << std::endl;
    //std::srand(time(NULL));
	std::srand ( (unsigned int)time(NULL) ); //Revised by Ying Yang
    std::stringstream ss;
    ss << "sbd_level_" << level << "_" << std::size_t(rand()) << ".png";
    std::string output_fname = ss.str();
    image_t output_image(height,width,CV_8UC3);
    output_image.setTo(cv::Vec3b(0,0,0));
    std::size_t ratio = (1<<level);
    bool regular_flag = (lattices == LATTICE_REGULAR) || (lattices == LATTICE_ALL);
    bool shifted_flag = (lattices == LATTICE_SHIFTED) || (lattices == LATTICE_ALL);
    std::size_t regular_size = 0;
    if (regular_flag) {
      regular_size += ratio * ratio;
      for (std::size_t i = 0; i < regular_size; ++i) {
 	cv::Rect rect = sbd_vec[i];
 	int r = int(rand()%255);
 	int g = int(rand()%255);
 	int b = int(rand()%255);
 	cv::rectangle(output_image,rect,cv::Scalar(r,g,b),-1);
      }
    }
    if (shifted_flag) {
      std::size_t shifted_size = (ratio - 1) * (ratio - 1);
      for (std::size_t i = 0; i < shifted_size; ++i) {
	cv::Rect rect = sbd_vec[regular_size+i];
	int r = int(rand()%255);
	int g = int(rand()%255);
	int b = int(rand()%255);
	cv::rectangle(output_image,rect,cv::Scalar(r,g,b),std::max(2,height/200));
      }
    }
    cv::imwrite(output_fname.c_str(),output_image);
    std::cerr << "    - writing level sbdivision image - " << output_fname << std::endl;
    std::cerr << std::endl;
    //sleep(1);
  }

} // namespace yalecg
