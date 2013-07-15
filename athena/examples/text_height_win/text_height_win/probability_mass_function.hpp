#ifndef YALECG_PROBABILITY_MASS_FUNCTION_HPP
#define YALECG_PROBABILITY_MASS_FUNCTION_HPP

#include <opencv2/core/core.hpp>

#include "predominant_frequency_descriptor.hpp"

#include "utility.hpp"

namespace yalecg {

  class probability_mass_function {
  public://----- typedefs -----//
    typedef struct pmf {
      float mu;
      float sigma;
      float weight;
    } pmf_data_t;

    typedef cv::Mat                                  image_t;
    typedef yalecg::utility                          utility_t;
    typedef utility_t::subdivision_vec_t             subdivision_vec_t;
    typedef yalecg::predominant_frequency_descriptor pfd_t;
    typedef std::size_t                              ch_map_key_t;
    typedef std::pair<std::size_t,float>             ch_map_value_t;
    typedef std::map<ch_map_key_t,ch_map_value_t>    coeff_hist_map_t;

  protected://----- attributes -----//  
    const image_t*    image_;
    int               height_;
    int               width_;
    int               level_;
    subdivision_vec_t sbd_vec_;
    coeff_hist_map_t  coeff_hist_map_;
    std::size_t       winner_idx_;
    std::size_t       winner_multiplicity_;
    float             winner_amplitude_integral_;
    bool              debug_flag_;

  public://----- co-destructors -----//
    probability_mass_function();
    ~probability_mass_function();

  public://----- inline public -----//    
    void enable_debug(bool b) { debug_flag_ = b; }
    bool is_enable_debug() { return debug_flag_; }

  public://----- public -----//
    void compute_in(pmf_data_t&        pmf,
		    const image_t&     image,
		    const std::size_t& level);

  protected://----- protected -----//
    void reset();
    void initialize(const image_t& image,
		    const int&     level);
    void compute_dft_coefficient_histogram();
    void update_coeff_hist(const std::size_t& coeff_idx,
			   const float&       coeff_amp);
    void compute_winner_attributes();
    void compute_pmf_in(pmf_data_t& pmf) const;

  protected://----- debug -----//
    void debug_compute_dft_coefficient_histogram() const;
    void draw_dft_hist(image_t& image) const;
    void assert_8uc1_image(const image_t& image) const;
    void assert_8uc3_image(const image_t& image) const;

  }; // class probability_mass_function

} // namespace yalecg

#endif // YALECG_PROBABILITY_MASS_FUNCTION_HPP
