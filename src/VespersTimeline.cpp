#include "VespersTimeline.h"

//--------------------------------------------------------------
void VespersTimeline::setup(){
	timelineWindow.setup("Timeline", 50, 50, 1024, 768, false);
}

//--------------------------------------------------------------
void VespersTimeline::update(){

}

//--------------------------------------------------------------
void VespersTimeline::draw(){

	timelineWindow.begin();
		ofBackground(0);
		ofSetColor(255, 255, 255);
		ofDrawBitmapString("Timeline", 30, 65);
	timelineWindow.end();

}
