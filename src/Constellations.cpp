#include "Constellations.h"

//--------------------------------------------------------------
void Constellations::setup(){

	// window dimensions for config mode
	configWindowWidth = 840;
	configWindowHeight = 720;
	// window dimensions for sequence mode
	sequenceWindowWidth = 640;
	sequenceWindowHeight = 480;
	// camera source dimensions
	camWidth = 640;
	camHeight = 480;
	// processing image dimensions
	procWidth = 320;
	procHeight = 240;
	// config mode gui dimensions
	guiWidth = 200;

	// toggle fullscreen mode
	isFullScreen = false;

	// shader vars
	period = 10;
	Constellations::resetSequenceTime();

	// just targeting opengl for now
	shader.load("shadersGL2/shader");
	vignette.load("shadersGL2/vignette");

	// set our framerate and initialize video grabber
	cam.setDesiredFrameRate(30);
	cam.initGrabber(camWidth,camHeight);

	// allocate contours image
	contours.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);

	// what does this do exactly?
	ofSetVerticalSync(true);

	// by default, set the north star to be in the center
	// since we're doubling canvas width/height in the 
	// final version, this should put the star in the center
	northStar = ofPoint(camWidth/2, camHeight/2);

	// setup gui
	gui.setup();
	gui.add(prepLabel.setup("// IMAGE PREP", ""));

	// image prep
	gui.add(useBilateralFilter.setup("Smoothing", true));
	// gui.add(bfDiameter.setup("Diameter", 10, 0, 20));
	bfDiameter = 10;
	// gui.add(bfSigmaColor.setup("Sigma Color", 140.0, 0.0, 300.0));
	bfSigmaColor = 140.0;
	// gui.add(bfSigmaSpace.setup("Sigma Space", 140.0, 0.0, 300.0));
	bfSigmaSpace = 140.0;
	// gui.add(useNormalize.setup("Normalize", true));
	useNormalize = true;
	// gui.add(useManualThreshold.setup("Manual threshold", true));
	gui.add(thresh.setup("Thresh level", 100.0, 0.0, 255.0));
	// gui.add(useAutoThreshold.setup("Auto threshold", false));
	// gui.add(useDilate.setup("Dilate", true));
	gui.add(dilateIterations.setup("Dilate iterations", 1, 0, 10));	
	// gui.add(useErode.setup("Erode", true));
	gui.add(erodeIterations.setup("Erode iterations", 1, 0, 10));	
	gui.add(dilateErodeInvert.setup("Invert order", false));

	// stars
	gui.add(starsLabel.setup("// STARS", ""));
	gui.add(showStars.setup("Show stars", true));
	gui.add(maxStarRadius.setup("Max star radius", 2.0, 0.0, 10.0));
	gui.add(minStarRadius.setup("Min star radius", 0.5, 0.0, 10.0));
	gui.add(maxStars.setup("Max star count", 40, 1, 100));
	// gui.add(maxStarSize.setup("max star size", 1.5, 0.0, 10.0));
	// gui.add(minStarSize.setup("min star size", 0.5, 0.0, 10.0));
	gui.add(qualityLevel.setup("Star quality level", 0.01, 0.0, 1.0));
	gui.add(minDistance.setup("Star min distance", 20.0, 0.0, 100.0));
	gui.add(blockSize.setup("Star block size", 3, 0, 10));

	// contours
	gui.add(contoursLabel.setup("// CONTOURS", ""));
	gui.add(showContours.setup("Show contours", false));

	// sequence mode
	gui.add(sequenceLabel.setup("// SEQUENCE", ""));
	gui.add(sequenceMode.setup("Sequence mode", true));
}

//--------------------------------------------------------------
void Constellations::update(){
	
	cam.update();


	if(cam.isFrameNew() && !sequenceMode) {

		// create base image
		Constellations::findStars();



		// if enabled, find contours
		if(showContours) {
			Constellations::findContours(
				cam
				, contours
				, 4
				, 2
				, 0.68
				, 6.0
				, 0.974
				, -8
				, 150
			);
		}
		

		// update all images
		base.update();
		gray.update();
		contours.update();
	} else {
		// when in sequence mode

	}
}

