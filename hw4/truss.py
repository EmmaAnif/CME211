import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from scipy.sparse import csr_matrix
from scipy.sparse.linalg import spsolve
import warnings

class Truss:
    """Truss class is used for loading and analyzing a 2D Truss 
    using the method of joints
    
    It uses 3 methods: __init__,PlotGeometry,SolveTruss,__repr__
    """

    def __init__(self,filename0,filename1,filename2):
        """Initialize an instance of the Truss class given 3 filenames
        
        filename0 - path of the joints.dat file
        filename1 - path of the beams.dat file
        filename2 - path of an output file for a matplotlib plot
        """
   
        #convert joints.dat to joints_arr: row-joints, column- x,y,Fx,Fy,zd
        try:
            with open(filename0) as f:
                joints_list = f.readlines()
        except:
            raise RuntimeError("{} cannot be found".format(filename0))
        self.nJoints = len(joints_list) - 1
        self.joints_arr = np.zeros((self.nJoints,5))
        for i in range(self.nJoints):
            self.joints_arr[i,:] = np.array(joints_list[i+1].split()[1:],dtype=float)

        #convert beams.dat to beams_arr: row-joints, columns-beams, values- 1/0
        try:
            with open(filename1) as f:
                beams_list = f.readlines()
        except:
            raise RuntimeError("{} cannot be found".format(filename1))
        self.nBeams = len(beams_list) - 1
        self.beams_arr = np.zeros((self.nJoints,self.nBeams))
        for i in range(self.nBeams):
            jIndex = np.array(beams_list[i+1].split()[1:],dtype=int)
            self.beams_arr[jIndex-1,i] = 1
        
        self.out_file = filename2
    
        zd = self.joints_arr[:,4] #zd is the zero displacement column
        self.nSups = np.shape(zd[zd==1])[0]
        self.n = 2*self.nJoints #matrix is size n x n
        warnings.filterwarnings("error") #elevate warnings to errors status

    def PlotGeometry(self):
        """Use matplotlib to create a simple plot showing geometry of truss
        """
        max_x,max_y = np.max(self.joints_arr[:,0]),np.max(self.joints_arr[:,1])
        min_x,min_y = np.min(self.joints_arr[:,0]),np.min(self.joints_arr[:,1])
        for i in np.arange(self.nBeams):
            joints = np.where(self.beams_arr[:,i] == 1)[0]
            x_coord = self.joints_arr[joints,0]
            y_coord = self.joints_arr[joints,1]
            plt.plot(x_coord,y_coord,'b-') #plot each beam one at a time
        plt.title("2D Truss Geometry")
        plt.xlim((min_x-0.5,max_x+0.5))
        plt.ylim((min_y-0.5,max_y+0.5))
        plt.savefig(self.out_file)

    def SolveTruss(self):
        """Analyze truss and compute beam forces
        """
        if (2*self.nSups)+self.nBeams != (2*self.nJoints):
            raise RuntimeError("Truss geometry not suitable for static equilibrium \
            analysis")
    
        #create angles_arr: row-joints, column-beams, values-angle of beam wrt +x axis
        self.angles_arr = np.zeros((self.nJoints,self.nBeams))
        for i in np.arange(self.nBeams):
            #find the two joints connected to each beam
            joints = np.where(self.beams_arr[:,i] == 1)[0]
            x_coord = self.joints_arr[joints,0]
            y_coord = self.joints_arr[joints,1]
            del_y,del_x = y_coord[1]-y_coord[0], x_coord[1]-x_coord[0]
            alpha = np.arctan2(del_y,del_x) #angle at first joint
            beta = np.pi + alpha #angle at second joint
            self.angles_arr[joints,i] = [alpha,beta]

        indR = self.nBeams #index of reaction force

        #horizontal force balance at each joint
        #for eaach joint, get the values of elements of the arr that are non-zero
        rhs_h = np.zeros((self.nJoints,1)) #right hand side of equation
        row,col,data = [],[],[] #store values that help to make csr matrix
        for i in np.arange(self.nJoints):
            beams = np.where(self.beams_arr[i,:] == 1)[0]
            beam_l = np.shape(beams)[0]
            row.extend([i]*beam_l)
            col.extend(beams)
            angle = self.angles_arr[i,beams]
            data.extend(np.cos(angle))
            if self.joints_arr[i,4] == 1: #for reaction forces at support
                row.append(i)
                col.append(indR)
                data.append(1)
                indR += 1
            rhs_h[i] = self.joints_arr[i,2] #for external forces

        #vertical force balance at each joint
        #for eaach joint, get the values of elements of the arr that are non-zero
        rhs_v = np.zeros((self.nJoints,1))
        for i in np.arange(self.nJoints):
            beams = np.where(self.beams_arr[i,:] == 1)[0]
            beam_l = np.shape(beams)[0]
            row.extend([self.nJoints+i]*beam_l)
            col.extend(beams)
            angle = self.angles_arr[i,beams]
            data.extend(np.sin(angle))
            if self.joints_arr[i,4]:
                row.append(self.nJoints+i)
                col.append(indR)
                data.append(1)
                indR += 1
            rhs_v[i] = self.joints_arr[i,3]
        rhs_arr = np.concatenate((rhs_h,rhs_v),axis = 0)
        #create sparse matrix
        sparseM = csr_matrix((data,(row,col)),shape = (self.n,self.n))
    
        try:
            self.solve_F = spsolve(sparseM,rhs_arr)
        except:
            raise RuntimeError("Cannot solve the linear system, unstable truss?")
    
    def __repr__(self):
        """Returns a string representation of an instance of the class
        """
        self.SolveTruss()
        beam_forces = self.solve_F[:self.nBeams] 
        ret_val = "{:>5}{:>11}\n{}\n".format("Beam","Force","-"*16)
        for i in range(self.nBeams):
            ret_val += "{:>5d}{:>11.3f}\n".format(i+1,beam_forces[i])
        return ret_val
