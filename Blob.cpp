#include "Blob.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
Blob::Blob(std::vector<cv::Point> _contour) {

	currentContour = _contour;

	currentBoundingRect = cv::boundingRect(currentContour);//ÏÖÖµ

	cv::Point currentCenter;

	currentCenter.x = (currentBoundingRect.x + currentBoundingRect.x + currentBoundingRect.width) / 2;//µ±Ç°ÖÐÐÄ¼ÆËãx
	currentCenter.y = (currentBoundingRect.y + currentBoundingRect.y + currentBoundingRect.height) / 2;//µ±Ç°ÖÐÐÄ¼ÆËãy

	centerPositions.push_back(currentCenter);

	dblCurrentDiagonalSize = sqrt(pow(currentBoundingRect.width, 2) + pow(currentBoundingRect.height, 2));//µ±Ç°¶Ô½ÇÏß³ß´ç

	dblCurrentAspectRatio = (float)currentBoundingRect.width / (float)currentBoundingRect.height;

	blnStillBeingTracked = true;
	blnCurrentMatchFoundOrNewBlob = true;
	intNumOfConsecutiveFramesWithoutAMatch = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Blob::predictNextPosition(void) {

	int numPositions = (int)centerPositions.size();
	int weightSum = (numPositions - 1) * numPositions / 2 > 0 ? (numPositions - 1) * numPositions / 2 : 1;
	int xShifting = 0, yShifting = 0;
	int limit = numPositions > 4 ? 4 : numPositions - 1;


	for (int weight = 1; weight <= limit; weight += 1)
	{
		xShifting += weight * (centerPositions[numPositions - limit + weight - 1].x - centerPositions[numPositions - limit + weight - 2].x);
		yShifting += weight * (centerPositions[numPositions - limit + weight - 1].y - centerPositions[numPositions - limit + weight - 2].y);
	}
	predictedNextPosition.x = centerPositions.back().x + (int)round((float)xShifting / weightSum);
	predictedNextPosition.y = centerPositions.back().y + (int)round((float)yShifting / weightSum);

	return;
	
}
