#include "VespersCv.h"


/*
 Create base image to be used for image processing
 */
void VespersCv::createBaseImage(
              ofVideoGrabber &src
              , ofImage &dst
              , int destWidth
              , int destHeight
              , bool convertToGray
              ) {
	// ofImage out;
	ofPixels pix = src.getPixelsRef();
    
	// only resize if necessary
	if(
       pix.getWidth() != destWidth
       || pix.getHeight() != destHeight
       ) {
		pix.resize(destWidth, destHeight);
	}
    
	// convert to grayscale?
	if(convertToGray) {
		pix.setImageType(OF_IMAGE_GRAYSCALE);
	}
    
	dst.setFromPixels(pix);
	// return out;
}

/*
 Prep image for processing
 */
void VespersCv::prepImage(
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
                ) {
	ofxCv::imitate(dst, src);
	cv::Mat srcMat = ofxCv::toCv(src);
	cv::Mat dstMat = ofxCv::toCv(dst);
    
	// smooth the image
	if (smoothing) {
		cv::bilateralFilter(srcMat, dstMat, smoothDiameter, smoothSigmaColor, smoothSigmaSpace);
	} else {
		ofxCv::copy(srcMat, dstMat);
	}
	// normalize the resulting image
	ofxCv::normalize(dstMat);
    
	// thresholding
	ofxCv::threshold(dstMat, threshAmount, true);
	ofxCv::invert(dstMat);
    
	// dilate & erode
	// can invert order for "opening" and "closing"
	if(!invertDilateErode) {
		ofxCv::dilate(dstMat, dilateIterations);
		ofxCv::erode(dstMat, erodeIterations);
	} else {
		ofxCv::erode(dstMat, erodeIterations);
		ofxCv::dilate(dstMat, dilateIterations);
	}
    
    
}

/*
 Find contours of image
 */
void VespersCv::findContours(
               ofVideoGrabber &src
               , ofImage &dst
               , int halfw
               , int smoothPasses
               , float sigma1
               , float sigma2
               , float tau
               , int black
               , int threshold
               ) {
	cv::Mat tmpMat;
	cv::Mat dstMat = ofxCv::toCv(dst);
	ofxCv::copyGray(src, tmpMat);
	ofxCv::imitate(dstMat, tmpMat);
    
	// coherent line drawing
	ofxCv::CLD(tmpMat, dstMat, halfw, smoothPasses, sigma1, sigma2, tau, black);
    
	// invert line
	ofxCv::invert(dstMat);
    
	// threshold image
	ofxCv::threshold(dstMat, threshold);
    
	// thin the contours
	ofxCv::thin(dstMat);
}


/*
 Find features within an image
 */
vector<ofPoint> VespersCv::findPoints(
                            ofImage &src,
                            int maxStars,
                            double qualityLevel,
                            double minDistance,
                            int blockSize,
                            int destWidth,
                            int destHeight
                            ) {
    
    
	float xScale = destWidth/src.getWidth();
	float yScale = destHeight/src.getHeight();
	vector<cv::Point2f> corners;
	vector<ofPoint> points;
	cv::Mat srcMat = ofxCv::toCv(src);
    
	// apply corner detection
	// http://docs.opencv.org/modules/imgproc/doc/feature_detection.html?highlight=goodfeaturestotrack#cv2.goodFeaturesToTrack
	cv::goodFeaturesToTrack(
                            srcMat,
                            // corners – Output vector of detected corners.
                            corners,
                            // maxCorners – Maximum number of corners to return. If there are more
                            // corners than are found, the strongest of them is returned.
                            maxStars,
                            // qualityLevel – Parameter characterizing the minimal accepted
                            // quality of image corners
                            qualityLevel,
                            // minDistance – Minimum possible Euclidean distance
                            // between the returned corners.
                            minDistance,
                            // mask – Optional region of interest. If the image is not empty
                            // (it needs to have the type CV_8UC1 and the same size as image ),
                            // it specifies the region in which the corners are detected.
                            cv::Mat(),
                            // blockSize – Size of an average block for computing a derivative
                            // covariation matrix over each pixel neighborhood. See cornerEigenValsAndVecs() .
                            blockSize
                            // don't use harris detector
                            );
    
    
	// convert cv vectors to of vectors
	for(int i = 0; i < corners.size(); i++) {
		// we are allowing for a range of star radius based on
		// "strength" of corner
		points.push_back(ofPoint(corners[i].x*xScale, corners[i].y*yScale));
	}
    
	// return corners as oF vectors
	return points;
}

