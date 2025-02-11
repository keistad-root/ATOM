#!/bin/bash

# Create a new tmux session named "g4simulation"
tmux new-session -d -s g4

# Create 10 additional windows (total 11 windows)
for i in {1..9}; do
    tmux new-window -t g4
done

# Send commands to each window
tmux send-keys -t g4:0 'htop' C-m
tmux send-keys -t g4:1 './G4Simulation L1F4AX_AX000; ./G4Simulation L1F4AX_AX005; ./G4Simulation L1F4AX_AY000; ./G4Simulation L1F4AX_AY005' C-m
tmux send-keys -t g4:2 './G4Simulation L1F4AX_AX010; ./G4Simulation L1F4AX_AX015; ./G4Simulation L1F4AX_AY000; ./G4Simulation L1F4AX_AY005' C-m
tmux send-keys -t g4:3 './G4Simulation L1F4AX_AX020; ./G4Simulation L1F4AX_AX025; ./G4Simulation L1F4AX_AY000; ./G4Simulation L1F4AX_AY005' C-m
tmux send-keys -t g4:4 './G4Simulation L1F4AX_AX030; ./G4Simulation L1F4AX_AX035; ./G4Simulation L1F4AX_AY000; ./G4Simulation L1F4AX_AY005' C-m
tmux send-keys -t g4:5 './G4Simulation L1F4AX_AX040; ./G4Simulation L1F4AX_AX045; ./G4Simulation L1F4AX_AY000; ./G4Simulation L1F4AX_AY005' C-m
tmux send-keys -t g4:6 './G4Simulation L1F4AX_AX050; ./G4Simulation L1F4AX_AX055; ./G4Simulation L1F4AX_AY000; ./G4Simulation L1F4AX_AY005' C-m
tmux send-keys -t g4:7 './G4Simulation L1F4AX_AX060; ./G4Simulation L1F4AX_AX065; ./G4Simulation L1F4AX_AY000; ./G4Simulation L1F4AX_AY005' C-m
tmux send-keys -t g4:8 './G4Simulation L1F4AX_AX070; ./G4Simulation L1F4AX_AX075; ./G4Simulation L1F4AX_AY000; ./G4Simulation L1F4AX_AY005' C-m
tmux send-keys -t g4:9 './G4Simulation L1F4AX_AX080; ./G4Simulation L1F4AX_AX085; ./G4Simulation L1F4AX_AY000; ./G4Simulation L1F4AX_AY005' C-m

# Attach to the tmux session
tmux attach-session -t g4