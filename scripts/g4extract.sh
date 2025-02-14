#!/bin/bash

# Create a new tmux session named "G4SimulationExtract"
tmux new-session -d -s g4

# Create 10 additional windows (total 11 windows)
for i in {1..11}; do
    tmux new-window -t g4
done

# Send commands to each window
tmux send-keys -t g4:0 'htop' C-m
tmux send-keys -t g4:1 './G4SimulationExtract L1F4AX_LS045; ./G4SimulationExtract L1F4AX_LA195' C-m
tmux send-keys -t g4:2 './G4SimulationExtract L1F4AX_LS046; ./G4SimulationExtract L1F4AX_LA196' C-m
tmux send-keys -t g4:3 './G4SimulationExtract L1F4AX_LS047; ./G4SimulationExtract L1F4AX_LA197' C-m
tmux send-keys -t g4:4 './G4SimulationExtract L1F4AX_LS048; ./G4SimulationExtract L1F4AX_LA198' C-m
tmux send-keys -t g4:5 './G4SimulationExtract L1F4AX_LS049; ./G4SimulationExtract L1F4AX_LA199' C-m
tmux send-keys -t g4:6 './G4SimulationExtract L1F4AX_LS050; ./G4SimulationExtract L1F4AX_LA200' C-m
tmux send-keys -t g4:7 './G4SimulationExtract L1F4AX_LS051; ./G4SimulationExtract L1F4AX_LA201' C-m
tmux send-keys -t g4:8 './G4SimulationExtract L1F4AX_LS052; ./G4SimulationExtract L1F4AX_LA202' C-m
tmux send-keys -t g4:9 './G4SimulationExtract L1F4AX_LS053; ./G4SimulationExtract L1F4AX_LA203' C-m
tmux send-keys -t g4:10 './G4SimulationExtract L1F4AX_LS054; ./G4SimulationExtract L1F4AX_LA204' C-m
tmux send-keys -t g4:11 './G4SimulationExtract L1F4AX_LS055; ./G4SimulationExtract L1F4AX_LA205' C-m

# Attach to the tmux session
tmux attach-session -t g4