//--------------------------------------------------------------
void Constellations::draw(){
 
 	// resize window for sequence mode
	if(!isFullScreen && sequenceMode && ofGetWidth() != sequenceWindowWidth) {
		ofSetWindowShape(sequenceWindowWidth, sequenceWindowHeight);
	// resize window for config mode
	} else if (!isFullScreen && !sequenceMode && ofGetWidth() != configWindowWidth) {
		ofSetWindowShape(configWindowWidth, configWindowHeight);
	}


	ofBackground(10, 10, 10);

	if(sequenceMode) {
		// shader.begin();
	    	
	 //    	// set uniforms
	 //    	shader.setUniform1f("time", Constellations::getSequenceTime());
	 //    	shader.setUniform1f("period", period);
	 //    	shader.setUniform1i("active", true);

	 //    	// draw our image plane
	 //    	ofPushMatrix();	
	 //    		cam.draw(0, 0);
	 //    	ofPopMatrix();
	    
	 //    // end the shader
	 //    shader.end();

		vignette.begin();
	    	
	    	// set uniforms
	    	vignette.setUniform1f("radius", ofMap(mouseX, 0, ofGetWidth(), 0, 1));
	    	vignette.setUniform1f("softness", 1);
	    	vignette.setUniform1f("opacity", 1.0);
	    	vignette.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	    	vignette.setUniform1f("time", ofMap(mouseY, 0, ofGetHeight(), 0, 1));
	    	// draw our image plane
	    	cam.draw(0, 0);
	    
	    // end the shader
	    vignette.end();

	} else {
		ofPushMatrix();

			// move everything to the right of gui
			ofTranslate(guiWidth, 0);

			base.draw(0,0);

			// smooth.draw(camWidth, 0);
			gray.draw(procWidth, 0);

			ofPushMatrix();

				ofTranslate(0, procHeight);

				ofSetColor(0, 0, 0);
				// draw rectangle
				canvas = ofRectangle(0, 0, camWidth, camHeight);
				ofSetColor(255, 255, 255);



				if(!sequenceMode) {

					//
					// SHOW STARS
					// 
					if(showStars) {
						Constellations::drawStars(ofColor(255,255,255), minStarRadius, maxStarRadius);
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
				}

			ofPopMatrix();

		ofPopMatrix();

		// show gui
		gui.setPosition(0, 0);
		gui.draw();


		// show HUD
		// 
		string hud = "";
		// display frames per second
		hud += ofToString(ofGetFrameRate(), 2) + " fps";
		// display elapsed time
		hud += " / " + ofToString(t, 1) + "sec";
		// draw hud
		ofDrawBitmapStringHighlight(hud, guiWidth + 5, 15);

	}
}



void Constellations::resetSequenceTime() {
	start_t = ofGetElapsedTimef();
	t = 0;
}

float Constellations::getSequenceTime() {
	return ofGetElapsedTimef() - start_t;
}

void Constellations::findStars() {
	// create base image
	Constellations::createBaseImage(
		cam
		, base
		, procWidth
		, procHeight
		, true
	);	

	// prep image for corner detection
	Constellations::prepImage(
		base
		, gray
		, useBilateralFilter
		, bfDiameter
		, bfSigmaColor
		, bfSigmaSpace
		, thresh
		, dilateIterations
		, erodeIterations
		, dilateErodeInvert
	);


	stars = Constellations::findPoints(
		gray,
		maxStars,
		qualityLevel,
		minDistance,
		blockSize,
		camWidth,
		camHeight
	);
	stars.push_back(northStar);
}

void Constellations::drawStars(
	ofColor color
	, float minRadius
	, float maxRadius
) {
	
	ofPushMatrix();
		ofSetColor(color);

		float starRadius;

		if(stars.size() > 0) {
			for(int i = 0; i< stars.size(); i++) {
				// calculate radius based on star "quality" (order) and
				// star radius max/min
				starRadius = (((maxRadius-minRadius)/stars.size())*i)+minRadius;
				// we are drawing this at 2x scale
				ofDrawCircle(stars[i], starRadius);
			}
		}
	ofPopMatrix();
}

/*
	Create base image to be used for image processing
 */
void Constellations::createBaseImage(
	ofVideoGrabber &src
	, ofImage &dst
	, int destWidth
	, int destHeight
	, bool convertToGray
) {
	// ofImage out;
	ofPixels pix = src.getPixels();

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
void Constellations::prepImage(
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
void Constellations::findContours(
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
vector<ofPoint> Constellations::findPoints(
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



//--------------------------------------------------------------
void Constellations::keyPressed(int key){

	switch(key) {
		case 's': sequenceMode = !sequenceMode; break;
		case ' ' :
			Constellations::findStars();
			Constellations::resetSequenceTime();
			break;
		case 'f': isFullScreen = !isFullScreen; ofToggleFullscreen(); break;
	}
}

//--------------------------------------------------------------
void Constellations::keyReleased(int key){

}

//--------------------------------------------------------------
void Constellations::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void Constellations::mouseDragged(int x, int y, int button){
	int transX = x-guiWidth;
	int transY = y-procHeight;

	if(canvas.inside(transX, transY)) {
		northStar.x = transX;
		northStar.y = transY;
	}
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
