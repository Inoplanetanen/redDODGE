#include <QCoreApplication>
#include <iostream>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int pixel;
long int M=0;
unsigned long int cX=0;
unsigned long int cY=0;
Mat bin_frame,frame,gray;
Mat frame2;
int x=320;
int y=240;
int x_zad=320;
int y_zad=240;
int red_pos=0;
void red();
int main(int argc, char *argv[])

{
    cout<<"Serial list"<<endl;
    foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts()){
        cout<<info.portName().toStdString()<<endl;
    }

    QSerialPort *serial = new QSerialPort;

    serial->setPortName("ttyUSB0");
    serial->setBaudRate(QSerialPort::Baud9600);
    if(!serial->open(QSerialPort::ReadWrite)){
       // return 10;
    }

    QByteArray data;
    bool fl = 1;

// Mat I = imread("/home/matthew/Документы/unnamed.jpg", 0);
 //Mat frame;

     VideoCapture cap(1);

 if (!cap.open(1)){
     qDebug()<<"Can't open camera";

 }
//Mat croppedImage;

    while(1){
        cap>>frame2;
resize(frame2, frame, Size(640, 480));
      //  Mat ROI(frame, Rect(548,260,500,500));
    //    ROI.copyTo(croppedImage);
  //    imshow("cropped",croppedImage);

     imshow("DED",frame);
     red();
        char c = waitKey(2);
        data.clear();
        string str;
        if(c == 'w'){
            str = "set200";
        }
        if(c == 's'){
            str = "set-200";
        }
        if(c == 'a'){
            str = "ung-45";
        }
        if(c == 'd'){
            str = "ung45";
        }
        if(c == 'q'){
            str = "set0";
        }
        qDebug()<<"d";
        if (c>27){
            for(int ii = 0; ii < str.size(); ii++){
                data.append(str[ii]);
            }
            serial->write(data);
            serial->waitForBytesWritten(10);
        }
       QByteArray cmd;
        if(serial->isReadable()){
            cmd = serial->readAll();
            for(int i = 0; i <= cmd.size(); i++){
                cout << cmd[i];
            }
        }

        if (c == 27){
            break;
        }
    }
    serial->close();
    delete serial;

    return 0;
}






void red(){

    M=0;


    //Mat redMask;
         cvtColor(frame, gray, COLOR_RGB2GRAY);
       threshold(gray,bin_frame,90,100,THRESH_BINARY_INV);
    //inRange(frame, Scalar(3, 30, 30), Scalar(10, 50, 140), redMask);
    //inRange(frame,  Scalar(100, 60, 220),Scalar(120, 80, 250), redMask);

    imshow("gray", bin_frame);

    cX = 0;
    cY = 0;
    M = 0;

    for(int y_tank=0;y_tank<bin_frame.rows;y_tank++){
        for(int x_tank=0;x_tank<bin_frame.cols;x_tank++){
            pixel =bin_frame.at<uchar>(y_tank,x_tank);
            //            pixel/=255;
            if(pixel==255){
                cX+=x_tank;
                cY+=y_tank;
                M+=1;
            }


        }
    }
    red_pos = cX*cY/1000000000;

    if(M!=0){
        cX=cX/M;
        cY=cY/M;

    }

    x_zad=cX;
    y_zad=cY;

    circle(frame, Point(x_zad+1, y_zad+1), 20, Scalar(0, 0, 250), 2);
//    if(red_pos>20) circle(frame, Point(x_zad, y_zad), 35, Scalar(0, 100, 200), 2);
//    if(red_pos>210) circle(frame, Point(x_zad, y_zad), 50, Scalar(0, 200, 0), 2);
//    if(red_pos>14010) circle(frame, Point(x_zad, y_zad), 100, Scalar(0, 200, 0), 2);
//    if(red_pos>20010) circle(frame, Point(x_zad, y_zad), 180, Scalar(0, 200, 0), 2);
    //    line(frame,Point(x_zad-10,y_zad),Point(x_zad+10,y_zad),Scalar(0,0,250),1);
    //    line(frame,Point(x_zad,y_zad-10),Point(x_zad,y_zad+10),Scalar(0,0,250),1);
    Rect r=Rect(cX/2,cY/2,cX,cY);

    cv::rectangle(frame, r , cv::Scalar(0, 255, 0));

}


