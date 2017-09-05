#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;
using namespace cv;
static int sub_to_ind(int *coords, int *cumprod, int num_dims)
{
    int index = 0;
    int k;

    assert(coords != NULL);
    assert(cumprod != NULL);
    assert(num_dims > 0);

    for (k = 0; k < num_dims; k++)
    {
        index += coords[k] * cumprod[k];
    }

    return index;
}

static void ind_to_sub(int p, int num_dims, const int size[],
    int *cumprod, int *coords)
{
    int j;

    assert(num_dims > 0);
    assert(coords != NULL);
    assert(cumprod != NULL);

    for (j = num_dims - 1; j >= 0; j--)
    {
        coords[j] = p / cumprod[j];
        p = p % cumprod[j];
    }
}

void getLocalEntropyImage(cv::Mat &gray, cv::Rect &roi, cv::Mat &entropy)
{
    using namespace cv;
    clock_t func_begin, func_end;
    func_begin = clock();
    //1.define nerghbood model,here it's 9*9
    int neighbood_dim = 2;
    int neighbood_size[] = {9, 9};

    //2.Pad gray_src
    Mat gray_src_mat(gray);
    Mat pad_mat;
    int left = (neighbood_size[0] - 1) / 2;
    int right = left;
    int top = (neighbood_size[1] - 1) / 2;
    int bottom = top;
    copyMakeBorder(gray_src_mat, pad_mat, top, bottom, left, right, BORDER_REPLICATE, 0);
    Mat *pad_src = &pad_mat;
    roi = cv::Rect(roi.x + top, roi.y + left, roi.width, roi.height);

    //3.initial neighbood object,reference to Matlab build-in neighbood object system
    //        int element_num = roi_rect.area();
    //here,implement a histogram by ourself ,each bin calcalate gray value frequence
    int hist_count[256] = {0};
    int neighbood_num = 1;
    for (int i = 0; i < neighbood_dim; i++)
        neighbood_num *= neighbood_size[i];

    //neighbood_corrds_array is a neighbors_num-by-neighbood_dim array containing relative offsets
    int *neighbood_corrds_array = (int *)malloc(sizeof(int)*neighbood_num * neighbood_dim);
    //Contains the cumulative product of the image_size array;used in the sub_to_ind and ind_to_sub calculations.
    int *cumprod = (int *)malloc(neighbood_dim * sizeof(*cumprod));
    cumprod[0] = 1;
    for (int i = 1; i < neighbood_dim; i++)
        cumprod[i] = cumprod[i - 1] * neighbood_size[i - 1];
    int *image_cumprod = (int*)malloc(2 * sizeof(*image_cumprod));
    image_cumprod[0] = 1;
    image_cumprod[1] = pad_src->cols;
    //initialize neighbood_corrds_array
    int p;
    int q;
    int *coords;
    for (p = 0; p < neighbood_num; p++){
        coords = neighbood_corrds_array + p * neighbood_dim;
        ind_to_sub(p, neighbood_dim, neighbood_size, cumprod, coords);
        for (q = 0; q < neighbood_dim; q++)
            coords[q] -= (neighbood_size[q] - 1) / 2;
    }
    //initlalize neighbood_offset in use of neighbood_corrds_array
    int *neighbood_offset = (int *)malloc(sizeof(int) * neighbood_num);
    int *elem;
    for (int i = 0; i < neighbood_num; i++){
        elem = neighbood_corrds_array + i * neighbood_dim;
        neighbood_offset[i] = sub_to_ind(elem, image_cumprod, 2);
    }

    //4.calculate entropy for pixel
    uchar *array = (uchar *)pad_src->data;
    //here,use entroy_table to avoid frequency log function which cost losts of time
    float entroy_table[82];
    const float log2 = log(2.0f);
    entroy_table[0] = 0.0;
    float frequency = 0;
    for (int i = 1; i < 82; i++){
        frequency = (float)i / 81;
        entroy_table[i] = frequency * (log(frequency) / log2);
    }
    int neighbood_index;
    //        int max_index=pad_src->cols*pad_src->rows;
    float e;
    int current_index = 0;
    int current_index_in_origin = 0;
    for (int y = roi.y; y < roi.height; y++){
        current_index = y * pad_src->cols;
        current_index_in_origin = (y - 4) * gray.cols;
        for (int x = roi.x; x < roi.width; x++, current_index++, current_index_in_origin++) {
            for (int j = 0; j<neighbood_num; j++) {
                neighbood_index = current_index + neighbood_offset[j];
                hist_count[array[neighbood_index]]++;
            }
            //get entropy
            e = 0;
            for (int k = 0; k < 256; k++){
                if (hist_count[k] != 0){
                    //                                        int frequency=hist_count[k];
                    e -= entroy_table[hist_count[k]];
                    hist_count[k] = 0;
                }
            }
            ((float *)entropy.data)[current_index_in_origin] = e;
        }
    }
    free(neighbood_offset);
    free(image_cumprod);
    free(cumprod);
    free(neighbood_corrds_array);

    func_end = clock();
    double func_time = (double)(func_end - func_begin) / CLOCKS_PER_SEC;
    std::cout << "func time  " << func_time << std::endl;
}

