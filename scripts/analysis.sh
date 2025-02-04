#!/bin/bash

cd ~/ATOM/build

for TAG in L1F2X L1F2O L1F3X L1F3O L1F4X L1F4O L1F7X L1F7O L1REFX L1REFO L3F2X L3F2O L3F3X L3F3O L3F4X L3F4O L3F7X L3F7O L3REFX L3REFO L7F2X L7F2O L7F3X L7F3O L7F4X L7F4O L7F7X L7F7O L7REFX L7REFO L20F2X L20F2O L20F3X L20F3O L20F4X L20F4O L20F7X L20F7O L20REFX L20REFO
do
	./ExperimentMasking $TAG
	./ExperimentAnalysis $TAG
	./ExperimentAnalysisPlot $TAG
done
