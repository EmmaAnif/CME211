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
    
checkVal = True
#check that maze was entered on the first row and left at last row in solution
if (int(solution_list[0].split()[0])) != 0 and \
    (int(solution_list[-1].split()[1])) != n_row - 1:
    checkVal = False
    
#store the rows and cols encountered in solution
row_list = []
col_list = []
    
#compare elements in maze file and solution file
try:
    for i in range(len(solution_list)):
        row = int(solution_list[i].split()[0])
        row_list.append(row)
        col = int(solution_list[i].split()[1])
        col_list.append(col)
        if arr[row,col] == 1: #means solution passes through a wall
            checkVal = False
            break
except IndexError: #check for indexError in numpy array
    checkVal = False
    
#check that one position step was taken at a time
try:
    for i in range(len(solution_list)-1):
        if abs((row_list[i+1] - row_list[i])) > 1 and \
            abs((col_list[i+1] - col_list[i])) > 1:
            checkVal = False
            break
except IndexError:
    checkVal = False
        
#check that bounds of the maze was not exceeded in solution file
max_row,max_col = max(row_list),max(col_list)
if max_row >= n_row or max_col >= n_col:
    checkVal = False

if checkVal == True: #signifies no conflict was found
    print("Solution is valid!")
else:
    print("Solution is not valid!")
