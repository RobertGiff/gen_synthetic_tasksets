#!/bin/bash

rm -fr ./experiments/uniform_medium_0.1_5_0.2-step
./generator 1.0 5.2 0.2 25 uniform_medium 0.1 1 uniform_medium_0.1_5_0.2-step
rm -fr ./experiments/uniform_light_0.1_5_0.2-step
./generator 1.0 5.2 0.2 25 uniform_light 0.1 1 uniform_light_0.1_5_0.2-step
rm -fr ./experiments/uniform_heavy_0.1_5_0.2-step
./generator 1.0 5.2 0.2 25 uniform_heavy 0.1 1 uniform_heavy_0.1_5_0.2-step

rm -fr ./experiments/bimodal_medium_0.1_5_0.2-step
./generator 1.0 5.2 0.2 25 bimodal_medium 0.1 1 bimodal_medium_0.1_5_0.2-step
rm -fr ./experiments/bimodal_light_0.1_5_0.2-step
./generator 1.0 5.2 0.2 25 bimodal_light 0.1 1 bimodal_light_0.1_5_0.2-step
rm -fr ./experiments/bimodal_heavy_0.1_5_0.2-step
./generator 1.0 5.2 0.2 25 bimodal_heavy 0.1 1 bimodal_heavy_0.1_5_0.2-step
