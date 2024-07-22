#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat src;
Rect Piece[16];
Rect emptyPiece;
int selectedIndex=-1;
Point ptold;

void swapPieces(int index1, int index2) {
	Rect temp = Piece[index1];
	Piece[index1] = Piece[index2];
	Piece[index2] = temp;
}

void on_mouse(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN) {
		for (int i = 0; i < 16; i++) {
			if (Piece[i].contains(Point(x, y))) {
				selectedIndex=i;
				ptold = Point(x, y);
				break;
			}
		}
	}
	else if (event == EVENT_LBUTTONUP) {
		if (selectedIndex != -1) {   // 선택된 조각이 있는 경우
			selectedIndex = -1;   // 선택된 조각 초기화

			// 선택된 조각이 비어 있는 조각과 겹치는지 확인
			if (Piece[selectedIndex].x <= emptyPiece.x + emptyPiece.width &&
				Piece[selectedIndex].x + Piece[selectedIndex].width >= emptyPiece.x &&
				Piece[selectedIndex].y <= emptyPiece.y + emptyPiece.height &&
				Piece[selectedIndex].y + Piece[selectedIndex].height >= emptyPiece.y) {

				// 선택된 조각과 비어 있는 조각 교환
				swapPieces(selectedIndex, 15);
				emptyPiece = Piece[15];   // 비어 있는 조각 위치 업데이트
			}
		}
	}
	else if (event == EVENT_MOUSEMOVE) {
		if (flags & EVENT_FLAG_LBUTTON && selectedIndex!=-1) {
			Point diff = Point(x, y) - ptold;
			int pieceWidth = Piece[selectedIndex].width;
			int pieceHeight = Piece[selectedIndex].height;

			// 조각의 이동을 한 칸 크기로 제한
			if (abs(diff.x) >= pieceWidth || abs(diff.y) >= pieceHeight) {
				// 이동 방향을 확인하여 한 칸씩 이동
				if (abs(diff.x) >= pieceWidth) {
					int direction = diff.x > 0 ? pieceWidth : -pieceWidth;
					Piece[selectedIndex].x += direction;
				}
				if (abs(diff.y) >= pieceHeight) {
					int direction = diff.y > 0 ? pieceHeight : -pieceHeight;
					Piece[selectedIndex].y += direction;
				}
				ptold = Point(x, y);

				// 선택된 조각이 비어 있는 조각과 겹치는지 확인
				if (Piece[selectedIndex].x <= emptyPiece.x + emptyPiece.width &&
					Piece[selectedIndex].x + Piece[selectedIndex].width >= emptyPiece.x &&
					Piece[selectedIndex].y <= emptyPiece.y + emptyPiece.height &&
					Piece[selectedIndex].y + Piece[selectedIndex].height >= emptyPiece.y) {

					// 선택된 조각과 비어 있는 조각 교환
					swapPieces(selectedIndex, 15);
					emptyPiece = Piece[15];   // 비어 있는 조각 위치 업데이트
				}

				// 퍼즐 조각과 비어 있는 조각을 다시 그림
				Mat puzzle = src.clone();   // 원본 이미지 복사
				for (int i = 0; i < 16; i++) {
					rectangle(puzzle, Piece[i], Scalar(0, 0, 0), 5);   // 각 조각을 검정색으로 테두리 그림
				}
				rectangle(puzzle, emptyPiece, Scalar(0, 0, 0), -1);   // 비어 있는 조각을 검정색으로 채움

				imshow("src", puzzle);   // 화면에 퍼즐 조각 표시
			}
		}		
	}
}

bool isComplete() {
	return false;
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

	namedWindow("src");   // 윈도우 생성
	setMouseCallback("src", on_mouse);   // 마우스 이벤트 콜백 함수 설정

	Mat puzzle = src.clone();   // 원본 이미지 복사
	for (int i = 0; i < 15; i++) {
		rectangle(puzzle, Piece[i], Scalar(0, 0, 0), 5);   // 각 조각을 검정색으로 테두리 그림
	}
	rectangle(puzzle, Piece[15], Scalar(0, 0, 0), -1);   // 비어 있는 조각을 검정색으로 채움

	imshow("src", puzzle);   // 초기 퍼즐 조각 화면에 표시

	while (true) {
		int key = waitKey(30);
		if (key == 27) break; // ESC key to exit
		if (isComplete()) {
			cout << "Puzzle complete!" << endl;
			break;
		}
	}

	destroyAllWindows();
}