#pragma once
#ifndef MY_BLOB
#define MY_BLOB

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////
int find_c(void);
void img_video();
void video_img();
class Blob {
public:
	// member variables ///////////////////////////////////////////////////////////////////////////
	std::vector<cv::Point> currentContour;//ָ����blob��Ϊ�ĸ�������룬������

	cv::Rect currentBoundingRect;//boundingRect����//�������ã�//���������Ĵ�ֱ�߽���С���Σ���������ͼ�����±߽�ƽ�е�

	std::vector<cv::Point> centerPositions; //  center points 

	double dblCurrentDiagonalSize; // current diagonal size;
	double dblCurrentAspectRatio; // aspect ratio

	bool blnCurrentMatchFoundOrNewBlob;  // 

	bool blnStillBeingTracked;

	int intNumOfConsecutiveFramesWithoutAMatch;

	cv::Point predictedNextPosition;

	// function prototypes ////////////////////////////////////////////////////////////////////////
	Blob(std::vector<cv::Point> _contour);
	void predictNextPosition(void);
	void writeExcel(int a);
};

#endif    // MY_BLOB