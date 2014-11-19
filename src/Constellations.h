#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"

class Constellations : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		vector<ofVec2f> findStars(cv::Mat &src);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// this will be the image that we prep for feature tracking
		ofImage smooth;
		ofImage grey;

		// store our stars
		vector<ofVec2f> stars;

		// a grabber for our camera
		ofVideoGrabber cam;

		// size of our image
		int camWidth;
		int camHeight;

		// gui
		ofxPanel gui;
		ofxToggle useAutoThreshold;

		
};
