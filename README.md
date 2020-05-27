# Camera Attribute Controller
camera attribute controller

Proactive camera control scheme for two main attributes of the camera: exposure time (shutter speed) and gain control.

Unlike existing approaches, this camera attributes control is formulated with optimization problems in which the underlying function is not known a priori.

# Modules
The overall procedure consists of three modules, 

(1) image synthesis, (2) metric evaluation, and (3) control module.


![ezgif com-resize](https://user-images.githubusercontent.com/21049835/82980471-817de600-a024-11ea-9283-4f260c658ddb.gif)


## Matlab

1) controller

Run `exp_gain_controller` to run with sample images.

2) synthetic

Run `run_synth_gen` to synthesize using a sample image.

3) Libraries: fcns_synth and fcns_gp

## C/C++
1) crf_fitting

Given four images, fit a CRF curve.

2) data_collector

Image collector using Bluefox2Driver.

3) exp_ctrl

Main executable, camera exposure controller. Similar funcitonality as in tester code except it captures an image from the camera directly.

4) gpgo

Using Gaussian Process Global Optimzer, compute optimal exposure time and gain.

5) irp_imgeval++ and irp_gpgo++

Image evaluation and camera attribute control library.

## Dependency

### Download OpenCV
1) OpenCV 3.4.0
 
2) mkdir opencv

3) cd opencv

4) wget -O opencv.zip https://github.com/opencv/opencv/archive/3.4.0.zip 

5) wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/3.4.0.zip

6) unzip opencv.zip

7) unzip opencv_contrib.zip

8) cd opencv-3.4.0

### Compile OpenCV
1) mkdir build

2) cd build

3) cmake -D CMAKE_BUILD_TYPE=RELEASE \
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
-D WITH_V4L=ON  \
-D WITH_FFMPEG=ON \
-D WITH_XINE=ON \
-D BUILD_NEW_PYTHON_SUPPORT=ON \
-D PYTHON2_INCLUDE_DIR=/usr/include/python2.7 \
-D PYTHON2_NUMPY_INCLUDE_DIRS=/usr/lib/python2.7/dist-packages/numpy/core/include/ \
-D PYTHON2_PACKAGES_PATH=/usr/lib/python2.7/dist-packages \
-D PYTHON2_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython2.7.so \
-D PYTHON3_INCLUDE_DIR=/usr/include/python3.6m \
-D PYTHON3_NUMPY_INCLUDE_DIRS=/usr/lib/python3/dist-packages/numpy/core/include/  \
-D PYTHON3_PACKAGES_PATH=/usr/lib/python3/dist-packages \
-D PYTHON3_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.6m.so \
../

4) make 
### Install OpenCV
1) sudo make install

2) sudo sh -c echo '/usr/local/lib/' > sudo /etc/ld.so.conf.d/opencv.conf

3) sudo ldconfig

## Compile
You can use the following commands to download and compile the package.

1) mkdir build

2) cd build

3) cmake ..

4) make


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
