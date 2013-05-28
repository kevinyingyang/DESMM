#ifndef YALECG_TEXT_HEIGHT_ESTIMATION_HPP
#define YALECG_TEXT_HEIGHT_ESTIMATION_HPP

#include <opencv2/core/core.hpp>

#include "probability_mass_function.hpp"

namespace yalecg {

  class text_height_estimation {
  public://----- typedefs -----//
    typedef cv::Mat image_t;
    typedef yalecg::probability_mass_function pmf_t;
    typedef pmf_t::pmf_data_t                pmf_data_t;

  protected://----- attributes -----// 
    const image_t*          image_;
    int                     height_;
    std::size_t             level_min_;
    std::size_t             level_max_;
    std::vector<pmf_data_t> pmf_data_vec_;
    std::vector<float>      voting_function_vec_;

  public://----- co-destructors -----//
    text_height_estimation();
    ~text_height_estimation();

  public://----- inline public -----//
    inline void set_level_min(const std::size_t& lm) { level_min_ = lm; }
    inline std::size_t level_min() { return level_min_; }
    inline void set_level_max(const std::size_t& lM) { level_max_ = lM; }
    inline std::size_t level_max() { return level_max_; }

  public://----- public -----//
    void compute_in(std::size_t&   text_height,
		    const image_t& image);

  protected://----- protected -----//
    void reset();
    void initialize(const image_t& image);
    void compute_pmf_vec();
    void compute_voting_function_vec();
    void compute_text_height(std::size_t& text_height);

  }; // class text_height_estimation

} // namespace yalecg

#endif // YALECG_TEXT_HEIGHT_ESTIMATION_HPP

