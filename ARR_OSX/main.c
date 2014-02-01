//#include <OpenCV/OpenCV.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ARR/include/ar.h"

#define THICKNESS 2

const char  * WINDOW_NAME  = "ARR_OSX";
//const CFIndex CASCADE_NAME_LEN = 2048;

IplImage *  draw_image; // global canvas.
/*
struct debugLine{
	int x1, y1, x2, y2, r, g, b, t;
};

std::vector< debugLine > debugLines;

void debugDrawAll() {
	CvPoint start, end;

	for( int i=0, s=debugLines.size(); i<s; i++ ) {
		start.x = debugLines[i].x1;
		start.y = debugLines[i].y1;

		end.x = debugLines[i].x2;
		end.y = debugLines[i].y2;

		cvLine( draw_image, start, end, CV_RGB(debugLines[i].r,debugLines[i].g,debugLines[i].b), debugLines[i].t);
	}

	debugLines.resize(0);
}

void debugDrawLine( int x1, int y1, int x2, int y2, int r, int g, int b, int t ) {
	debugLine newLine;

	newLine.x1 = x1; newLine.y1 = y1;
	newLine.x2 = x2; newLine.y2 = y2;
	newLine.r = r; newLine.g = g; newLine.b = b;
	newLine.t = t;

	debugLines.push_back( newLine );
}

void debugDrawPoint( int x1, int y1, int r, int g, int b, int t ) {
	debugDrawLine( x1-0, y1-1, x1+0, y1+1, r, g, b, t );
	debugDrawLine( x1-1, y1-0, x1+1, y1-0, r, g, b, t );
}
*/

void drawLine( int x1, int y1, int x2, int y2, int r, int g, int b, int t ) {
    CvPoint start, end;

    start.x = x1;
    start.y = y1;

    end.x = x2;
    end.y = y2;

    cvLine( draw_image, start, end, CV_RGB(r,g,b), t, 8, 0);
}

