#include "object_reg/setting.h"
#include "object_reg/ImageReader.h"

using namespace cv;
using namespace objectR;

int main()
{
        ImageReader *Imagereader = new ImageReader();
	{
	    Mat image = imread("/home/floatsdsds/document/objectR/image/destination.png");
	    Imagereader->TrackMonoImage("marker");
	    //Imagereader->GrabMonoImage(image, "circle");
	    if (Imagereader->objectFind) 
	    {
		    cout << Imagereader->objectCenter << endl;
		
		    namedWindow("Original image", CV_WINDOW_NORMAL);
		    imshow("Original image", Imagereader->imgOriginal);
		    
		    namedWindow("Processed image", CV_WINDOW_NORMAL);
		    imshow("Processed image", Imagereader->imgProcessed);
	    }
	    waitKey(0);
	}
         return 0;
}


 

