#include <fstream>
#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <text_height_estimation.hpp>

//####################################################//
//##### TYPEDEFS #####################################//
//####################################################//

typedef cv::Mat                        image_t;
typedef yalecg::text_height_estimation the_t;

//####################################################//
//##### DEBUG ########################################//
//####################################################//

void draw_verify_image(image_t&           image,
		       const std::size_t& text_height) {
  int height = image.size().height;
  int width  = image.size().width;
  int x0, y0, x1, y1;
  for (int ir = 0; ir < 5; ++ir) {
    //-----//
    x0 = width/2  + ir * text_height   + ir*5;
    y0 = height/2 + ir * text_height/4;
    x1 = width/2  + ir * text_height   + ir * 5 + text_height;
    y1 = height/2 + ir * text_height/4 + text_height;
    cv::rectangle(image,
		  cv::Point(x0,y0),
		  cv::Point(x1,y1),
		  cv::Scalar(0,0,255),3);
    //-----//
    x0 = width/2  - ir * text_height   - ir * 5;
    y0 = height/2 - ir * text_height/4;
    x1 = width/2  - ir * text_height   - ir * 5 + text_height;
    y1 = height/2 - ir * text_height/4 + text_height;
    cv::rectangle(image,
		  cv::Point(x0,y0),
		  cv::Point(x1,y1),
		  cv::Scalar(0,0,255),3);
    //-----//
    x0 = width/2  + ir * text_height/4;
    y0 = height/2 - ir * text_height - ir*5;
    x1 = width/2  + ir * text_height/4 + text_height;
    y1 = height/2 - ir * text_height - ir*5 + text_height;
    cv::rectangle(image,
		  cv::Point(x0,y0),
		  cv::Point(x1,y1),
		  cv::Scalar(0,0,255),3);
    //-----//
    x0 = width/2  - ir * text_height/4;
    y0 = height/2 + ir * text_height + ir*5;
    x1 = width/2  - ir * text_height/4 + text_height;
    y1 = height/2 + ir * text_height + ir*5 + text_height;
    cv::rectangle(image,
		  cv::Point(x0,y0),
		  cv::Point(x1,y1),
		  cv::Scalar(0,0,255),3);
  }
  std::string output_fname = "check_image.png";
  cv::imwrite(output_fname.c_str(),image);
}

void write_hwt_matlab(const image_t&     image,
		      const std::size_t& text_height) {
  std::string hwt_fname = "hwt.m";
  std::fstream hwt_fs(hwt_fname.c_str(),std::fstream::out); 
  hwt_fs << "hwt = [" << image.size().height << "," << image.size().width << "," << text_height << "];";
  hwt_fs.close();
}

//####################################################//
//##### MAIN #########################################//
//####################################################//

int main(int argc,char** argv) {  
  if (argc == 1) {
    std::cerr << "Missing input file name" << std::endl;
    exit(1);
  } 

  image_t image = cv::imread(argv[1]);
  image_t image_gray(image.size().height,image.size().width,CV_8UC1);
  cv::cvtColor(image,image_gray,CV_RGB2GRAY);
  the_t the;
  std::size_t text_height;
  the.compute_in(text_height,image_gray);
  std::cerr << "Text height - " << text_height << std::endl;
  std::cerr << "Writing Athena debug output" << std::endl;
  draw_verify_image(image,text_height);
  write_hwt_matlab(image,text_height);

  return 0;
}
