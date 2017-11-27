#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <thread>
#include <SFML/Audio.hpp>
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>
#include "Avatar.h"
#include "Monster.h"
#include "SpriteAnimation.h"

using namespace std;
using namespace sf;
using namespace cv;


//Global varibales for Image Processing
VideoCapture cap(0); //capture the video from web cam
Mat imgOriginal;
Mat imgTmp;
Mat imgLines;
Mat imgThresholded;
Mat templateArray[] = { imread("1.jpg", 0), imread("2.jpg", 0), imread("3.jpg", 0), imread("4.jpg", 0), imread("5.jpg", 0) };
Mat result;
Mat translatedImage;
Mat crop;

cv::Rect drawnShape;
cv::Rect roi;

Point centerOfImage;
Point center;

int iLastX = -1;
int iLastY = -1;
int posX;
int posY;
int templateWidth = templateArray[0].cols;
int templateHeight = templateArray[0].rows;
int percentage;
int handClosed = 0;
int shapeValue = 0;

Moments oMoments;

float dX;
float dY;

double dM01;
double dM10;
double dArea;
double areaHand;
double areaShape;



//Global variables for the game
Event windowEvt;		//Create an Event to be able to interact with the window; close, resize, etc.
time_t sec;

int gameW = 1920, gameH = 1080;
const int number = 7;
int position, axis, side;
int idle = 1, attack, dying;
int score = 0;
float difficulty = 0.0004f;
int monsterNumber = 1;

SpriteAnimation avatarAnimation;
Avatar wizard;

Texture bgr;
Texture hp0, hp1, hp2, hp3;
Texture avatarT, monsterT, monsterRT;
Sprite bgrSprite;
Sprite hpSprite;

Font font;
Text scoreText;

Clock spriteClock;
Vector2u idleMonsterTextureSize;
Monster monster[number];

float deltaTime;
int a_idle = 1, a_attack, a_damaged, a_dying;
int randomShape[number];

int startVariable = 0;

//Image Processing functions
void imageProcessing();
void convertRGB2HSI(Mat in_image);
void drawLine(double contourArea);
void translateImage(Mat input);
void scaleImage(Mat input, float x, float y);
int match(Mat input, double area);

//Game functions 
void startGame();
void game();
void separateMonsters(int i);
void destroyMonster(int i);
void setDifficulty();
void updateScore(int s);
void sound();

int main(int argc, char** argv) {
	for (int i = 0; i < 5; i++) {
		threshold(templateArray[i], templateArray[i], 127, 255, THRESH_BINARY);
	}

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 452); // Size of the camera
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 254);

	/*std::thread imageThread(imageProcessing);
	std::thread startThread(startGame);

	imageThread.join();
	startThread.join();

	cout << "................" << startVariable << endl;

	if (startVariable == 1) {
		std::thread sound1Thread(sound);
		std::thread gameThread(game);

		gameThread.join();
		sound1Thread.join();
	}*/
	std::thread imageThread(imageProcessing);
	std::thread sound1Thread(sound);
	std::thread gameThread(game);

	imageThread.join();
	gameThread.join();
	sound1Thread.join();
	
		
//	PlaySound("132.WAV", NULL, SND_ASYNC);
	
	return 0;
}

