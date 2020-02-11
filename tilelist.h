//this is the .h file of the programme that arranges tiles in a way that the front element in the linked list appears as the highest element in the z- axis.
//project includes several functions to arrange the tiles.

#ifndef _tilelist_h
#define _tilelist_h

#include <iostream>
#include <string>
#include "gwindow.h"
#include "tilenode.h"
using namespace std;

class TileList {
public:
    TileList();//O(1)
    ~TileList();//O(N)
    void addBack(int x, int y, int width, int height, string color);//O(1)
    void addFront(int x, int y, int width, int height, string color);//O(1)
    void clear();//O(N)
    void debug();
    void drawAll(GWindow& window) const;//O(N)
    TileNode* getBack() const;//O(1)
    TileNode* getFront() const;//O(1)
    bool highlight(int x, int y);//O(N)
    bool lower(int x, int y);//O(N)
    void merge(int x, int y);//O(N^2)
    bool raise(int x, int y);//O(N)
    bool remove(int x, int y);//O(N)
    int removeAll(int x, int y);//O(N^2)

private:
    TileNode *front;//pointer to the front most element.
    TileNode *back;//pointer to the bottom element.
    
    TileNode* frontTileClicked(TileNode * tile,int x, int y);
    void deleteTile(TileNode *tile);
    int removeAllHelper(int x,int y,TileNode* tile);
};

#endif // _tilelist_h
