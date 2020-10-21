import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import scipy as scp
from scipy.sparse import csr_matrix
import warnings

#convert joints.dat to joints_arr: row - joints, column - x,y,Fx,Fy,zd
with open("truss1/joints.dat") as f:
    joints_list = f.readlines()
nJoints = len(joints_list) - 1
joints_arr = np.zeros((nJoints,5))
for i in range(nJoints):
    joints_arr[i,:] = np.array(joints_list[i+1].split()[1:],dtype=float)
print("joints array \n{}".format(joints_arr))
    

#convert beams.dat to beams_arr: row - joints, columns - beams, values -  1/0
with open("truss1/beams.dat") as f:
    beams_list = f.readlines()
nBeams = len(beams_list) - 1
beams_arr = np.zeros((nJoints,nBeams))
for i in range(nBeams):
    jIndex = np.array(beams_list[i+1].split()[1:],dtype=int)
    beams_arr[jIndex-1,i] = 1
print("beams array \n{}\n".format(beams_arr))
    
zd = joints_arr[:,4]
nSups = np.shape(zd[zd==1])[0]
n = 2*nJoints #matrix is size n x n

#plot the geometry if given the filename
filename = "myfile.png"
max_x,max_y = np.max(joints_arr[:,0]),np.max(joints_arr[:,1])
min_x,min_y = np.min(joints_arr[:,0]),np.min(joints_arr[:,1])
for i in np.arange(nBeams):
    joints = np.where(beams_arr[:,i] == 1)[0]
    x_coord = joints_arr[joints,0]
    y_coord = joints_arr[joints,1]
    plt.plot(x_coord,y_coord,'b-')
plt.title("2D Truss Geometry")
plt.xlim((min_x-0.5,max_x+0.5))
plt.ylim((min_y-0.5,max_y+0.5))
plt.savefig(filename)

#analyze beam and calculate reaction forces
if (2*nSups)+nBeams != (2*nJoints):
    raise RuntimeError("Truss geometry not suitable for static equilibrium analysis")
    
#create angles_arr: row-joints, column-beams, values- angle of beam wrt +x axis
angles_arr = np.zeros((nJoints,nBeams))
for i in np.arange(nBeams):
    joints = np.where(beams_arr[:,i] == 1)[0]
    x_coord = joints_arr[joints,0]
    y_coord = joints_arr[joints,1]
    del_y,del_x = y_coord[1]-y_coord[0], x_coord[1]-x_coord[0]
    alpha = np.arctan2(del_y,del_x)
    beta = np.pi + alpha
    angles_arr[joints,i] = [alpha,beta]

indR = nBeams #index of reaction force

#horizontal force balance at each joint
#for eaach joint, get the values of elements of the arr that are non-zero
eqn_h = np.zeros((nJoints,n))
rhs_h = np.zeros((nJoints,1))
for i in np.arange(nJoints):
    beams = np.where(beams_arr[i,:] == 1)[0]
    angle = angles_arr[i,beams]
    eqn_h[i,beams] = np.cos(angle)
    if joints_arr[i,4] == 1:
        eqn_h[i,indR] = 1
        indR += 1
    rhs_h[i] = -joints_arr[i,2]
print(np.round(eqn_h,2))
print("\n")

#vertical force balance at each joint
#for eaach joint, get the values of elements of the arr that are non-zero
eqn_v = np.zeros((nJoints,n))
rhs_v = np.zeros((nJoints,1))
for i in np.arange(nJoints):
    beams = np.where(beams_arr[i,:] == 1)[0]
    angle = angles_arr[i,beams]
    eqn_v[i,beams] = np.sin(angle)
    if joints_arr[i,4] == 1:
        eqn_v[i,indR] = 1
        indR += 1
    rhs_v[i] = -joints_arr[i,2]
print(np.round(eqn_v,2))









