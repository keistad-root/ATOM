#!/bin/bash

# Create a new tmux session named "g4simulation"
tmux new-session -d -s g4

# Create 10 additional windows (total 11 windows)
for i in {1..11}; do
    tmux new-window -t g4
done

# Send commands to each window
tmux send-keys -t g4:0 'htop' C-m
tmux send-keys -t g4:1 './G4Simulation L1F2AX_W082; ./G4Simulation L1F2AO_W082; ./G4Simulation L1F3AX_W420; ./G4Simulation L1F3AO_W420; ./G4Simulation L1F4AX_W0990; ./G4Simulation L1F4AO_W0990; ./G4Simulation L1F7AX_W3460; ./G4Simulation L1F7AO_W3460; ./G4Simulation L1F2AX_L095; ./G4Simulation L1F2AO_L095; ./G4Simulation L1F3AX_L095; ./G4Simulation L1F3AO_L095; ./G4Simulation L1F4AX_L095; ./G4Simulation L1F4AO_L095; ./G4Simulation L1F7AX_L095; ./G4Simulation L1F7AO_L095' C-m
tmux send-keys -t g4:2 './G4Simulation L1F2AX_W085; ./G4Simulation L1F2AO_W085; ./G4Simulation L1F3AX_W428; ./G4Simulation L1F3AO_W428; ./G4Simulation L1F4AX_W1002; ./G4Simulation L1F4AO_W1002; ./G4Simulation L1F7AX_W3488; ./G4Simulation L1F7AO_W3488; ./G4Simulation L1F2AX_L096; ./G4Simulation L1F2AO_L096 ./G4Simulation L1F3AX_L096; ./G4Simulation L1F3AO_L096 ./G4Simulation L1F4AX_L096; ./G4Simulation L1F4AO_L096 ./G4Simulation L1F7AX_L096; ./G4Simulation L1F7AO_L096' C-m
tmux send-keys -t g4:3 './G4Simulation L1F2AX_W088; ./G4Simulation L1F2AO_W088; ./G4Simulation L1F3AX_W436; ./G4Simulation L1F3AO_W436; ./G4Simulation L1F4AX_W1014; ./G4Simulation L1F4AO_W1014; ./G4Simulation L1F7AX_W3516; ./G4Simulation L1F7AO_W3516; ./G4Simulation L1F2AX_L097; ./G4Simulation L1F2AO_L097; ./G4Simulation L1F3AX_L097; ./G4Simulation L1F3AO_L097; ./G4Simulation L1F4AX_L097; ./G4Simulation L1F4AO_L097; ./G4Simulation L1F7AX_L097; ./G4Simulation L1F7AO_L097' C-m
tmux send-keys -t g4:4 './G4Simulation L1F2AX_W091; ./G4Simulation L1F2AO_W091; ./G4Simulation L1F3AX_W444; ./G4Simulation L1F3AO_W444; ./G4Simulation L1F4AX_W1026; ./G4Simulation L1F4AO_W1026; ./G4Simulation L1F7AX_W3544; ./G4Simulation L1F7AO_W3544; ./G4Simulation L1F2AX_L098; ./G4Simulation L1F2AO_L098; ./G4Simulation L1F3AX_L098; ./G4Simulation L1F3AO_L098; ./G4Simulation L1F4AX_L098; ./G4Simulation L1F4AO_L098; ./G4Simulation L1F7AX_L098; ./G4Simulation L1F7AO_L098' C-m
tmux send-keys -t g4:5 './G4Simulation L1F2AX_W094; ./G4Simulation L1F2AO_W094; ./G4Simulation L1F3AX_W452; ./G4Simulation L1F3AO_W452; ./G4Simulation L1F4AX_W1038; ./G4Simulation L1F4AO_W1038; ./G4Simulation L1F7AX_W3572; ./G4Simulation L1F7AO_W3572; ./G4Simulation L1F2AX_L099; ./G4Simulation L1F2AO_L099; ./G4Simulation L1F3AX_L099; ./G4Simulation L1F3AO_L099; ./G4Simulation L1F4AX_L099; ./G4Simulation L1F4AO_L099; ./G4Simulation L1F7AX_L099; ./G4Simulation L1F7AO_L099' C-m
tmux send-keys -t g4:6 './G4Simulation L1F2AX_W097; ./G4Simulation L1F2AO_W097; ./G4Simulation L1F3AX_W460; ./G4Simulation L1F3AO_W460; ./G4Simulation L1F4AX_W1050; ./G4Simulation L1F4AO_W1050; ./G4Simulation L1F7AX_W3600; ./G4Simulation L1F7AO_W3600; ./G4Simulation L1F2AX_L100; ./G4Simulation L1F2AO_L100; ./G4Simulation L1F3AX_L100; ./G4Simulation L1F3AO_L100; ./G4Simulation L1F4AX_L100; ./G4Simulation L1F4AO_L100; ./G4Simulation L1F7AX_L100; ./G4Simulation L1F7AO_L100' C-m
tmux send-keys -t g4:7 './G4Simulation L1F2AX_W100; ./G4Simulation L1F2AO_W100; ./G4Simulation L1F3AX_W468; ./G4Simulation L1F3AO_W468; ./G4Simulation L1F4AX_W1062; ./G4Simulation L1F4AO_W1062; ./G4Simulation L1F7AX_W3628; ./G4Simulation L1F7AO_W3628; ./G4Simulation L1F2AX_L101; ./G4Simulation L1F2AO_L101; ./G4Simulation L1F3AX_L101; ./G4Simulation L1F3AO_L101; ./G4Simulation L1F4AX_L101; ./G4Simulation L1F4AO_L101; ./G4Simulation L1F7AX_L101; ./G4Simulation L1F7AO_L101' C-m
tmux send-keys -t g4:8 './G4Simulation L1F2AX_W103; ./G4Simulation L1F2AO_W103; ./G4Simulation L1F3AX_W476; ./G4Simulation L1F3AO_W476; ./G4Simulation L1F4AX_W1074; ./G4Simulation L1F4AO_W1074; ./G4Simulation L1F7AX_W3656; ./G4Simulation L1F7AO_W3656; ./G4Simulation L1F2AX_L102; ./G4Simulation L1F2AO_L102; ./G4Simulation L1F3AX_L102; ./G4Simulation L1F3AO_L102; ./G4Simulation L1F4AX_L102; ./G4Simulation L1F4AO_L102; ./G4Simulation L1F7AX_L102; ./G4Simulation L1F7AO_L102' C-m
tmux send-keys -t g4:9 './G4Simulation L1F2AX_W106; ./G4Simulation L1F2AO_W106; ./G4Simulation L1F3AX_W484; ./G4Simulation L1F3AO_W484; ./G4Simulation L1F4AX_W1086; ./G4Simulation L1F4AO_W1086; ./G4Simulation L1F7AX_W3684; ./G4Simulation L1F7AO_W3684; ./G4Simulation L1F2AX_L103; ./G4Simulation L1F2AO_L103; ./G4Simulation L1F3AX_L103; ./G4Simulation L1F3AO_L103; ./G4Simulation L1F4AX_L103; ./G4Simulation L1F4AO_L103; ./G4Simulation L1F7AX_L103; ./G4Simulation L1F7AO_L103' C-m
tmux send-keys -t g4:10 './G4Simulation L1F2AX_W109; ./G4Simulation L1F2AO_W109; ./G4Simulation L1F3AX_W492; ./G4Simulation L1F3AO_W492; ./G4Simulation L1F4AX_W1098; ./G4Simulation L1F4AO_W1098; ./G4Simulation L1F7AX_W3712; ./G4Simulation L1F7AO_W3712; ./G4Simulation L1F2AX_L104; ./G4Simulation L1F2AO_L104; ./G4Simulation L1F3AX_L104; ./G4Simulation L1F3AO_L104; ./G4Simulation L1F4AX_L104; ./G4Simulation L1F4AO_L104; ./G4Simulation L1F7AX_L104; ./G4Simulation L1F7AO_L104' C-m
tmux send-keys -t g4:11 './G4Simulation L1F2AX_W112; ./G4Simulation L1F2AO_W112; ./G4Simulation L1F3AX_W500; ./G4Simulation L1F3AO_W500; ./G4Simulation L1F4AX_W1110; ./G4Simulation L1F4AO_W1110; ./G4Simulation L1F7AX_W3740; ./G4Simulation L1F7AO_W3740; ./G4Simulation L1F2AX_L105; ./G4Simulation L1F2AO_L105; ./G4Simulation L1F3AX_L105; ./G4Simulation L1F3AO_L105; ./G4Simulation L1F4AX_L105; ./G4Simulation L1F4AO_L105; ./G4Simulation L1F7AX_L105; ./G4Simulation L1F7AO_L105' C-m

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
