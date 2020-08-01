
#include<windows.h> 

#include <SDKDDKVer.h>

#include <stdio.h>

#include <tchar.h>

#include<iostream>

#include<opencv2\opencv.hpp>

#include<opencv2\video\background_segm.hpp>

#include "Blob.h"

#include <io.h>
#include <fstream>
using namespace cv;
using namespace std;

using namespace std;
#define CAMERA_H  120    
#define CAMERA_W  188    
CvScalar col_white, col_black, col_blue, col_red, col_green, col_gray, col_purple;
////////////////////////////////////////////////////////////////////////////////////
IplImage* pImg_1 = NULL;							//Ô­Í¼Í¼ÏñÖ¸Õë
IplImage* pImg_2 = NULL;							//µ±Ç°Í¼Í¼ÏñÖ¸Õë
char path[1000];									//±£´æÂ·¾¶µÄ×Ö·ûÊý×é
uint8_t image_logo[CAMERA_H][CAMERA_W];
//////////////////////////////////////////////////////
static int flag_one = 0;
static int flag_two = 0;
static int flag_three = 0;
static int flag_four = 0;
static int flag_five = 0;
static int fre_num = 0;
///////////////////////////////////////////////////////

bool descSort(vector<Point> p1, vector<Point> p2);
void find_car();
void Video_Play();
void col_init(void);
void create_window(void);
void create_pic_now_background(void);
void on_mouse(int event, int x, int y, int flag, void* param);
void cvText(IplImage* image, const char* text, int x, int y, double h, double v, int line_width);
/************************************************************/
void col_init(void)
{
	col_white.val[0] = 255; col_white.val[1] = 255; col_white.val[2] = 255;
	col_black.val[0] = 0;   col_black.val[1] = 0;   col_black.val[2] = 0;
	col_blue.val[0] = 255;  col_blue.val[1] = 0;    col_blue.val[2] = 0;
	col_green.val[0] = 0;   col_green.val[1] = 255; col_green.val[2] = 0;
	col_purple.val[0] = 255;  col_purple.val[1] = 0; col_purple.val[2] = 255;
	col_red.val[0] = 0;     col_red.val[1] = 0;     col_red.val[2] = 255;
	col_gray.val[0] = 100;     col_gray.val[1] = 100;     col_gray.val[2] = 100;
}

bool descSort(vector<Point> p1, vector<Point> p2) {

	return contourArea(p1) > contourArea(p2);

}
void create_window(void)
{

	cvNamedWindow("123", 0);
}

void create_pic_now_background(void)
{
	int width = 0;
	int height = 0;
	int i, j;
	CvScalar s;
	width = CAMERA_W + 180;
	height = CAMERA_H + 180;
	pImg_2 = cvCreateImage(cvSize(width, height), 8, 3);


	for (i = 0; i < height; i++)//È«ÆÁÍ¿°×
	{
		for (j = 0; j < width; j++)
		{
			cvSet2D(pImg_2, i, j, col_white);
		}
	}

	cvText(pImg_2, "Made by lihaoran", 10, 150, 0.5, 0.5, 6);
	for (i = 5; i < 20; i++)//start
	{
		for (j = CAMERA_W + 100; j < CAMERA_W + 150; j++)
		{
			cvSet2D(pImg_2, i, j, col_gray);
		}
	}
	cvText(pImg_2, "play ", CAMERA_W + 100, 15, 0.4, 0.4, 1);

	for (i = 25; i < 40; i++)//midline
	{
		for (j = CAMERA_W + 100; j < CAMERA_W + 150; j++)
		{
			cvSet2D(pImg_2, i, j, col_gray);
		}
	}
	cvText(pImg_2, "f_car", CAMERA_W + 100, 35, 0.4, 0.4, 1);

	for (i = 45; i < 60; i++)//turn to
	{
		for (j = CAMERA_W + 100; j < CAMERA_W + 150; j++)
		{
			cvSet2D(pImg_2, i, j, col_gray);
		}
	}
	cvText(pImg_2, "f_car_T", CAMERA_W + 100, 55, 0.4, 0.4, 1);

	for (i = 65; i < 80; i++)//draw
	{
		for (j = CAMERA_W + 100; j < CAMERA_W + 150; j++)
		{
			cvSet2D(pImg_2, i, j, col_gray);
		}
	}
	cvText(pImg_2, "four", CAMERA_W + 100, 75, 0.4, 0.4, 1);

	for (i = 85; i < 100; i++)//restart
	{
		for (j = CAMERA_W + 100; j < CAMERA_W + 150; j++)
		{
			cvSet2D(pImg_2, i, j, col_gray);
		}
	}
	cvText(pImg_2, "five", CAMERA_W + 100, 95, 0.4, 0.4, 1);

}
/*
void logo()
{
	sprintf(path, "%d.jpg", 1);
	pImg_1 = cvLoadImage(path, 0);
	char my_text[60];
	CvScalar s;
	int i, j;
	for (i = 0; i < CAMERA_H; i++)
	{
		for (j = 0; j < CAMERA_W; j++)
		{
			s = cvGet2D(pImg_1, i, j);
			image_logo[i][j] = s.val[0];

		}
	}

	for (i = 0; i < CAMERA_H; i++)
	{
		for (j = 0; j < CAMERA_W; j++)
		{
			if (image_logo[i][j] < 150)  cvSet2D(pImg_2, i, j, col_black);

			else  cvSet2D(pImg_2, i, j, col_white);
		}
	}

	cvShowImage("123", pImg_2);
}
*/

