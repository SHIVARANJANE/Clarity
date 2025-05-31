#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;
using namespace cv;
void generateGaussianKernel1D(int ks, double sigma, vector<double>& kernel) 
{
    double sum = 0.0;
    int r = ks / 2;
    kernel.resize(ks);
    for (int i = -r; i <= r; ++i) 
    {
        kernel[i + r] = exp(-(i * i) / (2 * sigma * sigma)) / (sqrt(2 * M_PI) * sigma);
        sum += kernel[i + r];
    }
    for (int i = 0; i < ks; ++i) 
    {
        kernel[i] /= sum;
    }
}
void applyKernel1D(const Mat& src, Mat& dst, const vector<double>& kernel, bool horizontal) 
{
    int ks = kernel.size();
    int r = ks / 2;
    dst = src.clone();
    if (horizontal) 
    {
        for (int y = 0; y < src.rows; ++y) 
   	{
            for (int x = 0; x < src.cols; ++x) 
	    {
                double sumB = 0.0, sumG = 0.0, sumR = 0.0;
                for (int k = -r; k <= r; ++k) 
		{
                    int ix = x + k;
                    if (ix >= 0 && ix < src.cols) 
		    {
                        Vec3b p = src.at<Vec3b>(y, ix);
                        double w = kernel[k + r];
                        sumB += p[0] * w;
                        sumG += p[1] * w;
                        sumR += p[2] * w;
                    }
                }
                dst.at<Vec3b>(y, x)[0] = static_cast<unsigned char>(sumB);
                dst.at<Vec3b>(y, x)[1] = static_cast<unsigned char>(sumG);
                dst.at<Vec3b>(y, x)[2] = static_cast<unsigned char>(sumR);
                dst.at<Vec3b>(y, x)[2] = static_cast<unsigned char>(sumR);
            }
        }
    } 
    else 
    {
        for (int y = 0; y < src.rows; ++y) 
	{
            for (int x = 0; x < src.cols; ++x) 
	    {
                double sumB = 0.0, sumG = 0.0, sumR = 0.0;
                for (int k = -r; k <= r; ++k) 
		{
                    int iy = y + k;
                    if (iy >= 0 && iy < src.rows)
   		    {
                        Vec3b p = src.at<Vec3b>(iy, x);
                        double w = kernel[k + r];
                        sumB += p[0] * w;
                        sumG += p[1] * w;
                        sumR += p[2] * w;
                    }
                }
                dst.at<Vec3b>(y, x)[0] = static_cast<unsigned char>(sumB);
                dst.at<Vec3b>(y, x)[1] = static_cast<unsigned char>(sumG);
                dst.at<Vec3b>(y, x)[2] = static_cast<unsigned char>(sumR);
            }
        }
    }
}
void blur(const Mat& input, Mat& output, int ks, double sigma) 
{
    vector<double> kernel;
    generateGaussianKernel1D(ks, sigma, kernel);
    Mat temp;
    applyKernel1D(input, temp, kernel, true); 
    applyKernel1D(temp, output, kernel, false);
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
