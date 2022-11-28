# Page Replacement Algorithms Simulator

This is AOS final project which is terminal based application. 

# Libraries needed

python3 is needed to run driver.py and matplotlib ,numpy, os must be installed

## Instructions before executing driver.py

1) All files must be in same folder during execution.
2) The input file for NRU should be named as 'num_nru.txt' which should contain space seperated integers, and some integers can be with '$' as prefix like ex: '$3'  which means that this page is modified.
Ex: 7 2 $3 2 $1 0 $0
3) The input file for all other algorithms is 'num.txt'.
4) There are 2 options to choose from: 
				- Either run input on single algorithm, which returns the hit ratio.
				- OR Do Analysis and comparison for same input on all algorithms. Here we need to see the hit ratios of different algorithms over a range of different number of frames.  So give input as minimum and maximum number of frames.
5) Install required libraries using below command:
				pip install numpy
				pip install matplotlib

## Instructions to execute driver.py
run command : python3 driver.py

