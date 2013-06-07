#ifndef YALECG_UTILITY_HPP
#define YALECG_UTILITY_HPP

#include <vector>

#include <opencv2/core/core.hpp>

namespace yalecg {

  class utility {
  public://----- typedefs -----//
    typedef std::vector<cv::Rect> subdivision_vec_t;
    typedef cv::Mat               image_t;

    enum { LATTICE_REGULAR, LATTICE_SHIFTED, LATTICE_ALL };

  public://----- public -----//
    static void level_domain_subdivision_in(subdivision_vec_t& sbd_vec,
					    const int&         height,
					    const int&         width,
					    const int&         level,
					    const int&         lattices = LATTICE_ALL,
					    const bool&        debug_flag = false);

  protected://----- protected -----//

  protected://----- debug -----//
    static void assert_subdivision_vec_size(const int&         level,
					    const std::size_t& size,
					    const int&         lattices);
    static void debug_level_domain_subdivision_in(const subdivision_vec_t& sbd_vec,
						  const int&               height,
						  const int&               width,
						  const int&               level,
						  const int&               lattices = LATTICE_ALL);
    
  }; // class utility

} // namespace yalecg

#endif // YALECG_UTILITY_HPP
