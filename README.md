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

This is a dev app to work through a component of a larger project I am working on. The goal is to use openFrameworks
and openCV (or whatever other tools are helpful) to form
constellations from images.

It is also essentially a second crack at [starryNight]().

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

Todo
----
[ ] Develop a more specific plan of attach for the "constellations"
[ ] Finish contour implementation
	[ ] Develop an effective way to rescale the contour drawing
	[ ] Add some additional paramaters?