#include "Vespers.h"

//--------------------------------------------------------------
void Vespers::setup(){
    
//    cout << glGetString(GL_EXTENSIONS);
    
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
    ofEnableSmoothing();


	// window dimensions for config mode
	configWindowWidth = 1160;
	configWindowHeight = 480;
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
    sequenceMode = false;
    
	// just targeting opengl for now
	camShader.load("shadersGL2/camShader");
	starShader.load("shadersGL2/starShader.vert", "shadersGL2/starShader.frag", "shadersGL2/starShader.geom");
    starShader.setGeometryInputType(GL_POINTS);
    starShader.setGeometryOutputType(GL_LINE_STRIP);
    starShader.setGeometryOutputCount(2);

	afterImageShader.load("shadersGL2/afterImageShader");

	// set our framerate and initialize video grabber
	cam.setDesiredFrameRate(30);
	cam.initGrabber(camWidth,camHeight);

	// by default, set the north star to be in the center
	// since we're doubling canvas width/height in the
	// final version, this should put the star in the center
	northStar = ofPoint(camWidth/2, camHeight/2);

	mainFbo.allocate(camWidth, camHeight);
	starsFbo.allocate(camWidth, camHeight);
    afterImageFbo.allocate(camWidth, camHeight);
    

    drawGui = false;
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
	gui.add(maxStarRadius.setup("Max star radius", 1.5, 0.0, 10.0));
	gui.add(minStarRadius.setup("Min star radius", 0.5, 0.0, 10.0));
	gui.add(maxStars.setup("Max star count", 40, 1, 100));
	// gui.add(maxStarSize.setup("max star size", 1.5, 0.0, 10.0));
	// gui.add(minStarSize.setup("min star size", 0.5, 0.0, 10.0));
	gui.add(qualityLevel.setup("Star quality level", 0.01, 0.0, 1.0));
	gui.add(minDistance.setup("Star min distance", 20.0, 0.0, 100.0));
	gui.add(blockSize.setup("Star block size", 3, 0, 10));
	gui.add(drawStarsAsPoints.setup("Draw points", false ));
    gui.add(starsCamPan.setup("Stars Cam Pan", 0.f, 360.f, 0.f ));
//    gui.add(starsCamRotate.setup("Stars Cam Rotate", 0.f, 360.f, 0.f ));

    
	timeline.setup();
	timeline.setFrameRate(30);
	timeline.setDurationInFrames(570);
	timeline.setLoopType(OF_LOOP_NORMAL);

	timeline.addCurves("Vignette Radius", ofRange(0, 1));
	timeline.addCurves("Stars Alpha", ofRange(0, 1));
	timeline.addCurves("Random Stars", ofRange(0, 1));
    timeline.addCurves("Stars Rotation", ofRange(-90.f,90.f));
    timeline.addBangs("Capture Stars");
    timeline.addColors("Video Color");
    timeline.addCurves("Color Mix", ofRange(0,1));
    timeline.addCurves("AfterImage Alpha", ofRange(0,1));
    timeline.addCurves("AfterImage Blur", ofRange(0,50));

    ofAddListener(timeline.events().bangFired, this, &Vespers::receivedBang);
    timeline.play();
    
    
//    starsMesh.setMode(OF_PRIMITIVE_POINTS);
	starsCam.setScale(1,-1,1);

    randomStars.resize(50);
}

