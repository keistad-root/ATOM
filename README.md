### Purpose of this project.
The project title "ATOM" is abbreviation of APTS Task Organisation Machine. This project is based on [MLR1 DAQ Software](https://gitlab.cern.ch/alice-its3-wp3/apts-dpts-ce65-daq-software) made by CERN ALICE group. The existing code, however, was written by Python code. So the code was re-made by C++ and ROOT programing language for shorter runtime and efficient memory management.

## Merge Experimental ROOT File of ALPIDE
If for some reason the experiment was conducted in parts and you need to merge the files into one, you can use the Merge executable to combine them.
Before running the executable, you need to create a config file. 
Refer to the example config file at `config/merge.conf`.
Now, you can merge the files using the following command:
```
./Merge {Your Config}
```