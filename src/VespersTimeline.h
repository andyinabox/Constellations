#include "ofxSecondWindow.h"
#include "ofxTimeline.h"

class VespersTimeline {

	public:
		void setup();
		void update();
		void draw();

		ofxTimeline timeline;
		ofxSecondWindow timelineWindow;
};
