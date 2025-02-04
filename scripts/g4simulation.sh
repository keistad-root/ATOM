#!/bin/bash

# Create a new tmux session named "g4simulation"
tmux new-session -d -s g4simulation

# Create 10 additional windows (total 11 windows)
for i in {1..13}; do
    tmux new-window -t g4simulation
done

# Send commands to each window
tmux send-keys -t g4simulation:0 'htop' C-m
tmux send-keys -t g4simulation:1 './G4Simulation L1F2AO' C-m
tmux send-keys -t g4simulation:2 './G4Simulation L1F3AO' C-m
tmux send-keys -t g4simulation:3 './G4Simulation L3F2AO' C-m
tmux send-keys -t g4simulation:4 './G4Simulation L3F3AO' C-m
tmux send-keys -t g4simulation:5 './G4Simulation L3F4AO' C-m
tmux send-keys -t g4simulation:6 './G4Simulation L3F7AO' C-m
tmux send-keys -t g4simulation:7 './G4Simulation L7F2AO' C-m
tmux send-keys -t g4simulation:8 './G4Simulation L7F3AO' C-m
tmux send-keys -t g4simulation:9 './G4Simulation L7F4AO' C-m
tmux send-keys -t g4simulation:10 './G4Simulation L20F2AO' C-m
tmux send-keys -t g4simulation:11 './G4Simulation L20F3AO' C-m
tmux send-keys -t g4simulation:12 './G4Simulation L20F4AO' C-m
tmux send-keys -t g4simulation:13 './G4Simulation L20F7AO' C-m

# Attach to the tmux session
tmux attach-session -t g4simulation


# #!/bin/sh

# ./G4Simulation L1F4AX_W0990
# ./G4Simulation L1F4AX_W1002
# ./G4Simulation L1F4AX_W1014
# ./G4Simulation L1F4AX_W1026
# ./G4Simulation L1F4AX_W1038
# ./G4Simulation L1F4AX_W1050
# ./G4Simulation L1F4AX_W1062
# ./G4Simulation L1F4AX_W1074
# ./G4Simulation L1F4AX_W1086
# ./G4Simulation L1F4AX_W1098
# ./G4Simulation L1F4AX_W1110


# ./G4Simulation L1F2AX
# ./G4Simulation L1F2AO
# ./G4Simulation L1F3AX
# ./G4Simulation L1F3AO
# ./G4Simulation L1F4AX
# ./G4Simulation L1F4AO
# ./G4Simulation L1F7AX
# ./G4Simulation L1F7AO
# ./G4Simulation L1REFAX
# ./G4Simulation L1REFAO
# ./G4Simulation L3F2AX
# ./G4Simulation L3F2AO
# ./G4Simulation L3F3AX
# ./G4Simulation L3F3AO
# ./G4Simulation L3F4AX
# ./G4Simulation L3F4AO
# ./G4Simulation L3F7AX
# ./G4Simulation L3F7AO
# ./G4Simulation L3REFAX
# ./G4Simulation L3REFAO
# ./G4Simulation L7F2AX
# ./G4Simulation L7F2AO
# ./G4Simulation L7F3AX
# ./G4Simulation L7F3AO
# ./G4Simulation L7F4AX
# ./G4Simulation L7F4AO
# ./G4Simulation L7F7AX
# ./G4Simulation L7F7AO
# ./G4Simulation L7REFAX
# ./G4Simulation L7REFAO
# ./G4Simulation L20F2AX
# ./G4Simulation L20F2AO
# ./G4Simulation L20F3AX
# ./G4Simulation L20F3AO
# ./G4Simulation L20F4AX
# ./G4Simulation L20F4AO
# ./G4Simulation L20F7AX
# ./G4Simulation L20F7AO
# ./G4Simulation L20REFAX
# ./G4Simulation L20REFAO
