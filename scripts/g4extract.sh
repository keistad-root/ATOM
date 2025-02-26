#!/bin/bash

# Kill previous one
tmux kill-session -t g4

# Create a new tmux session named "G4SimulationExtract.out"
tmux new-session -d -s g4

# Create 10 additional windows (total 11 windows)
for i in {1..8}; do
    tmux new-window -t g4
done

# Send commands to each window
tmux send-keys -t g4:0 'htop' C-m
tmux send-keys -t g4:1 './G4SimulationExtract.out L7F2AX' C-m
tmux send-keys -t g4:2 './G4SimulationExtract.out L7F2AO' C-m
tmux send-keys -t g4:3 './G4SimulationExtract.out L7F3AX' C-m
tmux send-keys -t g4:4 './G4SimulationExtract.out L7F3AO' C-m
tmux send-keys -t g4:5 './G4SimulationExtract.out L7F4AX' C-m
tmux send-keys -t g4:6 './G4SimulationExtract.out L7F4AO' C-m
tmux send-keys -t g4:7 './G4SimulationExtract.out L7F7AX' C-m
tmux send-keys -t g4:8 './G4SimulationExtract.out L7F7AO' C-m


# Attach to the tmux session
tmux attach-session -t g4