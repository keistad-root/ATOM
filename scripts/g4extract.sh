#!/bin/bash

# Remove the old tmux session named "g4simulation"
tmux kill-session -t g4simulation

# Create a new tmux session named "g4simulation"
tmux new-session -d -s g4simulation

# Create 10 additional windows (total 11 windows)
for i in {1..9}; do
    tmux new-window -t g4simulation
done

# Send commands to each window
tmux send-keys -t g4simulation:0 'htop' C-m
tmux send-keys -t g4simulation:1 './G4SimulationExtract L1F2AX' C-m
tmux send-keys -t g4simulation:2 './G4SimulationExtract L1F2AO' C-m
tmux send-keys -t g4simulation:3 './G4SimulationExtract L1F3AX' C-m
tmux send-keys -t g4simulation:4 './G4SimulationExtract L1F3AO' C-m
tmux send-keys -t g4simulation:5 './G4SimulationExtract L1F4AX' C-m
tmux send-keys -t g4simulation:6 './G4SimulationExtract L1F4AO' C-m
tmux send-keys -t g4simulation:7 './G4SimulationExtract L1F7AX' C-m
tmux send-keys -t g4simulation:8 './G4SimulationExtract L1REFAX' C-m
tmux send-keys -t g4simulation:9 './G4SimulationExtract L1REFAO' C-m
# tmux send-keys -t g4simulation:10 './G4SimulationExtract L1F4AX_L104' C-m
# tmux send-keys -t g4simulation:11 './G4SimulationExtract L1F4AX_L105' C-m

# Attach to the tmux session
tmux attach-session -t g4simulation
