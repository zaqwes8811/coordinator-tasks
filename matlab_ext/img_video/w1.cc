#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <sstream>

using namespace cv;
using namespace std;

int main() 
{
	auto cap = VideoCapture( "../Documents/drop.avi" );

	auto frame_counter = 0;

	// не всегда можно получить
	cout << "fps:" << cap.get( CV_CAP_PROP_FPS ) << endl;
	while( cap.isOpened( ) ){
		Mat frame;
		auto success = cap.read( frame );

		frame_counter += 1;
		// If the last frame is reached, reset the capture and the frame_counter
		if( frame_counter == cap.get( CV_CAP_PROP_FRAME_COUNT ) ){
			frame_counter = 0;  // Or whatever as long as it is the same as next line
			cap.set( CV_CAP_PROP_POS_FRAMES, 0 );
		}

		Mat gray;
		cvtColor( frame, gray, COLOR_BGR2GRAY );

		// proc
		Mat img = gray / 5;
		circle( img, {100, 150}, 10, 255 );
		auto font = FONT_HERSHEY_SIMPLEX;
		stringstream s;
		s << frame_counter;
		string text = "marker:" + s.str();
		putText( img, text, {5,200}, font, 0.4, (225),1 );

		// play
		imshow( "frame", img );
		if( waitKey(1) >= 0 )
			break;
	}

	cap.release();
	destroyAllWindows();

	return 0;
}
