#ifndef YALECG_AUTOCORRELATION_FUNCTION_HPP
#define YALECG_AUTOCORRELATION_FUNCTION_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace yalecg {

  class autocorrelation_function {
  public://----- typedefs -----//
    typedef cv::Mat image_t;

  protected://----- attributes -----//  
    const image_t* image_;
    int            width_;
    int            height_;

  public://----- co-destructors -----//
    autocorrelation_function()  { image_ = 0; };
    ~autocorrelation_function() { image_ = 0; };

  public://----- public -----//
    /* image (input) 
          - has type CV_8UC1  - range [0,255]
	  - has type CV_32FC1
       acf (output)
          - has type CV_32FC1 
       is_normalized
	  - false (default) - acf output is not normalized
	  - true            - acf output is normalized (e.g., for displaying purposes)
     */
    void compute_in(image_t& acf, const image_t& image, const bool& is_normalized = false);

  protected://----- protected -----//
    void initialize(const image_t& image);
    void autoconv_fft_in(image_t* image) const;
    void rearrange_image_to_acf_in(image_t& acf, const image_t* image) const;
    bool is_valid_idx(const int& i, const int& j) const;
    void normalize_image_in(image_t& image) const;
    
  }; // class autocorrelation_function

} // namespace yalecg

#endif // YALECG_AUTOCORRELATION_FUNCTION_HPP
