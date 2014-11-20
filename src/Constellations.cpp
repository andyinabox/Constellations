#include "Constellations.h"

// using namespace cv;
// using namespace ofxCv;

//--------------------------------------------------------------
void Constellations::setup(){

	// set the camer height / width
	camWidth = 320;
	camHeight = 240;

	// set our framerate
	cam.setDesiredFrameRate(10);
	// initalize the `ofVideoGrabber`
	cam.initGrabber(camWidth,camHeight);
	
	ofxCv::imitate(smooth, cam);
	gray.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
	// contours.allocate(camWidth*2, camHeight*2, OF_IMAGE_GRAYSCALE);
	contours.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);


	ofSetVerticalSync(true);

	// setup gui
	gui.setup();
	gui.add(prepLabel.setup("// IMAGE PREP", ""));

	gui.add(useBilateralFilter.setup("Bilateral filter (blur)", true));
	gui.add(bfDiameter.setup("Diameter", 10, 0, 20));
	gui.add(bfSigmaColor.setup("Sigma Color", 140.0, 0.0, 300.0));
	gui.add(bfSigmaSpace.setup("Sigma Space", 140.0, 0.0, 300.0));

	gui.add(useNormalize.setup("Normalize", true));

	gui.add(useManualThreshold.setup("Manual threshold", true));
	gui.add(thresh.setup("Thresh top", 100.0, 0.0, 255.0));

	gui.add(useAutoThreshold.setup("Auto threshold", false));

	gui.add(useDilate.setup("Dilate", true));
	gui.add(dilateIterations.setup("Dilate iterations", 1, 0, 10));	
	gui.add(useErode.setup("Erode", true));
	gui.add(erodeIterations.setup("Erode iterations", 1, 0, 10));	
	gui.add(dilateErodeInvert.setup("Invert order", false));

	gui.add(starsLabel.setup("// STARS", ""));
	gui.add(showStars.setup("Show stars", true));
	gui.add(starRadius.setup("Star radius", 1.0, 0.0, 10.0));
	gui.add(maxStars.setup("Max stars", 20, 1, 100));
	// gui.add(maxStarSize.setup("max star size", 1.5, 0.0, 10.0));
	// gui.add(minStarSize.setup("min star size", 0.5, 0.0, 10.0));
	gui.add(qualityLevel.setup("Star quality level", 0.01, 0.0, 1.0));
	gui.add(minDistance.setup("Star min distance", 20.0, 0.0, 100.0));
	gui.add(blockSize.setup("Star block size", 3, 0, 10));

	gui.add(constellationLinesLabel.setup("// CONNECT THE DOTS", ""));
	gui.add(showConstellationLines.setup("Show constellations", false));

	gui.add(contoursLabel.setup("// CONTOURS", ""));
	gui.add(showContours.setup("Show contours", false));

}

