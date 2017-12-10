//Code by Matt sheckells http://www.mattsheckells.com/opencv-asift-c-implementation/

 
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
 
namespace cv {
 
class ASiftDetector
{
public:
  ASiftDetector();
 
  void detectAndCompute(const Mat& img, std::vector< KeyPoint >& keypoints, Mat& descriptors);
 
private:
  void affineSkew(double tilt, double phi, Mat& img, Mat& mask, Mat& Ai);
};

}