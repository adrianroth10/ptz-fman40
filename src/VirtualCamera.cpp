#include "VirtualCamera.hpp"

//#define _USE_MATH_DEFINES
#include <cmath>

using cv::Mat1d;


VirtualCamera::VirtualCamera(double tiltOffset, double angleIncrement):
       tiltOffsetAngle(tiltOffset), angleInc(angleIncrement)	
{
	tiltangle=0.0;
	panangle=0.0;
	tOM=Mat::eye(3,3,CV_64F);  //tilt Offset matrix
	RotTilt=Mat::eye(3,3,CV_64F); //tilt Rotation matrix
	RotPan=Mat::eye(3,3,CV_64F);  //Pan Rotation matrix
	double stO=sin(-tiltOffset); // sin tilt Offset
	double ctO=cos(-tiltOffset); // cos tilt Offset
	tOM.at<double>(1,1)=ctO;
	tOM.at<double>(1,2)=-stO;
	tOM.at<double>(2,1)=ctO;
	tOM.at<double>(2,2)=stO;
	/*for (int i=1;i<=2;++i){
		tOM.at<double>(i,i)=ctO;
		tOM.at<double>(i,3-i)=(-3+2*i)*stO;
	}*/
		
}


void VirtualCamera::updateView(char key, Mat& Rot, Mat& Zoom) 
{	
	bool tiltchange=false;
	bool panchange=false;
	switch(key) {
		case 'w':
			tiltangle-=angleInc;
			tiltchange=true;
			break;
		case 's':
			tiltangle+=angleInc;
			tiltchange=true;
			break;
		case 'd':
			panangle+=angleInc;
			panchange=true;
			break;
		case 'a':
			panangle-=angleInc;
			panchange=true;
			break;
		case 'r':
			Zoom.at<double>(0, 0) += 0.1;
			Zoom.at<double>(1, 1) += 0.1;			
			break;
		case 't':
			Zoom.at<double>(0, 0) -= 0.1;
			Zoom.at<double>(1, 1) -= 0.1;
			break;
		default :
			break;
	}
	double TAngDeg=(tiltangle+tiltOffsetAngle)*180/M_PI;
	std::cout<<"Tilt angle: "<<TAngDeg<<std::endl;
	if(tiltchange==true) //update RotTiltMatrix;
	{
		double sT=sin(tiltangle);
		double cT=cos(tiltangle);
		RotTilt.at<double>(1,1)=cT;
		RotTilt.at<double>(1,2)=-sT;	
		RotTilt.at<double>(2,1)=cT;
		RotTilt.at<double>(2,2)=sT;
		/*for (int i=1;i<=2;++i){
			RotTilt.at<double>(i,i)=cT;
			RotTilt.at<double>(i,3-i)=(-3+2*i)*sT;
		}*/
	}
	double PAngDeg=panangle*180.0/M_PI;
	std::cout<<"Pan angle : "<<PAngDeg<<std::endl;
	if(panchange) //update RotPanMatrix
	{
		double sP=sin(panangle);
		double cP=cos(panangle);
		RotPan.at<double>(0,0)=cP;
		RotPan.at<double>(0,2)=-sP;
		RotPan.at<double>(2,0)=cP;
		RotPan.at<double>(2,2)=sP;
	/*	for(int i=0;i<=1;++i){
			RotPan.at<double>(i*2,i*2)=cP;
			RotPan.at<double>(i*2,2-i*2)=(-1+i*2)*sP;
		}*/
	}
	Rot=RotTilt*RotPan*tOM;
	return;
}