void GammaCorrection(Mat& src, Mat& dst, float fGamma)
{
    CV_Assert(src.data);

    // accept only char type matrices
    CV_Assert(src.depth() != sizeof(uchar));

    // build look up table
    unsigned char lut[256];
    for (int i = 0; i < 256; i++)
    {
        lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0 ), fGamma / 100) * 255.0f);
    }

    dst = src.clone();
    const int channels = dst.channels();
    switch (channels)
    {
        case 2:
        {
            MatIterator_<uchar> it, end;
            for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
            //*it = pow((float)(((*it))/255.0), fGamma) * 255.0;
            *it = lut[(*it)];
            break;
        }
        case 3:
        {
            MatIterator_<Vec3b> it, end;
            for (it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++)
            {
            (*it)[0] = lut[((*it)[0])];
            (*it)[1] = lut[((*it)[1])];
            (*it)[2] = lut[((*it)[2])];
        }
            break;
        }
    }
}

int main(int argc, char** argv)
{
    cv::Mat img; 

    /// Load image
//    src = cv::imread(argv[1], 1);
    img = cv::imread("../matlab/figs/120.png", 1);
	Mat resized;
	cv::resize(img, img, cv::Size(320, 240));
    if (!img.data)
    {
        std::cout << "Usage: EntropyFilter <path_to_image>" << std::endl;
        return -1;
    }

	Mat img_gray,image_blur;
    GaussianBlur( img, image_blur, Size(5,5), 3, 3);
    cvtColor(img,img_gray,CV_RGB2GRAY);

    // Displaying grayscale image
    //imshow("Original Image",img_gray);
    //waitKey(0);

    int cols = img_gray.cols;
    int rows = img_gray.rows;
    // Creating sobel operator in x direction
    int sobel_x[3][3] = {-1,0,1,-2,0,2,-1,0,1};
    // Creating sobel operator in y direction
    int sobel_y[3][3] = {-1,-2,-1,0,0,0,1,2,1};
    int radius = 1;

    // Handle border issues
    Mat _src;
    copyMakeBorder(img_gray, _src, radius, radius, radius, radius, BORDER_REFLECT101);

    // Create output matrix
    Mat gradient_x = img_gray.clone();
    Mat gradient_y = img_gray.clone();
    Mat gradient_f = img_gray.clone();

    int max=0;

    // Correlation loop in x direction 

    // Iterate on image 
    for (int r = radius; r < _src.rows - radius; ++r)
    {
        for (int c = radius; c < _src.cols - radius; ++c)
        {
            int s = 0;

            // Iterate on kernel
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    s += _src.at<uchar>(r + i, c + j) * sobel_x[i + radius][j + radius];
                }
            }
            gradient_x.at<uchar>(r - radius, c - radius) = s;

        /*if(s>200)
        gradient.at<uchar>(r - radius, c - radius) = 255;
        else
        gradient.at<uchar>(r - radius, c - radius) = 0;
        */    
        }
    }

    Mat absGrad_x;
    convertScaleAbs( gradient_x, absGrad_x );

    // Conrrelation loop in y direction 

    // Iterate on image 
    for (int r = radius; r < _src.rows - radius; ++r)
    {
        for (int c = radius; c < _src.cols - radius; ++c)
        {
            int s = 0;

            // Iterate on kernel
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    s += _src.at<uchar>(r + i, c + j) * sobel_y[i + radius][j + radius];
                }
            }

        gradient_y.at<uchar>(r - radius, c - radius) = s;

        /*if(s>200)
        gradient.at<uchar>(r - radius, c - radius) = 255;
        else
        gradient.at<uchar>(r - radius, c - radius) = 0;
        */    
        }
    }

    Mat absGrad_y;
    convertScaleAbs( gradient_y, absGrad_y );

    //Calculating gradient magnitude
    for(int i=0; i<gradient_f.rows; i++)
    {
        for(int j=0; j<gradient_f.cols; j++)
        {
        gradient_f.at<uchar>(i,j) = sqrt( pow(gradient_x.at<uchar>(i,j),2) + pow(gradient_y.at<uchar>(i,j),2) );  

        if(gradient_f.at<uchar>(i,j) >240)
            gradient_f.at<uchar>(i,j) = 100;
        else
            gradient_f.at<uchar>(i,j) = 0;
        }
    }

    /// Convert to grayscale
    cvtColor(img, img, cv::COLOR_BGR2GRAY);

    //Calculate Entropy Filter
    cv::Rect roi(0, 0, img.cols, img.rows);
    cv::Mat dst(img.rows, img.cols, CV_32F);
    getLocalEntropyImage(img, roi, dst);
    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
    cv::Mat entropy;
    dst.convertTo(entropy, CV_8U);

	int wrows = dst.rows;
	int wcols = dst.cols; 

	cv::Size ss = dst.size();  
	wrows = ss.height;
	wcols = ss.width;	
	printf("Gradient sss is %d %d \n",wcols, wrows) ;
 	
	Mat dst_gray;

	// Create Entropy binary mask, wmask == [rs, cs] 
	Mat wmask(dst.size(), CV_32F, 1.); // ones
	Mat wmasktmp1(dst.size(), CV_32F, 0.);
    Mat wmasktmp2(dst.size(), CV_32F, 0.);
 	wmasktmp1 = dst>0;
    Mat tmp1; 
    wmasktmp1.convertTo(tmp1, CV_32F, 1.0/255.0);
