#!/bin/sh

dir = $pwd
cd /home/ychoi/ATOM/build/

./ExperimentAnalysisPlot L1F2X
# ./ExperimentAnalysisPlot L1F3X
# ./ExperimentAnalysisPlot L1F4X
./ExperimentAnalysisPlot L1REFX
./ExperimentAnalysisPlot L1F2O
# ./ExperimentAnalysisPlot L1F3O
# ./ExperimentAnalysisPlot L1F4O
./ExperimentAnalysisPlot L1REFO
./ExperimentAnalysisPlot L3F2X
# ./ExperimentAnalysisPlot L3F3X
# ./ExperimentAnalysisPlot L3F4X
./ExperimentAnalysisPlot L3REFX
./ExperimentAnalysisPlot L3F2O
# ./ExperimentAnalysisPlot L3F3O
# ./ExperimentAnalysisPlot L3F4O
./ExperimentAnalysisPlot L3REFO
./ExperimentAnalysisPlot L7F2X
# ./ExperimentAnalysisPlot L7F3X
# ./ExperimentAnalysisPlot L7F4X
./ExperimentAnalysisPlot L7REFX
./ExperimentAnalysisPlot L7F2O
# ./ExperimentAnalysisPlot L7F3O
# ./ExperimentAnalysisPlot L7F4O
./ExperimentAnalysisPlot L7REFO
./ExperimentAnalysisPlot L20F2X
# ./ExperimentAnalysisPlot L20F3X
# ./ExperimentAnalysisPlot L20F4X
./ExperimentAnalysisPlot L20REFX
./ExperimentAnalysisPlot L20F2O
# ./ExperimentAnalysisPlot L20F3O
# ./ExperimentAnalysisPlot L20F4O
./ExperimentAnalysisPlot L20REFO


cd $dir