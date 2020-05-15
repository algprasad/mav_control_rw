#!/bin/bash
cd ../solver_made_from_cpp/build/
cmake ..
make
cd ../../solver/
./nmpc_solver_setup
pushd /home/alg/ethz_mpc_ws/
catkin build -j2
popd
