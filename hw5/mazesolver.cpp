#include <fstream>
#include <iostream>
#include <string>

#define ni 201
#define nj 201

enum direction{
    up,
    down,
    right,
    left
};

//Read the array values from maze1.txt file
int main(int argc, char *argv[]){
    int a[ni][nj]= {0};
    
    //catch the case where insufficient arguments are provided
    if (argc < 3){
        std::cout << "Usage: " << std::endl;
        std::cout << " " << argv[0] << " <maze file> <solution file>" << std::endl;
        return 0;
    }
    
    std::string maze_file = argv[1];
    std:: string solution_file = argv[2];
    
    std::ifstream f(maze_file);
    std::ofstream g(solution_file);

    if (f.is_open() and g.is_open()){
        //Read the size of the data and make sure storage is sufficient
        int nif,njf;
        f >> nif >> njf;
        if (nif > ni or njf > nj){
            std::cout << "Not enough storage available" << std::endl;
            return 0; //quit the program
        }
        int lastRow = nif - 1; //store the last row of Maze
        
        //Read the data and populate the array
        int i_f,j_f;
        for (int i = 0; i < nif; i++){
            for (int j = 0; j < njf; j++){
                f >> i_f;
                f >> j_f;
                a[i_f][j_f] = 1;
                //std::cout<<"a["<<i<<"]["<<j<<"] = "<<a[i][j]<<std::endl; 
            }
        }
        f.close();
        
        int row, col; //keep track of row and column
        direction d = down; //initialize direction with down
        row = 0; col = 0;
        for (int j = 0; j < njf; j++){
            if (a[0][j] == 0){
                g << row << " " << j << std::endl;
                col = j;
                break;
            }
        }
        int numIters = 0;
        while (1){ //while still in the maze
            numIters++;
            /*
            if (numIters > 500){
               std::cout << "Terrible Bug in code" << std::endl;
               break;
            }
            */

            switch (d)
            {
                case down:
                {
                    if (a[row][col-1] == 0){ //check right
                        g << row << " " << col - 1 << std::endl;
                        col--; d = right;
                    }
                    else if (a[row+1][col] == 0){ //check down
                        g << row + 1 << " " << col << std::endl;
                        row++; d = down;
                    }
                    
                    else if (a[row][col+1] == 0){ // check left
                        g << row << " " << col + 1 << std::endl;
                        col++; d = left;
                    }
                    else{ //go back up
                        g << row - 1 << " " << col << std::endl;
                        row--; d = up;
                    } 
                }
                std::cout << row << " " << col << std::endl;
                break;
                
                case right:
                { 
                    if (a[row-1][col] == 0){ //check up
                        g << row - 1 << " " << col << std::endl;
                        row--; d = up;
                    }
                    else if (a[row][col-1] == 0){ //check right
                        g << row << " " << col - 1 << std::endl;
                        col--; d = right;
                    }
                    
                    else if (a[row+1][col] == 0){ //check down
                        g << row + 1 << " " << col << std::endl;
                        row++; d = down;
                    }
                    else{ // go back left
                        g << row << " " << col + 1 << std::endl;
                        col++; d = left;
                    }
                }
                std::cout << row << " " << col << std::endl;
                break;
                
                case up:
                {
                    if (a[row][col+1] == 0){ // check left
                        g << row << " " << col + 1 << std::endl;
                        col++; d = left;
                    }
                    else if (a[row-1][col] == 0){ //check up
                        g << row - 1 << " " << col << std::endl;
                        row--; d = up;
                    }
                    
                    else if (a[row][col-1] == 0){ //check right
                        g << row << " " << col - 1 << std::endl;
                        col--; d = right;
                    }
                    else{ //go back down
                        g << row + 1 << " " << col << std::endl;
                        row++; d = down;
                    }
                }
                std::cout << row << " " << col << std::endl;
                break;
                
                case left:
                {
                    if (a[row+1][col] == 0){ //check down
                        g << row + 1 << " " << col << std::endl;
                        row++; d = down;
                    }
                    else if (a[row][col+1] == 0){ // check left
                        g << row << " " << col + 1 << std::endl;
                        col++; d = left;
                    }
                    
                    else if (a[row-1][col] == 0){ //check up
                        g << row - 1 << " " << col << std::endl;
                        row--; d = up;
                    }
                    else{ //go back right
                        g << row << " " << col - 1 << std::endl;
                        col--; d = right;
                    }
                }
                std::cout << row << " " << col << std::endl;
                break;
                
            }
            if (row == lastRow){
                std::cout << "Got out of Maze!" << std::endl;
                break;
            }   
        }
        g.close();
    }
    else{
        std::cout << "Failed to open files" << std::endl;
    }
    return 0;
}
