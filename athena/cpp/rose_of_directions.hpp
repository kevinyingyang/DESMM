#ifndef YALECG_ROSE_OF_DIRECTIONS_HPP
#define YALECG_ROSE_OF_DIRECTIONS_HPP

#include "autocorrelation_function.hpp"

namespace yalecg {

  class rose_of_directions {
  public://----- typedefs -----//
    typedef cv::Mat                          image_t;
    typedef yalecg::autocorrelation_function acf_t;
    
  protected://----- attributes -----//
    bool  debug_flag_;
	
  public://----- co-destructors -----//
    rose_of_directions();
    ~rose_of_directions();

  public://----- inline public -----//    
    inline void enable_debug(bool b) { debug_flag_ = b; }
    inline bool is_enable_debug() { return debug_flag_; }

  public://----- public -----//
    void compute_from_image_in(std::vector<float>& rose_vec, const image_t& image);
    void compute_from_acf_in(std::vector<float>& rose_vec,const image_t& image);

  protected://----- debug -----//
    void draw_rod(const std::vector<float>& rose_vec) const;

  };  // class rose_of_directions

} // namespace yalecg

#endif // YALECG_ROSE_OF_DIRECTIONS_HPP
