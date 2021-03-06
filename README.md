A school project with the following description:

#Systematized pan/tilt/zoom-camera
When recording events using pan/tilt/zoom-camera there are physical limits as to how the camera can move.  This puts restrictions on what kind of camera motions are possible.  Also, if a mistake is made during the recording of an event, it’s hard to fix that after the event is over.  An alternative is to place several fixed cameras at the same location and zoom them in on different parts of the scene of interest.  The images from all those cameras can be stitching together and a synthesized ptz-camera image can be generated by cropping and scaling it.  If the original videos from the static camera is saved this can be performed offline and allow a producer to experiment with different camera-motions after the event was recorded.

##Interesting functions in openCV
* cv::imread
* cv::imwrite
* cv::findHomography
* cv::warpPerspective

##Linkz:
http://www.cambridgeincolour.com/tutorials/image-projections.htm

http://www.kolor.com/wiki-en/action/view/Understanding_Projecting_Modes

http://www.lidi.info.unlp.edu.ar/WorldComp2011-Mirror/CGV3304.pdf

https://github.com/tgdiriba/panoramic

http://liu.diva-portal.org/smash/get/diva2:822602/FULLTEXT01.pdf

###Linux
for fixing opencv with g++ compiler

add file /etc/ld.so.conf.d/opencv.conf containing

`/usr/local/lib` (path to the libraries)

then run `ldconfig -v`

##Jokes
```
int getRandomNumber()
{
	return 4; // chosen by fair dice roll.
		  // guaranteed to be random
}
```
