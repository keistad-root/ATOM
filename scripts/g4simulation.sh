#!/bin/bash

# Create a new tmux session named "g4simulation"
tmux new-session -d -s g4

# Create 10 additional windows (total 11 windows)
for i in {1..11}; do
    tmux new-window -t g4
done

# Send commands to each window
tmux send-keys -t g4:0 'htop' C-m
tmux send-keys -t g4:1 './G4Simulation L1F4AX_DX000; ./G4Simulation L1F4AX_DY000' C-m
tmux send-keys -t g4:2 './G4Simulation L1F4AX_DX002; ./G4Simulation L1F4AX_DY002' C-m
tmux send-keys -t g4:3 './G4Simulation L1F4AX_DX004; ./G4Simulation L1F4AX_DY004' C-m
tmux send-keys -t g4:4 './G4Simulation L1F4AX_DX006; ./G4Simulation L1F4AX_DY006' C-m
tmux send-keys -t g4:5 './G4Simulation L1F4AX_DX008; ./G4Simulation L1F4AX_DY008' C-m
tmux send-keys -t g4:6 './G4Simulation L1F4AX_DX010; ./G4Simulation L1F4AX_DY010' C-m
tmux send-keys -t g4:7 './G4Simulation L1F4AX_DX012; ./G4Simulation L1F4AX_DY012' C-m
tmux send-keys -t g4:8 './G4Simulation L1F4AX_DX014; ./G4Simulation L1F4AX_DY014' C-m
tmux send-keys -t g4:9 './G4Simulation L1F4AX_DX016; ./G4Simulation L1F4AX_DY016' C-m
tmux send-keys -t g4:10 './G4Simulation L1F4AX_DX018; ./G4Simulation L1F4AX_DY018' C-m
tmux send-keys -t g4:11 './G4Simulation L1F4AX_DX020; ./G4Simulation L1F4AX_DY020' C-m

# Attach to the tmux session
tmux attach-session -t g4