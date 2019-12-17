#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
double toDEG = 180 / M_PI;
double toRAD = M_PI / 180;

struct EOP {
	double X;
	double Y;
	double Z;
	double Omega;
	double Phi;
	double Kappa;
};

Mat SetRotationMatrix(EOP eop);

int main()
{

	Mat leftIMG, rightIMG;
	EOP leftEOP, rightEOP;

	leftIMG = imread("..\\data\\R0047624.jpg");
	rightIMG = imread("..\\data\\R0047625.jpg");

	leftEOP.X = 292171.678;
	leftEOP.Y = 4147334.36;
	leftEOP.Z = 150.8;
	leftEOP.Omega = 0.07400;
	leftEOP.Phi = 0.01972;
	leftEOP.Kappa = -1.9980;

	rightEOP.X = 292154.207;
	rightEOP.Y = 4147297.61;
	rightEOP.Z = 150.1;
	rightEOP.Omega = 0.08315;
	rightEOP.Phi = -0.04533;
	rightEOP.Kappa = -2.01552;

	//********************************************************
	// 1. Rotation & Position Matrix 
	//********************************************************
	Mat RMat_left, RMat_right;
	Mat PMat_left(3, 1, CV_64F), PMat_right(3, 1, CV_64F);

	RMat_left = SetRotationMatrix(leftEOP);
	RMat_right = SetRotationMatrix(rightEOP);

	PMat_left.at<double>(0, 0) = leftEOP.X;
	PMat_left.at<double>(1, 0) = leftEOP.Y;
	PMat_left.at<double>(2, 0) = leftEOP.Z;

	PMat_right.at<double>(0, 0) = rightEOP.X;
	PMat_right.at<double>(1, 0) = rightEOP.Y;
	PMat_right.at<double>(2, 0) = rightEOP.Z;

	//********************************************************
	// 3. EOP to Essential Matrix  
	//********************************************************
	Mat BaseVec(3, 1, CV_64F), BaseVec_unit(3, 1, CV_64F);
	Mat Skew = Mat::zeros(3, 3, CV_64F);
	Mat E_EOP;

	BaseVec = PMat_right - PMat_left;
	double norm = sqrt(BaseVec.at<double>(0, 0)*BaseVec.at<double>(0, 0) + BaseVec.at<double>(1, 0)*BaseVec.at<double>(1, 0) + BaseVec.at<double>(2, 0)*BaseVec.at<double>(2, 0));
	BaseVec_unit.at<double>(0, 0) = BaseVec.at<double>(0, 0) / norm;
	BaseVec_unit.at<double>(1, 0) = BaseVec.at<double>(1, 0) / norm;
	BaseVec_unit.at<double>(2, 0) = BaseVec.at<double>(2, 0) / norm;

	Skew.at<double>(0, 1) = -1 * BaseVec_unit.at<double>(2, 0);
	Skew.at<double>(1, 0) = BaseVec_unit.at<double>(2, 0);

	Skew.at<double>(0, 2) = BaseVec_unit.at<double>(1, 0);
	Skew.at<double>(2, 0) = -1 * BaseVec_unit.at<double>(1, 0);

	Skew.at<double>(1, 2) = -1 * BaseVec_unit.at<double>(0, 0);
	Skew.at<double>(2, 1) = BaseVec_unit.at<double>(0, 0);

	E_EOP = RMat_left.t()*Skew*RMat_right;

	//********************************************************
	// 4. Calcurate Overlap using Essential Matrix  
	//********************************************************
	Mat imgCorner_left[4], imgCorner_right[4];

	imgCorner_left[0] = (Mat_<double>(3, 1) << 0, 0, 1);
	imgCorner_left[1] = (Mat_<double>(3, 1) << leftIMG.cols - 1, 0, 1);
	imgCorner_left[2] = (Mat_<double>(3, 1) << 0, leftIMG.rows - 1, 1);
	imgCorner_left[3] = (Mat_<double>(3, 1) << leftIMG.cols - 1, leftIMG.rows - 1, 1);

	imgCorner_right[0] = (Mat_<double>(3, 1) << 0, 0, 1);
	imgCorner_right[1] = (Mat_<double>(3, 1) << rightIMG.cols - 1, 0, 1);
	imgCorner_right[2] = (Mat_<double>(3, 1) << 0, rightIMG.rows - 1, 1);
	imgCorner_right[3] = (Mat_<double>(3, 1) << rightIMG.cols - 1, rightIMG.rows - 1, 1);

	Mat K;
	double focal = 18.0;
	double dPixel = 0.00479;
	Point2d pricipal_point = Point2d(2464, 1632);

	K = (Mat_<double>(3, 3)
		<< dPixel / focal, 0, -1 * pricipal_point.x*dPixel / focal
		, 0, dPixel / focal, -1 * pricipal_point.y*dPixel / focal
		, 0, 0, 1);

	Mat result = imgCorner_left[1].t() * K.t() * E_EOP * K * imgCorner_right[1];


	return 0;
}


Mat SetRotationMatrix(EOP eop) {

	Mat RMat;
	Mat RMat_X = Mat::zeros(3, 3, CV_64F);
	Mat RMat_Y = Mat::zeros(3, 3, CV_64F);
	Mat RMat_Z = Mat::zeros(3, 3, CV_64F);

	RMat_X.at<double>(0, 0) = 1.;
	RMat_X.at<double>(1, 1) = cos(eop.Omega);
	RMat_X.at<double>(1, 2) = -sin(eop.Omega);
	RMat_X.at<double>(2, 1) = sin(eop.Omega);
	RMat_X.at<double>(2, 2) = cos(eop.Omega);

	RMat_Y.at<double>(1, 1) = 1.;
	RMat_Y.at<double>(0, 0) = cos(eop.Phi);
	RMat_Y.at<double>(0, 2) = sin(eop.Phi);
	RMat_Y.at<double>(2, 0) = -sin(eop.Phi);
	RMat_Y.at<double>(2, 2) = cos(eop.Phi);

	RMat_Z.at<double>(2, 2) = 1.;
	RMat_Z.at<double>(0, 0) = cos(eop.Kappa);
	RMat_Z.at<double>(0, 1) = -sin(eop.Kappa);
	RMat_Z.at<double>(1, 0) = sin(eop.Kappa);
	RMat_Z.at<double>(1, 1) = cos(eop.Kappa);

	RMat = RMat_X*RMat_Y*RMat_Z;	// 3-2-1 Rotation System
	//RMat = RMat_Z*RMat_Y*RMat_X;	// 1-2-3 Rotation System

	return RMat;
}

