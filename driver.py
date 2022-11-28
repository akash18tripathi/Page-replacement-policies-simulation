import os
import matplotlib.pyplot as plt
import numpy as np

print("===========Welcome to Page Replacement Algorithm Simulator=============")
print("Enter the type of Operation you want to perform:\n 1. Single algorithm \n 2. Multi-Algo analysis\n3.Exit ")
typ = int(input())

if typ==1:
    frames = int(input("Enter number of frames"))
    if(frames<=0):
        print("Number of frames should be positive!")
        exit(1)
    print("Choose the Algo number you want to perform:\n 1.Random\n2.Optimal\n3.NRU\n4.FIFO\n5.FIFO with second chance\n6.Clock\n7.LRU\n8.NFU\n9.Working Set\n10.Aging\n11.WSClock\n")
    algo=int(input())
    os.system("g++ main.cpp -o main")
    print("[*] Compilation Successful, Executing main \n")
    os.system("./main "+ str(typ)+" "+str(frames)+" "+str(algo))
elif typ==2:
    print("Enter the range of frames you want to perform analysis on:\n")
    left = int(input("Min frame:"))
    right = int(input("Max frame:"))
    os.system("g++ main.cpp -o main")
    print("[*] Compilation Successful, Executing main \n")
    os.system("./main "+str(typ)+" "+str(left)+" "+str(right))
    print("[*] Excecuted Succesfully \n")
    workingSet = open("working_set.txt", "r")
    fifo = open("FIFO.txt", "r")
    fifoSecond = open("FIFOSecond.txt", "r")
    Aging = open("Aging.txt", "r")
    Clock = open("Clock.txt", "r")
    Nru = open("NRU.txt", "r")
    Nfu = open("NFU.txt", "r")
    WSClock = open("WSClock.txt", "r")
    optimal = open("optimal.txt", "r")
    randomPRA = open("random.txt", "r")
    LRU =open("LRU.txt", "r")

    fig, axs = plt.subplots(4, 3)
    line = workingSet.readline()
    y = [float(i) for i in line.split(" ")[:-1]]
    x = list(range(left,right+1))
    axs[0,0].plot(np.array(x),np.array(y))
    axs[0,0].set_title("Working set")
    line = optimal.readline()
    y = [float(i) for i in line.split(" ")[:-1]]
    x = list(range(left,right+1))
    axs[0,1].plot(np.array(x),np.array(y))
    axs[0,1].set_title("Optimal")
    line = Nfu.readline()
    y = [float(i) for i in line.split(" ")[:-1]]
    x = list(range(left,right+1))
    axs[0,2].plot(np.array(x),np.array(y))
    axs[0,2].set_title("NFU")
    line = fifoSecond.readline()
    y = [float(i) for i in line.split(" ")[:-1]]
    x = list(range(left,right+1))
    axs[1,0].plot(np.array(x),np.array(y))
    axs[1,0].set_title("FIFO Second chance")
    line = fifo.readline()
    y = [float(i) for i in line.split(" ")[:-1]]
    x = list(range(left,right+1))
    axs[1,1].plot(np.array(x),np.array(y))
    axs[1,1].set_title("FIFO")
    line = Aging.readline()
    y = [float(i) for i in line.split(" ")[:-1]]
    x = list(range(left,right+1))
    axs[1,2].plot(np.array(x),np.array(y))
    axs[1,2].set_title("Aging")
    line = Clock.readline()
    y = [float(i) for i in line.split(" ")[:-1]]
    x = list(range(left,right+1))
    axs[2,0].plot(np.array(x),np.array(y))
    axs[2,0].set_title("Clock")
    line = WSClock.readline()
    y = [float(i) for i in line.split(" ")[:-1]]
    x = list(range(left,right+1))
    axs[2,1].plot(np.array(x),np.array(y))
    axs[2,1].set_title("WSClock")
    line = randomPRA.readline()
    y = [float(i) for i in line.split(" ")[:-1]]
    x = list(range(left,right+1))
    axs[2,2].plot(np.array(x),np.array(y))
    axs[2,2].set_title("Random")
    line = LRU.readline()
    y = [float(i) for i in line.split(" ")[:-1]]
    x = list(range(left,right+1))
    axs[3,0].plot(np.array(x),np.array(y))
    axs[3,0].set_title("LRU")

    

    line = Nru.readline()
    y = [float(i) for i in line.split(" ")[:-1]]
    x = list(range(left,right+1))
    axs[3,1].plot(np.array(x),np.array(y))
    axs[3,1].set_title("NRU")

    #plt.plot(np.array(x),np.array(y))
    fig.tight_layout()
    plt.show()    