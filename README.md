A school project with the following description:

#Systematized pan/tilt/zoom-camera
When recording events using pan/tilt/zoom-camera there are physical limits as to how the camera can move.  This puts restrictions on what kind of camera motions are possible.  Also, if a mistake is made during the recording of an event, it’s hard to fix that after the event is over.  An alternative is to place several fixed cameras at the same location and zoom them in on different parts of the scene of interest.  The images from all those cameras can be stitching together and a synthesized ptz-camera image can be generated by cropping and scaling it.  If the original videos from the static camera is saved this can be performed offline and allow a producer to experiment with different camera-motions after the event was recorded.

##Work plan
1. Connect two images from cameras with known positions relative each other
  * Calculate overlaped part of the images
  * Probably much computer vision:
    1. Find features from both images, by eiter SIFT, SURF, FAST or some other algorithm
    2. Find point corespondences by some classification method. At least 4 for homography transform, at least five for calibrated fundamental matrix, (essential matrix).
    3. Perform panoramic stitching
    4. Investigate cylindrical/spherical projection of stiched image. (see 160 degree videos on youtube for examples)
    5. find out how to perform pan/tilit/zoom (Probably easy if stiched images is projected unto sphere/cylinder)
    6. expand from still image to video streams:
      * find out how to aquire video streams
      * update features by tracking for increased framerate (Possibly by KLT-algorithm)
2. Connect more than two images with known positions
  * Same as before but more generalised
3. Same with unknown positions for two cameras
4. Same for more than two cameras
5. Track "interesting" objects in the generated image

##Interesting functions in openCV
* cv::imread

##Jokes
```
int getRandomNumber()
{
	return 4; // chosen by fair dice roll.
		  // guaranteed to be random
}
```

##Linkz:
http://www.cambridgeincolour.com/tutorials/image-projections.htm
http://www.kolor.com/wiki-en/action/view/Understanding_Projecting_Modes
http://www.lidi.info.unlp.edu.ar/WorldComp2011-Mirror/CGV3304.pdf
https://github.com/tgdiriba/panoramic
http://liu.diva-portal.org/smash/get/diva2:822602/FULLTEXT01.pdf
