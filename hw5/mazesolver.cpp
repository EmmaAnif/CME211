#include <fstream>
#include <iostream>
#include <string>

#define ni 201 //number of rows of static array
#define nj 201 //number of columns of static array

//enumeration of the four different possible directions
enum dir{
    up,
    down,
    right,
    left
};

//function prototype
bool checkSpace(enum dir check, enum dir *d, int arr[ni][nj], int *rp, int *cp); 

int main(int argc, char *argv[]){
    int arr[ni][nj]= {0}; //initialize static array with zeros
    
    //catch the case where insufficient arguments are provided
    if (argc < 3){
        std::cout << "Usage: " << std::endl;
        std::cout << " " << argv[0] << " <maze file> <solution file>" << std::endl;
        return 0;
    }
    
    //save name of maze and solution files
    std::string maze_file = argv[1];
    std:: string solution_file = argv[2];
    
    int nif,njf,lastRow;
    
    //open maze file
    std::ifstream f(maze_file);

    if (f.is_open()){
        //Read the size of the data and make sure storage is sufficient
        f >> nif >> njf;
        if (nif > ni or njf > nj){
            std::cout << "Not enough storage available" << std::endl;
            return 0; //quit the program
        }
        lastRow = nif - 1; //store the last row of Maze

//--style_0
//--A while loop would be more apporpriate and would ensure that you read
//--the whole file
//--START        
        //Read the data and populate the array
        int i_f,j_f;
        for (int i = 0; i < nif; i++){
            for (int j = 0; j < njf; j++){
                f >> i_f;
                f >> j_f;
                arr[i_f][j_f] = 1; //a value of 1 in array shows presence of wall
            }
        }
//--END
        f.close();
    }
    else{
        std::cout << "Failed to open maze file" << std::endl;
        return 0;
    }
    
    //open solution file
    std::ofstream g(solution_file);
    
    if (g.is_open()){
        
        int row, col; //keep track of row and column
        dir d = down; //initialize direction with down
        row = 0; col = 0;
        for (int j = 0; j < njf; j++){ //find entrance to the maze
            if (arr[0][j] == 0){
                col = j;
                g << row << " " << col << std::endl; //store in solution file
                break;
            }
        }
        while (1){ //while still in the maze

            switch (d) //check direction
            {
                case down:
                {
                    if (checkSpace(right,&d,arr,&row,&col)){ //check right
                        g << row << " " << col << std::endl;
                    }
                    else if (checkSpace(down,&d,arr,&row,&col)){ //check down
                        g << row << " " << col << std::endl;
                    }
                    
                    else if (checkSpace(left,&d,arr,&row,&col)){ // check left
                        g << row << " " << col << std::endl;
                    }
                    else{ //go back up
                        checkSpace(up,&d,arr,&row,&col);
                        g << row << " " << col << std::endl;
                    } 
                }
                break;
                
                case right:
                { 
                    if (checkSpace(up,&d,arr,&row,&col)){ //check up
                        g << row  << " " << col << std::endl;
                    }
                    else if (checkSpace(right,&d,arr,&row,&col)){ //check right
                        g << row << " " << col  << std::endl;
                    }
                    
                    else if (checkSpace(down,&d,arr,&row,&col)){ //check down
                        g << row << " " << col << std::endl;
                    }
                    else{ // go back left
                        checkSpace(left,&d,arr,&row,&col);
                        g << row << " " << col << std::endl;
                    }
                }
                break;
                
                case up:
                {
                    if (checkSpace(left,&d,arr,&row,&col)){ // check left
                        g << row << " " << col << std::endl;
                    }
                    else if (checkSpace(up,&d,arr,&row,&col)){ //check up
                        g << row << " " << col << std::endl;
                    }
                    
                    else if (checkSpace(right,&d,arr,&row,&col)){ //check right
                        g << row << " " << col << std::endl;
                    }
                    else{ //go back down
                        checkSpace(down,&d,arr,&row,&col);
                        g << row << " " << col << std::endl;
                    }
                }
                break;
                
                case left:
                {
                    if (checkSpace(down,&d,arr,&row,&col)){ //check down
                        g << row << " " << col << std::endl;
                    }
                    else if (checkSpace(left,&d,arr,&row,&col)){ // check left
                        g << row << " " << col << std::endl;
                    }
                    
                    else if (checkSpace(up,&d,arr,&row,&col)){ //check up
                        g << row << " " << col << std::endl;
                    }
                    else{ //go back right
                        checkSpace(right,&d,arr,&row,&col);
                        g << row << " " << col << std::endl;
                    }
                }
                break;
                
            }
//--style_1
//--This should be your condition in for the while loop
//--START
            if (row == lastRow){ //have gotten to the exit of the maze
                break;
            }   
//--END  
      }
        g.close();
    }
    else{
        std::cout << "Failed to open solution file" << std::endl;
    }
    return 0;
}

/*
checkSpace

Returns True if there is a space in the direction checked and False otherwise

Input: direction checked 
        pointer to current direction
        pointer to array storing maze
        pointer to row of array
        pointer to col of array
        
Output: modifies row, col and current direction to keep track of position in maze
*/

bool checkSpace(enum dir check, enum dir *d, int arr[ni][nj], int *rp, int *cp){
    switch (check)
    {
        case down:{
            if (arr[*rp + 1][*cp] == 0){ //check for space in maze
                (*rp)++; *d = down; //update row and current direction in place
                return 1;
            }
        }
        break; 
        case up:{
            if (arr[*rp - 1][*cp] == 0){ //check for space in maze
                (*rp)--; *d = up;
                return 1;
            }
        }
        break; 
        case right:{
            if (arr[*rp][*cp-1] == 0){ //check for space in maze
                (*cp)--; *d = right; //update column and current direciton in place
                return 1;
            }
        }
        break; 
        case left:{
            if (arr[*rp][*cp + 1] == 0){ //check for space in maize
                (*cp)++; *d = left;
                return 1;
            }
        }
    } 
    return 0; //this means there was no space in the direction checked
}
