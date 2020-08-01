#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<windows.h> 
#include<iostream>
#include<conio.h>           // it may be necessary to change or remove this line if not using Windows
#include "Blob.h"

using namespace cv;
using namespace std;
#define SHOW_STEPS            // un-comment or comment this line to show steps or not

/************************************/

int num_car = 0;
/************************************/

const Scalar SCALAR_black = Scalar(0.0, 0.0, 0.0);//눼접힛繫돛，늪珂鞫刻槨겜
const Scalar SCALAR_white = Scalar(255.0, 255.0, 255.0);
const Scalar SCALAR_yellow = Scalar(0.0, 255.0, 255.0);
const Scalar SCALAR_green = Scalar(0.0, 200.0, 0.0);
const Scalar SCALAR_red = Scalar(0.0, 0.0, 255.0);
/*************************************************************/
//覩暠，覩暠돨뿍똑경，커깃暠
Mat g_srcImage1, g_srcGrayImage1, g_Imageout1,
g_srcImage2, g_srcGrayImage2, g_Imageout2;
Mat difference;
//Canny긋鍍쇱꿎宮밑긴좆

int g_cannyLowThreshold = 80;//TrackBar貫零꽝鑒  

Mat imgFrame1;

Mat imgFrame2;
/***********************************************************/
void on_Canny_1();

void on_Canny_2();

void writeExcel(int a);

void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob>& existingBlobs, std::vector<Blob>& currentFrameBlobs);
//쉥뎠품煉견듐宅君唐견듐튈토

void addBlobToExistingBlobs(Blob& currentFrameBlob, vector<Blob>& existingBlobs, int& intIndex);
//蕨君唐돨견듐警속견듐

void addNewBlob(Blob& currentFrameBlob, vector<Blob>& existingBlobs);
//警속君唐견듐

double distanceBetweenPoints(cv::Point point1, Point point2);
//듐쇌약잼

void drawAndShowContours(cv::Size imageSize, vector<vector<cv::Point> > contours, std::string strImageName);
//뺌놔쫴윌窟

void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName);
//뺌놔쫴윌窟

bool checkIfBlobsCrossedTheLine_left(std::vector<Blob>& blobs, int& intHorizontalLinePosition, int& carCount);
//쇱꿴寧苟角꼇角견듐督썹죄
bool checkIfBlobsCrossedTheLine_right(std::vector<Blob>& blobs, int& intHorizontalLinePosition, int& carCount);
//쇱꿴寧苟角꼇角견듐督썹죄

void drawBlobInfoOnImage(std::vector<Blob>& blobs, cv::Mat& imgFrame2Copy);
//瞳暠獗삥齡견듐斤口

void drawCarCountOnImage(int& carCount, cv::Mat& imgFrame2Copy);
//몽앴暠獗삥齡났종