void on_mouse(int event, int x, int y, int flag, void* param)
{
	char my_text[60];
	int i, j;
	CvPoint pt;
	CvScalar s;
	pt.x = x;
	pt.y = y;
	pt = { 0, 0 };
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		pt = cvPoint(x, y);
		// printf("%d  %d\n", pt.x, pt.y);
		if ((pt.x > 188 + 100) && (pt.x < 188 + 150))
		{
			if (pt.y > 5 && pt.y < 20)   flag_one = 1;
			if (pt.y > 25 && pt.y < 40)   flag_two = 1;
			if (pt.y > 45 && pt.y < 60)   flag_three = 1;
			if (pt.y > 65 && pt.y < 80)   flag_four = 1;
			if (pt.y > 85 && pt.y < 100)   flag_five = 1;
		}

	}



	if (flag_one == 1)
	{
		Video_Play();
	}

	if (flag_two == 1)
	{
		find_c();
	}
	if (flag_three == 1)
	{

	}
	if (flag_four == 1)
	{
		
		Sleep(1000);
	}
	if (flag_five == 1)
	{
		
		Sleep(1000);
	}
	flag_one = 0;
	flag_two = 0;
	flag_three = 0;
	flag_four = 0;
	flag_five = 0;
	fre_num = 0;

}

void cvText(IplImage* image, const char* text, int x, int y, double h, double v, int line_width)
{
	CvFont font;
	double hscale = h;
	double vscale = v;
	int linewidth = line_width;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, hscale, vscale, 0, 1, linewidth);
	CvScalar textColor;
	textColor.val[0] = 255;
	textColor.val[1] = 0;
	textColor.val[2] = 0;
	CvPoint textPos = cvPoint(x, y);
	cvPutText(image, text, textPos, &font, textColor);
}

void Video_Play()
{

	VideoCapture capture("test2.avi");
	int a;
	while (1)
	{
		Mat frame;//¶¨ÒåÒ»¸öMat±äÁ¿£¬ÓÃÓÚ´æ´¢Ã¿Ò»Ö¡µÄÍ¼Ïñ
		capture >> frame;  //¶ÁÈ¡µ±Ç°Ö¡
		imshow("114", frame);  //ÏÔÊ¾µ±Ç°Ö¡
		waitKey(30);
		a = GetKeyState(VK_ESCAPE);//»ñÈ¡Ä³¼ü×´Ì¬ 
		if (a < 0)
		{
			cvDestroyWindow("114");
			return;
		}

	}
}

int main()
{

	//col_init();
	//create_window();
	//create_pic_now_background();
	//logo();
	//cvSetMouseCallback("³µÁ÷Á¿Í³¼Æ", on_mouse, 0);

	find_c();
	//video_img();
	//img_video();


}



void img_video()
{
	VideoWriter video("test.avi", CV_FOURCC('X', 'V', 'I', 'D'), 27.0, Size(1280, 720));
	String img_path = "C:\\Users\\Administrator\\Desktop\\12\\";
	vector<String> img;
	glob(img_path, img, false);
	size_t count = img.size();
	for (size_t i = 0; i < count; )
	{
		stringstream str;
		str << i << ".jpg";
		Mat image = imread(img_path + str.str());
		if (!image.empty())
		{
			resize(image, image, Size(1280, 720));
			video << image;
			
		}
		i = i + 3;
	}
	cout << "Count" << endl;

}

void video_img()
{
	VideoCapture capture("My Video.mp4");
	int i = 0;	//std::string image_name;	
	char image_name[20];
	while (1)
	{
		Mat frame;
		capture >> frame;
		//imshow("dqsp", frame); 		//×ª»»¸ñÊ½	
		sprintf_s(image_name, "%s%d%s", "", i, ".jpg");
		std::string str(image_name);
		cv::imwrite("C:\\Users\\Administrator\\Desktop\\12\\" + str, frame);
		waitKey(10);
		i = i + 2;

	}

}