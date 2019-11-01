# EHealth-fall-detection
This repository contains code written for a fall-detection wearable device, implement on an Arduino Uno board with the EHealth shield. Early protoype code can be found in 'fall_detection_implementation', however it was then decided to send measurement data to a remove server via wifi and use machine learning techniques to detect falls. 

A wifi communication library is contained in EHealthWifi, and an example of using this library is contained in EHealthWifi_lib_test. Some data pre-processing and SVM analysis is contained in SVM_slip_detection.ipynb.
