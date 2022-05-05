#!/bin/bash

#rm -fr ./experiments/uniform_medium_0.1_5_0.2-step_0.2-dep
#./generator 1.0 5.2 0.2 25 uniform_medium 0.1 1 0.2 uniform_medium_0.1_5_0.2-step_0.2-dep
#rm -fr ./experiments/uniform_medium_0.1_5_0.2-step_0.8-dep
#./generator 1.0 5.2 0.2 25 uniform_medium 0.1 1 0.8 uniform_medium_0.1_5_0.2-step_0.8-dep
#
#rm -fr ./experiments/uniform_light_0.1_5_0.2-step_0.2-dep
#./generator 1.0 5.2 0.2 25 uniform_light 0.1 1 0.2 uniform_light_0.1_5_0.2-step_0.2-dep
#rm -fr ./experiments/uniform_light_0.1_5_0.2-step_0.8-dep
#./generator 1.0 5.2 0.2 25 uniform_light 0.1 1 0.8 uniform_light_0.1_5_0.2-step_0.8-dep
#
#rm -fr ./experiments/uniform_heavy_0.1_5_0.2-step_0.2-dep
#./generator 1.0 5.2 0.2 25 uniform_heavy 0.1 1 0.2 uniform_heavy_0.1_5_0.2-step_0.2-dep
#rm -fr ./experiments/uniform_heavy_0.1_5_0.2-step_0.8-dep
#./generator 1.0 5.2 0.2 25 uniform_heavy 0.1 1 0.8 uniform_heavy_0.1_5_0.2-step_0.8-dep
#
#rm -fr ./experiments/bimodal_medium_0.1_5_0.2-step_0.2-dep
#./generator 1.0 5.2 0.2 25 bimodal_medium 0.1 1 0.2 bimodal_medium_0.1_5_0.2-step_0.2-dep
#rm -fr ./experiments/bimodal_medium_0.1_5_0.2-step_0.8-dep
#./generator 1.0 5.2 0.2 25 bimodal_medium 0.1 1 0.8 bimodal_medium_0.1_5_0.2-step_0.8-dep
#
#rm -fr ./experiments/bimodal_light_0.1_5_0.2-step_0.2-dep
#./generator 1.0 5.2 0.2 25 bimodal_light 0.1 1 0.2 bimodal_light_0.1_5_0.2-step_0.2-dep
#rm -fr ./experiments/bimodal_light_0.1_5_0.2-step_0.8-dep
#./generator 1.0 5.2 0.2 25 bimodal_light 0.1 1 0.8 bimodal_light_0.1_5_0.2-step_0.8-dep
#
#rm -fr ./experiments/bimodal_heavy_0.1_5_0.2-step_0.2-dep
#./generator 1.0 5.2 0.2 25 bimodal_heavy 0.1 1 0.2 bimodal_heavy_0.1_5_0.2-step_0.2-dep
#rm -fr ./experiments/bimodal_heavy_0.1_5_0.2-step_0.8-dep
#./generator 1.0 5.2 0.2 25 bimodal_heavy 0.1 1 0.8 bimodal_heavy_0.1_5_0.2-step_0.8-dep


rm -fr ./experiments/uniform_light_0.5_1.5_0.25-step
./generator 0.5 1.75 0.25 10 uniform_light 0.1 1 0 10 uniform_light_0.5_1.5_0.25-step

rm -fr ./experiments/uniform_medium_0.5_1.5_0.25-step
./generator 0.5 1.75 0.25 10 uniform_medium 0.1 1 0 10 uniform_medium_0.5_1.5_0.25-step

rm -fr ./experiments/uniform_heavy_0.5_1.5_0.25-step
./generator 0.5 1.75 0.25 10 uniform_heavy 0.1 1 0 10 uniform_heavy_0.5_1.5_0.25-step
