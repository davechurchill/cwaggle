#!/bin/sh
clear
# reset numRobots to 1 and clear policy
sed -i 's/loadQFromFile.*/loadQFromFile  0/' rl_config.txt
sed -i 's/numRobots.*/numRobots      1/' rl_config.txt
echo "learning policy..."
./cwaggle_rl
# begin loading policy from file
sed -i 's/loadQFromFile.*/loadQFromFile  1/' rl_config.txt
sed -i 's/numRobots.*/numRobots      2/' rl_config.txt
./cwaggle_rl
sed -i 's/numRobots.*/numRobots      3/' rl_config.txt
./cwaggle_rl
sed -i 's/numRobots.*/numRobots      4/' rl_config.txt
./cwaggle_rl
sed -i 's/numRobots.*/numRobots      5/' rl_config.txt
./cwaggle_rl
sed -i 's/numRobots.*/numRobots      6/' rl_config.txt
./cwaggle_rl
sed -i 's/numRobots.*/numRobots      7/' rl_config.txt
./cwaggle_rl
sed -i 's/numRobots.*/numRobots      8/' rl_config.txt
./cwaggle_rl
sed -i 's/numRobots.*/numRobots      9/' rl_config.txt
./cwaggle_rl
sed -i 's/numRobots.*/numRobots      10/' rl_config.txt
./cwaggle_rl

exit 0
