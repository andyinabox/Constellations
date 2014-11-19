#include "Constellations.h"

//--------------------------------------------------------------
void Constellations::setup(){

	// set the camer height / width
	camWidth = 320;
	camHeight = 240;

	// set our framerate
	cam.setDesiredFrameRate(30);
	// initalize the `ofVideoGrabber`
	cam.initGrabber(camWidth,camHeight);
	
	ofxCv::imitate(prep, cam);

	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void Constellations::update(){
	cam.update();
	if(cam.isFrameNew()) {
		cv::Mat camMat = ofxCv::toCv(cam);
		cv::Mat	prepMat = ofxCv::toCv(prep);

		// ofxCv::copy(camMat, prepMat);

		// ofxCv::blur(prepMat, 3);
		cv::bilateralFilter(camMat, prepMat, 5, 100, 100);
		prep.update();
	}
}

//--------------------------------------------------------------
void Constellations::draw(){
	cam.draw(0,0);
	prep.draw(camWidth, 0);
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
