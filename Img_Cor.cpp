#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <vector>  

#define PI 3.14159265
#define WINDOW "Image Correction"

using namespace std;
using namespace cv;

vector<Point2f> roi_corners(4);
vector<Point2f> dst_corners(4);
Mat img1;
Mat cache;
int roi_id;

void On_mouse(int event, int x, int y, int flags, void*);


int main(int argc, char const *argv[])
{
    
    //import image
    roi_id = 0;
    img1 = imread("Board.jpg");
    if ( img1.empty() )
    {
        cerr << "Please Import an Image!!" << endl;
    }
    int factor = 60;//for pixel adjustment
    cache = img1.clone();//copy for retake points
    imshow(WINDOW, img1);
    
    
    /*Run the point taking procedure*/
    while(true){
     setMouseCallback(WINDOW, On_mouse, 0);
     char c = (char)waitKey( 10 );
     if(c=='n') break;//press 'n' when determine the four point you want 
     if(c=='e') {roi_id=0; img1 = cache.clone();} //press 'e' to retake the foru point

    }
    

    /*For adjustment point estimation*/
    dst_corners[0].x = roi_corners[0].x;
    dst_corners[0].y = roi_corners[0].y;
    dst_corners[1].x = roi_corners[0].x+factor*1;
    dst_corners[1].y = roi_corners[0].y;
    dst_corners[2].x = roi_corners[0].x+factor*1;
    dst_corners[2].y = roi_corners[0].y+factor*1;
    dst_corners[3].x = roi_corners[0].x;
    dst_corners[3].y = roi_corners[0].y+factor*1;

    Mat M = getPerspectiveTransform(roi_corners, dst_corners);
    Mat warped_image;
    

    /*Print the corrected picture*/
    Size sz = cache.size();
    warpPerspective(cache, warped_image, M, Size(sz.width, sz.height)); // do perspective transformation
    imshow("Corrected Image", warped_image);
    waitKey(0);
    cout<<"complete"<<endl;
    return 0;
}


void On_mouse(int event, int x, int y, int flags, void*)
{   imshow(WINDOW, img1);
    if(roi_id<4){
        if (event == EVENT_LBUTTONDOWN){
            roi_corners[roi_id].x=x;
            roi_corners[roi_id].y=y;
            cout<<"The Point You Take is: "<<x<<' '<<y<<endl;
            roi_id++;
            circle(img1, Point(x,y), 2, Scalar(0, 0, 255), LINE_8 ,0);
            imshow(WINDOW, img1);
        }

    }

}
