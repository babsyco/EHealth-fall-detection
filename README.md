# EHealth-fall-detection
This repository contains code written for a fall-detection wearable device, implemented on an Arduino Uno with the EHealth shield. Code for an early protoype can be found in 'fall_detection_implementation', however it was then decided to send measurement data to a remote server via wifi and use machine learning techniques to detect falls. 

An original wifi communication library is contained in EHealthWifi, and an example of using this library is contained in EHealthWifi_lib_test. Some data pre-processing and SVM analysis is contained in SVM_slip_detection.ipynb.
