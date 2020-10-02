import math
import sys
import time

def checkSimilar(rTup):
    """ Returns the similarity coefficient of the two rates represented 
    as a list of tuples

    rTup is a list of pairs of rate1 and rate2
    """

    num = 0; den1 = 0; den2 = 0
    rLen = len(rTup)
    #get average of the two lists
    unzip_rTup = list(zip(*rTup)) #unzip the list of tuples
    r1_m = sum(unzip_rTup[0])/rLen
    r2_m = sum(unzip_rTup[1])/rLen
    
    #use the cosine similarity equation to calculate similarity
    for tup in rTup:
        r1 = tup[0]
        r2 = tup[1]
        num += (r1-r1_m)*(r2-r2_m)
        den1 += (r1-r1_m)**2
        den2 += (r2-r2_m)**2
    den = math.sqrt(den1*den2)
    if den == 0: #deal with division by zero
        simVal = 0
    else:
        simVal = num/den
    return simVal

def compareMovies(movie_dict,thresh,outFile):
    """ Creates a file with movie_id as key, corresponding movie,
    corresponding similarity and corresponding users as values
    
    input - a dict with movie_id as key, user_id  and rating  as another dict
    - threshold value below which similarity is zero
     """
    dict_store = dict() #used to save values to avoid unnecessary computations
    sim_f = open(outFile,'w')

    compareList = sorted(movie_dict.keys())  #get sorted list of movies

    for i in range(len(compareList)):
        sim_dict = dict()
        cL_i = compareList[i]
        maxSimilarity = -2 #ensures first pass of loop below works
        for j in range(len(compareList)):
            cL_j = compareList[j]

            #use a set to easily get common users
            user_i_set = set(movie_dict[cL_i].keys())
            user_j_set = set(movie_dict[cL_j].keys())

            if i == j:
                continue #similarity of a movie to itself is invalid
            if i > j:
                #go to a past occurence and copy its value
                similarity = dict_store[(cL_j,cL_i)][1]
                corr_movie = cL_j
                corr_user = dict_store[(cL_j,cL_i)][2]
                if similarity > maxSimilarity: #keep track of local maxima
                    maxSimilarity = similarity
                    maxMovie = corr_movie
                    maxUser = corr_user
                continue

            common_users = user_i_set & user_j_set
            if len(common_users) < thresh: #check for threshold cut off
                similarity = 0
            else:
                #get ratings of common users for movies i and j
                rate_ij = [(movie_dict[cL_i][user],movie_dict[cL_j][user]) \
                 for user in common_users]
                similarity = checkSimilar(rate_ij) #calculate the similarity

            #save the corr movies, sim and n of common users for each movie i
            corr_movie = cL_j; corr_user = len(common_users)
            dict_store[(cL_i,cL_j)] = (corr_movie,similarity,corr_user)
            if similarity > maxSimilarity: #keep track of local maxima
                maxSimilarity = similarity
                maxMovie = corr_movie
                maxUser = corr_user
            
        sim_f.write('{} ({}, {:.2f}, {})\n'.format \
        (cL_i,maxMovie,maxSimilarity,maxUser))
        
    sim_f.close()
    
def loadMovies(in_file):
    """ Return a dict of movies as key and a dict of user and rating as values
    
    prints out details of in_file at the end
    """
    movie_dict = dict()
    user_list = []
    #load file and extract the values interested in
    with open(in_file,'r') as data_f:
        for data_line in data_f:
            data_line = data_line.split()
            movie = int(data_line[1]); user = int(data_line[0]) 
            rating = int(data_line[2])
            user_list.append(user)
            #map a key of movies to correspondng users and ratings
            if movie in movie_dict:
                movie_dict[movie].update({user:rating})
            else:
                movie_dict[movie] = {user:rating}
    data_len = len(user_list)
    print("Read {} lines with total of {} movies and {} users".format \
    (data_len,len(movie_dict),len(set(user_list))))
    return movie_dict
    
if __name__ == "__main__":
    #incorrect number of arguments, print usage message
    if len(sys.argv) < 3:
        print("Usage")
        print("$ python3 similarity.py <data_file> <output_file> \
         [user_thresh (default = 5)]")
        sys.exit(0)

    if len(sys.argv) == 4:
        thresh = sys.argv[3]
    else:
        thresh  = 5;
    
    in_file = str(sys.argv[1])
    out_file = str(sys.argv[2])
    
    #display to terminal as feedback to user
    print("Input MovieLens file: {}".format(sys.argv[1]))
    print("Output file for  similarity data: {}".format(sys.argv[2]))
    print("Minimum number of common users: {}".format(thresh))
    
    init_time = time.time()
    
    #create a dict of movies with corresponding users and ratings
    myMovies = loadMovies(in_file)
            
    #create a file of movies with corresponding movies of maximum similarity
    sim_dict = compareMovies(myMovies,thresh,out_file)
    
    final_time = time.time()
    
    #compute time it takes for program to run
    elapsed_time = final_time - init_time
    
    print("Computed similarities in {} seconds".format(round(elapsed_time,3)))
