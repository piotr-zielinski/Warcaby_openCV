/*
 * main.cpp
 *
 *  Created on: 07.05.2017
 *      Author: piotr
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/core/core.hpp>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

//wyœwietlanie obrazka
/*int main()
{
    const string& file_name = "sunflower.jpg";
    Mat img = imread(file_name, CV_LOAD_IMAGE_COLOR );
    if( !img.data )
    {
        cout << "Nie odnalezionu pliku " << file_name;
        return -1;
    }
    const string window_name = "OpenCV_1";
    namedWindow(window_name, CV_WINDOW_AUTOSIZE);
    imshow(window_name, img);
    waitKey(0);
    return 0;
}*/

//konwersja przestrzeni barw
/*int main()
{
    const string file_name = "sunflower.jpg";    //Nazwa obrazka
    Mat img, hsv;                    //Miejsce na obrazki
    vector<Mat> img_split;                //Miejsce na sk³adowe HSV
    img = imread(file_name);                //Wczytanie obrazka
    if ( !img.data )                    //Sprawdzenie poprawnosci odczytu
    {
        cout << "Nie znaleziono pliku " << file_name << ".";
        return -1;
    }
    const string named_window[] = {"BGR", "HSV", "HUE", "SAT", "VAL" };
    namedWindow(named_window[0], CV_WINDOW_AUTOSIZE);    //Utworzenie okien
    namedWindow(named_window[1], CV_WINDOW_AUTOSIZE);
    namedWindow(named_window[2], CV_WINDOW_AUTOSIZE);
    namedWindow(named_window[3], CV_WINDOW_AUTOSIZE);
    namedWindow(named_window[4], CV_WINDOW_AUTOSIZE);
    cvtColor(img, hsv, CV_BGR2HSV );                //Konwersja BGR -> HSV
    split(hsv, img_split);            //Rozdzielenie HSV na poszczególne kana³y
    imshow(named_window[0], img);        //Obraz oryginalny
    imshow(named_window[1], hsv);        //Obraz w wersji HSV
    imshow(named_window[2], img_split[0]);    //Barwa
    imshow(named_window[3], img_split[1]);    //Nasycenie
    imshow(named_window[4], img_split[2]);    //Wartosc
    vector<int> compression_params;    //Element przechowuj¹cy dane o sposobie zapisu
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);    //Konwersja jpg
    compression_params.push_back(100);                //Jakosc 100
    imwrite("hsv.jpg", hsv, compression_params);    //Zapis poszczegolnych obrazow
    imwrite("hue.jpg", img_split[0], compression_params);
    imwrite("sat.jpg", img_split[1], compression_params);
    imwrite("val.jpg", img_split[2], compression_params);
    waitKey();                    //Oczekiwanie na wciesniecie klawisza
    return 0;
}*/

//wykrywanie kolorowych elementów
/*int main()
{
	VideoCapture capture = VideoCapture(0);//Przechwycienie uchwytu kamery o nr. 0
    string window_name [] = { "Kamera", "Binary" };//Nazwy dla okien
    Mat frame, img, hsv_img, binary; //Miejsce na obrazki
    vector<Mat> hsv_split;        //Miejsce na kana³y hsv
    for ( int i = 0; i < 2; i++ ) namedWindow(window_name[i], CV_WINDOW_AUTOSIZE);    //Utworzenie 2 okien
    int lowerb = 100, upperb = 109;    //Ustalenie wartosci poczatkowych dla progowania
    createTrackbar( "lb", window_name[1], &lowerb, 255, NULL );
    //Utworzenie trackbar'ow
    createTrackbar( "ub", window_name[1], &upperb, 255, NULL );
    while ( waitKey(10) != 27 )                    //Odczekanie 20 ms
    {
        capture >> frame;                    //Pobranie kolejnej klatki
        frame.copyTo(img);                //Skopiowanie klatki do img
        cvtColor(img, hsv_img, CV_BGR2HSV);        //Konwrsja do HSV
        split(hsv_img, hsv_split);        //Podzial HSV na poszczegolne kanaly
        inRange(hsv_split[0], lowerb, upperb, binary);                        //Progowanie zgodnie z wartosciami lowerb, i upperb
        cv::Mat element(7,7,CV_8U,cv::Scalar(1));    //Okreslenie opcji erozji
        blur(binary, binary, cv::Size(3,3) );        //Rozmycie
        erode(binary, binary, element);            //Erozja
        imshow(window_name[0], img );            //Obrazek Orginalny
        imshow(window_name[1], binary);            //Obraz binarny
    }
    capture.release();                        //Zwolnienie uchwytu kamery
    return 0;
}*/

