// This is the CPP file you will edit and turn in. (Remove this comment!)

#include <iostream>
#include "console.h"
#include "grid.h"
#include "filelib.h"
#include <fstream>
#include "strlib.h"
#include "simpio.h"
#include "console.h"
#include "lifegui.h"
using namespace std;


void intro();
void printgrid(Grid<char>& med);
int countlivneg(int i, int j, Grid<char> matrix);
bool isalive(int i, int j,Grid<char> matrix);
void newgeneration(Grid<char> matrix,Grid<char>& grd1);
bool menu (Grid<char>& grd, bool& b);
void filetogrid(ifstream &infile,Grid<char>& matrix,int & rows ,int & cols);

int main() {

    intro();
    LifeGUI::initialize();
    //boolean 'a' is used to determine when should the while loop finishes.It ıs uesd with the help of menu function.
    bool a=true;
    while(a){
        ifstream infile;
        Grid<char> grd;
        int rows;
        int cols;
        promptUserForFile(infile, "Grid input file name?","Unable to open that file. Try again.");
        filetogrid(infile,grd,rows,cols);
        printgrid(grd);
        LifeGUI::repaint();
        a=menu(grd,a);
    }

    cout << "Have a nice Life!" << endl;
    LifeGUI::shutdown();
    return 0;
}



//Function takes no parameters.It is used as the intro of the code to inform users.
void intro(){
    cout<<"Welcome to the CS 106B/X Game of Life!"<<endl<<
    "This program simulates the lifecycle of a bacterial colony."<<endl<<
    "Cells (X) live and die by the following rules:"<<endl<<
    "* A cell with 1 or fewer neighbors dies."<<endl<<
    "* Locations with 2 neighbors remain stable."<<endl<<
    "* Locations with 3 neighbors will create life."<<endl<<
    "* A cell with 4 or more neighbors dies."<<endl<<endl;
}

//Function takes a grid as the parameterand prints it.
void printgrid(Grid<char>& med) {

    for (int i = 0; i<med.numRows(); i++) {
        for (int j = 0; j<med.numCols(); j++) {
            char cell = med[i][j];
            cout << cell;
        }
        cout<< endl;
    }
}

//Function takes 3 parameters.2 of them(i and j)are used to determine the place of a cell in a matrix which is another parameter.
//Aim of the function is to find the number of neighbours that a specific cell has.
int countlivneg(int i, int j, Grid<char> matrix){
    int count=0;
    for (int k = 0; k < matrix.height()-1; k++){
        for (int v = 0; v < matrix.width()-1; v++){
            if(((abs(k-i))% (matrix.height()-2)) <= 1 && ((abs(v-j))%(matrix.width()-2)) <=1 && !(k==i & j==v) && matrix[k][v]=='X' ){
                count++;
            }
        }
    }
    return count;
}

//Function takes 3 parameters.2 of them(i and j)are used to determine the place of a cell in a matrix which is another parameter.
//Aim of function is to determine whether cell dies in the next generation according to the number of neighbours.
bool isalive(int i, int j,Grid<char> matrix){
    if(countlivneg(i,j,matrix)<= 1 || (countlivneg(i,j,matrix)>=4 )){
        return false;
    }
    else if(countlivneg(i, j, matrix)==3){
        return true;

    }
    else if(countlivneg(i, j, matrix)==2 && matrix[i][j]=='X'){
         return true;
    }
    else{
         return false;
    }
}

//Takes two grid parameters.One of them is the old generation which is our reference to create the new one and the other one is our new generation.
//Aim of the function is to create the new generation according to isalive function.
void newgeneration(Grid<char> matrix,Grid<char>& grd1){
    LifeGUI::clear();
    for (int i = 0; i < matrix.height(); i++){
        for (int j = 0; j < matrix.width(); j++){
            if(isalive(i,j,matrix)){
            grd1[i][j]='X';
            LifeGUI::fillCell(i, j);
            }
            else{
            grd1[i][j]='-';
            }
        }
    }
}

//takes two parameters.One them is the grid that we want to operate on and the other one is a boolean which helps us to finish the entire program by returning it.
//Aim of the function is to provide a menu to the user in which they can select the operations they want on the generation.
bool menu (Grid<char>& grd, bool& b){
    while(true){
        string line1=getLine("a)nimate, t)ick, q)uit?");
        if (line1=="t"||line1=="T"||line1==""){

            Grid<char> grd1;
            grd1.resize(grd.height(),grd.width());
            newgeneration(grd,grd1);
            printgrid(grd1);
            LifeGUI::repaint();
            grd=grd1;
        }
        else if(line1=="a"||line1=="A"){
            int frames=getInteger("How many frames?","Illegal integer format.  Try again.\n");
            for (int u=0;u<frames;u++) {
                Grid<char> grd1;
                grd1.resize(grd.height(),grd.width());
                newgeneration(grd,grd1);
                clearConsole();
                printgrid(grd1);
                LifeGUI::repaint();
                pause(100);
                grd=grd1;
            }
        }
        else if (line1=="q"||line1=="Q") {
            if(!getYesOrNo("Load another file? (y/n)","Please type a word that starts with 'Y' or 'N'.")){
                b=false;
            }
            break;
        }
        else {
            cout<<"invalid choise; please try again.";
        }
    }
    return b;
}



//Function takes 4 parameters which are the file that should be read, grid that we want to transform to the file and rows and cols of the grid.
//Aim of the function is to create a grid which displays the generation via the provided file.
void filetogrid(ifstream &infile,Grid<char>& matrix,int & rows ,int & cols){

    string line;
    getline(infile,line);
    rows = stringToInteger(line);
    getline(infile,line);
    cols = stringToInteger(line);
    matrix.resize(rows,cols);
    LifeGUI::resize(rows, cols);
    for(int i=0;i<rows;i++) {
        getline(infile, line);
        int j=0;
        for(char ch:line){
            if(ch=='X'){
                LifeGUI::fillCell(i, j);
            }
            matrix[i][j]=ch;
            j++;
        }
    }
}
