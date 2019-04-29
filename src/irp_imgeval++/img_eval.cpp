#include "img_eval.h"
#include <math.h>
#include <iostream>
#include<fstream>
//TODO get synthtic imgs  


double
Img_eval::syn_grab_and_return_ewg (cv::Mat &synth_img_g, Img_eval &eval, double exp_t, double gain_t)
{
    double ewg = calc_img_ent_grad (synth_img_g, false);
    ewg = ewg / 50;
    return ewg;
}



double
Img_eval::syn_grab_and_return_snr (cv::Mat &synth_img_g,  cv::Mat &synth_img_t, Img_eval &eval, double exp_t, double gain_t)
{
    double snr = eval.getPSNR (synth_img_g, synth_img_t);

    return snr;
}



void 
Img_eval::_synth_img_t (cv::Mat &init_img, double &next_exp, cv::Mat &synth_img_t, bool visualize)
{

// TODO : initially generate crf curve 
    double CRF[256] = {-4.5241,-4.409,-4.2939,-4.1788,-4.0637,-3.9486,-3.8335,-3.7184,-3.6033,-3.4882,-3.3731,-3.2583,-3.1444,-3.0321,-2.9222,-2.8153,-2.7119,-2.6126,-2.5175,-2.4268,-2.3406,-2.2589,-2.1815,-2.1083,-2.0392,-1.9739,-1.9122,-1.8541,-1.7993,-1.7476,-1.6988,-1.6529,-1.6095,-1.5685,-1.5299,-1.4934,-1.4588,-1.4259,-1.3945,-1.3645,-1.3357,-1.3079,-1.281,-1.2549,-1.2296,-1.2049,-1.1807,-1.1571,-1.134,-1.1112,-1.0888,-1.0668,-1.0451,-1.0237,-1.0026,-0.9818,-0.96132,-0.94112,-0.92121,-0.9016,-0.88228,-0.86327,-0.84457,-0.82619,-0.80814,-0.7904,-0.77297,-0.75584,-0.73899,-0.72241,-0.70607,-0.68996,-0.67405,-0.65833,-0.64279,-0.62742,-0.61222,-0.59719,-0.58232,-0.56762,-0.55307,-0.53867,-0.52442,-0.51031,-0.49634,-0.48251,-0.4688,-0.45521,-0.44175,-0.42841,-0.41519,-0.40208,-0.38911,-0.37625,-0.36352,-0.35091,-0.33841,-0.32604,-0.31378,-0.30164,-0.28962,-0.27771,-0.26593,-0.25425,-0.24269,-0.23123,-0.21989,-0.20865,-0.19752,-0.1865,-0.1756,-0.16482,-0.15417,-0.14364,-0.13325,-0.12299,-0.11286,-0.10286,-0.092974,-0.083214,-0.073569,-0.064034,-0.054605,-0.045278,-0.036048,-0.026911,-0.017861,-0.008892,-1.3186e-10,0.0088179,0.017559,0.026222,0.03481,0.043329,0.051781,0.060173,0.068511,0.076796,0.08503,0.093215,0.10135,0.10943,0.11746,0.12544,0.13335,0.1412,0.14899,0.1567,0.16434,0.17191,0.1794,0.18683,0.19418,0.20147,0.2087,0.21585,0.22294,0.22996,0.23693,0.24384,0.2507,0.25751,0.26428,0.271,0.27767,0.28431,0.29089,0.29744,0.30395,0.31043,0.31688,0.3233,0.32969,0.33605,0.34239,0.34869,0.35496,0.36117,0.36734,0.37344,0.37949,0.38549,0.39143,0.39732,0.40317,0.40897,0.41472,0.42044,0.42613,0.43181,0.43747,0.44312,0.44876,0.45439,0.45999,0.46558,0.47115,0.4767,0.48222,0.48773,0.49321,0.49868,0.50413,0.50957,0.51498,0.52038,0.52576,0.53112,0.53647,0.5418,0.54713,0.55244,0.55773,0.56299,0.56822,0.5734,0.57855,0.58364,0.58867,0.59364,0.59855,0.6034,0.60818,0.61291,0.61759,0.62221,0.62678,0.63131,0.63579,0.64022,0.64464,0.64902,0.65338,0.65772,0.66202,0.6663,0.67057,0.67483,0.67909,0.68334,0.68759,0.69184,0.69609,0.70033,0.70457,0.70881,0.71304,0.71727,0.7215,0.72572,0.72995,0.73418,0.73842,0.74265,0.74688,1.4093}; 

//    double CRF[256] ={-8.3864,-8.2303,-8.0742,-7.9182,-7.7621,-7.606,-7.4499,-7.2939,-7.1378,-6.9817,-6.8256,-6.6696,-6.5135,-6.3574,-6.2016,-6.0466,-5.8926,-5.7402,-5.5898,-5.4417,-5.2963,-5.1541,-5.0155,-4.8807,-4.7502,-4.6242,-4.5028,-4.3862,-4.2744,-4.1673,-4.065,-3.9672,-3.8738,-3.7845,-3.699,-3.6169,-3.5379,-3.4617,-3.388,-3.3165,-3.247,-3.1793,-3.1134,-3.0489,-2.9858,-2.9236,-2.8625,-2.8022,-2.7428,-2.6842,-2.6263,-2.5691,-2.5127,-2.4572,-2.4024,-2.3486,-2.2956,-2.2437,-2.1927,-2.1426,-2.0936,-2.0456,-1.9986,-1.9527,-1.9079,-1.8642,-1.8217,-1.7804,-1.7402,-1.7012,-1.6631,-1.6261,-1.5898,-1.5543,-1.5195,-1.4852,-1.4514,-1.418,-1.3848,-1.3518,-1.3189,-1.2861,-1.2534,-1.2207,-1.1881,-1.1554,-1.1227,-1.09,-1.0572,-1.0243,-0.9914,-0.9585,-0.9257,-0.8929,-0.8602,-0.8278,-0.7956,-0.7637,-0.7321,-0.7009,-0.6701,-0.6396,-0.6096,-0.5801,-0.5511,-0.5225,-0.4945,-0.4669,-0.4399,-0.4134,-0.3874,-0.362,-0.337,-0.3126,-0.2887,-0.2653,-0.2424,-0.2199,-0.1979,-0.1764,-0.1553,-0.1346,-0.1143,-0.0944,-0.0749,-0.0557,-0.0368,-0.0182,0,0.018,0.0356,0.0529,0.07,0.0868,0.1033,0.1195,0.1355,0.1512,0.1667,0.182,0.1971,0.212,0.2266,0.241,0.2551,0.2689,0.2825,0.2958,0.3089,0.3216,0.3341,0.3464,0.3585,0.3705,0.3822,0.3939,0.4054,0.4169,0.4282,0.4395,0.4507,0.4618,0.4728,0.4838,0.4946,0.5055,0.5162,0.527,0.5377,0.5484,0.5592,0.57,0.5808,0.5916,0.6025,0.6135,0.6245,0.6355,0.6467,0.6579,0.6692,0.6806,0.6922,0.7038,0.7157,0.7277,0.7399,0.7523,0.7649,0.7777,0.7907,0.8038,0.817,0.8302,0.8435,0.8568,0.8701,0.8834,0.8967,0.9101,0.9233,0.9365,0.9494,0.9622,0.9746,0.9869,0.9988,1.0105,1.0219,1.0329,1.0436,1.0541,1.0643,1.0743,1.0842,1.0939,1.1034,1.1128,1.122,1.1311,1.14,1.1489,1.1577,1.1666,1.1755,1.1845,1.1936,1.2027,1.2119,1.2211,1.2304,1.2399,1.2495,1.2593,1.2693,1.2794,1.2896,1.2998,1.3101,1.3203,1.3305,1.3408,1.351,1.3614,1.3718,1.3822,1.3927,1.4033,1.414,1.4247,1.4354,1.4462,1.457,1.4678,1.4785,3.1675};

    int t0 = 1000;      // initial expose time
    int exp_itv = 500;  // exposure interval 
    // irradiance E
    int E = 100; 
    double init_irr = log( E * (0.001 + 0 * (0.0005)));  // initial CRF = 2ms
//    double init_irr = log( E * (0.0005 + 0 * (0.00005)));  // initial CRF = 2ms
    double intensity_ratio ;

    // Search CRF and find minimum exposure time greater than t corr. init_irr
    int A;
    for(int i = 0; i < 255; i++){
        if(CRF[i] >= init_irr) {
            A = i;
            break;
        }
    }

    int exp_step = ((int)next_exp - (int)t0) / exp_itv + 1; //exptime = 1000 + (i-1)*500;
//    double syn_n_exp = init_irr + log(exp_step);
    double syn_n_exp = log(E *  (0.001 + ((exp_step-1) * 0.0005))); // for indoor 
//    double syn_n_exp = log(E * (0.0005 + ((exp_step-1) * 0.00005))); // for outdoor 
    for(int k = 0; k < 255; k++){
        if(CRF[k] > syn_n_exp){ 
            int B = k;  // 6~~
            intensity_ratio = (double)B / (double)A;
//    std::cout << "next_exp= " << syn_n_exp <<  ", " << next_exp<< ", " << exp_step << " A= " << A << ", B= " << B << ", Ratio= "<< intensity_ratio << std::endl;
            synth_img_t = init_img * intensity_ratio;   // Synth img along exposure time
            break;
        }
    }

    // for visualization
    if (visualize) {     
        // show text 
        CvFont font;
        cvInitFont (&font, CV_FONT_HERSHEY_PLAIN, 2.0, 2.0, 0, 2, 8);
        CvScalar red = CV_RGB (255, 0, 0);
        CvPoint str_pos = cvPoint (50, 50);
        char str[30];
        snprintf (str, sizeof str, "syn_t%d", 1000+ (exp_step-1)*500 );
        cv::Mat result;
        cv::cvtColor(synth_img_t, result, cv::COLOR_GRAY2BGR);
        cv::resize (result, result, cv::Size(320, 240));
        cv::namedWindow("synth_img_t", cv::WINDOW_AUTOSIZE);
        cv::putText(result, str, Point(50,30), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(10,255,10), 2, 8);
        cv::imshow("synth_img_t", result);
        cv::imshow("init img", init_img);
        cv::waitKey(0);
    }
}


