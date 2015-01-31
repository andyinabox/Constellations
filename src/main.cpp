#include "ofMain.h"
#include "Vespers.h"

//========================================================================
int main( ){
    
    #ifndef OF_TARGET_LINUXARMV6L
        ofSetupOpenGL(1024,768,OF_WINDOW);
    #else
        ofSetupOpenGL(840,720,OF_WINDOW);
    #endif


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new Vespers());

}
