#include<bangtal.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Timer.h>
SceneID scene1;
ObjectID piece[9], startButton, whole[9], restartButton;
TimerID timer;
int mixCount;

const char* piece_image[9] = {
	"1.jpg",
	"2.jpg",
	"3.jpg",
	"4.jpg",
	"5.jpg",
	"6.jpg",
	"7.jpg",
	"8.jpg",
	"9.jpg",
};
int pieceX[9] = { 0, 426, 852, 0, 426, 852, 0,  426, 852 };
int pieceY[9] = { 480, 480, 480, 240, 240, 240, 0, 0, 0 };
int blank;

int piece_index(ObjectID object) {
	for (int i = 0; i < 9; i++) {
		if (piece[i] == object) return i;
	}
	return -1;
}

void change(int index) {
	ObjectID p = piece[blank];
	piece[blank] = piece[index];
	piece[index] = p;  
	locateObject(piece[blank], scene1, pieceX[blank], pieceY[blank]);
	locateObject(piece[index], scene1, pieceX[index], pieceY[index]);
	blank = index; 
}

bool movable(int index) {
	if (blank < 0 or blank > 8) return false;
	if (blank % 3 != 0 && index == blank - 1) return true;
	if (blank % 3 != 2 && index == blank + 1) return true;
	if (blank / 3 != 0 && index == blank - 3) return true;
	if (blank / 3 != 2 && index == blank + 3) return true;
	return false;
}

void shuffle() {
	int index;

	do {
		switch (rand() % 4) {
		case 0: index = blank - 1; break;
		case 1: index = blank + 1; break;
		case 2: index = blank - 3; break;
		case 3: index = blank + 3; break;
		}
	} while (!movable(index));
	change(index);

}

bool completed() {
	for (int i = 0; i < 9; i++) {
		if (piece[i] != whole[i]) return false;
	}
	return true;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction) {
	if (object == startButton) {
		mixCount = 15;
		setTimer(timer, 1/10.f);
		startTimer(timer);
		hideObject(startButton);
	}
	else {
		int index = piece_index(object);
		if (movable(index)) {
			change(index);
			if (completed()) {
				showMessage("성공!!");
				showObject(restartButton);
			}
		}
	}
	if (object == restartButton) {
		mixCount = 15;
		setTimer(timer, 1 / 10.f);
		startTimer(timer);
		hideObject(restartButton);
	}
}

void timerCallback(TimerID timer) {
	mixCount--;
	if (mixCount > 0) {
		shuffle(); 

		setTimer(timer, 1/10.f);
		startTimer(timer);
	}
}

int main() {
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene1 = createScene("sunflowerPuzzle", "background.jpg");
	startButton = createObject("start.jpg");
	restartButton = createObject("restart.jfif");

	for (int i = 0; i < 9; i++) {
		piece[i] = createObject(piece_image[i]);
		whole[i] = piece[i];
		locateObject(piece[i], scene1, pieceX[i], pieceY[i]);
		showObject(piece[i]);
	}
	blank = 8;
	hideObject(piece[blank]);
	locateObject(startButton, scene1, 640, 100);
	locateObject(restartButton, scene1, 580, 60);
	showObject(startButton);
	
	timer = createTimer(1/10.f);
	startGame(scene1);
}
