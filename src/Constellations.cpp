#include "Constellations.h"

// using namespace cv;
// using namespace ofxCv;

//--------------------------------------------------------------
void Constellations::setup(){

	configWindowWidth = 840;
	configWindowHeight = 720;
	sequenceWindowWidth = 640;
	sequenceWindowHeight = 480;

	isFullScreen = false;

	// set the camer height / width
	camWidth = 640;
	camHeight = 480;
	procWidth = 320;
	procHeight = 240;

	guiWidth = 200;




	// set our framerate
	// cam.setDesiredFrameRate(10);
	// initalize the `ofVideoGrabber`
	cam.initGrabber(camWidth,camHeight);

	// how about cloning the camera pixes into another img,
	// resizing it, and using that as the base for the transformations?
	
	// base.allocate(procWidth, procHeight, OF_IMAGE_GRAYSCALE);
	// ofxCv::imitate(smooth, base);
	// smooth.allocate(procWidth, procHeight, OF_IMAGE_COLOR)
	// gray.allocate(procWidth, procHeight, OF_IMAGE_GRAYSCALE);
	// contours.allocate(camWidth*2, camHeight*2, OF_IMAGE_GRAYSCALE);
	contours.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);

	ofSetVerticalSync(true);

	// by default, set the north star to be in the center
	// since we're doubling canvas width/height in the 
	// final version, this should put the star in the center
	northStar = ofPoint(camWidth/2, camHeight/2);

	// setup gui
	gui.setup();
	gui.add(prepLabel.setup("// IMAGE PREP", ""));

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

	gui.add(constellationLinesLabel.setup("// CONNECT THE DOTS", ""));
	gui.add(showConstellationLines.setup("Show constellations", false));

	gui.add(contoursLabel.setup("// CONTOURS", ""));
	gui.add(showContours.setup("Show contours", false));

	gui.add(sequenceLabel.setup("// SEQUENCE", ""));
	gui.add(sequenceMode.setup("Sequence mode", false));

	period = 10;

	// setup to load the correct shaders
	// based on GPU
	#ifdef TARGET_OPENGLES
		shader.load("shadersES2/shader");
	#else
		shader.load("shadersGL2/shader");
	#endif


}

