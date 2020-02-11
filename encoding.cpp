//The following code involves the methods neccessary to compress, decompress,
//encode, decode files and strings. The compression uses Huffman encoding algortihm.
//The code uses abstract data structures such as maps, queues and trees in order to
// perform Huffman's encoding algorithm.

#include "encoding.h"
#include "priorityqueue.h"
#include "filelib.h"


//buildFrequencyTable Builds a frequency table from the a given input stream containing data.
//parameter input is the given input stream whose data will be used to generate a frequency table.
//return is the frequency table, with the frequencies of the characters in the input stream's data.
Map<char, int> buildFrequencyTable(istream& input)
{
    Map<char, int> map;
    char ch;
    while (input.get(ch)) {
        if(map.containsKey(ch)){
            map[ch]++;
        }
        else {
            map.put(ch,1);
        }
    }
    return map;
}

// buildEncodingTree builds an encoding tree to from a given frequency table. The tree is
//built according to the Huffman's algorithm and a priority queue is used in the steps.
//parameter freqTable is the given frequency table, which contains the data for the tree.
//return is the encoding tree built from Huffman's algorithm.
HuffmanNode* buildEncodingTree(Map<char, int>& freqTable)
{
    PriorityQueue<HuffmanNode*> pq;
    for (char key:freqTable) {//creates priority queue
        HuffmanNode* leaf=new HuffmanNode(key);
        pq.enqueue(leaf,freqTable[key]);
    }
    while(pq.size()>1){//constantly updates priority queue
        double zeropri=pq.peekPriority();
        HuffmanNode* zero=pq.dequeue();
        double onepri=pq.peekPriority();
        HuffmanNode* one=pq.dequeue();
        HuffmanNode* parent=new HuffmanNode(zero,one);
        pq.enqueue(parent,zeropri+onepri);
    }

    return pq.dequeue();//returns the only element left of the priority queue
}

//function is used as the helper function of flattenTreeToHeader function
//uses recursion and generates the string representation of the tree by changing the parameter str.
//Other parameter t is the pointer to the tree.
void flattenTreeToHeaderHelper(string& str,HuffmanNode* t){
    if(t!=nullptr){
        if(t->isLeaf()){
            str+=".";
            str+=t->ch;
        }
        else {
            str+="(";//if t is not a leaf adds both side of the tree and puts paranthesis outside of it.
            flattenTreeToHeaderHelper(str,t->zero);
            flattenTreeToHeaderHelper(str,t->one);
            str+=")";
        }
    }
}

//functions main aim is to store the tree in a string and uses the recursive flattenTreeToHeaderHelper function to reach that aim
//returns a string presentation of the tree which will contain the necessary information and structure of
//the tree that allows us to recreate the tree later.
//takes a pointer to the tree as a parameter.
string flattenTreeToHeader(HuffmanNode* t)
{
    string str="";
    flattenTreeToHeaderHelper(str,t);
    return str;
}

//this function is the helper function of the function recreateTreeFromHeader.
//uses recursion to reach its aim which is returning the tree hidden in the string copy that is passed as parameter.
//function repeatedly changes the copy to read the entire string and updates tree as it keeps reading it.
HuffmanNode* recreateTreeFromHeaderHelper(string& copy){
    if(copy[0]=='.'){
        HuffmanNode* leaf=new HuffmanNode(copy[1]);
        copy=copy.substr(2);
        return leaf;
    }
    else if(copy[0]=='('){
        copy=copy.substr(1);
        HuffmanNode* leaf1=recreateTreeFromHeaderHelper(copy);//these two lines are the main reason to pass copy as reference
        HuffmanNode* leaf2=recreateTreeFromHeaderHelper(copy);//copy is deducted in the above line so we can create the other side of the leaf with the new copy
        HuffmanNode* leaf=new HuffmanNode(leaf1,leaf2);
        return leaf;
    }
    else {
        copy=copy.substr(1);
        return recreateTreeFromHeaderHelper(copy);//used to pass the characters other than '.' or '('.
    }

}