void 
Img_eval::_synth_img_g (cv::Mat &synth_img_t, double &next_gain, cv::Mat &synth_img_g, bool visualize)
{
    double g0 = 0.0;    // 0.0 = initial gai
    double init_db = next_gain;
    double g_factor, gain_step;
    gain_step = (init_db*1.23412)/20; 
    g_factor = pow(7.01, gain_step); //factor = 7^((ii_g-1)/20); 
    synth_img_g = synth_img_t * g_factor ; // Synth img along gain

    if (visualize) {
        CvFont font;
        cvInitFont (&font, CV_FONT_HERSHEY_PLAIN, 2.0, 2.0, 0, 2, 8);
        CvScalar red = CV_RGB (255, 0, 0);
        CvPoint str_pos = cvPoint (50, 50);
        char str[30];
        snprintf (str, sizeof str, "syn_gain%d", (int)init_db );
        cv::Mat result;
        cv::cvtColor(synth_img_g, result, cv::COLOR_GRAY2BGR);
        cv::resize (result, result, cv::Size(320, 240));
        cv::namedWindow("synth_img_g", cv::WINDOW_AUTOSIZE);
        cv::putText(result, str, Point(50,30), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0,255,0), 2, 8);
        cv::imshow("synth_img_g", result);
        cv::waitKey(5);
    }           
}


