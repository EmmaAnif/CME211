import sys
import time

if __name__ == "__main__":
    #incorrect number of arguments, print usage message
    if len(sys.argv) <= 3:
        print("Usage")
        print("$ python3 processdata.py <ref_file> <reads_file> <align_file>")
        sys.exit(0)

    #load reference file
    with open(str(sys.argv[1]),'r') as ref_f:
        ref = ref_f.read()
    ref = ref[:-1] # -1 to remove the new line character added
    ref_length = len(ref)
        
    #load read file
    read_list = []
    with open(str(sys.argv[2]),'r') as read_f:
        for read_line in read_f:
            read_list.append(read_line[:-1]) # -1 to remove the new line character added
    nreads = len(read_list)
    
    #perform alignment
    
    initial_time = time.time()
    
    align_f = open(str(sys.argv[3]),'w')
    
    align0 = 0
    align1 = 0
    align2 = 0
    
    for each_read in read_list:
        count = 0
        flag = True
        ind_list = []
        read_len = len(each_read)
        
        while flag:
            if count == 0: #first pass through the loop
                ind = ref.find(each_read)
            else: #search part of reference that has not been searched
                ind = ref.find(each_read,last_ind+read_len) 
            if ind == -1: #read not found in reference
                flag = False
            else:
                last_ind = ind #used to know where not to search on next loop pass
                count += 1
            ind_list.append(ind) #keep track of the 0 based indices of aligned reads
            if count == 2:
                break
        
        if count == 0:
            align0 += 1
            align_f.write('{} -1\n'.format(each_read))
        elif count == 1:
            align1 += 1
            align_f.write('{} {}\n'.format(each_read,ind_list[0]))
        else:
            align2 += 1
            align_f.write('{} {} {}\n'.format(each_read,ind_list[0],ind_list[1]))
            
    align_f.close()
    
    final_time = time.time()
    
    #alignment file created at this point
    
    time_elapsed = final_time - initial_time #time it took to carry out alignment
    
    #calculate the percentages for each align case
    aligns0_percent = float(align0/nreads)
    aligns1_percent = float(align1/nreads)
    aligns2_percent = float(align2/nreads)
    
    #display on the terminal
    print("reference length: {}".format(ref_length))
    print("number reads: {}".format(nreads))
    print("aligns0: {}".format(aligns0_percent))
    print("aligns1: {}".format(aligns1_percent))
    print("aligns2: {}".format(aligns2_percent))
    print("elapsed time: {:f}".format(time_elapsed))
