# Camera Attribute Controller
camera attribute controller

# Modules

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

## Testers (C/C++)
