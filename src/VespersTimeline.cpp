#include "VespersTimeline.h"

//--------------------------------------------------------------
void VespersTimeline::setup(){

	timelineWindow.setup("Timeline", 50, 50, 1024, 768, false);

	ofxTimeline::removeCocoaMenusFromGlut("CurvesColorsDemo");
	timeline.setup();
	timeline.setFrameRate(30);
	timeline.setDurationInFrames(90);
	timeline.setLoopType(OF_LOOP_NORMAL);

	//each call to "add keyframes" add's another track to the timeline
	timeline.addCurves("Rotate X", ofRange(0, 360));
	timeline.addCurves("Rotate Y", ofRange(0, 360));

	//Flags are little markers that you can attach text to
		//They are only useful when listening to bangFired() events
		//so that you know when one has passed
	timeline.addColors("Colors");

	//setting framebased to true results in the timeline never skipping frames
	//and also speeding up with the
	//try setting this to true and see the difference
	timeline.setFrameBased(false);

}

//--------------------------------------------------------------
void VespersTimeline::update(){

}

//--------------------------------------------------------------
void VespersTimeline::draw(){

	timelineWindow.begin();
		ofBackground(0);
		timeline.draw();
	timelineWindow.end();

}