void 
Img_eval::CreatErf(vector<vector<double> > &res1)
{
	ifstream infile("/home/irap-dron/git/cam_attr_contoller/src/exp_tester/erf.txt");
	string line;
	while (getline(infile,line)) // Read a line
	{
		res1.push_back(vector<double>()); // Add a new row to the matrix
		vector<double>& row = res1.back();
		cout << res1.size() << endl;
		istringstream iss(line);
		double evalue;
		while (iss >> evalue) // Read columns
			row.push_back(evalue); // Add a column to the current row
			cout << row.size() << endl;
	}
	infile.close();
}

void 
Img_eval::PrintErf(const vector<vector<double> > &res1) {
	for (auto row : res1) {
		for (auto evalue : row)
			cout <<  evalue  << "\t";
		cout << endl;
	}
}


double 
Img_eval::getPSNR(const Mat& I1, const Mat& I2)
{
    Mat s1;
    absdiff(I1, I2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2
     
    Scalar s = sum(s1);         // sum elements per channel
     
     double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels
 
        if( sse <= 1e-10) // for small values return zero
            return 0;
        else
        {
            double  mse =sse /(double)(I1.channels() * I1.total());
            double psnr = 10.0*log10((255*255)/mse);
        cout << "Psnr = " << psnr << endl;
            return psnr;

        }
}


double
Img_eval::calc_img_ent_grad (cv::Mat &img, bool visualize)
{
    // Convert to grayscale
    cvtColor(img, img, cv::COLOR_BGR2GRAY);
    cv::resize (img, img, cv::Size(188, 120));
    cv::Mat entropy, grad ;
	cv::Mat wmask(entropy.size(), CV_32F, 1.0); // ones
    img_entropy (img, entropy);
    img_wmask (entropy, wmask);
    img_grad (img, grad);
    getPSNR(img, img);

    double Gmean, Emean;
    img_Gmean (grad, Gmean);
    img_Emean (entropy, Emean);
    
	//printf("Gradient sss is %d %d \n", wcols, wrows) ;
 	
	Mat gradW = grad > Gmean * 0.01;
	gradW *= 1;
    gradW.convertTo (gradW, CV_32F, 1.0 / 255.0);
    


    double satparam = -2.5; //determine the satuation ratio
	Mat columnSum, mu;   
    img_columnSum (entropy, columnSum, mu);
	Mat Smask = satparam * Gmean * wmask;  //Smask == Sval, how to - value
    Mat Gour = ((gradW.mul(grad))+ Smask) ;

	Mat Gourstmp1, Gourstmp2;
    double Gours;
    img_Gours (Gour, Gourstmp1, Gourstmp2, Gours);
//     std::cout << "Gradsum=  " << Gmean * img.cols * img.rows <<" mean" << Gmean <<"SatSum= " << Gours-(Gmean * img.cols * img.rows) << " Gours= " << Gours << "img size = " << img.size() << std::endl;
//     cv::waitKey(1);

    if (visualize) {
        cv::resize (img, img, cv::Size(752, 480));
//	    cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
	    cv::namedWindow("Entropy Mask", cv::WINDOW_AUTOSIZE);	
//	    cv::namedWindow("Entropy Filter", cv::WINDOW_AUTOSIZE);
	    cv::namedWindow("Gradient Image", cv::WINDOW_AUTOSIZE);
//	    cv::imshow("Original", img);
	    cv::imshow("Entropy Mask", wmask);  //dst 
//	    cv::imshow("Entropy Filter", entropy);
	    cv::imshow("Gradient Image",grad);
        /// Wait until user exits the program
        cv::waitKey(10);

    }

    return Gours;
}


void
Img_eval::img_entropy (Mat &img, Mat &entropy)
{
    //Calculate Entropy Filter
    cv::Rect roi (0, 0, img.cols, img.rows);
    //cv::Mat dst (img.rows, img.cols, CV_32F);
    cv::Mat dst = Mat::zeros(img.rows, img.cols, CV_32F); // init with zero padded

    getLocalEntropyImage (img, roi, dst);
    cv::normalize (dst, dst, 0, 1, cv::NORM_MINMAX);
    dst.convertTo (entropy, CV_32F, 1.0);
    // std::cout << "Entropy:  " << entropy <<std::endl;
    //imshow("Entropy", entropy);
}

void 
Img_eval::img_wmask (Mat &entropy, Mat &wmask)
{
    // Create Entropy binary mask, wmask == [rs, cs] 
	Mat wmasktmp1(entropy.size(), CV_32F, 0.);
    Mat wmasktmp2(entropy.size(), CV_32F, 0.);

 	wmasktmp1 = entropy > 0.01;
    Mat tmp1; // 1 1 1 ...
    wmasktmp1.convertTo(tmp1, CV_32F, 0.0/-255.0);
    wmask = tmp1;
    wmasktmp2 = entropy <=  0.01; 
    Mat tmp2; 
    wmasktmp2.convertTo(tmp2, CV_32F, 1.0/255.0);
    
    wmask = tmp2;
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
	addWeighted( abs_grad_x, 1.49999 , abs_grad_y, 1.49999, 0, grad );

    grad.convertTo(grad , CV_32F, 1.0/255.0);
    // std::cout << "grad:  " << grad <<std::endl;
}

void 
Img_eval::img_Gmean (Mat &grad, double &Gmean)
{
    int wrows = grad.rows;
	int wcols = grad.cols; 
	Gmean = sum(grad)[0]  / (wrows * wcols);  // mean(mean(Gmag))) 
}

void 
Img_eval::img_Emean (Mat &entropy, double &Emean)
{
    int wrows = entropy.rows;
	int wcols = entropy.cols; 
	Emean = sum(entropy)[0] / (wrows * wcols); // mean(mean(J))) 
}


void 
Img_eval::img_columnSum (Mat &entropy, Mat &columnSum, Mat &mu)
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

void 
Img_eval::img_Gours (Mat &Gour, Mat &Gourstmp1, Mat &Gourstmp2, double &Gours)
{
	for (int i=0; i<Gour.rows; i++)	{
	    Gourstmp1.push_back(cv::sum(Gour.row(i))[0]);
	}
	
	for (int j=0; j<Gourstmp1.cols; j++) {
    	Gourstmp2.push_back(cv::sum(Gourstmp1.col(j))[0]);
	}

	Gours = Gourstmp2.at<double>(0);
}


// TODO normal pdf  normpdf equvalent !! 	
//	const float ONE_OVER_SQRT_2PI = 0.39894228040143267793994605993438;
//    std::cout << "Mean: kkk " << k <<std::endl;

//	Mat normal_pdf(columnSum, mu.at<double>(0,0), std[0]); 
//	{ 
//    return (ONE_OVER_SQRT_2PI/std)*exp(-0.5*squared((columnSum-mu)/std));
//	}

int 
Img_eval::sub_to_ind (int *coords, int *cumprod, int num_dims)
{
    int index = 0;
    int k;

    assert(coords != NULL);
    assert(cumprod != NULL);
    assert(num_dims > 0);

    for (k = 0; k < num_dims; k++) {
        index += coords[k] * cumprod[k];
    }

    return index;
}



void 
Img_eval::ind_to_sub (int p, int num_dims, const int size[],
                      int *cumprod, int *coords)
{
    int j;

    assert(num_dims > 0);
    assert(coords != NULL);
    assert(cumprod != NULL);

    for (j = num_dims - 1; j >= 0; j--) {
        coords[j] = p / cumprod[j];
        p = p % cumprod[j];
    }
}

void 
Img_eval::getLocalEntropyImage (cv::Mat &gray, cv::Rect &roi, cv::Mat &entropy)
{
    using namespace cv;
    clock_t func_begin, func_end;
    func_begin = clock();
    //1.define nerghbood model,here it's 9*9
    int neighbood_dim = 2;
    int neighbood_size[] = {3, 3};

    //2.Pad gray_src
    Mat gray_src_mat(gray);
    Mat pad_mat;
    int left = (neighbood_size[0] - 1) / 2;  // 9 = > 4 , 5 => 2 ,  3 => 1
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
    float entroy_table[10];  // 9 = > 82 , 5 => 26, 3 => 10 
    const float log2 = log(2.0f);
    entroy_table[0] = 0.0;
    float frequency = 0;
    for (int i = 1; i < 10; i++){  // 82, 50, 26, 10 
        frequency = (float)i / 3;  // 9, 8, 5, 3 
        entroy_table[i] = frequency * (log(frequency) / log2);
    }

    int neighbood_index;
    //        int max_index=pad_src->cols*pad_src->rows;
    float e;
    int current_index = 0;
    int current_index_in_origin = 0;
    for (int y = roi.y; y < roi.height; y++){
        current_index = y * pad_src->cols;
        current_index_in_origin = (y - 1) * gray.cols; //     neighbood_size[1]  9->4, 7->3, 5->2 3 -> 1
//cerr << "yy = " << y  << "gray = " << (y - 4) * gray.cols << endl;
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
    //double func_time = (double)(func_end - func_begin) / CLOCKS_PER_SEC;
    //std::cout << "func time  " << func_time << std::endl;
}

//void Img_eval::GammaCorrection(Mat& src, Mat& dst, float fGamma)
//{
//    CV_Assert(src.data);

//    // accept only char type matrices
//    CV_Assert(src.depth() != sizeof(uchar));

//    // build look up table
//    unsigned char lut[256];
//    for (int i = 0; i < 256; i++)
//    {
//        lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0 ), fGamma / 100) * 255.0f);
//    }

//    dst = src.clone();
//    const int channels = dst.channels();
//    switch (channels)
//    {
//        case 2:
//        {
//            MatIterator_<uchar> it, end;
//            for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
//            //*it = pow((float)(((*it))/255.0), fGamma) * 255.0;
//            *it = lut[(*it)];
//            break;
//        }
//        case 3:
//        {
//            MatIterator_<Vec3b> it, end;
//            for (it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++)
//            {
//            (*it)[0] = lut[((*it)[0])];
//            (*it)[1] = lut[((*it)[1])];
//            (*it)[2] = lut[((*it)[2])];
//        }
//            break;
//        }
//    }
//}


