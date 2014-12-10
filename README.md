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

Todo
----
 - [ ] Set up a basic animation sequence (use ofxMSATimer?)
 	- [ ] Vignette entrance and cutoff
 	- [ ] "stars" entrance
 	- [ ] Repeat
 - [ ] Fullscreen in target display for "sequence mode"
 - [ ] Improve performance
 - [ ] Testing in target display
 - [ ] Improve visual effects
 	- [ ] Color for main sunrise/sunset sequence
 	- [ ] Star "twinkling"
 	- [ ] Background stars
 	- [ ] Star rotation
 	- [ ] Will inverted "after-image" be necessary?
 - [ ] Hardware stuff
	- [ ] Port back to rPi?
 	- [ ] Integrate physical lighting control
 	- [ ] Rpi camera