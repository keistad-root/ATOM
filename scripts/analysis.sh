#!/bin/sh

../build/Merge ~/ATOM/config/experiment/$1/merge.conf
../build/ExperimentMasking ~/ATOM/config/experiment/$1/masking.conf
../build/ExperimentPloting ~/ATOM/config/experiment/$1/masking_plot.conf
../build/ExperimentMasking ~/ATOM/config/experiment/$1/analysis.conf
../build/ExperimentPloting ~/ATOM/config/experiment/$1/analysis_plot.conf
