#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxTriangle.h"

class Constellations : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		vector<ofPoint> findStars(
			cv::Mat &src
			, int maxStars
			, double qualityLevel
			, double minDistance
			, int blockSize
			, float resize
		);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		int configWindowWidth;
		int configWindowHeight;
		int sequenceWindowWidth;
		int sequenceWindowHeight;

		// a grabber for our camera
		ofVideoGrabber cam;

		// size of our image
		int camWidth;
		int camHeight;
		int procWidth;
		int procHeight;
		int guiWidth;

		ofImage base;
		// image after smoothing
		ofImage smooth;
		// image to set to grayscale and prep for feature detection
		ofImage gray;
		// image to apply coutnours to
		ofImage contours;

		// canvas for final image
		ofRectangle canvas;

		// this will be the placeable "central" star
		ofPoint northStar;
 
		// store our stars
		vector<ofPoint> stars;

		// triangulator
		ofxTriangle triangle;

		// gui
		ofxPanel gui;

		ofxLabel prepLabel;

		ofxToggle useBilateralFilter;
		// ofxIntSlider bfDiameter;
		int bfDiameter;
		// ofxFloatSlider bfSigmaColor;
		float bfSigmaColor;
		// ofxFloatSlider bfSigmaSpace;
		float bfSigmaSpace;
		// ofxToggle useNormalize;
		bool useNormalize;

		// ofxToggle useManualThreshold;
		ofxFloatSlider thresh;

		// ofxToggle useAutoThreshold;

		ofxToggle useDilate;
		ofxIntSlider dilateIterations;
		ofxToggle useErode;
		ofxIntSlider erodeIterations;
		ofxToggle dilateErodeInvert;

		ofxLabel starsLabel;
		ofxToggle showStars;
		ofxFloatSlider maxStarRadius;
		ofxFloatSlider minStarRadius;
		ofxIntSlider maxStars;
		ofxFloatSlider qualityLevel;
		ofxFloatSlider minDistance;
		ofxFloatSlider blockSize;

		ofxLabel constellationLinesLabel;
		ofxToggle showConstellationLines;

		ofxLabel contoursLabel;
		ofxToggle showContours;

		ofxLabel sequenceLabel;
		ofxToggle sequenceMode;

		// shader stuff
		ofShader shader;
		int period;
		int t;
		
};
