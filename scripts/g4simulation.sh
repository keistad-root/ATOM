#!/bin/bash

# Create a new tmux session named "g4simulation"
tmux new-session -d -s g4

# Create 10 additional windows (total 11 windows)
for i in {1..9}; do
    tmux new-window -t g4
done

# Send commands to each window
tmux send-keys -t g4:0 'htop' C-m
tmux send-keys -t g4:1 './G4Simulation L1F4AX_AY000; ./G4Simulation L1F4AX_AY005' C-m
tmux send-keys -t g4:2 './G4Simulation L1F4AX_AY010; ./G4Simulation L1F4AX_AY015' C-m
tmux send-keys -t g4:3 './G4Simulation L1F4AX_AY020; ./G4Simulation L1F4AX_AY025' C-m
tmux send-keys -t g4:4 './G4Simulation L1F4AX_AY030; ./G4Simulation L1F4AX_AY035' C-m
tmux send-keys -t g4:5 './G4Simulation L1F4AX_AY040; ./G4Simulation L1F4AX_AY045' C-m
tmux send-keys -t g4:6 './G4Simulation L1F4AX_AY050; ./G4Simulation L1F4AX_AY055' C-m
tmux send-keys -t g4:7 './G4Simulation L1F4AX_AY060; ./G4Simulation L1F4AX_AY065' C-m
tmux send-keys -t g4:8 './G4Simulation L1F4AX_AY070; ./G4Simulation L1F4AX_AY075' C-m
tmux send-keys -t g4:9 './G4Simulation L1F4AX_AY080; ./G4Simulation L1F4AX_AY085' C-m

# Attach to the tmux session
tmux attach-session -t g4