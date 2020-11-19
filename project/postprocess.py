import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import sys

if __name__ == "__main__":

    #check for correct usage
    if len(sys.argv) < 3:
        print('Usage:')
        print('  python3 {} [input file] [solution file]'.format(sys.argv[0]))
        sys.exit(0)
    
    #save input and solution files
    inputfile = sys.argv[1]
    solutionfile = sys.argv[2]
    
    #read the input file and get relevant data from it
    try:
        with open(inputfile, 'r') as g:
            line = g.readline()
            geometry = line.split()
            length, width, h = float(geometry[0]), float(geometry[1]), float(geometry[2])
        nrows = int(width/h) + 1
        ncols = int(length/h) + 1
    except IOError:
        raise RuntimeError(inputfile + " does not exist")
    
    #x,y coordinates of discrete points in the pipe wall
    x,y = np.arange(0,length+h,h), np.arange(0,width+h,h)
    X,Y = np.meshgrid(x,y)
    T_arr = np.zeros((nrows,ncols))

    #check to ensure that the solution file is valid
    try:
        check_file = open(solutionfile,'r')
    except IOError:
        raise RuntimeError(solutionfile + " does not exist")
    check_nrows = len(check_file.readline().split())
    check_ncols = len(check_file.readlines()) + 2 #+2 is because one line was already read
    if check_nrows != nrows or check_ncols != ncols:
        raise RuntimeError("Incompatible input and solution files")
    check_file.close() #close if file is open
    
    #store the discrete Temperature values in the pipe wall
    with open(solutionfile, 'r') as f:
        col = 0
        for line in f.readlines():
            T_arr[:,col] = [float(i) for i in line.split()]
            col += 1
    T_arr[:,col] = T_arr[:,0] #periodic boundary condition
    T_arr = np.flipud(T_arr)

    #store the mean temperature in the pipe wall
    T_mean = np.mean(np.mean(T_arr,axis = 0))

    #plot isoline and pseudocolor plot
    isoline = np.zeros(ncols)
    for i in np.arange(ncols):
        isoline[i] = np.interp(T_mean,T_arr[:,i],y)
    plt.plot(x,isoline,linewidth=2)
    plt.pcolormesh(X,Y,T_arr,shading="gouraud")
    
    #format plot
    plt.colorbar()
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.xlim((0,length))
    plt.ylim((-width,2*width))
    plt.title("Input file processed: " + inputfile)

    #save plot
    figname = "pcolor" + inputfile[5] + ".png"
    plt.savefig(figname)
    
    #commandline feedback
    print("Input file processed: " + inputfile)
    print("Mean Temperature: {}".format(round(T_mean,2)))
    print("Output plot created: " + figname)