int main (int argc, char * const argv[]) {
	int i;

	Bool useCamera = TRUE;
//	BOOL useCamera = FALSE;
	Bool writeVideo = FALSE;

    // create all necessary instances
    cvNamedWindow (WINDOW_NAME, CV_WINDOW_AUTOSIZE);


	CvCapture * camera;
	CvVideoWriter *writer = 0;
	int isColor = 1;
	int fps     = 15;  // or 30
	int frameW  = 320; // 744 for firewire cameras
	int frameH  = 240; // 480 for firewire cameras

	if( useCamera ) {
		camera = cvCreateCameraCapture (CV_CAP_ANY);
	}
	if( useCamera && writeVideo ) {
		writer=cvCreateVideoWriter("out.mpeg",
                                   //								   CV_FOURCC('P','I','M','1'),
								   CV_FOURCC('M','J','P','G'),
								   fps,cvSize(frameW,frameH),isColor);
	}

    //////////////////////////////////////////////////

	// marker detection

	ARREdgeDetector *detector = (ARREdgeDetector*)malloc(sizeof(ARREdgeDetector));

    //debug setting

    // edgelDetector->debugDrawMarkers( true );

	if( !useCamera ) {
		draw_image = cvLoadImage( "resource/test2.png", CV_LOAD_IMAGE_COLOR);
		if(!draw_image){
			printf("Could not load image file");
			exit(0);
		}

        // show the image
        cvShowImage( WINDOW_NAME, draw_image);

        // load the image to buffer

        detector->image = (ARRImage*)malloc(sizeof(ARRImage));
        detector->image->width = draw_image->width;
        detector->image->height = draw_image->height;

        detector->image->data = (unsigned char *)draw_image->imageData;

        // detect markers
        ARRMarker *markers = NULL;
        int markers_num = 0;
        arrFindMarkers(detector, &markers, &markers_num);

        //        debugDrawAll();
        for (i=0; i<markers_num; i++) {
            drawLine( markers[i].c1.x, markers[i].c1.y, markers[i].c2.x, markers[i].c2.y, 255, 0, 0, THICKNESS);
            drawLine( markers[i].c2.x, markers[i].c2.y, markers[i].c3.x, markers[i].c3.y, 255, 0, 0, THICKNESS);
            drawLine( markers[i].c3.x, markers[i].c3.y, markers[i].c4.x, markers[i].c4.y, 255, 0, 0, THICKNESS);
            drawLine( markers[i].c4.x, markers[i].c4.y, markers[i].c1.x, markers[i].c1.y, 255, 0, 0, THICKNESS);
        }

        // Show the processed image
        cvShowImage( WINDOW_NAME, draw_image);

        cvWaitKey(0);
        cvReleaseImage( &draw_image );
        cvDestroyWindow( WINDOW_NAME );
        return 0;

	} else {
        // you do own an iSight, don't you ?!?
        if (! camera)
            abort ();

        // get an initial frame and duplicate it for later work
        IplImage *  current_frame = cvQueryFrame (camera);
        draw_image    = cvCreateImage(cvSize (640, 480), IPL_DEPTH_8U, 3);

        detector->image = (ARRImage*)malloc(sizeof(ARRImage));
        detector->image->width = draw_image->width;
        detector->image->height = draw_image->height;

        // as long as there are images ...
        while (current_frame = cvQueryFrame (camera))
        {

            // 缩放原图像到目标图像
            cvResize (current_frame, draw_image, CV_INTER_LINEAR);

            // Perform a Gaussian blur
            //cvSmooth( draw_image, draw_image, CV_GAUSSIAN, 3, 3 );

            detector->image->data = (unsigned char *)draw_image->imageData;

            ARRMarker *markers = NULL;
            int markers_num = 0;
            arrFindMarkers(detector, &markers, &markers_num);

            //        debugDrawAll();
            for (i=0; i<markers_num; i++) {
                drawLine( markers[i].c1.x, markers[i].c1.y, markers[i].c2.x, markers[i].c2.y, 255, 0, 0, THICKNESS);
                drawLine( markers[i].c2.x, markers[i].c2.y, markers[i].c3.x, markers[i].c3.y, 255, 0, 0, THICKNESS);
                drawLine( markers[i].c3.x, markers[i].c3.y, markers[i].c4.x, markers[i].c4.y, 255, 0, 0, THICKNESS);
                drawLine( markers[i].c4.x, markers[i].c4.y, markers[i].c1.x, markers[i].c1.y, 255, 0, 0, THICKNESS);
            }

            // just show the image
            cvShowImage (WINDOW_NAME, draw_image);

            if( writeVideo ) {
                cvWriteFrame(writer,draw_image);      // add the frame to the file
            }

            // wait a tenth of a second for keypress and window drawing
            int key = cvWaitKey (10);

            if (key == 'q' || key == 'Q')
                break;
            // toggle debug setting.
            // switch( key ) {
            //     case '4':	edgelDetector->debugDrawLineSegments( !edgelDetector->drawLineSegments );
            //         break;
            //     case '5':	edgelDetector->debugDrawPartialMergedLineSegments( !edgelDetector->drawPartialMergedLineSegments );
            //         break;
            //     case '6':	edgelDetector->debugDrawMergedLineSegments( !edgelDetector->drawMergedLineSegments );
            //         break;
            //     case '7':	edgelDetector->debugDrawExtendedLineSegments( !edgelDetector->drawExtendedLineSegments );
            //         break;
            //     case '9':	edgelDetector->debugDrawMarkers( !edgelDetector->drawMarkers );
            //         break;
            //     case '1':	edgelDetector->debugDrawSectors( !edgelDetector->drawSectors );
            //         break;
            //     case '2':	edgelDetector->debugDrawSectorGrids( !edgelDetector->drawSectorGrids );
            //         break;
            //     case '3':	edgelDetector->debugDrawEdges( !edgelDetector->drawEdges );
            //         break;
            //     case '8':	edgelDetector->debugDrawCorners( !edgelDetector->drawCorners );
            //         break;
            //     default:
            //         break;
            // }

        }
    }
	if( writeVideo ) {
		cvReleaseVideoWriter(&writer);
	}

    // be nice and return no error
    return 0;
}
