Constellations
==============

Making stars from images.

		  *    .  *       .             *
	                         *
	 *   .        *       .       .       *
	   .     *
	           .     .  *        *
	       .                .        .
	.  *           *                     *
	                             .
	         *          .   *

This is a dev app to work through a component of a larger project I am working on. The goal is to use [openFrameworks](http://openframeworks.cc/)
and [openCV](http://opencv.org/) (or whatever other tools are helpful) to form
constellations from images.

It is also essentially a second crack at [starryNight](https://github.com/andyinabox/starryNight).

References
----------

The look I'm going for is similar to Memo's work on the [Wombats "Techno Fan" video](http://memo.tv/archive/the_wombats_techno_fan), so this was a starting point for me.

I am also using Kyle McDonald's notes from the 2012 Eyeo Festival workshop ["Intermediate Computer Vision with openFrameworks"](https://github.com/kylemcdonald/ofxCv/wiki/Intermediate-Computer-Vision-with-openFrameworks) and the examples (and source) in the [ofxCv repository](https://github.com/kylemcdonald/ofxCv) to learn how to use the ofxCv library. I created [my own fork of the ofxCv repo](https://github.com/andyinabox/ofxCv) so that I can comment some of the example code for my own reference (Kyle doesn't seem to believe in code comments ;) ).

And of course there's the [openCv docs](http://docs.opencv.org/modules/refman.html).

Basic Procedure (work in progress)
----------------------------------

1. Grab video from a camera input
2. Prep the image for feature tracking
	* smooth
	* normalize
	* threshold
	* erode/dilate
3. Use `cv::goodFeaturesToTrack` to find "corners" in the image
4. Draw those corners as "stars" using openFrameworks
5. Algorithmically draw lines between stars (still fuzzy on how this will work) for "constellations".
6. Find contours in the image and overlay as an illustration of the "constellation" (optional)

Next Steps
----------
From an initial go at triangulation using James George's [ofxTriangle library](https://github.com/obviousjim/ofxTriangle) (which wraps Jonathan Shewchuk's [Triangle](http://people.sc.fsu.edu/~jburkardt/c_src/triangle/triangle.html) library), it's clear that I need to reconsider how I'm drawing lines.

However, there may be some clues in the [`ofxTriangle` openCv example](https://github.com/obviousjim/ofxTriangle/tree/master/example-opencv) he's using diffing frames to find motion, and then the openCv contour finder (via `ofxOpenCv::contourFinder`) to find blobs in the resulting image, and then extracting vectors from that for the triangles.

I think using something similar to this could serve as a good approach for my purpose. Some additional thoughts:

 - Calibrate the image by taking an "empty" frame of the environment and using that to diff with the "live" image
 - Combine feature detection and blobs from contour detection. The blobs would be used for any line drawing, but the remaining points could be used to fill out the frame.
 - Some of this is actually handled by Kyle's `ofxCv` classes (`ContourFinder`, `RunningBackground`, etc.) Should play around with those a bit more.
 - This is already starting to seem a bit too complex, think about some techniques for simplifying.

Todo
----
 - [ ] Develop a more specific plan of attach for the "constellations"
 	- [x] Try using the [ofxTriangle](https://github.com/obviousjim/ofxTriangle) library from obviousjim
 - [ ] Finish contour implementation
	- [ ] Develop an effective way to rescale the contour drawing
	- [ ] Add some additional paramaters?
 - [ ] Add a GLSL layer for effects
	- [ ] Star "twinkling" ?
	- [ ] Perhaps some color gradients	
 - [ ] Add "snapshot" functionality, which will take a single image and run all of the transformations on it.