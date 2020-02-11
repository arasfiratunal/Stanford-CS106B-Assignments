//Projects main aim is to cretae a game called Boggle which is a board game with a 4x4 square grid of letter cubes where players try to make words by connecting letters on adjacent cubes.
//human player plays against computer and they will receive points depend on the lenght of the word they choose.At the end of game the one with more points win.


#include "bogglegui.h"
#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "vector.h"
using namespace std;

//functions aim is to decide how many points that a word can get depend on its lenght.
//takes one parameter which is the word.
int points(string word){
    if(word.size()==4){
        return 1;
    }
    else if (word.size()==5) {
        return 2;
    }
    else if (word.size()==6) {
        return 3;
    }
    else if (word.size()==7) {
        return 5;
    }
    else {
        return 11;
    }
}

//Functions main aim is to verify the validation of the word that player find.
//Function also highligts the word found starting from the first character to the last.
//It is the helper function of the function humanWordSearch and it uses recursion to suceed its aim.
//Takes 5 parameters:the board in which we can see the caharacters,the word that user entered,row and col number to specify the part of board and a vector to mark the already used parts of board..
bool humanwordSearchHelper(Grid<char>& board, string word,int row, int col,Vector<string>& vec){
    if(!board.inBounds(row,col)||vec.contains(integerToString(row)+integerToString(col))){
        return false;
    }
    else if(word==""){
        return true;
    }
    else if (board[row][col]== toUpperCase(word)[0]){
        vec.add(integerToString(row)+integerToString(col));//choose to mark
        BoggleGUI::setHighlighted(row, col, true);//choose
        //explore
        //search for every part of board which is adjacent to the current part.
        if(humanwordSearchHelper(board, word.substr(1),row+1,col,vec)||humanwordSearchHelper(board, word.substr(1),row,col+1,vec)
                ||humanwordSearchHelper(board, word.substr(1),row-1,col,vec)||humanwordSearchHelper(board, word.substr(1),row+1,col+1,vec)
                ||humanwordSearchHelper(board, word.substr(1),row,col-1,vec)||humanwordSearchHelper(board, word.substr(1),row-1,col-1,vec)
                ||humanwordSearchHelper(board, word.substr(1),row+1,col-1,vec)||humanwordSearchHelper(board, word.substr(1),row-1,col+1,vec)){

            return true;}
        else{
            vec.removeValue(integerToString(row)+integerToString(col));//unchoose to unmark
            BoggleGUI::setHighlighted(row, col, false);//unchoose
            return false;
            }
    }
    return false;
}

//Functions main aim is to verify the validation of the word that player find.
//takes three parameters:the board that stores characters, the dictionary which used as a reference to decide if word is a valid word,and the word that entered by user.
bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    if (!dictionary.contains(word)||word.size()< BoggleGUI::MIN_WORD_LENGTH){
        return false;
    }
    Vector<string> vec;//used to mark the parts of board used.
    //uses two for loops to try every character on board that could be the starting character of word.
    for (int i=0;i<board.numRows();i++) {
        for (int j=0;j<board.numCols();j++) {
            if(humanwordSearchHelper(board,word,i,j,vec)){
                BoggleGUI::scorePointsHuman(points(word));
                return true;
            }
            BoggleGUI::clearHighlighting();
        }
    }

    return false;
}

//Functions main aim together with the computerWordSearch function is to find every possible valid word on board and then substract the ones that user find.
//Takes 7 parameters:The board used,the dictionary which is both used to prune the search tree and to validate the word, A set of words which contains the words found so far,
//A string str which is the part of the word so far,row and col to specify the part of board and a vector to mark the parts of board that are already used.

void computerWordSearchHelper(Grid<char>& board, Lexicon& dictionary, Set<string>& Words,string str,int row,int col,Vector<string>& vec){
    if(!vec.contains(integerToString(row)+integerToString(col))){
        if(dictionary.contains(str)&& str.size()>=4){//base case
            Words.add(str);
        }
        if (dictionary.containsPrefix(str)) {//used to prune the search tree.If dictionary does not contain the prefix of word then its neeedless to search.
            vec.add(integerToString(row)+integerToString(col));//choose
            //explore
            //explores every adjacent parts of board which can be the subsequent caharcter of word.
            for (int r=row-1;r<=row+1;r++) {
                for (int c=col-1;c<=col+1;c++) {
                    if(board.inBounds(r,c)){
                        computerWordSearchHelper(board,dictionary,Words,str+board[r][c],r, c,vec);
                    }
                }
            }
            vec.removeValue(integerToString(row)+integerToString(col));//unchoose
        }
    }
}

//Uses computerWordSearchHelper function.
//Takes three parameters:board which stores the caharacters, dictionary to verify the validation of word found and a set of humanWords to substract from the ones found by computer.
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    Set<string> words;
    //to search every possible starting point.
    for (int i=0;i<board.numRows();i++) {
        for (int j=0;j<board.numCols();j++) {
            Vector<string> vec;
            string str;
            computerWordSearchHelper(board,dictionary,words,str+board[i][j],i,j,vec);
        }
    }
    words=words-humanWords;//substract the ones human find.
    for (string word:words) {//give points to computer for each word the human can not find.
        BoggleGUI::scorePointsComputer(points(word));
    }
    return words;
}
