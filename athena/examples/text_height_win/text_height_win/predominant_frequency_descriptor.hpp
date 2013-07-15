#ifndef YALECG_PREDOMINANT_FREQUENCY_DESCRIPTOR_HPP
#define YALECG_PREDOMINANT_FREQUENCY_DESCRIPTOR_HPP

#include <opencv2/imgproc/imgproc.hpp>

#include "autocorrelation_function.hpp"

namespace yalecg {

  class predominant_frequency_descriptor {
  public://----- typedefs -----//
    typedef cv::Mat                          image_t;
    typedef yalecg::autocorrelation_function acf_t;

  protected://----- attributes -----//  
    const image_t*                       image_;
    std::size_t                          height_;
    std::size_t                          width_;
    image_t                              nacf_image_;
    std::vector<float>                   yPP_vec_;
    std::vector<std::pair<float,float> > dft_vec_;
    bool                                 debug_flag_;

  public://----- co-destructors -----//
    predominant_frequency_descriptor();
    ~predominant_frequency_descriptor();

  public://----- inline public -----//   
    void enable_debug(bool b) { debug_flag_ = b; }
    bool is_enable_debug() { return debug_flag_; }

  public://----- public -----//
    void compute_in(std::size_t&   coeff_idx, 
		    float&         coeff_amp, 
		    const image_t& image);

  protected://----- protected -----//
    void reset();
    void initialize(const image_t& image);
    void normalized_autocorrelation_function();
    void compute_yPP();
    void discrete_fourier_transform();
    void predominant_frequency_in(std::size_t& coeff_idx,
				  float&       coeff_amp) const;

  protected://----- debug -----//
    void debug_normalized_autocorrelation_function() const;
    void debug_compute_yPP() const;
    void debug_discrete_fourier_transform() const;
    void debug_predominant_frequency_in(int         max_0,
					int         max_1,
					std::size_t coeff_idx,
					float       coeff_amp) const;
    void draw_nacf(image_t& image) const;
    void draw_yPP(image_t& image) const;
    void draw_dft(image_t& image) const;
    void assert_valid_image(const image_t& image) const;
    void assert_8uc1_image(const image_t& image) const;

  }; // class predominant_frequency_descriptor

} // namespace yalecg

#endif // YALECG_PREDOMINANT_FREQUENCY_DESCRIPTOR_HPP
