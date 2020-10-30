This is the readme file for hw5.

Brief State if the Problem
Given a file containing a maze, the task is to write a c++ program that computes
a solution to the maze using the right following algorithm and then store 
the solution in a file.

Description of mazesolver.cpp
The program reads maze file then creates a 2D array of the maze where 1 represents 
a wall and 0 a space. It then uses a function checkSpace to march along the maze 
using the right following algorithm until it gets to the end of the maze. While 
going through the maze, it stores all its locations in the solution file.

Summary of Code verification with checksoln.py
The program reads the maze file and stores it in a numpy array with 1 showing 
presence of wall while 0 shows space.
It then reads the solution file and checks that there are no conflicting values 
where a solution path goes through a wall.