//	wmasktmp1 *= 0.006; // 0.005;	
	wmasktmp2 = dst ==0; 
	wmasktmp2 *= 0.0;
	wmask = wmasktmp1 + wmasktmp2;
    wmask = tmp1;


//	std::cout << "func dst  " << wmask << std::endl;

//	findContours( entropy, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//	Mat result(dst.size(), CV_8U, Scalar(255));
//	drawContours(result, contours, -0, Scalar(100), 2); 

	// calculate Gradient (sobel)
  	cv::Mat Gx, Gy, grad; double ksize=1;
    Mat abs_grad_x, abs_grad_y;
    cv::Sobel(img, Gx, CV_8U, 1, 0, ksize);
    convertScaleAbs( Gx, abs_grad_x );
	cv::Sobel(img, Gy, CV_8U, 0, 1, ksize);
 	convertScaleAbs( Gy, abs_grad_y );
	addWeighted( abs_grad_x, 1.49999, abs_grad_y, 1.49999, 0, grad );
	double Gmean = sum(grad)[0] / (wrows * wcols);  // mean(mean(Gmag))) 
	double Emean = sum(entropy)[0] / (wrows * wcols); // mean(mean(J))) 
//	double Emean = sum(entropy)[0] / (wcols); // mean(sum(J))) 
	Mat gradW = grad > Emean * 0.3;
	gradW *= 1;
	imshow("gradW", gradW);
	
	Mat columnSum;   
	Mat mu;
	double Sval = -Gmean;  
	for (int i = 0; i<entropy.cols; i++)    
		columnSum.push_back(cv::sum(entropy.col(i))[0]);    // K
		mu.push_back(cv::sum(columnSum)[0] / entropy.cols);  // mu cols=320
//	Mat sigma = pow((columnSum-mu),2)  / (entropy.cols); 
 		Mat temp = columnSum-mu;
		
		Scalar std;
		meanStdDev(columnSum, mu, std);
		vector <Mat> HSV;
		split(columnSum, HSV);
	
	Mat Smask = Sval*wmask;  //Smask == Sval, how to - value
	grad.convertTo(grad, CV_32F, 1.0);
	Mat Gour = grad+Smask;
	Mat Gourstmp, Gours;
	double SSd = sum(Gour)[0];
	for (int i=0; i<Gour.rows; i++)	{
	Gourstmp.push_back(cv::sum(Gour.row(i))[0]);
	}	
	for (int j=0; j<Gourstmp.cols; j++) {
	Gours.push_back(cv::sum(Gourstmp.col(j))[0]);
		}
	double Gourss = Gours.at<double>(0);
	

    std::cout << "Mean: " << mu.at<double>(0,0) << "   Sval: "<< Gour  << "   std: " << std[0] << std::endl;

// TODO normal pdf  normpdf equvalent !! 	
//	const float ONE_OVER_SQRT_2PI = 0.39894228040143267793994605993438;
//    std::cout << "Mean: kkk " << k <<std::endl;

//	Mat normal_pdf(columnSum, mu.at<double>(0,0), std[0]); 
//	{ 
//    return (ONE_OVER_SQRT_2PI/std)*exp(-0.5*squared((columnSum-mu)/std));
//	}


    /// Display results
	cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Entropy Mask", cv::WINDOW_AUTOSIZE);	
	cv::namedWindow("Entropy Filter", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Gradient Image", cv::WINDOW_AUTOSIZE);

	cv::imshow("Original", img);
	cv::imshow("Entropy Mask", wmask);  //dst 
	cv::imshow("Entropy Filter", entropy);
	cv::imshow("Gradient Image",grad);

    /// Wait until user exits the program
    cv::waitKey(0);

    return 0;
}


