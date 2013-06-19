#ifndef YALECG_JOURNET_FEATURES_HPP
#define YALECG_JOURNET_FEATURES_HPP

#include <opencv2/core/core.hpp>

#include "rose_of_directions.hpp"

namespace yalecg {

  class journet_features {
  public://----- typedefs -----//
    typedef cv::Mat                    image_t;
    typedef yalecg::rose_of_directions rod_t;

  protected://----- attributes -----//
    const image_t*     image_;
    std::vector<float> rose_vec_;
    int                angle_gap_;
    bool               debug_flag_;
	
  public://----- co-destructors -----//
    journet_features();
    ~journet_features();

  public://----- inline public -----//    
    inline void set_angle_gap(const int& ag) { angle_gap_ = ag; }
    inline int  angle_gap() { return angle_gap_; }
    inline void enable_debug(bool b) { debug_flag_ = b; }
    inline bool is_enable_debug() { return debug_flag_; }

  public://----- public -----//
    void compute_in(float& angle, float& intensity, float& std,
		    float& transition, float& white_space,
		    const image_t& image);
	
  protected://----- protected -----//
    void rod_features_in(float& angle, float& intensity, float& std) const;
    bool is_valid_theta(const int& theta, const int& pos_max) const;
    void transition_in(float& transition) const;
    void white_space_in(float& white_space) const;

  };  // class rose_of_directions

} // namespace yalecg

#endif // YALECG_ROSE_OF_DIRECTIONS_HPP
