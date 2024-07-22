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
		if (selectedIndex != -1) {   // ���õ� ������ �ִ� ���
			selectedIndex = -1;   // ���õ� ���� �ʱ�ȭ

			// ���õ� ������ ��� �ִ� ������ ��ġ���� Ȯ��
			if (Piece[selectedIndex].x <= emptyPiece.x + emptyPiece.width &&
				Piece[selectedIndex].x + Piece[selectedIndex].width >= emptyPiece.x &&
				Piece[selectedIndex].y <= emptyPiece.y + emptyPiece.height &&
				Piece[selectedIndex].y + Piece[selectedIndex].height >= emptyPiece.y) {

				// ���õ� ������ ��� �ִ� ���� ��ȯ
				swapPieces(selectedIndex, 15);
				emptyPiece = Piece[15];   // ��� �ִ� ���� ��ġ ������Ʈ
			}
		}
	}
	else if (event == EVENT_MOUSEMOVE) {
		if (flags & EVENT_FLAG_LBUTTON && selectedIndex!=-1) {
			Point diff = Point(x, y) - ptold;
			int pieceWidth = Piece[selectedIndex].width;
			int pieceHeight = Piece[selectedIndex].height;

			// ������ �̵��� �� ĭ ũ��� ����
			if (abs(diff.x) >= pieceWidth || abs(diff.y) >= pieceHeight) {
				// �̵� ������ Ȯ���Ͽ� �� ĭ�� �̵�
				if (abs(diff.x) >= pieceWidth) {
					int direction = diff.x > 0 ? pieceWidth : -pieceWidth;
					Piece[selectedIndex].x += direction;
				}
				if (abs(diff.y) >= pieceHeight) {
					int direction = diff.y > 0 ? pieceHeight : -pieceHeight;
					Piece[selectedIndex].y += direction;
				}
				ptold = Point(x, y);

				// ���õ� ������ ��� �ִ� ������ ��ġ���� Ȯ��
				if (Piece[selectedIndex].x <= emptyPiece.x + emptyPiece.width &&
					Piece[selectedIndex].x + Piece[selectedIndex].width >= emptyPiece.x &&
					Piece[selectedIndex].y <= emptyPiece.y + emptyPiece.height &&
					Piece[selectedIndex].y + Piece[selectedIndex].height >= emptyPiece.y) {

					// ���õ� ������ ��� �ִ� ���� ��ȯ
					swapPieces(selectedIndex, 15);
					emptyPiece = Piece[15];   // ��� �ִ� ���� ��ġ ������Ʈ
				}

				// ���� ������ ��� �ִ� ������ �ٽ� �׸�
				Mat puzzle = src.clone();   // ���� �̹��� ����
				for (int i = 0; i < 16; i++) {
					rectangle(puzzle, Piece[i], Scalar(0, 0, 0), 5);   // �� ������ ���������� �׵θ� �׸�
				}
				rectangle(puzzle, emptyPiece, Scalar(0, 0, 0), -1);   // ��� �ִ� ������ ���������� ä��

				imshow("src", puzzle);   // ȭ�鿡 ���� ���� ǥ��
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

	namedWindow("src");   // ������ ����
	setMouseCallback("src", on_mouse);   // ���콺 �̺�Ʈ �ݹ� �Լ� ����

	Mat puzzle = src.clone();   // ���� �̹��� ����
	for (int i = 0; i < 15; i++) {
		rectangle(puzzle, Piece[i], Scalar(0, 0, 0), 5);   // �� ������ ���������� �׵θ� �׸�
	}
	rectangle(puzzle, Piece[15], Scalar(0, 0, 0), -1);   // ��� �ִ� ������ ���������� ä��

	imshow("src", puzzle);   // �ʱ� ���� ���� ȭ�鿡 ǥ��

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