void sound() {
	PlaySound("132.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}


void imageProcessing() {
	namedWindow("Control", CV_WINDOW_NORMAL); //create a window called "Control"

	int iLowH = 102;
	int iHighH = 132;

	int iLowS = 76;
	int iHighS = 255;

	int iLowI = 11;
	int iHighI = 179;

	//int iLowH = 0;	
	//int iHighH = 255;

	//int iLowS = 0;
	//int iHighS = 255;

	//int iLowI = 0;
	//int iHighI = 255;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowI", "Control", &iLowI, 255); //Value (0 - 255)
	cvCreateTrackbar("HighI", "Control", &iHighI, 255);

	//Capture a temporary image from the camera
	cap.read(imgTmp);

	//Create a black image with the size as the camera output
	imgLines = Mat::zeros(imgTmp.rows, imgTmp.cols, CV_8UC3);
	translatedImage = Mat::zeros(imgLines.rows, imgLines.cols, CV_8UC1);

	while (true)
	{
		bool bSuccess = cap.read(imgOriginal); // read a new frame from video
		centerOfImage = Point(imgOriginal.cols / 2, imgOriginal.rows / 2);
		flip(imgOriginal, imgOriginal, 1);
		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		convertRGB2HSI(imgOriginal);

		GaussianBlur(imgOriginal, imgOriginal, Size(11, 11), 0, 0);
		inRange(imgOriginal, Scalar(iLowH, iLowS, iLowI), Scalar(iHighH, iHighS, iHighI), imgThresholded); //Threshold the image

																										   //morphological opening (remove small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		findContours(imgThresholded, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

		//Calculate the moments of the thresholded image
		oMoments = moments(imgThresholded);
		dM01 = oMoments.m01;
		dM10 = oMoments.m10;
		dArea = oMoments.m00;

		vector<vector<Point> > contours_poly(contours.size());
		vector<cv::Rect> boundRect(contours.size());

		for (size_t i = 0; i < contours.size(); i++)
		{
			areaHand = contourArea(contours[i], false); // Area of hand
														//cout << "Area : " << areaHand << "\n";
		}

		drawLine(areaHand);
		imshow("Thresholded Image", imgThresholded); //show the thresholded image

		imgOriginal = imgOriginal + imgLines;
		//imshow("Line", imgLines);
		imshow("Original", imgOriginal); //show the original image

		if (waitKey(1) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
}

void convertRGB2HSI(Mat in_image) {
	double PI = 3.14;
	double R = 0, G = 0, B = 0;
	double hue = 0, saturation = 0, intensity = 0;
	double minimum = 0;


	for (int x = 0; x < in_image.cols; x++) {
		for (int y = 0; y < in_image.rows; y++) {
			B = in_image.at<Vec3b>(y, x)[0];
			G = in_image.at<Vec3b>(y, x)[1];
			R = in_image.at<Vec3b>(y, x)[2];

			//Calculate Hue
			if (B <= G) {
				hue = acos(0.5 * ((R - G) + (R - B)) / sqrt((R - G) * (R - G) + (R - B) * (G - B)));
			}
			else if (B > G) {
				hue = 2 * PI - acos(0.5 * ((R - G) + (R - B)) / sqrt((R - G) * (R - G) + (R - B) * (G - B)));
			}

			//Calculating Saturation
			if (R < G && R < B)
				minimum = R;

			if (G < R && G < B)
				minimum = G;

			if (B < R && B < G)
				minimum = B;

			saturation = 1 - 3 * minimum / (R + G + B);

			//Calculating Intensity
			intensity = (R + G + B) / 3;

			if (R + G + B == 0) {
				hue = 0;
				saturation = 0;
				intensity = 0;
			}

			in_image.at<Vec3b>(y, x)[0] = (unsigned char)(hue * 255 / (2 * PI));
			in_image.at<Vec3b>(y, x)[1] = (unsigned char)(saturation * 255);
			in_image.at<Vec3b>(y, x)[2] = (unsigned char)(intensity);
		}
	}
}

void drawLine(double contourArea) {
	if (contourArea > 400 && contourArea < 1300) {
		handClosed = 1;
		posX = (int)(dM10 / dArea);
		posY = (int)(dM01 / dArea);

		if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
		{
			//Draw a line from the previous point to the current point
			line(imgLines, Point(iLastX, iLastY), Point(posX, posY), Scalar(255, 255, 255), 12);
		}

		iLastX = posX;
		iLastY = posY;
	}

	

	if (contourArea > 1700 && contourArea < 2700 && handClosed == 1) {
		handClosed = 0;
		iLastX = -1;
		iLastY = -1;

		translateImage(imgLines);
		scaleImage(translatedImage, dX, dY);
		match(crop, areaShape);
		cout << "The returned value is " << shapeValue << endl;
		//imshow("Drawn Shape", imgLines);
		imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);
		translatedImage = Mat::zeros(imgLines.rows, imgLines.cols, CV_8UC1);
	}
}

void translateImage(Mat input) {
	cvtColor(input, input, CV_BGR2GRAY);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(input, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<cv::Rect> boundRect(contours.size());

	for (size_t i = 0; i < contours.size(); i++)
	{
		areaShape = contourArea(contours[i], false); // Area of drawn shape
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::Rect drawnShape(boundRect[i].tl(), boundRect[i].br());
		center = (drawnShape.br() + drawnShape.tl())*0.5;
		dX = (float)templateWidth / drawnShape.width;
		dY = (float)templateHeight / drawnShape.height;
	}

	if (areaShape < 200) {
		input = Mat::zeros(imgTmp.size(), CV_8UC3);
	}
	else {
		int deltaX = centerOfImage.x - center.x;
		int deltaY = centerOfImage.y - center.y;

		for (int y = 0; y < input.rows; y++) {
			for (int x = 0; x < input.cols; x++) {
				int newX = x + deltaX;
				int newY = y + deltaY;

				if (input.at<uchar>(y, x) == 255) {
					translatedImage.at<uchar>(newY, newX) = input.at<uchar>(y, x);
				}
			}
		}
	}
	//imshow("translatedImage", translatedImage);
}

void scaleImage(Mat input, float x, float y) {
	if (x == 0 || y == 0) {
		x = 1;
		y = 1;
	}

	resize(input, input, Size(), x, y);
	//imshow("scaled", input);
	roi.x = input.cols / 2 - templateWidth / 2;
	roi.y = input.rows / 2 - templateHeight / 2;
	roi.width = templateWidth;
	roi.height = templateHeight;
	crop = input(roi);
	//imshow("cropped", crop);
}

int match(Mat input, double area) {
	if (area > 1000) {
		compare(input, templateArray[0], result, CMP_NE);
		percentage = countNonZero(result);
		imshow("result", result);
		cout << "Percentage match for shape 1: " << percentage << endl;
		if (percentage < 6000) {
			shapeValue = 1;

			cout << percentage << endl;
			cout << "kill shape 1" << endl;
			imshow("result", result);

			return shapeValue;
		}
		else if (percentage > 5000) {
			compare(input, templateArray[1], result, CMP_NE);
			percentage = countNonZero(result);
			cout << "Percentage match for shape 2: " << percentage << endl;
			if (percentage < 6000) {
				shapeValue = 2;

				cout << percentage << endl;
				cout << "kill shape 2" << endl;
				imshow("result", result);

				return shapeValue;
			}
			else if (percentage > 5000) {
				compare(input, templateArray[2], result, CMP_NE);
				percentage = countNonZero(result);
				cout << "Percentage match for shape 3: " << percentage << endl;
				if (percentage < 8000) {
					shapeValue = 3;

					cout << percentage << endl;
					cout << "kill shape 3" << endl;
					imshow("result", result);

					return shapeValue;
				}
				else if (percentage > 5000) {
					compare(input, templateArray[3], result, CMP_NE);
					percentage = countNonZero(result);
					cout << "Percentage match for shape 4: " << percentage << endl;
					if (percentage < 8000) {
						shapeValue = 4;

						cout << percentage << endl;
						cout << "kill shape 4" << endl;
						imshow("result", result);

						return shapeValue;
					}
					else if (percentage > 5000) {
						compare(input, templateArray[4], result, CMP_NE);
						percentage = countNonZero(result);
						cout << "Percentage match for shape 5: " << percentage << endl;
						if (percentage < 15000) {
							shapeValue = 5;

							cout << percentage << endl;
							cout << "kill shape 5" << endl;
							imshow("result", result);

							return shapeValue;
						}
						else if (percentage > 15000) {
							shapeValue = 0;
						}
					}
				}
			}
		}
	}
	else {
		shapeValue = 0;
	}
}

void startGame() {
	Mat startScreen;

	startScreen = imread("Background.png", 1);
	imshow("Start Screen", startScreen);

	cout << "................" << startVariable << endl;

	compare(templateArray[0], templateArray[0], result, CMP_NE);
	int percentageStart = countNonZero(result);
	if (percentageStart < 6000) {
		destroyWindow("Start Screen");
		startVariable = 1;

		cout << "HEEEEEEELLLLLLLLLLOOOOOOOOOOOOOOOO" << endl;
	}

	cout << "................" << startVariable << endl;

	waitKey(0);
}

void game() {
	time(&sec);
	srand((unsigned char)sec);

	//Create a window for the game
	RenderWindow gameWindow(VideoMode((unsigned int)gameW, (unsigned int)gameH), "Game", Style::Default);


	RenderWindow startWindow(VideoMode((unsigned int)gameW, (unsigned int)gameH), "Game", Style::Default);

	//Background 
	bgr.loadFromFile("Background.png");
	bgrSprite.setTexture(bgr);

	//Avatar HP
	hp0.loadFromFile("Hearts0.png");
	hp1.loadFromFile("Hearts1.png");
	hp2.loadFromFile("Hearts2.png");
	hp3.loadFromFile("Hearts3.png");

	updateScore(score);

	//Avatar
	wizard = Avatar();
	wizard.createAvatar(1);

	//Monster
	for (int i = 0; i < sizeof(monster) / sizeof(monster[0]); i++) {
		monster[i].createMonsters();
		separateMonsters(i);

		monster[i].monsterSprite.setPosition((float)(monster[i].monsterX), (float)(monster[i].monsterY));
		monster[i].monstersSpeed(wizard.avatarSprite.getPosition().x, wizard.avatarSprite.getPosition().y, difficulty);

		//cout << monster[i].monsterX << endl;
	}

	//Sprite Animation
	avatarT.loadFromFile("WizardSheet.png");
	monsterT.loadFromFile("MonsterAll.png");

	avatarAnimation = SpriteAnimation(&avatarT, Vector2u(20, 4), 0.05f);

	for (int x = 0; x < number; x++) {
		monster[x].anim = { &monsterT, Vector2u(20, 14), 0.05f };
	}

	//Spawn different shapes
	for (int i = 0; i < sizeof(monster) / sizeof(monster[0]); i++) {
		randomShape[i] = rand() % 300;
		if (randomShape[i] <= 60) {
			randomShape[i] = 0;
		}
		if (randomShape[i] > 60 && randomShape[i] <= 120) {
			randomShape[i] = 1;
		}
		if (randomShape[i] > 120 && randomShape[i] <= 180) {
			randomShape[i] = 2;
		}
		if (randomShape[i] > 180 && randomShape[i] <= 240) {
			randomShape[i] = 3;
		}
		if (randomShape[i] > 240 && randomShape[i] <= 300) {
			randomShape[i] = 4;
		}
		if (monster[i].monsterX > gameW / 2) {
			randomShape[i] += 7;
		}
		cout << "................................." << randomShape[i] << endl;
	}


	while (startWindow.isOpen()) {

		startWindow.draw(bgrSprite);
		startWindow.display();

		if (shapeValue == 1) {
			
			break;
		}
	}

	startWindow.close();

	//Start the game loop in order for the window to stay open
	while (gameWindow.isOpen()) {

		while (gameWindow.pollEvent(windowEvt)) {

			//Switch case for handling the events performed when interacting with the game window
			switch (windowEvt.type) {

			case Event::Closed:				//Close the window when Close is clicked
				gameWindow.close();

				break;

			case Event::Resized:			//Print to the console the size of the window when resizing 
				cout << "New game window width = " << windowEvt.size.width << " New game window height = " << windowEvt.size.height << endl;
				break;

			default:
				break;
			}
		}


		deltaTime = spriteClock.restart().asSeconds();

		//Update the animation of the avatar sprite
		if (a_idle == 1) {
			avatarAnimation.updateAnimation(0, deltaTime);
			wizard.avatarSprite.setTextureRect(avatarAnimation.textureRect);
		}
		if (a_attack == 1) {
			avatarAnimation.updateAnimation(1, deltaTime);
			wizard.avatarSprite.setTextureRect(avatarAnimation.textureRect);
		}
		if (a_damaged == 1) {
			avatarAnimation.updateAnimation(2, deltaTime);
			wizard.avatarSprite.setTextureRect(avatarAnimation.textureRect);
		}
		if (a_dying == 1) {
			avatarAnimation.updateAnimation(3, deltaTime);
			wizard.avatarSprite.setTextureRect(avatarAnimation.textureRect);
		}

		//Update the animation of the monster sprite
		for (int i = 0; i < number; i++) {
			if (monster[i].idle == 1) {
				monster[i].anim.updateAnimation(randomShape[i], deltaTime);
				monster[i].monsterSprite.setTextureRect(monster[i].anim.textureRect);
			}
			if (monster[i].attack == 1) {
				if (monster[i].monsterX <= gameW / 2) {
					monster[i].anim.updateAnimation(5, deltaTime);
					monster[i].monsterSprite.setTextureRect(monster[i].anim.textureRect);
				}
				if (monster[i].monsterX > gameW / 2) {
					monster[i].anim.updateAnimation(12, deltaTime);
					monster[i].monsterSprite.setTextureRect(monster[i].anim.textureRect);
				}
			}
			if (monster[i].dying == 1) {
				if (monster[i].monsterX <= gameW / 2) {
					monster[i].anim.updateAnimation(6, deltaTime);
					monster[i].monsterSprite.setTextureRect(monster[i].anim.textureRect);
				}
				if (monster[i].monsterX > gameW / 2) {
					monster[i].anim.updateAnimation(13, deltaTime);
					monster[i].monsterSprite.setTextureRect(monster[i].anim.textureRect);
				}
			}
		}

		//Move the monster, switch between the different animations, and reposition the monster when it dies
		for (int i = 0; i < monsterNumber; i++) {

			//cout << "Monster X is " << monster[i].monsterX << endl;
			//cout << "The returned value is " << shapeValue << endl;

			monster[i].moveMonsters();

			if (monster[i].monsterSprite.getGlobalBounds().intersects(wizard.decoyAvatarSprite.getGlobalBounds())) {
				//cout << "Monster X for i = " << i << " is " << monster[i].monsterX << endl;

				if (monster[i].idle == 1) {
					monster[i].idle = 0;
					monster[i].attack = 1;
					monster[i].speedX = 0;
					monster[i].speedY = 0;
					monster[i].anim.currentImage.x = 0;
					avatarAnimation.currentImage.x = 0;
				}

				if (monster[i].attack == 1 && monster[i].anim.currentImage.x == 19) {
					monster[i].dying = 1;
					monster[i].attack = 0;
					a_idle = 0;
					a_attack = 0;
					a_damaged = 1;
					monster[i].anim.currentImage.x = 0;
					avatarAnimation.currentImage.x = 0;
				}

				if (monster[i].dying == 1 && monster[i].anim.currentImage.x == 19) {
					monster[i].dying = 0;
					monster[i].idle = 1;
					a_damaged = 0;
					a_idle = 1;

					setDifficulty();
					separateMonsters(i);
					monster[i].monsterSprite.setPosition((float)(monster[i].monsterX), (float)(monster[i].monsterY));

					randomShape[i] = rand() % 300;
					if (randomShape[i] <= 60) {
						randomShape[i] = 0;
					}
					if (randomShape[i] > 60 && randomShape[i] <= 120) {
						randomShape[i] = 1;
					}
					if (randomShape[i] > 120 && randomShape[i] <= 180) {
						randomShape[i] = 2;
					}
					if (randomShape[i] > 180 && randomShape[i] <= 240) {
						randomShape[i] = 3;
					}
					if (randomShape[i] > 240 && randomShape[i] <= 300) {
						randomShape[i] = 4;
					}
					if (monster[i].monsterX > gameW / 2) {
						randomShape[i] += 7;
					}

					//cout << "Monster X for i = " << i << " is " << monster[i].monsterX << endl;

					cout << "Another spawn random____________________ " << randomShape[i] << endl;
					monster[i].anim.currentImage.y = randomShape[i];

					monster[i].monstersSpeed(wizard.avatarSprite.getPosition().x, wizard.avatarSprite.getPosition().y, difficulty);
					monster[i].moveMonsters();

					//score += 100;
					//updateScore(score);

					wizard.avatarLife--;
				}
			}

			destroyMonster(i);
		}

		


		//cout << "The avatar's hp is " << wizard.avatarLife << endl;
		if (wizard.avatarLife == 3) {
			hpSprite.setTexture(hp3);
		}
		else if (wizard.avatarLife == 2) {
			hpSprite.setTexture(hp2);
		}
		else if (wizard.avatarLife == 1) {
			hpSprite.setTexture(hp1);
		}
		else if (wizard.avatarLife == 0) {
			hpSprite.setTexture(hp0);
		}
		if (wizard.avatarLife < 0) {
			wizard.avatarLife = 0;
		}

		gameWindow.clear();

		gameWindow.draw(bgrSprite);
		gameWindow.draw(hpSprite);
		gameWindow.draw(scoreText);
		gameWindow.draw(wizard.avatarSprite);
		for (int i = 0; i < sizeof(monster) / sizeof(monster[0]); i++) {
			gameWindow.draw(monster[i].monsterSprite);
		}
		gameWindow.display();
	}
}

//Spawn the monsters without them overlapping 
void separateMonsters(int i) {
	int x = 0, v = 0;

	idleMonsterTextureSize = monster[0].monsterTexture.getSize();
	idleMonsterTextureSize.x /= 20;
	idleMonsterTextureSize.y /= 14;

	while (i < sizeof(monster) / sizeof(monster[0])) {
		//axis = 0, side = 0	top edge
		//axis = 0, side = 1	bottom edge
		//axis = 1, side = 0	left edge
		//axis = 1, side = 1	right edge

		axis = rand() % 120;
		side = rand() % 120;

		if (axis <= 60) {
			axis = 0;
		}
		else {
			axis = 1;
		}

		if (side <= 60) {
			side = 0;
		}
		else {
			side = 1;
		}

		if (axis == 0) {
			position = rand() % gameW;
		}
		else {
			position = rand() % gameH;
		}

		if (side == 0 && axis == 0) {
			monster[i].monsterX = (float)position;
			monster[i].monsterY = -140;
		}
		else if (side == 1 && axis == 0) {
			monster[i].monsterX = (float)position;
			monster[i].monsterY = (float)gameH + 51;
		}
		else if (side == 0 && axis == 1) {
			monster[i].monsterX = -140;
			monster[i].monsterY = (float)position;
		}
		else if (side == 1 && axis == 1) {
			monster[i].monsterX = (float)gameW + 51;
			monster[i].monsterY = (float)position;
		}

		if (i >= 1) {
			//x-axis
			for (int number = 0; number < i; number++) {
				if (v <= 5) {
					if (monster[i].monsterX + (idleMonsterTextureSize.x * monster[i].scaleW) / 2 > monster[number].monsterX - (idleMonsterTextureSize.x * monster[i].scaleW) / 2 && monster[i].monsterX + (idleMonsterTextureSize.x * monster[i].scaleW) / 2 < monster[number].monsterX + (idleMonsterTextureSize.x * monster[i].scaleW) / 2 || monster[i].monsterX - (idleMonsterTextureSize.x * monster[i].scaleW) / 2 < monster[number].monsterX + (idleMonsterTextureSize.x * monster[i].scaleW) / 2 && monster[i].monsterX - (idleMonsterTextureSize.x * monster[i].scaleW) / 2 > monster[number].monsterX - (idleMonsterTextureSize.x * monster[i].scaleW) / 2) {
						v++;
						separateMonsters(i);
						x++;
					}
					v = 0;
				}
			}

			//y-axis
			for (int number = 0; number < i; number++) {
				if (v <= 5) {
					if (monster[i].monsterY + (idleMonsterTextureSize.y * monster[i].scaleH) / 2 > monster[number].monsterY - (idleMonsterTextureSize.y * monster[i].scaleH) / 2 && monster[i].monsterY + (idleMonsterTextureSize.y * monster[i].scaleH) / 2 < monster[number].monsterY + (idleMonsterTextureSize.y * monster[i].scaleH) / 2 || monster[i].monsterY - (idleMonsterTextureSize.y * monster[i].scaleH) / 2 < monster[number].monsterY + (idleMonsterTextureSize.y * monster[i].scaleH) / 2 && monster[i].monsterY - (idleMonsterTextureSize.y * monster[i].scaleH) / 2 > monster[number].monsterY - (idleMonsterTextureSize.y * monster[i].scaleH) / 2) {
						v++;
						separateMonsters(i);
						x++;
					}
					v = 0;
				}
			}
		}
		i++;
	}
	cout << "The separate is running " << x << " times" << endl;
}

void destroyMonster(int i) {

	if (shapeValue == 1 && (randomShape[i] == 3 || randomShape[i] == 10) && monster[i].idle == 1) {
		a_idle = 0;
		a_attack = 1;
		monster[i].idle = 0;
		monster[i].dying = 1;
		monster[i].speedX = 0;
		monster[i].speedY = 0;
		monster[i].anim.currentImage.x = 0;
		avatarAnimation.currentImage.x = 0;
	}

	if (shapeValue == 2 && (randomShape[i] == 4 || randomShape[i] == 11) && monster[i].idle == 1) {
		a_idle = 0;
		a_attack = 1;
		monster[i].idle = 0;
		monster[i].dying = 1;
		monster[i].speedX = 0;
		monster[i].speedY = 0;
		monster[i].anim.currentImage.x = 0;
		avatarAnimation.currentImage.x = 0;
	}

	if (shapeValue == 3 && (randomShape[i] == 2 || randomShape[i] == 9) && monster[i].idle == 1) {
		a_idle = 0;
		a_attack = 1;
		monster[i].idle = 0;
		monster[i].dying = 1;
		monster[i].speedX = 0;
		monster[i].speedY = 0;
		monster[i].anim.currentImage.x = 0;
		avatarAnimation.currentImage.x = 0;
	}

	if (shapeValue == 4 && (randomShape[i] == 1 || randomShape[i] == 8) && monster[i].idle == 1) {
		a_idle = 0;
		a_attack = 1;
		monster[i].idle = 0;
		monster[i].dying = 1;
		monster[i].speedX = 0;
		monster[i].speedY = 0;
		monster[i].anim.currentImage.x = 0;
		avatarAnimation.currentImage.x = 0;
	}

	if (shapeValue == 5 && (randomShape[i] == 0 || randomShape[i] == 7) && monster[i].idle == 1) {
		a_idle = 0;
		a_attack = 1;
		monster[i].idle = 0;
		monster[i].dying = 1;
		monster[i].speedX = 0;
		monster[i].speedY = 0;
		monster[i].anim.currentImage.x = 0;
		avatarAnimation.currentImage.x = 0;
	}

	if (monster[i].dying == 1 && monster[i].anim.currentImage.x == 19) {

		cout << "CAN YOU FIND ME HERE!!!!" << endl;

		monster[i].dying = 0;
		monster[i].idle = 1;
		a_attack = 0;
		a_damaged = 0;
		a_idle = 1;

		setDifficulty();
		separateMonsters(i);
		monster[i].monsterSprite.setPosition((float)(monster[i].monsterX), (float)(monster[i].monsterY));

		randomShape[i] = rand() % 300;
		if (randomShape[i] <= 60) {
			randomShape[i] = 0;
		}
		if (randomShape[i] > 60 && randomShape[i] <= 120) {
			randomShape[i] = 1;
		}
		if (randomShape[i] > 120 && randomShape[i] <= 180) {
			randomShape[i] = 2;
		}
		if (randomShape[i] > 180 && randomShape[i] <= 240) {
			randomShape[i] = 3;
		}
		if (randomShape[i] > 240 && randomShape[i] <= 300) {
			randomShape[i] = 4;
		}
		if (monster[i].monsterX > gameW / 2) {
			randomShape[i] += 7;
		}

		cout << "Another spawn random____________________ " << randomShape[i] << endl;
		monster[i].anim.currentImage.y = randomShape[i];

		monster[i].monstersSpeed(wizard.avatarSprite.getPosition().x, wizard.avatarSprite.getPosition().y, difficulty);
		monster[i].moveMonsters();

		score += 100;
		updateScore(score);
	}
}

void setDifficulty() {
	difficulty += 0.000005f;
	if (difficulty >= 0.00041) {
		monsterNumber = 2;
	}
	if (difficulty >= 0.00045) {
		monsterNumber = 3;
	}
	if (difficulty >= 0.00051) {
		monsterNumber = 4;
	}
	if (difficulty >= 0.00062) {
		monsterNumber = 5;
	}
	if (difficulty >= 0.00073) {
		monsterNumber = 6;
	}
	if (difficulty >= 0.00085) {
		monsterNumber = 7;
	}
}

void updateScore(int s) {
	//Font & Text
	std::string currentScore = "SCORE  " + to_string(s);

	font.loadFromFile("Pixeled.ttf");
	scoreText = Text(currentScore, font, 24);
	scoreText.setFillColor(Color::Yellow);
	scoreText.setStyle(Text::Italic | Text::Bold);
	scoreText.setOrigin(scoreText.getGlobalBounds().width / 2, scoreText.getGlobalBounds().height / 2);
	scoreText.setPosition((float)gameW - 230, (float)30);
}