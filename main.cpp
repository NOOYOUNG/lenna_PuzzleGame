#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat src;
Rect Piece[16];
int selectedIndex=-1;
Point ptold;


void on_mouse(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN) {
		for (int i = 0; i < 16; i++) {
			if (Piece[i].contains(Point(x, y))) {
				selectedIndex=i;
			}
		}
	}
	else if (event == EVENT_LBUTTONUP) {
		ptold = Point(x, y);
	}
	else if (event == EVENT_MOUSEMOVE) {
		if (flags & EVENT_FLAG_LBUTTON) {
			//if(Piece[selectedIndex])
		}
	}
}

int main(void) {
	src = imread("lenna.bmp");

	if (src.empty()) {
		cerr << "fail" << endl;
		return -1;
	}
	
	int pieceWidth = src.rows / 4;
	int pieceHeight = src.cols / 4;

	int index = 0;
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			Piece[index++] = Rect(col * pieceWidth, row * pieceHeight, pieceWidth, pieceHeight);
		}
	}

	for (int i = 0; i < 15; i++) {
		rectangle(src, Piece[i], Scalar(0, 0, 0), 5);
	}

	rectangle(src, Piece[15], Scalar(0, 0, 0), -1);

	//Mat puzzle=

	namedWindow("src");
	setMouseCallback("src", on_mouse);

	imshow("src", src);
	waitKey();

	destroyAllWindows();
}

