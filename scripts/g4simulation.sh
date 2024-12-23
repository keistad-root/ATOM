#!/bin/sh

./../build/G4Simulation ~/ATOM/config/g4simulation/$1_7phi_alpha_screen.conf &
./../build/G4Simulation ~/ATOM/config/g4simulation/$1_7phi_alpha_noscreen.conf &
./../build/G4Simulation ~/ATOM/config/g4simulation/$1_4phi_alpha_screen.conf &
./../build/G4Simulation ~/ATOM/config/g4simulation/$1_4phi_alpha_noscreen.conf &
./../build/G4Simulation ~/ATOM/config/g4simulation/$1_3phi_alpha_screen.conf &
./../build/G4Simulation ~/ATOM/config/g4simulation/$1_3phi_alpha_noscreen.conf &
./../build/G4Simulation ~/ATOM/config/g4simulation/$1_2phi_alpha_screen.conf &
./../build/G4Simulation ~/ATOM/config/g4simulation/$1_2phi_alpha_noscreen.conf &
wait