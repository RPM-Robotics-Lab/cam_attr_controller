#include "img_eval.h"

void
Img_eval::img_entropy (Mat &img, Mat &entropy)
{
    //Calculate Entropy Filter
    cv::Rect roi (0, 0, img.cols, img.rows);
    cv::Mat dst (img.rows, img.cols, CV_32F);

    getLocalEntropyImage (img, roi, dst);
    cv::normalize (dst, dst, 0, 255, cv::NORM_MINMAX);
    dst.convertTo (entropy, CV_8U);

}

void 
Img_eval::img_wmask (Mat &entropy, Mat &wmask)
{
// Create Entropy binary mask, wmask == [rs, cs] 

	Mat wmasktmp1(entropy.size(), CV_32F, 0.);
    Mat wmasktmp2(entropy.size(), CV_32F, 0.);


 	wmasktmp1 = entropy > 0;
    Mat tmp1; // 1 1 1 ...
    wmasktmp1.convertTo(tmp1, CV_32F, 1.0/255.0);
    wmask = tmp1;

    wmasktmp2 = entropy ==0; 
    Mat tmp2; 
    wmasktmp2.convertTo(tmp2, CV_32F, 1.0/-255.0);
    
    wmask = tmp1+tmp2;
     
//    std::cout << "wmask: kkk " << wmask <<std::endl;
}


void
Img_eval::img_grad (Mat &img, Mat &grad)
{
// calculate Gradient (sobel)
  	cv::Mat Gx, Gy; double ksize= 1;
    Mat abs_grad_x, abs_grad_y;
    cv::Sobel(img, Gx, CV_8U, 1, 0, ksize);
    convertScaleAbs( Gx, abs_grad_x );
	cv::Sobel(img, Gy, CV_8U, 0, 1, ksize);
 	convertScaleAbs( Gy, abs_grad_y );
	addWeighted( abs_grad_x, 1.49999, abs_grad_y, 1.49999, 0, grad );

    grad.convertTo(grad, CV_32F, 1.0);
}

void Img_eval::img_Gmean (Mat &grad, double &Gmean)
{
    int wrows = grad.rows;
	int wcols = grad.cols; 
	Gmean = sum(grad)[0] / (wrows * wcols);  // mean(mean(Gmag))) 
}
void Img_eval::img_Emean (Mat &entropy, double &Emean)
{
    int wrows = entropy.rows;
	int wcols = entropy.cols; 
	Emean = sum(entropy)[0] / (wrows * wcols); // mean(mean(J))) 
}


void Img_eval::img_columnSum (Mat &entropy, Mat &columnSum, Mat &mu)
{
for (int i = 0; i<entropy.cols; i++)    
		columnSum.push_back(cv::sum(entropy.col(i))[0]);    // K
		mu.push_back(cv::sum(columnSum)[0] / entropy.cols);  // mu cols=320
//	Mat sigma = pow((columnSum-mu),2)  / (entropy.cols); 
 		Mat temp = columnSum-mu;
		
		Scalar std;
		meanStdDev(columnSum, mu, std);
		vector <Mat> HSV;
		split(columnSum, HSV);
}

void Img_eval::img_Gours (Mat &Gour, Mat &Gourstmp, Mat &Gours)
{
	for (int i=0; i<Gour.rows; i++)	{
	Gourstmp.push_back(cv::sum(Gour.row(i))[0]);
	}	
	for (int j=0; j<Gourstmp.cols; j++) {
	Gours.push_back(cv::sum(Gourstmp.col(j))[0]);
		}
//	double Gourss = Gours.at<double>(0);
}


// TODO normal pdf  normpdf equvalent !! 	
//	const float ONE_OVER_SQRT_2PI = 0.39894228040143267793994605993438;
//    std::cout << "Mean: kkk " << k <<std::endl;

//	Mat normal_pdf(columnSum, mu.at<double>(0,0), std[0]); 
//	{ 
//    return (ONE_OVER_SQRT_2PI/std)*exp(-0.5*squared((columnSum-mu)/std));
//	}



int Img_eval::sub_to_ind(int *coords, int *cumprod, int num_dims)
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



void Img_eval::ind_to_sub(int p, int num_dims, const int size[],
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

void Img_eval::getLocalEntropyImage(cv::Mat &gray, cv::Rect &roi, cv::Mat &entropy)
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

void Img_eval::GammaCorrection(Mat& src, Mat& dst, float fGamma)
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