//--------------------------------------------------------------
void Constellations::update(){
	
	cam.update();
	t = ofGetElapsedTimef();

	// if(!cam.isFrameNew()) {
	// 	cout << 'not a new frame' << endl;
	// }

	if(cam.isFrameNew() && !sequenceMode) {
		// ofPixels pix = cam.getPixels();
		// pix.resize(procWidth, procHeight);
		// base.setFromPixels(pix);

		Constellations::createBaseImage(
			cam
			, base
			, procWidth
			, procHeight
			, true
		);

		//
		// IMAGE PREP
		// 

		// now lets create a mat from prep and gray images
		// cv::Mat baseMat = ofxCv::toCv(base);
 		// cv::Mat	smoothMat = ofxCv::toCv(smooth);
 		// cv::Mat grayMat = ofxCv::toCv(gray);


		// using bilateral filter because it's supposed to be better
		// for edge detection than other smoothing/blurring algorithms
		// ("Learning OpenCv", Ch. 5)
		// if(useBilateralFilter) {
		// 	// cv::bilateralFilter(baseMat, grayMat, bfDiameter, bfSigmaColor, bfSigmaSpace);
		// 	Constellations::smoothImage(base, gray, bfDiameter, bfSigmaColor, bfSigmaSpace);
		// } else {
		// 	imitate(grayMat, baseMat);
		// 	ofxCv::copy(baseMat, grayMat);
		// }

		// now let's switch to grayscale
		// (maybe we should just do this before blur?)
		// ofxCv::convertColor(smoothMat, grayMat, CV_RGB2GRAY);

		// may want to add some manual high/low thresholding here

		// if(useNormalize) {
		// 	ofxCv::normalize(grayMat);
		// }

		// if(useManualThreshold) {
			// float threshValue = thresh;
			// ofxCv::threshold(grayMat, threshValue, true);
			// ofxCv::invert(grayMat);		
		// }

		// add some auto thresholding
		// should replace this with `cvAdaptiveThreshold`
		// http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#adaptivethreshold
		// if(useAutoThreshold) {
		// 	ofxCv::autothreshold(grayMat);
		// }

		// need to assign them explicitly, ofxCv doesn't
		// always like ofxGui values
		// int dilateIterationsValue = dilateIterations;
		// int erodeIterationsValue = erodeIterations;

		// dilate first if enabled and order IS NOT inverted
		// if(useDilate && !dilateErodeInvert) {
		// 	ofxCv::dilate(grayMat, dilateIterationsValue);
		// }

		// erode if enabled
		// if(useErode) {
		// 	ofxCv::erode(grayMat, erodeIterationsValue);
		// }

		// dilate last if enabled and order IS inverted
		// if(useDilate && dilateErodeInvert) {
		// 	ofxCv::dilate(grayMat, dilateIterationsValue);
		// }

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

		//
		// CORNER DETECTION
		//
		if(showStars) {
			stars = Constellations::findPoints(
				gray,
				maxStars,
				qualityLevel,
				minDistance,
				blockSize,
				2 // we are doubling the scale
			);
			stars.push_back(northStar);
		}

		// clear the decks
		triangle.clear();

		if(showConstellationLines) {
			triangle.triangulate(stars);
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
			ofxCv::copyGray(cam, grayMat2);

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
		

		base.update();
		// smooth.update();
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
		shader.begin();
	    	
	    	// set uniforms
	    	shader.setUniform1f("time", t);
	    	shader.setUniform1f("period", period);
	    	shader.setUniform1i("active", true);

	    	// draw our image plane
	    	ofPushMatrix();	
	    		cam.draw(0, 0);
	    	ofPopMatrix();
	    
	    // end the shader
	    shader.end();
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
						ofSetColor(255, 255, 255);

						float starRadius;

						if(stars.size() > 0) {
							for(int i = 0; i< stars.size(); i++) {
								// calculate radius based on star "quality" (order) and
								// star radius max/min
								starRadius = (((maxStarRadius-minStarRadius)/stars.size())*i)+minStarRadius;
								// we are drawing this at 2x scale
								ofDrawCircle(stars[i], starRadius);
							}
						}
					}

					//
					// SHOW CONSTELLATION LINES
					// 
					
					if(showConstellationLines) {
						ofPushMatrix();
							ofEnableBlendMode(OF_BLENDMODE_ADD);
							triangle.draw(255, 255, 255);
						ofPopMatrix();
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

		gui.setPosition(0, 0);
		gui.draw();

		string hud = "";
		hud += ofToString(ofGetFrameRate(), 2) + " fps";
		hud += " / " + ofToString(t, 1) + "sec";

		ofDrawBitmapStringHighlight(hud, guiWidth + 5, 15);

	}
}

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

void Constellations::findContours(
	ofImage &src
	, ofImage &dst
	, int halfw
	, int smoothPasses
	, float sigma1
	, float sigma2
	, float tau
	, int black
	, int threshold
) {
	ofxCv::imitate(dst, src);
	cv::Mat srcMat = ofxCv::toCv(src);
	cv::Mat dstMat = ofxCv::toCv(dst);	

	// coherent line drawing
	ofxCv::CLD(srcMat, dstMat, halfw, smoothPasses, sigma1, sigma2, tau, black);
	
	// invert line
	ofxCv::invert(dstMat);

	// threshold image
	ofxCv::threshold(dstMat, threshold);

	// thin the contours
	ofxCv::thin(dstMat);	
}

// void Constellations::smoothImage(
// 	ofImage &src
// 	, ofImage &dst
// 	, int diameter
// 	, float sigmaColor
// 	, float sigmaSpace
// ) {
// 	// create output image
// 	// ofImage out;
// 	// allocate output image based on source
// 	ofxCv::imitate(dst, src);
// 	// get openCv-friendly cv::Mat of each img
// 	cv::Mat srcMat = ofxCv::toCv(src);
// 	cv::Mat dstMat = ofxCv::toCv(dst);

// 	// run bilateral filter
// 	cv::bilateralFilter(srcMat, dstMat, diameter, sigmaColor, sigmaSpace);

// 	// return out;
// }


// use cv::goodFeaturesToTrack to return a vector of ofPoints
vector<ofPoint> Constellations::findPoints(
	ofImage &src,
	int maxStars,
	double qualityLevel,
	double minDistance,
	int blockSize,
	float resize
) {
	vector<cv::Point2f> corners;
	vector<ofPoint> points;
	cv::Mat srcMat = ofxCv::toCv(src);
	// these should probably be passed via args
	// double qualityLevel = 0.01;
	// double minDistance = 10;
	// int blockSize = 3;


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
		points.push_back(ofPoint(corners[i].x, corners[i].y)*resize);
	}

	// return corners as oF vectors
	return points;
}

//--------------------------------------------------------------
void Constellations::keyPressed(int key){

	switch(key) {
		case 's': sequenceMode = !sequenceMode; break;
		case ' ': isFullScreen = !isFullScreen; ofToggleFullscreen(); break;
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

	cout << "mouseDragged: " << x << ", " << y << ", " << button << endl;
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
