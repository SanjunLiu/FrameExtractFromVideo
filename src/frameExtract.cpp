/***
 * this file is used to extract frames from video
 * Copyright (c) 2017, Sanjun Liu, email: Sunite.liu@gmail.com
 * All rights reserved
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/stat.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    std::string VideoFile;
    std::string SaveDir = "resultFrames";

    if (argc == 2){
        VideoFile = argv[1];
    }else if (argc == 3){
        VideoFile = argv[1];
        SaveDir = argv[2];
    }else {
        cout << "Usage: " << argv[0]
            << " VideoFile [FrameSavedDir]" << endl;
        return -1;
    }

    int fileMode = 06; // Read and write permision
    if (0 != access(SaveDir.c_str(), fileMode)){
        if ( 0 != mkdir(SaveDir.c_str(), S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)){
            cout << "file " << SaveDir
                 <<"is not exist and mkdir failed" <<endl;
            return -1;
        }
    }

    // Open Video
    VideoCapture capture(VideoFile.c_str());
    if(!capture.isOpened())
        cout<<"fail to open!"<<endl;

    long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
    std::cout << "Total frames[ " << totalFrameNumber << " ]"<< std::endl;


    //set start frame
    long frameToStart = 0;
    capture.set( CV_CAP_PROP_POS_FRAMES,frameToStart);
    cout << "The start frame to extract: " << frameToStart << endl;


    //set stop frame
    //TODO
    //long frameToStop = 20;

    //get frame rate
    double rate = capture.get(CV_CAP_PROP_FPS);
    cout<<"The frame rate is: "<< rate << endl;

    //flag to control stop reading
    bool stop = false;

    Mat frame;
    std::string img_name;
#ifdef DISPLAY
    //Display the extracted frame
    namedWindow("Extracted frame");
    //the internel time between two frames
    int delay = 1000/rate;
#endif

    long currentFrame = frameToStart;

#ifdef DISPLAY
    // add a filter
    int kernel_size = 3;
    Mat kernel = Mat::ones(kernel_size,
            kernel_size,CV_32F)/(float)(kernel_size*kernel_size);
#endif

    while(!stop)
    {
        //read a frame
        if(!capture.read(frame))
        {
            goto RELEASE;
        }
        img_name = SaveDir + "/" + std::to_string(currentFrame) + ".jpg";
        cv::imwrite(img_name.c_str(), frame);

#ifdef DISPLAY
        imshow("Extracted frame", frame);
        filter2D(frame, frame, -1, kernel);
        imshow("after filter", frame);
#endif
        if (((currentFrame + 1) % 100) == 0) cout << "-" << endl;
        else cout << "-";

#ifdef DISPLAY
        // when delay ≤ 0, wait forever, else wait delay ms
        int c = waitKey(delay);
        // click ESC or get the stop frame, exit
        if((char) c == 27){
            stop = true;
        }
        // click key to wait extracting the next frame
        if( c >= 0){
            waitKey(0);
        }
#endif
        currentFrame++;

    }
    //release
RELEASE:
    cout << "\nRead total frames: " << currentFrame - frameToStart << endl;
    cout << "please check the results in dir: " << SaveDir << endl;
    capture.release();
    waitKey(0);
    return 0;
}


