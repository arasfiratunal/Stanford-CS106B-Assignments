

#include "fractals.h"
#include "gwindow.h"
using namespace std;

//this is a recursive function that draws the Sierpinski triangle fractal image.
//it takes starting points coordinate(x,y) size of the triangle and order of the triangle as parameters.
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    if(order<0||size<0||x<0||y<0){
        throw "invalid values";
    }
    else if(order==1){
        window.drawLine(x,y,x+size,y);
        window.drawLine(x+size,y,x+size/2,y+size*sqrt(3)/2);
        window.drawLine(x,y,x+size/2,y+size*sqrt(3)/2);
    }
    else if(order>1){
        drawSierpinskiTriangle(window,x,y,size/2,order-1);
        drawSierpinskiTriangle(window,x+size/2,y,size/2,order-1);
        drawSierpinskiTriangle(window,x+size/4,y+size*sqrt(3)/4,size/2,order-1);

    }
}

//The functions aim together with the floodFill function is to
//fill the pixels which are adherent and have the same colour with a new colour selected
//uses the coordinates of the chosen starting point and new colour of pixels as parameter.
//Also takes another parameter count to show the number of effected pixels.
void floodfillHelper(GWindow& window, int x, int y, int color,int color1,int& count){
    if(color==color1){
        count=0;
    }
    else if(window.getPixel(x, y)==color1){
        window.setPixel(x, y, color);
        count+=1;
        if(window.inCanvasBounds(x+1, y)){
            floodfillHelper(window,x+1,y,color,color1,count);
        }
        if(window.inCanvasBounds(x, y+1)){
            floodfillHelper(window,x,y+1,color,color1,count);
        }
        if(window.inCanvasBounds(x-1, y)){
            floodfillHelper(window,x-1,y,color,color1,count);
        }
        if(window.inCanvasBounds(x, y-1)){
            floodfillHelper(window,x,y-1,color,color1,count);
        }
    }
}

//It is the fuction which provides condition to implement the recursive helper function.
//throws error if parameters are invalid.
//returns the number of pixels effected.
//creates the new window.
int floodFill(GWindow& window, int x, int y, int color) {
    if (x<0||y<0||!window.inCanvasBounds(x,y)) {
        throw "Invalid input";
    }
    int count=0;
    int color1 = window.getPixel(x, y);//for copying the colour of starting pixel.
    floodfillHelper(window,x,y,color,color1,count);

    return count;
}

