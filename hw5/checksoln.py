import numpy as np
import sys

#check for correct usage
if len(sys.argv) < 3:
    print('Usage:')
    print('  python3 {} <maze file> <solution file>'.format(sys.argv[0]))
    sys.exit(0)

#save names of maze and solution files
maze_file = sys.argv[1]
solution_file = sys.argv[2]

#catch file IO error in opening maze file
try:
    with open(maze_file) as f:
        maze_list = f.readlines()
except:
    raise RuntimeError("The file {} cannot be opened".format(maze_file))

#store relevant data from the maze file
n_row = int(maze_list[0].split()[0])
n_col = int(maze_list[0].split()[1])
n_values = len(maze_list) - 1

#create a maze array with wall as 1 and space as 0
arr = np.zeros((n_row,n_col))
for i in range(n_values):
    row = int(maze_list[i+1].split()[0])
    col = int(maze_list[i+1].split()[1])
    arr[row,col] = 1;
    
#catch file IO error in opening solution file
try:
    with open(solution_file) as f:
        solution_list = f.readlines()
except:
    raise RuntimeError("The file {} cannot be opened".format(solution_file))
    
#compare elements in maze file and solution file
checkVal = True
for i in range(len(solution_list)):
    row = int(solution_list[i].split()[0])
    col = int(solution_list[i].split()[1])
    if arr[row,col] == 1: #means solution passes through a wall
        checkVal = False
        break

if checkVal == True: #signifies no conflict was found
    print("Solution is valid!")
else:
    print("Solution is not valid!")
