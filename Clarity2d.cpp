#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;
void gformula(int ks, double s, double** k) 
{
    double sum = 0.0;
    int r = ks / 2;
    for (int x = -r; x <= r; ++x) 
    {
        for (int y = -r; y <= r; ++y) 
	{
            double e = -(x * x + y * y) / (2 * s * s);
            k[x + r][y + r] = exp(e) / (2 * M_PI * s * s);
            sum += k[x + r][y + r];
        }
    }
    for (int x = 0; x < ks; ++x) 
    {
        for (int y = 0; y < ks; ++y) k[x][y] /= sum;
    }
}
void blur(const Mat &input, Mat &output, int ks, double s) 
{
    int r = ks / 2;
    double** k = new double*[ks];
    for (int i = 0; i < ks; ++i)
    { 
 	k[i] = new double[ks];
    }
    gformula(ks, s, k);
    for (int x = 0; x < input.rows; ++x) 
    {
        for (int y = 0; y < input.cols; ++y) 
	{
            double R = 0.0, G = 0.0, B = 0.0;
            for (int kx = -r; kx <= r; ++kx) 
    	    {
                for (int ky = -r; ky <= r; ++ky) 
		{
                    int ix = x + kx;
                    int iy = y + ky;
                    if (ix >= 0 && ix < input.rows && iy >= 0 && iy < input.cols) 
		    {
                        Vec3b p = input.at<Vec3b>(ix, iy);
                        double w = k[kx + r][ky + r];
                        R += p[2] * w; // Note: p[2] is Red
                        G += p[1] * w; // p[1] is Green
                        B += p[0] * w; // p[0] is Blue
                    }
                }
            }
            output.at<Vec3b>(x, y)[2] = static_cast<unsigned char>(R); // Red
            output.at<Vec3b>(x, y)[1] = static_cast<unsigned char>(G); // Green
            output.at<Vec3b>(x, y)[0] = static_cast<unsigned char>(B); // Blue
        }
    }

    for (int i = 0; i < ks; ++i) 
    {
	delete[] k[i];
    }
    delete[] k;
}
void greyscale(const Mat& input, Mat& output)
{
    output = Mat::zeros(input.size(), CV_8UC1); // Ensure output is grayscale
    for (int x = 0; x < input.rows; ++x)
    {
        for (int y = 0; y < input.cols; ++y)
        {
            Vec3b p = input.at<Vec3b>(x, y);
            uchar gray = static_cast<uchar>(( p[2] + p[1] + p[0])/3 );
            output.at<uchar>(x, y) = gray;
        }
    }
}

void brightness(const Mat& input, Mat& output, int b)
{
    for (int x = 0; x < input.rows; ++x)
    {
        for (int y = 0; y < input.cols; ++y)
        {
            Vec3b p = input.at<Vec3b>(x, y);
            for (int c = 0; c < 3; ++c)
            {
                int newValue = p[c] + b;
                newValue = min(max(newValue, 0), 255);
                output.at<Vec3b>(x, y)[c] = newValue;
            }
        }
    }
}
int main(int argc, char** argv)
{
    if (argc != 2) 
    {
        cout << "Usage: ./blurImage <image_path>" << endl;
        return -1;
    }
    Mat inputImage = imread(argv[1]);
    if (inputImage.empty()) 
    {
        cerr << "Error: Could not open or find the image!" << endl;
        return -1;
    }
     Mat FinalImage = inputImage.clone();
    int c;
    cout<<"Enter the process:\n1.Blurring\n2.Greyscale\n3.Brightness\n";
    cin>>c;
    if(c==1)
    {
        int ks;
        double s;
        cout << "Enter Kernel size and sigma value: ";
        cin >> ks >> s;
        if (ks % 2 == 0)
        {
            ks += 1;
        }
        blur(inputImage, FinalImage, ks, s);
    }
    if(c==2)
    {
        greyscale(inputImage,FinalImage);
    }
    if(c==3)
    {
        int b;
        cout<<"Enter Brightness level(-255 to 255):";
        cin>>b;
        brightness(inputImage,FinalImage,b);
    }
    imshow("Original Image", inputImage);
    imshow("Final Image", FinalImage);
    imwrite("Final_image.jpg", FinalImage);
    waitKey(0);
    return 0;
}