///////////////////////////////////////////////////////////////////////////////////////////////////
int find_c(void)
{
	int counter = 0;

	VideoCapture capVideo;

	vector<Blob> blobs;

	Point crossingLine_left[2], crossingLine_right[2];

	int carCount = 0;
	capVideo.open("2.mp4");

	if (!capVideo.isOpened()) {
		cout << "###############################Can't find Video source###############################" << endl;
		return -1;
	}



	capVideo.read(imgFrame1);
	capVideo.read(imgFrame2);

	resize(imgFrame1, imgFrame1, Size(1024, 768));
	resize(imgFrame2, imgFrame2, Size(1024, 768));


	int img_long = imgFrame1.cols;
	int img_width = imgFrame1.rows;

	cout << "낀 " << img_long << "욱" << img_width << endl;
	//Mat 잚謹暠튬낀욱뗍혤 imgFrame1.cols,imgFrame1.rows



	int intHorizontalLinePosition_left = img_long * 0.1;
	crossingLine_left[0].y = 0;
	crossingLine_left[0].x = intHorizontalLinePosition_left;
	crossingLine_left[1].y = imgFrame1.cols - 1;
	crossingLine_left[1].x = intHorizontalLinePosition_left;

	int intHorizontalLinePosition_right = img_long * 0.9;
	crossingLine_right[0].y = 0;
	crossingLine_right[0].x = intHorizontalLinePosition_right;
	crossingLine_right[1].y = imgFrame1.cols - 1;
	crossingLine_right[1].x = intHorizontalLinePosition_right;


	char chCheckForEscKey = 0;
	bool blnFirstFrame = true;
	int frameCount = 2;
	int qw = 0;
	Mat imgDifference;
	Mat imgThresh;

	while (capVideo.isOpened() && chCheckForEscKey != 27)
	{
		counter++;
		qw = GetKeyState(VK_ESCAPE);//삿혤캐숩榴檄 
		if (qw < 0)
		{
			cvDestroyWindow("imgFrame2Copy");
			return 0;
		}

		vector<Blob> currentFrameBlobs;

		Mat imgFrame1Copy = imgFrame1.clone();//옙굔변鑒，눼쉔劤돨왕쇌
		Mat imgFrame2Copy = imgFrame2.clone();




		cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);//뿍똑暠瘻뺏
		cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);

		int a = 25;
		GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(3, 3), 0);//멕薑쫀꺼변鑒
		GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(3, 3), 0);

		absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);//좃蠟暠튬삿혤뀌롸暠
		threshold(imgDifference, imgThresh, 15, 255.0, CV_THRESH_BINARY);
		/*	imshow("1", imgDifference);
			imshow("sd", imgThresh);*/


			//	//埴令랗令뺏뇹잿변鑒
			//	//近꽝：（暠튬，埴令，뎠뿍똑令댕黨（샀鬼黨）埴令珂쉥맡뿍똑令립냥돨令，뎠품랗令뺏돨렘駕）
			//erode(imgThresh, imgThresh, structuringElement44);


			//dilate(imgThresh, imgThresh, structuringElement13);
			//dilate(imgThresh, imgThresh, structuringElement32);

		Mat dil_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 2.5));
		Mat erd_element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
		for (unsigned int i = 0; i < 6; i++)//暠獗툇郞뇹잿
		{

			dilate(imgThresh, imgThresh, dil_element);
			erode(imgThresh, imgThresh, erd_element);

		}



		imshow("12", imgThresh);
		Mat imgThreshCopy = imgThresh.clone();//暠獗옙굔
		vector<vector<Point> > contours;

		findContours(imgThreshCopy, contours,
			RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//쫴윌쇱꿎변鑒

		//drawAndShowContours(imgThresh.size(), contours, "imgContours");

		vector<vector<Point> > convexHulls(contours.size());

		for (unsigned int i = 0; i < contours.size(); i++)
		{
			convexHull(contours[i], convexHulls[i]);//攷관
		}

		//drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

		for (auto& convexHull : convexHulls)
		{
			Blob possibleBlob(convexHull);
			if (possibleBlob.currentBoundingRect.area() > 800 &&
				possibleBlob.dblCurrentAspectRatio > 1.3 &&		//뷘李궐
				possibleBlob.dblCurrentAspectRatio < 8.0 &&
				possibleBlob.currentBoundingRect.width > 25 &&
				possibleBlob.currentBoundingRect.height > 13 &&
				(cv::contourArea(possibleBlob.currentContour)
					/ (double)possibleBlob.currentBoundingRect.area()) > 0.3)
			{
				currentFrameBlobs.push_back(possibleBlob);
			}
		}


		//drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

		if (blnFirstFrame == true)
		{
			for (auto& currentFrameBlob : currentFrameBlobs)
			{
				blobs.push_back(currentFrameBlob);
			}
		}
		else
		{
			matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
		}

		/********************
		교쒼롸잼供냥
		********************/
		//drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");
		imgFrame2Copy = imgFrame2.clone();
		drawBlobInfoOnImage(blobs, imgFrame2Copy);//

		//쉥났종움놔윱
		if (counter < 5)
		{
			carCount = num_car;
			cout << carCount << endl;
		}
		else
		{
			bool blnAtLeastOneBlobCrossedTheLine_left =
				checkIfBlobsCrossedTheLine_left(blobs, intHorizontalLinePosition_left, carCount);
			if (blnAtLeastOneBlobCrossedTheLine_left == true)
			{
				line(imgFrame2Copy, crossingLine_left[0], crossingLine_left[1], SCALAR_green, 2);
			}
			else
			{
				line(imgFrame2Copy, crossingLine_left[0], crossingLine_left[1], SCALAR_red, 2);
			}

			bool blnAtLeastOneBlobCrossedTheLine_right =
				checkIfBlobsCrossedTheLine_right(blobs, intHorizontalLinePosition_right, carCount);
			if (blnAtLeastOneBlobCrossedTheLine_right == true)
			{
				line(imgFrame2Copy, crossingLine_right[0], crossingLine_right[1], SCALAR_green, 2);
			}
			else
			{
				line(imgFrame2Copy, crossingLine_right[0], crossingLine_right[1], SCALAR_red, 2);
			}

			drawCarCountOnImage(carCount, imgFrame2Copy);//법窟났종셕鑒
		}


		//	cv::rectangle(imgFrame2Copy, cvPoint(10,10), cvPoint(70, 30), Scalar(255, 0, 0), 1, 1, 0);
			//Rect(int a,int b,int c,int d)a,b槨앤近돨璘실麟깃,c,d槨앤近돨낀뵨욱

		imshow("imgFrame2Copy", imgFrame2Copy);


		/***********써監************/
		currentFrameBlobs.clear();

		imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is

		if ((capVideo.get(CV_CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CV_CAP_PROP_FRAME_COUNT))
		{
			capVideo.read(imgFrame2);
			resize(imgFrame2, imgFrame2, Size(1024, 768));
		}
		else
		{
			std::cout << "end of video\n";
			break;
		}

		blnFirstFrame = false;
		frameCount++;
		chCheckForEscKey = cv::waitKey(1);
	}

	if (chCheckForEscKey != 27)
	{               // if the user did not press esc (i.e. we reached the end of the video)
		cv::waitKey(0);                         // hold the windows open to allow the "end of video" message to show
	}
	// note that if the user did press esc, we don't need to hold the windows open, we can simply let the program end which will close the windows
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(vector<Blob>& existingBlobs, vector<Blob>& currentFrameBlobs)
{
	for (auto& existingBlob : existingBlobs)
	{
		existingBlob.blnCurrentMatchFoundOrNewBlob = false;

		existingBlob.predictNextPosition();
	}

	for (auto& currentFrameBlob : currentFrameBlobs)
	{

		int intIndexOfLeastDistance = 0;
		double dblLeastDistance = 100000.0;

		for (unsigned int i = 0; i < existingBlobs.size(); i++) {

			if (existingBlobs[i].blnStillBeingTracked == true) {

				double dblDistance = distanceBetweenPoints(currentFrameBlob.centerPositions.back(), existingBlobs[i].predictedNextPosition);

				if (dblDistance < dblLeastDistance) {
					dblLeastDistance = dblDistance;
					intIndexOfLeastDistance = i;
				}
			}
		}

		if (dblLeastDistance < currentFrameBlob.dblCurrentDiagonalSize * 1.0) {
			addBlobToExistingBlobs(currentFrameBlob, existingBlobs, intIndexOfLeastDistance);
		}
		else {
			addNewBlob(currentFrameBlob, existingBlobs);
		}

	}

	for (auto& existingBlob : existingBlobs) {

		if (existingBlob.blnCurrentMatchFoundOrNewBlob == false) {
			existingBlob.intNumOfConsecutiveFramesWithoutAMatch++;
		}

		if (existingBlob.intNumOfConsecutiveFramesWithoutAMatch >= 37) {
			existingBlob.blnStillBeingTracked = false;
		}

	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addBlobToExistingBlobs(Blob& currentFrameBlob, std::vector<Blob>& existingBlobs, int& intIndex) {

	existingBlobs[intIndex].currentContour = currentFrameBlob.currentContour;
	existingBlobs[intIndex].currentBoundingRect = currentFrameBlob.currentBoundingRect;

	existingBlobs[intIndex].centerPositions.push_back(currentFrameBlob.centerPositions.back());

	existingBlobs[intIndex].dblCurrentDiagonalSize = currentFrameBlob.dblCurrentDiagonalSize;
	existingBlobs[intIndex].dblCurrentAspectRatio = currentFrameBlob.dblCurrentAspectRatio;

	existingBlobs[intIndex].blnStillBeingTracked = true;
	existingBlobs[intIndex].blnCurrentMatchFoundOrNewBlob = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addNewBlob(Blob& currentFrameBlob, std::vector<Blob>& existingBlobs) {

	currentFrameBlob.blnCurrentMatchFoundOrNewBlob = true;

	existingBlobs.push_back(currentFrameBlob);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
double distanceBetweenPoints(Point point1, Point point2) {

	int intX = abs(point1.x - point2.x);
	int intY = abs(point1.y - point2.y);
	return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(Size imageSize, vector<vector<Point> > contours, string strImageName) {
	Mat image(imageSize, CV_8UC3, SCALAR_black);
	drawContours(image, contours, -1, SCALAR_white, -1);
	imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(Size imageSize, vector<Blob> blobs, string strImageName) {
	Mat image(imageSize, CV_8UC3, SCALAR_black);
	vector<vector<Point> > contours;
	for (auto& blob : blobs) {
		if (blob.blnStillBeingTracked == true) {
			contours.push_back(blob.currentContour);
		}
	}
	drawContours(image, contours, -1, SCALAR_white, -1);
	imshow(strImageName, image);
}

/********************************************************
**
**  변鑒묘콘：났종법窟쇱꿎
**
*********************************************************/

bool checkIfBlobsCrossedTheLine_left(vector<Blob>& blobs, int& intHorizontalLinePosition, int& carCount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;
	for (auto blob : blobs)
	{
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2)
		{
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
			if (blob.centerPositions[prevFrameIndex].x < intHorizontalLinePosition
				&& blob.centerPositions[currFrameIndex].x >= intHorizontalLinePosition)
			{
				carCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
			}
		}
	}
	return blnAtLeastOneBlobCrossedTheLine;
}
bool checkIfBlobsCrossedTheLine_right(vector<Blob>& blobs, int& intHorizontalLinePosition, int& carCount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;
	for (auto blob : blobs)
	{
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2)
		{
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
			if (blob.centerPositions[prevFrameIndex].x > intHorizontalLinePosition
				&& blob.centerPositions[currFrameIndex].x <= intHorizontalLinePosition)
			{
				carCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
			}
		}
	}
	return blnAtLeastOneBlobCrossedTheLine;
}
/************************************************
*
*묘콘：쉥났종움朞놔윱，깻댔丹셕鑒斤口
*
*****************************************************/
void drawBlobInfoOnImage(vector<Blob>& blobs, Mat& imgFrame2Copy)
{
	num_car = blobs.size();
	for (unsigned int i = 0; i < blobs.size(); i++)
	{

		if (blobs[i].blnStillBeingTracked == true) {
			rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_red, 2);
			int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
			double dblFontScale = blobs[i].dblCurrentDiagonalSize / 60.0;
			int intFontThickness = (int)round(dblFontScale * 1.0);
			putText(imgFrame2Copy, to_string(i), blobs[i].centerPositions.back(),
				intFontFace, dblFontScale, SCALAR_green, intFontThickness);

			//暠튬 警속돨匡俚 貫零 俚竟 俚竟댕鬼 俚竟奈 俚竟닛玖

			//location_now_x[i] = blobs[i].centerPositions.back().x;
			//location_now_y[i] = blobs[i].centerPositions.back().y;

			//speed[i] = sqrt(pow((location_now_x[i] - location_last_x[i]), 2)
			//	+ pow((location_now_y[i] - location_last_y[i]), 2))
			//	* 25 * 30 / 720 * 3.6;
			////cout << "났종" << i << "  " << speed[i] << "km/h" << endl;
			//location_last_x[i] = location_now_x[i];
			//location_last_y[i] = location_now_y[i];
		}

	}
	//if (counter > 2)
	//{
	//	for (unsigned int i = 1; i < blobs.size(); i++)
	//	{
	//		cout << "났종" << i << "  " << speed[i] << "km/h" << endl;
	//	}
	//}
}

/***********************************************
**
**변鑒묘콘：법窟났종셕鑒
**
*************************************************/
void drawCarCountOnImage(int& carCount, cv::Mat& imgFrame2Copy) {

	int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
	double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 300000.0;
	int intFontThickness = (int)round(dblFontScale * 1.5);

	Size textSize = getTextSize(to_string(carCount),
		intFontFace, dblFontScale, intFontThickness, 0);

	Point ptTextBottomLeftPosition;

	ptTextBottomLeftPosition.x = imgFrame2Copy.cols - 1 - (int)((double)textSize.width * 1.25);
	ptTextBottomLeftPosition.y = (int)((double)textSize.height * 1.25);

	putText(imgFrame2Copy, to_string(carCount), ptTextBottomLeftPosition,
		intFontFace, dblFontScale, SCALAR_red, intFontThickness);

}

void writeExcel(int a)
{
	//FILE* fp = NULL;
	//fp = fopen("났종섬鑒.xls", "w");
	//fprintf(fp, "났종悧鑒%d\n", a);
	//fclose(fp);
}
void on_Canny_1()
{
	// 邱賈痰 3x3코뵙윱슉臀
	blur(g_srcGrayImage1, g_srcImage1, Size(3, 3));

	// 頓契乖쳬돨Canny炬綾
	Canny(g_srcImage1, g_srcImage1, g_cannyLowThreshold, g_cannyLowThreshold * 3, 3);

	//邱쉥g_dstImage코돨杰唐禱羹零槨0 
	g_Imageout1 = Scalar::all(0);

	//賈痰Canny炬綾渴놔돨긋鍍暠g_cannyDetectedEdges鱗槨拿쯤，윱쉥覩暠g_srcImage옙돕커깃暠g_dstImage櫓
	g_srcImage1.copyTo(g_Imageout1, g_srcImage1);

	//鞫刻槻벎暠
	//imshow("±槻벎暠×1", g_Imageout1);
}
void on_Canny_2()
{
	// 邱賈痰 3x3코뵙윱슉臀
	blur(g_srcGrayImage2, g_srcImage2, Size(3, 3));

	// 頓契乖쳬돨Canny炬綾
	Canny(g_srcImage2, g_srcImage2, g_cannyLowThreshold, g_cannyLowThreshold * 3, 3);

	//邱쉥g_dstImage코돨杰唐禱羹零槨0 
	g_Imageout2 = Scalar::all(0);

	//賈痰Canny炬綾渴놔돨긋鍍暠g_cannyDetectedEdges鱗槨拿쯤，윱쉥覩暠g_srcImage옙돕커깃暠g_dstImage櫓
	g_srcImage2.copyTo(g_Imageout2, g_srcImage2);

	//鞫刻槻벎暠
	//imshow("±槻벎暠×2", g_Imageout2);
}
