
#include <iostream>
#include "console.h"
#include "filelib.h"
#include <fstream>
#include "simpio.h"
#include "random.h"
#include "queue.h"
#include "vector.h"
#include "map.h"
#include "random.h"
using namespace std;

void createWindow(ifstream& infile,int value,Queue<string>& window, string word);
void upgradeMap(Queue<string>& window,string word1,Map<Queue<string>, Vector<string> >& map);
void createMap(ifstream& infile,Queue<string>& window,string& word,Map<Queue<string>, Vector<string> >& map);
void wrapAround(Queue<string>& copy,Queue<string>& window,Map<Queue<string>, Vector<string> >& map);
void createWindow1(Queue<string>& window1,const Map<Queue<string>, Vector<string> >& map);
void upgradeWindow1(Queue<string>& window1,const Map<Queue<string>, Vector<string> >& map,string& word1);
void printText(Queue<string>& window1,int textnumber,int valueN,const Map<Queue<string>, Vector<string> >& map,string& word1);
void input(int& valueN);
void intro();

int main() {
    int valueN;
    intro();
    ifstream infile;
    promptUserForFile(infile,"Input file name?","Unable to open that file.  Try again.");
    input(valueN);
    Queue<string> window;
    Map<Queue<string>, Vector<string> > map;
    string word;
    createWindow(infile,valueN,window,word);
    Queue<string> copy=window;
    createMap(infile,window,word,map);
    wrapAround(copy,window,map);
    Queue<string> window1;
    string word1;
    createWindow1(window1,map);
    while(true){
        int textnumber= getInteger("# of random words to generate (0 to quit)?","Illegal integer format. Try again.");
        if(textnumber==0){
            break;
        }
        while(true){
            if(textnumber>valueN){
                break;
            }
            cout<<"Must be at least "<<valueN<<" words."<<endl<<endl;
            textnumber= getInteger("# of random words to generate (0 to quit)?","Illegal integer format. Try again.");
        }
        printText(window1,textnumber,valueN,map,word1);
    }
    cout << "Exiting." << endl;
    return 0;
}

//This function is used to create the windoe which heps us to create the map.
//takes 4 parameters window,our file,value of N and word which helpes us to read file and store its words in a string.
void createWindow(ifstream& infile,int value,Queue<string>& window, string word){
    for(int i=0;i<value-1;i++){
        infile >>word;
        window.enqueue(word);
    }
}

//Function is used to upgrade our map which helpes us to create random texts.
//takes 3 parameters: the window, map, and word1 which is used to read from file.
void upgradeMap(Queue<string>& window,string word1,Map<Queue<string>, Vector<string> >& map){
    Vector<string> vector;
    if (map.containsKey(window)){
        map[window]+=word1;
    }
    else{
        vector+=word1;
        map.put(window, vector);
    }
}

//Functions aim is to create the last situation of the map using upgradeMap function.
void createMap(ifstream& infile,Queue<string>& window,string& word,Map<Queue<string>, Vector<string> >& map){

    while(infile >>word){
        upgradeMap(window,word,map);
        window.enqueue(word);
        window.dequeue();

    }
}

//Functions main aim is to connect the end and first part of the file.
//uses a parameter copy that stores the first queue of the map and window which is the last queue of the map.
//provides connection between queues and adds the new queue(key) to the map.
void wrapAround(Queue<string>& copy,Queue<string>& window,Map<Queue<string>, Vector<string> >& map){

    int size = window.size();
    for (int i = 0; i < size; i++) {
        string str=copy.dequeue();
        upgradeMap(window,str,map);
        window.dequeue();
        window.enqueue(str);

    }
}

//Creates window1 which is used to print the random text.Uses map to create it.
void createWindow1(Queue<string>& window1,const Map<Queue<string>, Vector<string> >& map){
    Vector<Queue<string> > vector;
    vector= map.keys();
    window1=vector[randomInteger(0,vector.size()-1 )];
}

//Upgrades the window1 and uses it as the key of the value to be printed next.Uses randomInteger function to maintain the randomness of the text.
void upgradeWindow1(Queue<string>& window1,const Map<Queue<string>, Vector<string> >& map,string& word1){
    word1=map[window1][randomInteger(0,map[window1].size()-1 )];
    window1.enqueue(word1);
    window1.dequeue();
}

//Prints the text in the appropriate format.
//Uses two for loops one for printing the first window1 and one for printing each value that corresponds to the upgraded window1.
void printText(Queue<string>& window1,int textnumber,int valueN,const Map<Queue<string>, Vector<string> >& map,string& word1){
    Queue<string> copy1;
    copy1=window1;
    cout<<"...";
    for(int i=0;i<window1.size();i++){
        cout<<copy1.dequeue()<<" ";
    }
    for (int i=0;i<textnumber-valueN+1;i++) {
        upgradeWindow1(window1,map,word1);
        cout<<word1<<" ";
    }
    cout<<"..."<<endl<<endl;
}

//Used to get the value of N as an input from the user.Gives errors if input is inappropriate.
void input(int& valueN){
    valueN= getInteger("Value of N? ","Illegal integer format. Try again.");
    while(true){
        if(valueN>=2){
            break;
        }
        else {
            cout<<"N must be 2 or greater."<<endl;
            valueN= getInteger("Value of N? ","Illegal integer format. Try again.");
        }
    }
    cout<<endl;
}

//prints intro.
void intro(){
    cout<<"Welcome to CS 106B/X Random Writer ('N-Grams')!"<<endl<<
    "This program generates random text based on a document."<<endl<<
    "Give me an input file and an 'N' value for groups"<<endl<<
    "of words, and I'll create random text for you."<<endl<<endl;
}
