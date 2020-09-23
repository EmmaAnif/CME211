import sys
import random

if __name__ == "__main__":
    #incorrect number of arguments, print usage message
    if len(sys.argv) <= 5:
        print("Usage:")
        print(" $ python3 generatedata.py <ref_length> <nreads> <read_len> <ref_file> <reads_file>")
        sys.exit(0)
    
    #create reference data
    ref_length = int(sys.argv[1])
    three_quat_length = int(0.75*ref_length)
    rem_ref_length = ref_length - three_quat_length
    
    ref = '' 
    for _ in range(three_quat_length): #creates 75% of reference data
        guess = random.randint(0,3)
        if guess == 0:
            lett = 'A'
        elif guess == 1:
            lett = 'C'
        elif guess == 2:
            lett = 'G'
        else:
            lett = 'T'
        ref += lett
        
    repeat = ref[-rem_ref_length:] #this copies the last 25% of the first 75%
    ref = ref + repeat
    
    #write to reference file
    with open(str(sys.argv[4]),'w') as f:
        f.write(ref + "\n")

    #create read data
    nreads = int(sys.argv[2])
    read_len = int(sys.argv[3])
    
    aligns0 = 0
    aligns1 = 0
    aligns2 = 0
    read_list = []
    
    for _ in range(nreads):
        guess = random.random()
        
        if guess < 0.75: #read aligns only once
            half_ref_len = int(0.5*ref_length)
            posit_in_ref = random.randint(0,half_ref_len)
            read = ref[posit_in_ref: posit_in_ref+read_len]
            aligns1 += 1
        elif guess < 0.9: #read does not align at all
            flag = True
            read = ''
            while flag:
                for _ in range(read_len):
                    new_guess = random.randint(0,3)
                    if new_guess == 0:
                        lett = 'A'
                    elif new_guess == 1:
                        lett = 'C'
                    elif new_guess == 2:
                        lett = 'G'
                    else:
                        lett = 'T'
                    read += lett
                if ref.find(read) == -1:
                    flag = False
                    aligns0 += 1
                    
        else: #read aligns twice
            safe_region = len(repeat) - read_len #to prevent read from going beyond the reference
            val = random.randint(1,safe_region)
            start_index = three_quat_length - 1  + val # -1 is used to prevent slicing from going out of bounds in worst case
            read = ref[start_index: start_index+read_len]
            aligns2 += 1
        read_list.append(read)
        
    #write to read file
    with open(str(sys.argv[5]),'w') as f:
        for each_read in read_list:
            f.write('{}\n'.format(each_read))
    
    #calculate the percentages for each align case    
    aligns0_percent = float(aligns0/nreads)
    aligns1_percent = float(aligns1/nreads)
    aligns2_percent = float(aligns2/nreads)

    #display on the terminal
    print("reference length:  {}".format(ref_length))
    print("number reads: {}".format(nreads))
    print("read length: {}".format(read_len))
    print("aligns 0: {}".format(aligns0_percent))
    print("aligns 1: {}".format(aligns1_percent))
    print("aligns 2: {}".format(aligns2_percent))