//--------------------------------------------------------------
void Constellations::update(){
	cam.update();
	if(cam.isFrameNew()) {

		//
		// IMAGE PREP
		// 

		// now lets create a mat from prep and gray images
		cv::Mat camMat = ofxCv::toCv(cam);
 		cv::Mat	smoothMat = ofxCv::toCv(smooth);
 		cv::Mat grayMat = ofxCv::toCv(gray);


		// using bilateral filter because it's supposed to be better
		// for edge detection than other smoothing/blurring algorithms
		// ("Learning OpenCv", Ch. 5)
		if(useBilateralFilter) {
			cv::bilateralFilter(camMat, smoothMat, bfDiameter, bfSigmaColor, bfSigmaSpace);
		} else {
			ofxCv::copy(camMat, smoothMat);
		}

		// now let's switch to grayscale
		// (maybe we should just do this before blur?)
		ofxCv::convertColor(smoothMat, grayMat, CV_RGB2GRAY);

		// may want to add some manual high/low thresholding here

		if(useNormalize) {
			ofxCv::normalize(grayMat);
		}

		if(useManualThreshold) {
			float threshValue = thresh;
			ofxCv::threshold(grayMat, threshValue, true);
			ofxCv::invert(grayMat);		
		}

		// add some auto thresholding
		if(useAutoThreshold) {
			ofxCv::autothreshold(grayMat);
		}

		// need to assign them explicitly, ofxCv doesn't
		// always like ofxGui values
		int dilateIterationsValue = dilateIterations;
		int erodeIterationsValue = erodeIterations;

		// dilate first if enabled and order IS NOT inverted
		if(useDilate && !dilateErodeInvert) {
			ofxCv::dilate(grayMat, dilateIterationsValue);
		}

		// erode if enabled
		if(useErode) {
			ofxCv::erode(grayMat, erodeIterationsValue);
		}

		// dilate last if enabled and order IS inverted
		if(useDilate && dilateErodeInvert) {
			ofxCv::dilate(grayMat, dilateIterationsValue);
		}

		//
		// CORNER DETECTION
		//
		if(showStars) {
			stars = Constellations::findStars(
				grayMat,
				maxStars,
				qualityLevel,
				minDistance,
				blockSize
			);
		}

		//
		// CONTOURS
		// 

		if(showContours) {
 			// cv::Mat resizeMat = ofxCv::toCv(cam);
 			cv::Mat grayMat2 = cv::Mat(camWidth, camHeight, CV_8UC1);
 			// cv::Mat resizeMat = cv::Mat(camWidth*2, camHeight*2, CV_8UC1);
 			cv::Mat contoursMat = ofxCv::toCv(contours);	
			int halfw = 4;
			int smoothPasses = 2;
			float sigma1 = 0.68;
			float sigma2 = 6.0;
			float tau = 0.974;
			int black = -8;
			int thresh = 150;

			// copy to grayscale image
			ofxCv::copyGray(camMat, grayMat2);

			// resize before we draw contour lines
			// ofxCv::resize(grayMat2, resizeMat);

			// coherent line drawing
			ofxCv::CLD(grayMat2, contoursMat, halfw, smoothPasses, sigma1, sigma2, tau, black);
			
			// invert line
			ofxCv::invert(contoursMat);

			// threshold image
			ofxCv::threshold(contoursMat, thresh);

			// thin the contours
			ofxCv::thin(contoursMat);

		}
		


		smooth.update();
		gray.update();
		contours.update();
	}
}

//--------------------------------------------------------------
void Constellations::draw(){
	ofBackground(10, 10, 10);

	ofPushMatrix();

		// move everything to the right of gui
		ofTranslate(200, 0);

		cam.draw(0,0);
		// smooth.draw(camWidth, 0);
		gray.draw(camWidth, 0);

		ofPushMatrix();

			ofTranslate(0, camHeight);

			ofSetColor(0, 0, 0);
			ofDrawRectangle(0, 0, camWidth*2, camHeight*2);

			//
			// SHOW STARS
			// 
			if(showStars) {
				ofSetColor(255, 255, 255);

				if(stars.size() > 0) {
					for(int i = 0; i< stars.size(); i++) {
						ofDrawCircle(stars[i]*2, starRadius);
					}
				}
			}

			//
			// SHOW CONTOURS
			//
			if(showContours) {
				// enable additive blending
				ofEnableBlendMode(OF_BLENDMODE_ADD);
				// draw the countours image on top of the stars
				contours.draw(0, 0);
				// reset to alpha blending
				ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			}

		ofPopMatrix();

	ofPopMatrix();

	gui.setPosition(0, 0);
	gui.draw();
}

// use cv::goodFeaturesToTrack to return a vector of ofPoints
vector<ofVec2f> Constellations::findStars(
	cv::Mat &src,
	int maxStars,
	double qualityLevel,
	double minDistance,
	int blockSize
) {
	vector<cv::Point2f> cvCorners;
	vector<ofVec2f> ofCorners;

	// these should probably be passed via args
	// double qualityLevel = 0.01;
	// double minDistance = 10;
	// int blockSize = 3;


	// apply corner detection
	// http://docs.opencv.org/modules/imgproc/doc/feature_detection.html?highlight=goodfeaturestotrack#cv2.goodFeaturesToTrack
	cv::goodFeaturesToTrack(
		src,
		// corners – Output vector of detected corners.
		cvCorners,
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
	for(int i = 0; i < cvCorners.size(); i++) {
		ofCorners.push_back(ofVec2f(cvCorners[i].x, cvCorners[i].y));
	}

	// return corners as oF vectors
	return ofCorners;
}

//--------------------------------------------------------------
void Constellations::keyPressed(int key){

}

//--------------------------------------------------------------
void Constellations::keyReleased(int key){

}

//--------------------------------------------------------------
void Constellations::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void Constellations::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void Constellations::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void Constellations::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void Constellations::windowResized(int w, int h){

}

//--------------------------------------------------------------
void Constellations::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void Constellations::dragEvent(ofDragInfo dragInfo){ 

}
