import glob
import math
import os

class Airfoil:
    """create an Aifoil class that will handle the loading and basic processing
    of data associated with an airfoil at multiple angles of attack
    
    Methods associated with it are __init__, load_xy,load_alpha,calc_cl,stag_pt
    and __str__
    """
    def __init__(self,inputdir):
        """initilalize an instance of the Airfoil class
        """
        if not os.path.exists(inputdir): #check if input direcotory exists
            raise RuntimeError("input directory does not exist")
        #check if <xy.dat> and <alpha.dat> files exist
#--functionality_1
#--This doesn't work if I provide the directory without a "/"
#-- ex: python3 main.py naca0012
#--START
        if glob.glob(inputdir + "xy*") == [] or \
         glob.glob(inputdir + "alpha*") == []: 
#--END
            raise RuntimeError("required data files cannot be found in the \
            data directory")
        #store the files for each angle of attack
        ang_files = glob.glob(inputdir + "alpha*")
        try:
            self.ang_dict = {float(ang_file[len(inputdir)+5:-4]):ang_file \
            for ang_file in ang_files} #dict that maps angles to corr. files
        except ValueError:
            print("angle of attack needs to be a number")
        self.inputdir = inputdir #store the input directory
        self.ang_att = sorted(self.ang_dict.keys()) #sort the angles 
        self.panels = [] #initialize a list of panels
        self.chord = 0 #initialize chord length
        self.x_data = [] #initialize a list of x data
        self.y_data = [] #initialize a list of y data
        self.cp_dict = {} #intialize dict that maps angles to list of Cp values
        self.cl_dict = {} #intiailize dict that maps angles to cl values
        self.stag_dict = {} #initialize dict that maps angles to stag. points
        
    def load_xy(self):
        """load the <xy.dat> file, store the (x,y) data points,the chord length
        and the (delta_x,delta_y) for each panel
        """
        inputdir = self.inputdir
        #catch error with file opening
        try:
            with open(inputdir + "xy.dat") as f:
                xy_list = f.readlines()
#--functionality_1
#--You should probably raise an exception here, otherwise the code is going to keep going and fail
#--START
        except IOError:
            print("cannot open the <xy.dat> file")
#--END

#--design_1
#--Really ugly and potentially really slow
#--When you delete and element off the list, all the other elements need to be moved in memory
#--It is much better to just start the for loop at 1
#--START
        del(xy_list[0]) #delete the header
#--END
        len_xy = len(xy_list)
        #delete the newline character and store as (x,y) pair
        xy_list = [tuple(xy_list[j][:-1].split()) for j in range(len_xy)]
#--style_1
#--Don't use semi-colons in python
#--Better to do x_data, y_data = [],[]
#--START
        x_data = []; y_data = []
#--END
        for x,y in xy_list:
            x_data.append(float(x))
#--style_0
#--Weird trailing space at the end of the next line
#--START
            y_data.append(float(y)) 
#--END
        chord = max(x_data) - min(x_data)
#--design_0
#--This line split hinders readability a lot
#--START
        xy_panels = [(x_data[j+1]-x_data[j],y_data[j+1]-y_data[j]) \
        for j in range(len_xy) if j < len_xy-1] #definition of panels: (dx,dy)
#--END
        self.panels = xy_panels
        self.chord = chord
        self.x_data = x_data
        self.y_data = y_data
        
    def load_alpha(self):
        """load each <alpha(angle).dat> file and store Cp values for each
        """
        for ang,ang_file in self.ang_dict.items():
            try:
                with open(ang_file) as f:
                    cp_list = f.readlines()
            except IOError:
                print("cannot open the <alpha(angle).dat> file")
            del(cp_list[0]) #delete the header
            #delete the newline character
            cp_list = [float(cp[:-1]) for cp in cp_list]
            
            self.cp_dict[ang] = cp_list
    
    def calc_cl(self):
        """calculate the lift coefficient for each angle of attack
        """
        chord = self.chord
        for ang,cp_list in self.cp_dict.items():
            cp_len = len(cp_list)
            cx = 0; cy = 0; alpha = math.radians(ang)
            for j in range(cp_len):
                cp = cp_list[j]
                del_x = self.panels[j][0]; del_y = self.panels[j][1]
                #use the formulas for delta_cx and delta_cy
                del_cx = (-cp*del_y)/chord
                del_cy = (cp*del_x)/chord
                cx += del_cx; cy += del_cy
            cl = (cy*math.cos(alpha)) - (cx*math.sin(alpha)) #lift coefficient
            
            self.cl_dict[ang] = cl
            
    def stag_pt(self):
        """identify the stagnation point for each angle of attack
        """
        for ang,cp_list in self.cp_dict.items():
#--design_0
#--Slightly inefficient before you loop twice over the list by doing this
#--START
            stag_cp = max(cp_list) #stagnation point corresponds to max Cp
            panel_num = cp_list.index(stag_cp)
#--END
            x_dat = self.x_data; y_dat = self.y_data
            #stagnation point is defined as middle of "stagnation panel"
            stag_x = (x_dat[panel_num] + x_dat[panel_num+1])/2
            stag_y = (y_dat[panel_num] + y_dat[panel_num+1])/2
            
            self.stag_dict[ang] = [(stag_x,stag_y), stag_cp]
    
    def __str__(self):
        """string representation of an instance of the class
        """
        inputdir = self.inputdir
        
        #header
#--functionality_0
#--Here you assume that inputdir has a specific form, but what if inputdir is a long path
#--START
        print("Test case: {} {}\n".format(inputdir[:4].upper(),inputdir[4:-1]))
#--END
        print("{:^7}".format("alpha"),end = ' ')
        print("{:^9}".format("cl"),end = ' ')
        print("{:^30}".format("stagnation point"))
    
        print("{:^7}".format("-"*5),end = ' ')
        print("{:^9}".format("-"*7),end = ' ')
        print("{:^30}".format("-"*26))
        
        #method calls
        self.load_xy()
        self.load_alpha()
        self.calc_cl()
        self.stag_pt()
        
        #actual representation for each angle of attack with lots of formatting
        for ang in self.ang_att:
            cl = self.cl_dict[ang]
            stag_pt_x = self.stag_dict[ang][0][0]
            stag_pt_y = self.stag_dict[ang][0][1]
            stag_cp = self.stag_dict[ang][1]
            angle = float(ang)
#--design_1
#--This is really strange: you shouldn't print in this method
#-- but instead you should return a string containing everything that you want to print
#--START
            print("{:6.2f}".format(angle),end = ' ')
            print("{:9.4f}".format(cl),end = ' ')
            print("    ({:5.4f},".format(stag_pt_x),end = ' ')
            print("{:7.4f})".format(stag_pt_y),end = ' ')
            print("{:6.4f}".format(stag_cp))
#--END
        return(' ') #ensures no error is generated
