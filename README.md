# Proactive Camera Attribute Control using Bayesian Optimization

This Github repo is for a proactive camera controller. We control two main camera attributes: exposure time (shutter speed) and gain simultaneously. The overall procedure consists of three modules,

* image synthesis module
* metric evaluation module
* control module module

![ezgif com-resize](https://user-images.githubusercontent.com/21049835/82980471-817de600-a024-11ea-9283-4f260c658ddb.gif)

We provide Matlab and C/C++ code. Since this code is to control a camera, the code should be compiled together with the camera driver. In this sample code, we implement our controller code with Bluefox camera driver.

# Matlab

Matlab folder contains the following subfolders.

* **controller**<br>
  Run `exp_gain_controller` to run with sample images.
* **synthetic**<br>
  Run `run_synth_gen` to synthesize using a sample image.
* **data**<br>
  This folder contains sample images.
* **fcns_synth** and **fcns_gp**<br>
  These folders contain libraries to run camera control and synthesis.

Matlab scripts illustrate how the algorithm works using sample images. In the Matlab examples, we do not ***control*** the camera.

# C/C++

To effectively use this camera controller, the algorithm should run begin coupled with the camera driver. In this sample code, we show how to use with Bluefox camera driver. Thus some code has a high dependency with the `Bluefox2Driver`, but you are welcome to adjust the code to couple with your own camera driver.

The `src` folder contains C/C++ code to run camera controller.

* **crf_fitting**<br>
  Given four images, we fit a CRF curve.
* **gpgo**<br>
  Using Gaussian Process Global Optimzer, compute optimal exposure time and gain.
* **data_collector**<br>
  Image collector using `Bluefox2Driver`.
* **exp_ctrl**<br>
  This contains the main executable camera exposure controller. Similar functionality as in tester code except it captures an image from the camera directly using `Bluefox2Driver`.
* **irp_imgeval++** and **irp_gpgo++**<br>
  Image evaluation and camera attribute control library.


## Dependency (Pre-installation)

### 1) OpenCV

We have tested the code using `OpenCV 3.4.0`. You can skip this steps if you have `OpenCV 3.4.0` or higher installed.

```
mkdir opencv
cd opencv
wget -O opencv.zip https://github.com/opencv/opencv/archive/3.4.0.zip
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/3.4.0.zip
unzip opencv.zip
unzip opencv_contrib.zip
cd opencv-3.4.0
```

Compile OpenCV

```
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D WITH_TBB=OFF \
-D WITH_IPP=OFF \
-D WITH_1394=OFF \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_DOCS=OFF \
-D INSTALL_C_EXAMPLES=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D BUILD_EXAMPLES=OFF \
-D BUILD_TESTS=OFF \
-D BUILD_PERF_TESTS=OFF \
-D WITH_QT=OFF \
-D WITH_GTK=ON \
-D WITH_OPENGL=ON \
-D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-3.4.0/modules \
-D WITH_V4L=ON \
-D WITH_FFMPEG=ON \
-D WITH_XINE=ON \
-D BUILD_NEW_PYTHON_SUPPORT=ON \
-D PYTHON2_INCLUDE_DIR=/usr/include/python2.7 \
-D PYTHON2_NUMPY_INCLUDE_DIRS=/usr/lib/python2.7/dist-packages/numpy/core/include/ \
-D PYTHON2_PACKAGES_PATH=/usr/lib/python2.7/dist-packages \
-D PYTHON2_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython2.7.so \
-D PYTHON3_INCLUDE_DIR=/usr/include/python3.6m \
-D PYTHON3_NUMPY_INCLUDE_DIRS=/usr/lib/python3/dist-packages/numpy/core/include/ \
-D PYTHON3_PACKAGES_PATH=/usr/lib/python3/dist-packages \
-D PYTHON3_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.6m.so \
../
make
```

Install OpenCV

```
sudo make install
sudo sh -c echo '/usr/local/lib/' > sudo /etc/ld.so.conf.d/opencv.conf
sudo ldconfig
```

### 2) Bluefox2Driver

Goto third-party and install the driver.

```
cd third-party
./install_mvBlueFOX.sh
```

## Camera Controller Compile and Installation

You can use the following commands to download and compile the package.

```
mkdir build
cd build
cmake ..
make
```

## Testers (C/C++)


## Cite Camera Attribute Controller

@ARTICLE{jwkim-2020-tro,

author={J. {Kim} and Y. {Cho} and A. {Kim}},

journal={IEEE Transactions on Robotics},

title={Proactive Camera Attribute Control Using Bayesian Optimization for Illumination-Resilient Visual Navigation},

year={2020},

volume={},

number={},

pages={1-16},}
