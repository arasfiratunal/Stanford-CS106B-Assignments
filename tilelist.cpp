//Note for the z-axis I am treating the tiles furthest on top
//as the front most tiles and the tiles lowest (below the others)
//as the back most tiles.  So prev will go lower and next will go higher
//this is the cpp file.

#include "tilelist.h"
#include <algorithm>
#include "strlib.h"
using namespace std;

//Creates an empty Tile List
TileList::TileList() {
    front = nullptr;
    back = nullptr;
}

// Deconstructs the Tile List freeing any allocated memory
TileList::~TileList() {
    while (front != nullptr) {
        TileNode* trash = front;
        front = front->next;
        delete trash;
    }

}


 //Adds a new tile to the back of the Tile List

//parametereter x is the upper left x coordinate of the tile
// paramerter y is the upper left y coordinate of the tile
// parameter width is the width of the tile
// parameter height is the height of the tile
// parameter color is the color of the tile
void TileList::addBack(int x, int y, int width, int height, string color) {
    TileNode *newTile = new TileNode(x, y, width, height, color,back,nullptr);
    if (back != nullptr){
        back->next=newTile;
    }
    else{
        front = newTile;
    }
    back = newTile;
    
}

//Adds a new tile to the front of the Tile List

//parametereter x is the upper left x coordinate of the tile
// paramerter y is the upper left y coordinate of the tile
// parameter width is the width of the tile
// parameter height is the height of the tile
// parameter color is the color of the tile
void TileList::addFront(int x, int y, int width, int height, string color) {
    TileNode *newTile = new TileNode(x, y, width, height, color,nullptr,front);
    if (front != nullptr){
        front->prev = newTile;
    }
    else{
        back = newTile;
    }
    front = newTile;
}

//Clears the Tile List of any existing tiles inside of it
void TileList::clear() {

    while (front != nullptr) {
        TileNode* trash = front;
        if(front==back){front=back=nullptr;}
        else{front = front->next;}
        delete trash;
    }

}


//used for debugging via printing the front most element.
void TileList::debug() {
    cout<<front->toString();

}

//  Draws all of the Tiles in Tile List on a GUI window
//from bottom (back) to top (front)
// parameter window is the window to draw the tiles on
void TileList::drawAll(GWindow& window) const {
    TileNode *tile = back;
    while (tile != nullptr) {
        tile->draw(window);
        tile = tile->prev;
    }

}

//returns the back element.
TileNode* TileList::getBack() const {

    return back;
}

//returns the front element.
TileNode* TileList::getFront() const {

    return front;
}

// Highlights the front most tile containing a given coordinate
//returns a boolean value depend on the success of the operation.
//parameter x is the x part of the coordinate
//parameter y is the y part of the coordinate

bool TileList::highlight(int x, int y) {
    TileNode *ptr = front;
    while (ptr != nullptr) {//iterates over the list until a tile satisfies the given condition.
        if(ptr->contains(x,y)){
            ptr->color = "yellow";
            return true;
        }
        else{ptr = ptr->next;}
    }

    return false;
}

// Lowers the front most tile containing a given coordinate to the back of the Tile List
//returns a boolean value depend on the success of the operation.
//parameter x is the x part of the coordinate
//parameter y is the y part of the coordinate
bool TileList::lower(int x, int y) {
    TileNode *tile = frontTileClicked(front, x, y);
    if (tile == nullptr) {return false;}
    if(tile == back){return true;}
    if (tile == front){
       front = front->next;
    }
    tile->prev->next = tile->next;//connects the previous and the next tiles to each other.
    tile->next->prev = tile->prev;
    back->next = tile;
    tile->prev = back;
    tile->next = nullptr;
    back = tile;
    return true;

}
//combines all tiles from the list that touch the given x/y coordinates into a single tile whose size is exactly large enough to cover the entire area of all of the original rectangles
//first removes all rectengales that touch the given coordinates and then creates a new rectengale.
//uses removeAllHelper to delete rectengales.
//parameter x is the x part of the coordinate
//parameter y is the y part of the coordinate
void TileList::merge(int x, int y) {
   TileNode *tile = frontTileClicked(front, x, y);
   if(tile!=nullptr) {
       string colour=tile->color;
       int minleft=tile->x;
       int maxright=tile->x+tile->width;
       int mintop=tile->y;
       int maxtop=tile->y+tile->height;
       while(tile!=nullptr){//keeps updating the values above which will be used to create the new tile.
           if(tile->x<minleft){minleft=tile->x;}
           if(tile->x+tile->width>maxright){maxright=tile->x+tile->width;}
           if(tile->y<mintop){mintop=tile->y;}
           if(tile->y+tile->height>maxtop){maxtop=tile->y+tile->height;}
           tile=frontTileClicked(tile->next,x,y);
       }
       removeAllHelper(x,y,frontTileClicked(front, x, y));
       addFront(minleft,mintop,maxright-minleft,maxtop-mintop,colour);
   }
}


