
#include "map.h"
#include "grammarsolver.h"
#include "vector.h"
#include <fstream>
#include "filelib.h"
#include <string>
#include "strlib.h"
#include "random.h"
#include "error.h"
using namespace std;

//Function takes a map as a parameter which is used to store data and a file which is taken as the reference of grammer.
//Main aim of function is to map symbols to the rules.
void dataStorage(Map<string,Vector<string> >& map,istream& infile){
    string line;
    while(getline(infile, line)) {
        Vector<string> v = stringSplit(line, "::=");
        if(map.containsKey(v[0])){throw "Illegal file format";}
        map[v[0]]=stringSplit(v[1], "|");
    }
}

//The main aim of this function together with the grammerGenerate function is to randomly choose rules from the map and implementing them.
//This function is where the recursion is used.
//It is used with the help of a for-each loop which helps implementing rules by choosing each word in them
void grammerGenerateHelper(const Map<string,Vector<string> >& map, string symbol,string& str){
    if(!map.containsKey(symbol)){
        str+=symbol;
        str+=" ";
    }
    else{
        int size= map[symbol].size();
        string str1=trim(map[symbol][randomInteger(0,size-1)]);
        Vector<string> v = stringSplit(str1, " ");
        for(string element:v) {
            grammerGenerateHelper(map,element,str);

        }
    }
}

//Function is the main part which cretes random lines via the rules of the symbol.
//Creates "times" times the an occurence of the symbol.
//other parameter is input which is our reference to create map.
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if(symbol==""){throw "Symbol can not be an empty string";}
    Vector<string> v;
    Map<string,Vector<string> > map;
    dataStorage(map,input);
    for (int i=0;i<times;i++) {
        string str;
        grammerGenerateHelper(map,symbol,str);
        v.add(str);
    }
    return v;
}


