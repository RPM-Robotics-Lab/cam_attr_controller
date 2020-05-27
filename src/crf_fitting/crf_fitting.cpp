#include <vector>
#include <iostream>
#include <fstream>

#include <opencv2/photo.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgcodecs.hpp"

using namespace cv;
using namespace std;


void readImagesAndTimes(vector<Mat> &images, vector<float> &times)
{

  int numImages = 4;

//  static const float timesArray[] = {0.001, 0.007, 0.013, 0.020 }; //1ms, 7ms, 13. 20ms
  static const float timesArray[] = {1/30.0f,0.25,2.5,15.0};
  times.assign(timesArray, timesArray + numImages);

  static const char* filenames[] = {"../../data/crf_fitting/1.png",
                                    "../../data/crf_fitting/13.png",
                                    "../../data/crf_fitting/25.png",
                                    "../../data/crf_fitting/39.png"};
  for(int i=0; i < numImages; i++)
  {
    Mat im = imread(filenames[i]);
    images.push_back(im);
  }

}

void writeCSV(string filename, Mat m)
{
    std::ofstream myfile;
    myfile.open(filename.c_str());
    myfile<< cv::format(m, cv::Formatter::FMT_CSV) << std::endl;
    myfile.close();
}


int main(int, char**argv)
{
  // Read images and exposure times
  cout << "Reading images ... " << endl;
  vector<Mat> images;
  vector<float> times;
  readImagesAndTimes(images, times);


  // Align input images
  cout << "Aligning images ... " << endl;
  Ptr<AlignMTB> alignMTB = createAlignMTB();
  alignMTB->process(images, images);

  // Obtain Camera Response Function (CRF)
  cout << "Calculating Camera Response Function (CRF) ... " << endl;
  Mat responseDebevec;
  Ptr<CalibrateDebevec> calibrateDebevec = createCalibrateDebevec();
  calibrateDebevec->process(images, responseDebevec, times);
  cout << "CRF = " << responseDebevec << endl;
  writeCSV("crf.csv", responseDebevec);

  return EXIT_SUCCESS;
}
