#!/bin/bash

# Create a new tmux session named "G4SimulationExtract"
tmux new-session -d -s g4

# Create 10 additional windows (total 11 windows)
for i in {1..9}; do
    tmux new-window -t g4
done

# Send commands to each window
tmux send-keys -t g4:0 'htop' C-m
tmux send-keys -t g4:1 './G4SimulationExtract L1F4AX_AX000; ./G4SimulationExtract L1F4AX_AX005; ./G4SimulationExtract L1F4AX_AY000; ./G4SimulationExtract L1F4AX_AY005' C-m
tmux send-keys -t g4:2 './G4SimulationExtract L1F4AX_AX010; ./G4SimulationExtract L1F4AX_AX015; ./G4SimulationExtract L1F4AX_AY010; ./G4SimulationExtract L1F4AX_AY015' C-m
tmux send-keys -t g4:3 './G4SimulationExtract L1F4AX_AX020; ./G4SimulationExtract L1F4AX_AX025; ./G4SimulationExtract L1F4AX_AY020; ./G4SimulationExtract L1F4AX_AY025' C-m
tmux send-keys -t g4:4 './G4SimulationExtract L1F4AX_AX030; ./G4SimulationExtract L1F4AX_AX035; ./G4SimulationExtract L1F4AX_AY030; ./G4SimulationExtract L1F4AX_AY035' C-m
tmux send-keys -t g4:5 './G4SimulationExtract L1F4AX_AX040; ./G4SimulationExtract L1F4AX_AX045; ./G4SimulationExtract L1F4AX_AY040; ./G4SimulationExtract L1F4AX_AY045' C-m
tmux send-keys -t g4:6 './G4SimulationExtract L1F4AX_AX050; ./G4SimulationExtract L1F4AX_AX055; ./G4SimulationExtract L1F4AX_AY050; ./G4SimulationExtract L1F4AX_AY055' C-m
tmux send-keys -t g4:7 './G4SimulationExtract L1F4AX_AX060; ./G4SimulationExtract L1F4AX_AX065; ./G4SimulationExtract L1F4AX_AY060; ./G4SimulationExtract L1F4AX_AY065' C-m
tmux send-keys -t g4:8 './G4SimulationExtract L1F4AX_AX070; ./G4SimulationExtract L1F4AX_AX075; ./G4SimulationExtract L1F4AX_AY070; ./G4SimulationExtract L1F4AX_AY075' C-m
tmux send-keys -t g4:9 './G4SimulationExtract L1F4AX_AX080; ./G4SimulationExtract L1F4AX_AX085; ./G4SimulationExtract L1F4AX_AY080; ./G4SimulationExtract L1F4AX_AY085' C-m
# tmux send-keys -t g4:10 './G4SimulationExtract L1F4AX_AX045; ./G4SimulationExtract L1F4AX_AX018' C-m
# tmux send-keys -t g4:11 './G4SimulationExtract L1F4AX_AX050; ./G4SimulationExtract L1F4AX_AX020' C-m

# Attach to the tmux session
tmux attach-session -t g4