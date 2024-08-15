#!/bin/sh

# ./Merge config/experiment/merge_1mm_1phi_alphagamma.conf
# ./Merge config/experiment/merge_1mm_1phi_gamma.conf
# ./Merge config/experiment/merge_1mm_2phi_alphagamma.conf
# ./Merge config/experiment/merge_1mm_2phi_gamma.conf
# ./Merge config/experiment/merge_1mm_3phi_alphagamma.conf
# ./Merge config/experiment/merge_1mm_3phi_gamma.conf
# ./Merge config/experiment/merge_1mm_4phi_alphagamma.conf
# ./Merge config/experiment/merge_1mm_4phi_gamma.conf
# ./Merge config/experiment/merge_1mm_7phi_alphagamma.conf
# ./Merge config/experiment/merge_1mm_7phi_gamma.conf
# ./Merge config/experiment/merge_3mm_7phi_alphagamma.conf
# ./Merge config/experiment/merge_3mm_7phi_gamma.conf
# ./Merge config/experiment/merge_7mm_7phi_alphagamma.conf
# ./Merge config/experiment/merge_7mm_7phi_gamma.conf
# ./Merge config/experiment/merge_20mm_7phi_alphagamma.conf
# ./Merge config/experiment/merge_20mm_7phi_gamma.conf

./ExperimentAnalysis config/experiment/experiment_1mm_1phi_alphagamma.conf
./ExperimentAnalysis config/experiment/experiment_1mm_1phi_gamma.conf
./ExperimentAnalysis config/experiment/experiment_1mm_2phi_alphagamma.conf
./ExperimentAnalysis config/experiment/experiment_1mm_2phi_gamma.conf
./ExperimentAnalysis config/experiment/experiment_1mm_3phi_alphagamma.conf
./ExperimentAnalysis config/experiment/experiment_1mm_3phi_gamma.conf
./ExperimentAnalysis config/experiment/experiment_1mm_4phi_alphagamma.conf
./ExperimentAnalysis config/experiment/experiment_1mm_4phi_gamma.conf
./ExperimentAnalysis config/experiment/experiment_1mm_7phi_alphagamma.conf
./ExperimentAnalysis config/experiment/experiment_1mm_7phi_gamma.conf
./ExperimentAnalysis config/experiment/experiment_3mm_7phi_alphagamma.conf
./ExperimentAnalysis config/experiment/experiment_3mm_7phi_gamma.conf
./ExperimentAnalysis config/experiment/experiment_7mm_7phi_alphagamma.conf
./ExperimentAnalysis config/experiment/experiment_7mm_7phi_gamma.conf
./ExperimentAnalysis config/experiment/experiment_20mm_7phi_alphagamma.conf
./ExperimentAnalysis config/experiment/experiment_20mm_7phi_gamma.conf

# ./CompareExperimentData config/experiment/comparison_entry_calibrated.conf
# ./CompareExperimentData config/experiment/comparison_time_calibrated.conf