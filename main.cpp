#include <QApplication>
#include "ImageGrid.h"
#include "operatii.h"

/* Shortcuts:

Scroll wheel - zoom in/out

Middle button drag - deplasare in imagine cand aceasta este mai mare decat fereastra

Ctrl+A - activeaza / dezactiveaza ajustarea automata a dimensiunii imaginii
		 pe care se afla cursorul mouse-ului
Shift+A - activeaza / dezactiveaza ajustarea automata a dimensiunii
		  tuturor imaginilor
Ctrl+R - reseteaza imaginea curenta la dimensiunile sale initiale

*/

int main(int argc, char* argv[])
{
	VideoCapture video("FIRE.mp4"); 

	if (!video.isOpened()) {
		cerr << "EROARE: Nu s-a putut incarca fisierul video !" << endl;
		return EXIT_FAILURE;
	}

	Mat f,    frame1,frame2, frameIntermediar;

	// Citim frame-urile 1 si 2 ce urmeaza a fi comparate, iar frame-ul intermediar nu face nimic altceva decat sa sara un frame
	// in ideea de a compara 2 frame-uri mai distantate pentru a obtine diferente mai mari
	video.read(frame1);
	video.read(frameIntermediar);
	video.read(frame2);

	while (video.read(frame1))
	{
		frame2 = FireDetect(frame1, 150);

		for (unsigned int y = 0; y < frame1.rows; y += frame1.rows / 8)
			for (unsigned int x = 0; x < frame1.cols; x += frame1.cols / 16)
			{
				if (FireOnRect(frame2, x, y))
					rectangle(frame1, Point(x, y), Point(x + frame1.cols / 16, y + frame1.rows / 8), (0, 0, 255), 2);
			}
		imshow("video", frame1);
		imshow("video2", frame2);
		//Se opreste videoclipul la apasarea tastei SPACEBAR
		if ((char)waitKey(1) == 32)    //codul ASCII pentru tasta SPACEBAR
			break;
	}
	

	waitKey();

	/* Eliberam memoria */
	video.release();
	destroyAllWindows();


}