//--------------------------------------------------------------
void Vespers::update(){

	cam.update();

	if(cam.isFrameNew()) {
		// update all images
		base.update();
		gray.update();
	}
    
 	// resize window for sequence mode
	if(!isFullScreen && sequenceMode && ofGetWidth() != sequenceWindowWidth) {
		ofSetWindowShape(sequenceWindowWidth, sequenceWindowHeight);
        // resize window for config mode
	} else if (!isFullScreen && !sequenceMode && ofGetWidth() != configWindowWidth) {
		ofSetWindowShape(configWindowWidth, configWindowHeight);
	}
    
    if(timeline.getValue("Stars Alpha") > 0) {
        // draw stars
        Vespers::drawStars(
                           ofColor(255, 255, 255)
                           , minStarRadius
                           , maxStarRadius
                           );
    }
    
    // render the main fbo
    if(timeline.getValue("Vignette Radius") > 0) {
        mainFbo.begin();
        ofClear(0, 0, 0, 255);
        camShader.begin();
        
        // set uniforms
        camShader.setUniform2f("center", ofMap(northStar.x, 0, sequenceWindowWidth, 0, 1), ofMap(northStar.y, 0, sequenceWindowHeight, 0, 1));
        camShader.setUniform1f("radius", timeline.getValue("Vignette Radius"));
        camShader.setUniform1f("softness", 1);
        camShader.setUniform1f("opacity", 1.0);
        camShader.setUniform2f("resolution", camWidth, camHeight);
        
        ofColor camColor = timeline.getColor("Video Color");
        camShader.setUniform3f("inputColor",
                               ofMap(camColor.r, 0, 255, 0, 1),
                               ofMap(camColor.g, 0, 255, 0, 1),
                               ofMap(camColor.b, 0, 255, 0, 1)
                               );
        camShader.setUniform1f("colorMix", timeline.getValue("Color Mix"));
        camShader.setUniform1f("time", ofMap(mouseY, 0, ofGetHeight(), 0, 1));
        // draw our image plane
        cam.draw(0, 0);
        
        // end the shader
        camShader.end();
        mainFbo.end();
    }
    
    //    if (timeline.isSwitchOn("Show AfterImage")) {
    if (timeline.getValue("AfterImage Alpha") > 0) {
        
        afterImageFbo.begin();
        afterImageShader.begin();
        afterImageShader.setUniform1f(
                                      "alpha",
                                      timeline.getValue("AfterImage Alpha")
                                      );
        afterImageShader.setUniform1f(
                                      "blur",
                                      timeline.getValue("AfterImage Blur")
                                      );
        afterImageShader.setUniform2f("center", ofMap(northStar.x, 0, sequenceWindowWidth, 0, 1), ofMap(northStar.y, 0, sequenceWindowHeight, 0, 1));
        
        afterImageShader.setUniform2f("resolution", camWidth, camHeight);
        afterImage.draw(0, 0);
        afterImageShader.end();
        afterImageFbo.end();
    }
    
}

//--------------------------------------------------------------
void Vespers::draw(){
    ofClear(0,0,0);


    // move everything to the right of gui
    ofTranslate(0, 0);
    // draw the main image FBO
    if(timeline.getValue("Vignette Radius") > 0) {
        mainFbo.draw(0, 0);
    }

    // draw stars
    if(timeline.getValue("Stars Alpha") > 0) {
        // seemed to fix the star flickering problem when
        // i set up the shader here rather than in the fbo
        starShader.begin();

            starShader.setUniform1f("alpha", timeline.getValue("Stars Alpha"));
            starShader.setUniform1f("time", ofGetElapsedTimef());
            starsFbo.draw(0, 0);

        starShader.end();
    }

    // draw afterimage
    if (timeline.getValue("AfterImage Alpha") > 0) {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        afterImageFbo.draw(0,0);
        ofDisableBlendMode();
    }

    
    if(!sequenceMode) {

        // show gui
        if(drawGui) {
            gui.setPosition(sequenceWindowWidth, 0);
            gui.draw();
        }
        
        // draw base image in greyscale
        base.draw(sequenceWindowWidth+guiWidth,0);
        // draw thresholded image
        gray.draw(sequenceWindowWidth+guiWidth, procHeight);


        // draw the Hud
        Vespers::drawHud(5, 15);

//        if(isFullScreen) {
//            ofTranslate(0, sequenceWindowHeight);
//        }
        
        timeline.draw();
    }

}

void Vespers::receivedBang(ofxTLBangEventArgs& bang) {
    ofLogNotice("Bang fired from track " + bang.track->getName());
    if(bang.track->getName() == "Capture Stars"){
        Vespers::findStars();
        afterImage.setFromPixels(cam.getPixelsRef());
    }
}


void Vespers::drawHud(int x, int y) {
    
    // draw HUD
    string hud = "";
    // display frames per second
    hud += ofToString(ofGetFrameRate(), 2) + " fps";
    // display elapsed time
    hud += " / " + ofToString(t, 1) + "sec";
    // draw hud
    ofDrawBitmapStringHighlight(hud, x, y);
}


