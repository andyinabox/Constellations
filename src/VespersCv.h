#include "ofxCv.h"

class VespersCv {

	public:
        static void createBaseImage(
             ofVideoGrabber &src
             , ofImage &dst
             , int destWidth
             , int destHeight
             , bool convertToGray
             );

        static void prepImage(
           ofImage &src
           , ofImage &dst
           , bool smoothing
           , int smoothDiameter
           , float smoothSigmaColor
           , float smoothSigmaSpace
           , float threshAmount
           , int dilateIterations
           , int erodeIterations
           , bool invertDilateErode
           );
    
        static void findContours(
          ofVideoGrabber &src
          , ofImage &dst
          , int halfw
          , int smoothPasses
          , float sigma1
          , float sigma2
          , float tau
          , int black
          , int thresh
          );
    
        static vector<ofPoint> findPoints(
           ofImage &src
           , int maxStars
           , double qualityLevel
           , double minDistance
           , int blockSize
           , int destWidth
           , int destHEight
           );

    
};
