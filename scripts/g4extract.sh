#!/bin/bash

# Remove the old tmux session named "g4simulation"
tmux kill-session -t g4simulation

# Create a new tmux session named "g4simulation"
tmux new-session -d -s g4simulation

# Create 10 additional windows (total 11 windows)
for i in {1..11}; do
    tmux new-window -t g4simulation
done

# Send commands to each window
tmux send-keys -t g4simulation:0 'htop' C-m
tmux send-keys -t g4simulation:1 './G4SimulationExtract L1F4AX_L095' C-m
tmux send-keys -t g4simulation:2 './G4SimulationExtract L1F4AX_L096' C-m
tmux send-keys -t g4simulation:3 './G4SimulationExtract L1F4AX_L097' C-m
tmux send-keys -t g4simulation:4 './G4SimulationExtract L1F4AX_L098' C-m
tmux send-keys -t g4simulation:5 './G4SimulationExtract L1F4AX_L099' C-m
tmux send-keys -t g4simulation:6 './G4SimulationExtract L1F4AX_L100' C-m
tmux send-keys -t g4simulation:7 './G4SimulationExtract L1F4AX_L101' C-m
tmux send-keys -t g4simulation:8 './G4SimulationExtract L1F4AX_L102' C-m
tmux send-keys -t g4simulation:9 './G4SimulationExtract L1F4AX_L103' C-m
tmux send-keys -t g4simulation:10 './G4SimulationExtract L1F4AX_L104' C-m
tmux send-keys -t g4simulation:11 './G4SimulationExtract L1F4AX_L105' C-m

# Attach to the tmux session
tmux attach-session -t g4simulation

# ./G4SimulationExtractExtract L1F2AX
# ./G4SimulationExtractExtract L1F2AO
# ./G4SimulationExtractExtract L1F3AX
# ./G4SimulationExtractExtract L1F3AO
# ./G4SimulationExtractExtract L1F4AX
# ./G4SimulationExtractExtract L1F4AO
# ./G4SimulationExtractExtract L1F7AX
# ./G4SimulationExtractExtract L1F7AO
# ./G4SimulationExtractExtract L1REFAX
# ./G4SimulationExtractExtract L1REFAO
# ./G4SimulationExtractExtract L3F2AX
# ./G4SimulationExtractExtract L3F2AO
# ./G4SimulationExtractExtract L3F3AX
# ./G4SimulationExtractExtract L3F3AO
# ./G4SimulationExtractExtract L3F4AX
# ./G4SimulationExtractExtract L3F4AO
# ./G4SimulationExtractExtract L3F7AX
# ./G4SimulationExtractExtract L3F7AO
# ./G4SimulationExtractExtract L3REFAX
# ./G4SimulationExtractExtract L3REFAO
# ./G4SimulationExtractExtract L7F2AX
# ./G4SimulationExtractExtract L7F2AO
# ./G4SimulationExtractExtract L7F3AX
# ./G4SimulationExtractExtract L7F3AO
# ./G4SimulationExtractExtract L7F4AX
# ./G4SimulationExtractExtract L7F4AO
# ./G4SimulationExtractExtract L7F7AX
# ./G4SimulationExtractExtract L7F7AO
# ./G4SimulationExtractExtract L7REFAX
# ./G4SimulationExtractExtract L7REFAO
# ./G4SimulationExtractExtract L20F2AX
# ./G4SimulationExtractExtract L20F2AO
# ./G4SimulationExtractExtract L20F3AX
# ./G4SimulationExtractExtract L20F3AO
# ./G4SimulationExtractExtract L20F4AX
# ./G4SimulationExtractExtract L20F4AO
# ./G4SimulationExtractExtract L20F7AX
# ./G4SimulationExtractExtract L20F7AO
# ./G4SimulationExtractExtract L20REFAX
# ./G4SimulationExtractExtract L20REFAO