void Vespers::findStars() {
    
    // add extra stars
    for(int j = 0; j < randomStars.size(); j++) {
        randomStars[j] = ofPoint(ofRandom(camWidth*1.5), ofRandom(camHeight*1.5), 0);
    }
    
	// create base image
    VespersCv::createBaseImage(
		cam
		, base
		, procWidth
		, procHeight
		, true
	);

	// prep image for corner detection
	VespersCv::prepImage(
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


	stars = VespersCv::findPoints(
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

void Vespers::drawStars(
	ofColor color
	, float minRadius
	, float maxRadius
) {
    

    float starRadius;
    float starDistance;

//	float rotateAmount = ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, 360);
	
//	float rotateAmount = 0;

    
	//move the camera around the mesh
	ofVec3f camDirection(0,0,1);
	ofVec3f centre(cam.getWidth()/2.f,cam.getHeight()/2.f, 255/2.f);
	ofVec3f camDirectionRotated = camDirection.getRotated(starsCamPan, ofVec3f(1,0,0));
//    camDirectionRotated = camDirectionRotated.getRotated(starsCamRotate, ofVec3f(0,0,1));
//	ofVec3f camDirectionRotated = camDirection.rotated(starsCamRotate, ofVec3f(0,0,1));
    // found 280 distance just via guess & check
	ofVec3f camPosition = centre + camDirectionRotated * 280;
	
	starsCam.setPosition(camPosition);
//    starsCam.roll(starsCamRotate);
	starsCam.lookAt(centre);
    starsCam.roll(timeline.getValue("Stars Rotation"));
    
//    if(stars.size() > 0) {
//        for(int i = 0; i< stars.size(); i++) {
//            // calculate radius based on star "quality" (order) and
//            // star radius max/min
//            starRadius = (((maxRadius-minRadius)/stars.size())*i)+minRadius;
//            
//            starDistance = 100/stars.size()*i;
//
//            ofEnableDepthTest();
//                starsMesh.addVertex(ofVec3f(stars[i].x, stars[i].y, starDistance));
//            ofDisableDepthTest();
//        }
//    }
    
    starsFbo.begin();
        ofClear(0,0,0,0);

//        starShader.begin();

            starShader.setUniform3f("color",
                ofMap(color.r, 0, 255, 0, 1)
                , ofMap(color.g, 0,  255, 0, 1)
                , ofMap(color.b, 0,  255, 0, 1)
            );
    
            float alpha = timeline.getValue("Stars Alpha");

            starShader.setUniform1f("alpha", alpha);
            starShader.setUniform1f("time", ofGetElapsedTimef());
    
            // draw the mesh
            starsCam.begin();
            ofEnableDepthTest();

//                starsMesh.drawVertices();
            if(stars.size() > 0) {
                for(int i = 0; i< stars.size()*alpha; i++) {
                    // calculate radius based on star "quality" (order) and
                    // star radius max/min
                    starRadius = (((maxRadius-minRadius)/stars.size())*i)+minRadius;
                    
//                    starDistance = 100/stars.size()*i;
                    
//                    starsMesh.addVertex(ofVec3f(stars[i].x, stars[i].y, starDistance));
                    ofDrawSphere(stars[i], starRadius);
                }
            }
    
            float randomStarsAmt = timeline.getValue("Random Stars");
            for(int j=0; j < randomStars.size()*randomStarsAmt; j++) {
                ofDrawSphere(randomStars[j], minRadius);
            }
    
            ofDisableDepthTest();
            starsCam.end();
//        starShader.end();

    starsFbo.end();
}


//--------------------------------------------------------------
void Vespers::keyPressed(int key){

	switch(key) {
		case 's': sequenceMode = !sequenceMode; break;
        case 't' :
            drawGui = !timeline.toggleShow();
            break;
//		case ' ' :
//			Vespers::findStars();
//			Vespers::resetSequenceTime();
//			break;
		case 'f': isFullScreen = !isFullScreen; ofToggleFullscreen(); break;
	}
}

//--------------------------------------------------------------
void Vespers::keyReleased(int key){

}

//--------------------------------------------------------------
void Vespers::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void Vespers::mouseDragged(int x, int y, int button){
	int transX = x-guiWidth;
	int transY = y-procHeight;

	if(canvas.inside(transX, transY)) {
		northStar.x = transX;
		northStar.y = transY;
	}
}

//--------------------------------------------------------------
void Vespers::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void Vespers::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void Vespers::windowResized(int w, int h){

}

//--------------------------------------------------------------
void Vespers::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void Vespers::dragEvent(ofDragInfo dragInfo){

}