// Lowers the front most tile containing a given coordinate to the front of the Tile List
//returns a boolean value depend on the success of the operation.
//parameter x is the x part of the coordinate
//parameter y is the y part of the coordinate
bool TileList::raise(int x, int y) {
    TileNode *tile = frontTileClicked(front, x, y);
    if (tile == nullptr) {return false;}
    if(tile==front){return true;}
    if (tile == back){
       back = back->prev;
    }
    tile->prev->next = tile->next;
    tile->next->prev = tile->prev;
    front->prev = tile;
    tile->next = front;
    tile->prev = nullptr;
    front = tile;
    return true;
}


//Deletes the top most tile containing the given coordinate from the Tile List
//returns a boolean value depend on the success of removal operation.
//param x is the x part of the coordinate
//param y is the y part of the coordinate
bool TileList::remove(int x, int y) {
    TileNode* tile=frontTileClicked(front,x,y);
    if(tile!=nullptr){
        deleteTile(tile);return true;
    }
    return false;
}


//Deletes all tiles containing the given coordinate from the Tile List
//returns the total amount of rectengales deleted.
//parameter x is the x part of the coordinate
//parameter y is the y part of the coordinate
int TileList::removeAll(int x, int y) {

    TileNode* tile=frontTileClicked(front,x,y);
    return removeAllHelper(x,y,tile);
}

//Private helper method that returns a reference to the front most tile containing the specified coordinates
//parameter x is the x part of the coordinate
//parameter y is the y part of the coordinate
//parameter tile is the tile which iteration starts at.
//returns the reference to the front most tile containing the coordinates.Returns nullptr if the tile is not found.

TileNode* TileList::frontTileClicked(TileNode * tile,int x, int y) {
    TileNode *ptr = tile;
    while (ptr != nullptr) {
        if(ptr->contains(x,y)){
            return ptr;
        }
        ptr = ptr->next;
    }

    return nullptr;
}
//Private helper method that is used to delete the tile that is passed as a parameter and construct the connection between the previous and next tiles.
//completes its mission in several ways depend on the condition and place of the tile.
void TileList::deleteTile(TileNode *tile) {
    if(front==back){front=back=nullptr;}
    else if (tile == front){
        tile->next->prev = nullptr;
        front = front->next;
       }
    else if (tile == back){
        back = back->prev;
    }
    else{
        tile->prev->next = tile->next;
        tile->next->prev = tile->prev;

    }
    delete tile;//deletes to avoid memmory leak.
}


//Private helper method which is the helper function of both merge and removeAll functions.
//its main aim is to remove all the tiles that contains the specific x and y coordinates.
//completes its mission using recursion.
//parameter x is the x part of the coordinate
//parameter y is the y part of the coordinate
//parameter tile is the current tile which will be deleted.
int TileList::removeAllHelper(int x,int y,TileNode* tile){
    if(tile==nullptr){return 0;}
    else{
       TileNode* copy;
       if(tile->next!=nullptr){copy=tile->next;}//makes a copy to use it in the frontTileClicked function.
       else {
           copy=nullptr;
       }
       deleteTile(tile);
       return removeAllHelper(x,y,frontTileClicked(copy,x,y))+1;
    }
}
