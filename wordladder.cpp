

#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "filelib.h"
#include <fstream>
#include "stack.h"
#include "queue.h"
#include "strlib.h"
#include "simpio.h"
using namespace std;

void printBackwards(Stack<string> oldstack);
bool upgradeQueue(Queue<Stack<string> >& queue, const Lexicon& lexicon, string word1,string word2,Set<string>& alreadyused );
void word1ToWord2(string word1, string word2,const Lexicon& lexicon,Queue< Stack<string> >& queue,Stack<string>& stack1);
void menu(string word1,string word2,const Lexicon& lexicon);
void intro();

int main() {
    intro();
    ifstream infile;
    promptUserForFile(infile, "Dictionary file name:");
    cout<<endl;
    Lexicon lexicon(infile);
    string word1;
    string word2;
    menu(word1,word2,lexicon);
    cout << "Have a nice day." << endl;
    return 0;
}

//This function prints intro.
void intro(){
    cout<<"Welcome to CS 106B/X Word Ladder!"<<endl<<
    "Give me two English words, and I will change the first"<<endl<<
    "into the second by changing one letter at a time."<<endl<<endl;
}

//Functions main aim is to print the stack(ladder) properly.
//takes one parameter which is the stack contains the ladder.
void printBackwards(Stack<string> oldstack){
    int size=oldstack.size();
    for (int i = 0; i <size ; i++) {
        cout << oldstack.pop() << " ";
    }
    cout<<endl<<endl;
}

//Function takes 5 parameters which are: queue used to store stacks,lexicon functions as dictionary,
//our first and second words respectively word1 and word2 and a set which functions as storing already used words to prevent infinite loops.
//functions main aim is to get the ladder and it upgrades queue several times to reach that aim.
bool upgradeQueue(Queue<Stack<string> >& queue, const Lexicon& lexicon, string word1,string word2,Set<string>& alreadyused ){
    for (int i=0;i< word1.length();i++) {
        for (char ch = 'a'; ch <= 'z'; ch++) {
            Stack<string> oldstack;
            oldstack=queue.peek();
            string copy= oldstack.peek();
            copy[i]=ch;
            if(lexicon.contains(copy) && !(alreadyused.contains(copy))){
                alreadyused.add(copy);
                oldstack.push(copy);
                if(copy==word2){
                    cout<<"A ladder from "<< word2<<" back to "<<word1<<":"<<endl;
                    printBackwards(oldstack);
                    return true;
                }
                queue.enqueue(oldstack);
            }
        }
    }
    queue.dequeue();
    return false;
}

//this function uses the already defined function upgradeQueue and some of the parameters it use are for that function.
//It also uses a parameter stack1 which helpes function to initialize first stack of queue and queue itself.
//Functions main aim is to display the ladder and appropriate error messages if necessary by using upgradeQueue function and if/else statements.
void word1ToWord2(string word1, string word2,const Lexicon& lexicon,Queue< Stack<string> >& queue,Stack<string>& stack1){
    stack1.push(word1);
    queue.enqueue(stack1);
    Set<string> alreadyused;
    alreadyused.add(word1);
    while (!queue.isEmpty()){
        if (upgradeQueue(queue,lexicon,word1,word2,alreadyused)){
            break;
        }
    }
    if(queue.isEmpty() && word1==word2){
        cout<<"The two words must be different."<<endl<<endl;
    }
    else if(queue.isEmpty() && (!lexicon.contains(word1) || !lexicon.contains(word2))){
        cout<<"The two words must be found in the dictionary."<<endl<<endl;
    }
    else if(queue.isEmpty()) {
        cout<<"No word ladder found from "<<word2<< " back to "<<word1<<"."<<endl<<endl;
    }
}

//Functions main aim is to create a menu for the user which takes inputs and by these inputs it reaches the main aim of the project.
//It uses word1ToWord2 function and three parameters which are the lexicon that keeps dictionary,first and second words.
void menu(string word1,string word2,const Lexicon& lexicon){
    word1= toLowerCase(getLine("Word 1 (or Enter to quit):"));
    if (word1!=""){
        word2= toLowerCase(getLine("Word 2 (or Enter to quit):"));
    }
    while(word1!=""&& word2!=""){
        if(word1.length()!=word2.length()){
            cout<<"The two words must be the same length."<<endl<<endl;
            word1= toLowerCase(getLine("Word 1 (or Enter to quit):"));
            if(word1==""){
                break;
            }
            word2= toLowerCase(getLine("Word 2 (or Enter to quit):"));
            if (word2==""){
                break;
            }
        }
        Queue< Stack<string> > queue;
        Stack<string> stack1;
        word1ToWord2(word1,word2,lexicon,queue,stack1);
        word1= toLowerCase(getLine("Word 1 (or Enter to quit):"));
        if(word1==""){
            break;
        }
        word2= toLowerCase(getLine("Word 2 (or Enter to quit):"));
        if (word2==""){
            break;
        }
    }
}