//wykrywanie konturu
int main()
{
    VideoCapture capture = VideoCapture(0);
    string window_name [] = { "Kamera", "Contour", "Binary" };
    Mat frame, img, hsv_img, binary;

    //***
    Mat cont;
    //***

    vector<Mat> hsv_split;
    for ( int i = 0; i < 3; i++ ) namedWindow(window_name[i], CV_WINDOW_AUTOSIZE);
    int lowerb = 100, upperb = 109;
    createTrackbar( "Thresh lb", window_name[2], &lowerb, 255, NULL );
    createTrackbar( "Thresh ub", window_name[2], &upperb, 255, NULL );
    while ( waitKey(20) != 27 )
    {
        capture >> frame;
        frame.copyTo(img);
        cvtColor(img, hsv_img, CV_BGR2HSV);
        split(hsv_img, hsv_split);
        inRange(hsv_split[0], lowerb, upperb, binary);
        blur(binary, binary, cv::Size(3,3) );
        erode(binary, binary, cv::Mat() );

        //***
        vector<vector<Point> > contours;
        vector<Point> contours_poly;
        Rect boundRect;
        binary.copyTo(cont);
        findContours( cont, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        int max = 0, i_cont = -1;
        Mat drawing = Mat::zeros( cont.size(), CV_8UC3 );
        for( unsigned int i = 0; i < contours.size(); i++ )
        {
            if ( abs(contourArea(Mat(contours[i]))) > max )
            {
                max = abs(contourArea(Mat(contours[i])));
                i_cont = i;
            }
        }
        if ( i_cont >= 0 )
        {
            approxPolyDP( Mat(contours[i_cont]), contours_poly, 3, true );
            boundRect = boundingRect( Mat(contours_poly) );
            fillConvexPoly(img, contours_poly, contours_poly.size() );
            rectangle( img, boundRect.tl(), boundRect.br(), Scalar(125, 250, 125), 2, 8, 0 );
            line( img, boundRect.tl(), boundRect.br(), Scalar(250, 125, 125), 2, 8, 0);
            line( img, Point(boundRect.x + boundRect.width, boundRect.y), Point(boundRect.x, boundRect.y + boundRect.height), Scalar(250, 125, 125), 2, 8, 0);
            string s;
            stringstream out;
            out << boundRect.x + boundRect.width/2 << "x" << boundRect.y + boundRect.height/2;
            s = out.str();
            putText( img, s, Point(50, 50), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(20, 40, 80), 3, 8 );
            drawContours( drawing,  contours, i_cont, Scalar(125, 125, 250), 2 );
        }
        imshow(window_name[1], drawing);
        //***

        imshow(window_name[0], img );
        imshow(window_name[2], binary);
    }
    capture.release();
    return 0;
}

//detekcja twarzy
/*string face_cascade_name = "haarcascade_frontalface_alt.xml";    //Nazwa kaskady któr¹ wykorzystujemy do rozpoznania twarzy
CascadeClassifier face_cascade;                                //Utworzenie obiektu cascady twarzy
string window_name = "Hello Face !";
const string img_name = "cotillard.jpg";

void detectFace( Mat img );

int main( int argc, char** argv )
{
    Mat img;                                            //Utworzenie nowej macierzy na nasz obrazek
    img = imread( img_name );                            //Wczytanie obrazka z dysku
    if ( !img.data )                                    //Sprawdzenie czy ³adowanie obrazka przebieg³o pomyslnie
    {
        cout << "Nie znaleziono pliku " <<  img_name << ".";
        return -1;
    }
    if( !face_cascade.load( face_cascade_name ) )        //£adowanie pliku ze sprawdzeniem poprawnoci
    {
        cout << "Nie znaleziono pliku " << face_cascade_name << ".";
        return -2;
    }
    namedWindow(window_name, CV_WINDOW_AUTOSIZE);    //Utworzenie okna (nazwa okna, ustalenie rozmiaru)
    detectFace(img);
    waitKey(0);                                            //Odczekanie na wcisniêce klawisza z opóŸnieniem 0ms
    return 0;
}

void detectFace( Mat img )
{
    vector<Rect> faces;                            //Utworzenie bufora na twarze
    Mat img_gray;                                //Utworzenie bufora na obrazek w odcieniach szarosci

    cvtColor(img, img_gray,  CV_BGR2GRAY );                //Konwersja obrazu do odcieni szarosci
    //equalizeHist(img_gray, img_gray);                    //Zaaplikowanie do obrazu jego historogramu, poprawa kontrastu

    face_cascade.detectMultiScale(img_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50) );
    for( unsigned i = 0; i < faces.size(); i++ )
    {
        Rect rect_face( faces[i] );    //Kwadrat okreslaj¹cy twarz
        //ellipse( img, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 120, 12 ), 2, 2, 0 );
        rectangle(img, rect_face, Scalar( 120, 5, 86 ), 2, 2, 0  );
    }
    imshow(window_name, img);                        //Pokazanie obrazka w oknmie o nazwie "Hello Face !"
}*/
