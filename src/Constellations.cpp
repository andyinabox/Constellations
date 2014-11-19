#include "Constellations.h"

// using namespace cv;
// using namespace ofxCv;

//--------------------------------------------------------------
void Constellations::setup(){

	// set the camer height / width
	camWidth = 320;
	camHeight = 240;

	// set our framerate
	cam.setDesiredFrameRate(30);
	// initalize the `ofVideoGrabber`
	cam.initGrabber(camWidth,camHeight);
	
	ofxCv::imitate(smooth, cam);
	grey.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);

	ofSetVerticalSync(true);

	// setup gui
	gui.setup();
	gui.add(useAutoThreshold.setup("Auto threshold", true));
}

//--------------------------------------------------------------
void Constellations::update(){
	cam.update();
	if(cam.isFrameNew()) {

		//
		// prep the image
		// 

		// now lets create a mat from prep and grey images
		cv::Mat camMat = ofxCv::toCv(cam);
 		cv::Mat	smoothMat = ofxCv::toCv(smooth);
 		cv::Mat greyMat = ofxCv::toCv(grey);

		// using bilateral filter because it's supposed to be better
		// for edge detection than other smoothing/blurring algorithms
		// ("Learning OpenCv", Ch. 5)
		cv::bilateralFilter(camMat, smoothMat, 5, 100, 100);

		// now let's switch to greyscale
		// (maybe we should just do this before blur?)
		ofxCv::convertColor(smoothMat, greyMat, CV_RGB2GRAY);

		// may want to add some manual high/low thresholding here

		ofxCv::normalize(greyMat);


		// add some auto thresholding
		if(useAutoThreshold) {
			ofxCv::autothreshold(greyMat);
		}

		ofxCv::dilate(greyMat, 2);

		ofxCv::erode(greyMat, 2);

		//
		// corner detection
		//
		stars = Constellations::findStars(greyMat);

		cout << "found " << stars.size() << " stars!" << endl;

		// for(int i = 0; i< stars.size(); i++) {
		// 	cout << "*" << i << ": " << stars[i].x << ", " << stars[i].y << endl;
		// }
		


		smooth.update();
		grey.update();
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
		grey.draw(camWidth, 0);

		ofPushMatrix();

			ofTranslate(0, camHeight);

			ofSetColor(0, 0, 0);
			ofDrawRectangle(0, 0, camWidth*2, camHeight*2);

			ofSetColor(255, 255, 255);
			float circleRadius = 1;

			if(stars.size() > 0) {
				for(int i = 0; i< stars.size(); i++) {
					ofDrawCircle(stars[i]*2, circleRadius);
				}
			}

		ofPopMatrix();

	ofPopMatrix();

	gui.setPosition(0, 0);
	gui.draw();
}

// use cv::goodFeaturesToTrack to return a vector of ofPoints
vector<ofVec2f> Constellations::findStars(cv::Mat &src) {
	vector<cv::Point2f> corners;
	vector<ofVec2f> ofCorners;

	// these should probably be passed via args
	int maxCorners = 23;
	int maxTrackbar = 100;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;

	// apply corner detection
	goodFeaturesToTrack(
		src,
		corners,
		maxCorners,
		qualityLevel,
		minDistance,
		cv::Mat(),
		blockSize,
		useHarrisDetector,
		k
	);

	for(int i = 0; i < corners.size(); i++) {
		ofCorners.push_back(ofVec2f(corners[i].x, corners[i].y));
	}

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
