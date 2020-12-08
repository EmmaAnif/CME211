import matplotlib
matplotlib.use("Agg")
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import sys

if __name__ == "__main__":

    #check for correct usage
    if len(sys.argv) < 3:
        print('Usage:')
        print('  python3 {} [input file] [solution file prefix]'.format(sys.argv[0]))
        sys.exit(0)

    #save the inputfile and solution file prefix
    inputfile = sys.argv[1]
    solfile_prefix = sys.argv[2]

    #read the input file and get relevant data from it
    try:
        with open(inputfile, 'r') as g:
            line = g.readline()
            geometry = line.split()
            length, width, h = float(geometry[0]), float(geometry[1]), float(geometry[2])
            line = g.readline()
            Tc, Th = float(line.split()[0]), float(line.split()[1])
        nrows = int(width/h) + 1
        ncols = int(length/h) + 1
    except IOError:
        raise RuntimeError(inputfile + " does not exist")

    #x,y coordinates of discrete points in the pipe wall
    x,y = np.arange(0,length+h,h), np.arange(0,width+h,h)
    X,Y = np.meshgrid(x,y)

    sol_num = 0 #used to go through all valid solution files
    T_frames = [] #stores frames to be displayed in animation

    #check to ensure that the solution file is valid
    solution0_file = solfile_prefix + "0.txt"
    try:
        check_file = open(solution0_file,'r')
    except IOError:
        raise RuntimeError(solfile_prefix + " is not a valid solution prefix")
    check_nrows = len(check_file.readline().split())
    check_ncols = len(check_file.readlines()) + 2 #+2 because one line was already read

    if check_nrows != nrows or check_ncols != ncols:
        raise RuntimeError("Incompatible input and solution files")
    check_file.close() #close the file if open

    #read color plot of each solution file into different frames
    fig = plt.figure()
    while True:
        T_arr = np.zeros((nrows,ncols)) #stores discrete Temperature values in pipe wall
        solutionfile = solfile_prefix + str(sol_num) + ".txt"
        try:
            with open(solutionfile, 'r') as f:
                col = 0
                for line in f.readlines():
                    T_arr[:,col] = [float(i) for i in line.split()]
                    col += 1
            T_arr[:,col] = T_arr[:,0]
            T_arr = (np.flipud(T_arr)- Tc)/(Th-Tc)
            T_frame = plt.pcolormesh(X,Y,T_arr,shading="gouraud")
            T_frames.append([T_frame])
            sol_num += 1
        except IOError:
            break

    #format the plot
    plt.colorbar()
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.xlim((0,length))
    plt.ylim((-width,2*width))
    plt.title("Input file processed: " + inputfile)

    #create and save animation
    ani = animation.ArtistAnimation(fig,T_frames)
    ani_name = "pcolor" + inputfile[5] + ".gif"
    ani.save(ani_name, writer = "imagemagick")

    #commandline feedback
    print("Input file processed: " + inputfile)
    print("Output animation created: " + ani_name)

#--functionality_-1
#--bonus point rewarded
#--END