//functions main aim is to recreate the tree which is stored in the header of the file.
//This tree will be later used on reading the compressed file and decompressing it.
//takes one parameter str which is a string represantation of tree that is used as our reference to reconstruct the tree.
//returns tree.
//uses the recursive helper function recreareTreeFromHeaderHelper to reach its aim.
//helper function is necessary beacuse the string represantation of the tree does not passed by reference in this function
//so the function makes a copy of str and pass it with reference as a parameter to its helper function.
HuffmanNode* recreateTreeFromHeader(string str)
{
    string copy=str;
    return recreateTreeFromHeaderHelper(copy) ;
}

//BuildEncodingMapHelper is a recursive helper function used by the buildEncodingMap function.
//Basically performs pre-order traversal on the encoding tree and adds the binary code of the data
// , which meets the conditions to the map. Stores the binary code through the recursive calls.
//parameter map is the reference to the encoding map.
//parameter encodingTree is a pointer to the encoding tree.
//parameter str is the stored binary code.
void BuildEncodingMapHelper(Map<char, string>& map,HuffmanNode* encodingTree,string str){
    if(encodingTree==nullptr){return;}
    if(!encodingTree->isLeaf()){
        BuildEncodingMapHelper(map,encodingTree->one,str+"1");
        BuildEncodingMapHelper(map,encodingTree->zero,str+"0");
    }
    else {
        map.put(encodingTree->ch,str);
    }
}

//buildEncodingMap Builds an encoding map, where every character in a given encoding tree
//have a suitable Huffman binary code. Those binary codes are added to the encoding map by
//traversing the given encoding tree with a call to the buildEncodingMapHelper function.
//param encodingTree is the given encoding tree which is used to build the encoding map.
//return is the map which is encoding map built from Huffman's algorithm.
Map<char, string> buildEncodingMap(HuffmanNode* encodingTree)
{
    Map<char, string> map;
    string str="";
    BuildEncodingMapHelper(map,encodingTree,str);
    return map;
}

//freeTree frees and deletes all the memory associated with a given tree.
//parameter t is the reference to the tree to be deleted
void freeTree(HuffmanNode* t)
{
    if(t!=nullptr){
        freeTree(t->zero);
        freeTree(t->one);
        delete t;
    }
}

//compress function encodes the data in a file and makes it smaller in size. Uses Huffman
//encoding by calling several functions. Generates a compressed version of data.
//parameter input is the input stream to be compressed.
//parameter output is the compressed data's output stream.
void compress(istream& input, HuffmanOutputFile& output)
{
    Map<char, int> freqTable=buildFrequencyTable(input);
    rewindStream(input);//necessary because we should read the input again
    HuffmanNode* encodingTree= buildEncodingTree(freqTable);
    Map<char, string> encodingMap= buildEncodingMap(encodingTree);
    string header=flattenTreeToHeader(encodingTree);
    output.writeHeader(header);
    char ch;
    while (input.get(ch)) {//iterates over each char
        string str=encodingMap[ch];
        for (char chr:str) {//iterates over the value of each char to convert that value into a form that we can easily write to our output
            int bitValueChar=chr-'0';//this line is used to convert the char to integer and uses the principle that each integer character numbers are consecutive
            output.writeBit(bitValueChar);
        }
    }
}

// decompress function decodes the data in a file and makes it larger in size. Used Huffman
//decoding by calling several functions. Generates a decompressed version of data.
//parameter input is the input stream to be decompressed.
//parameter output is the decompressed data's output stream.
void decompress(HuffmanInputFile& input, ostream& output)
{
    HuffmanNode* tree= recreateTreeFromHeader(input.readHeader());
    HuffmanNode* copy=tree;
    int bit=input.readBit();
    while(bit!=-1){
        if(copy->isLeaf()){
            output<<copy->ch;
            copy=tree;//reinitializes the tree since we start to look for a new character
        }
        if(bit==0){
            copy=copy->zero;//looks to the left side
        }
        if(bit==1){//looks to the right side
            copy=copy->one;
        }
        bit=input.readBit();
    }
    if(bit==-1){output<<copy->ch;}//we should write the character to the output even if the next bits value is "-1"
}
