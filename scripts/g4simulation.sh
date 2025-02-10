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
