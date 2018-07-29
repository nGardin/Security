#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
using namespace std;

string removeDuplicate(string key){
    for (string::size_type i = 0; i < key.size(); i++ )
    {
        string::size_type j = i + 1;
        while ( j < key.size() ){
            if ( key[i] == key[j] ){
                key.erase( j, 1 );
            }
            else{++j;}
        }
    }
    return key;
};

vector<string> generateAlph(string key){
    vector<string> vecLines;        
    bool endOfAlph = false; 
    bool goodToAdd = true;
    vecLines.push_back(key);

    int currLetter = 65;        //ASCII 'A' start at 65 and 'Z' finishes at 90
    while(!endOfAlph){
        string newLine = "";     //generate an empty "new line" to add to
        while( !(newLine.length() >= key.length()) ){  
            char letter = char(currLetter);         //get the new ASCII letter as a character     
            for(int i=0; i<key.length(); i++){
                if(key[i] == letter){goodToAdd = false;}
            }
            if(goodToAdd){newLine += letter; currLetter++;}
            else{  currLetter++; }
            goodToAdd = true;
            if(currLetter == 91){endOfAlph = true; break;}         
        }
        vecLines.push_back(newLine);                    //push the new line to our vector
    }
    //fill last line with zeros for place holders
    if(vecLines[vecLines.size()-1].length() != key.length()){
        bool full = false;
        while(!full){
            vecLines[vecLines.size()-1] += 'a';
            if( (vecLines[vecLines.size()-1]).length() == key.length()){
                full = true;
            }
        }
    }
    //TESTING
    // for(int i=0; i<vecLines.size(); i++){       //print the lines
    //     cout << vecLines[i] << endl;
    // }
    // cout << endl << endl << endl << endl;
    return vecLines;
};

vector<string> sortAlph(vector<string> curAlph){
    string key = curAlph[0];

    //bubble sort (not optimal but it works for the time being)
    for(int i=0; i<key.length(); i++){
        for(int j=0; j<key.length()-i-1; j++){

            //first sort the key
            if(key[j] > key[j+1]){
                char temp = key[j+1];
                key[j+1] = key[j];
                key[j] = temp;
                curAlph[0] = key;

                //then sort the rest of the rows - depending on the sort of the key
                for(int k=1; k<curAlph.size(); k++){
                    string line = curAlph[k];
                    char temp = line[j+1];
                    line[j+1] = line[j];
                    line[j] = temp; 
                    curAlph[k] = line;
                }
            }
        }
    }
    //TESTING
    // for(int i=0; i<curAlph.size(); i++){       //print the lines
    //     cout << curAlph[i] << endl;
    // }
    // cout << endl << endl << endl << endl;
    return curAlph;
};

vector<string> rowToColumn(vector<string> sortedAlph){
    vector<string> encRowsVec;

    for(int i=0; i<sortedAlph[0].length(); i++){
        string newLine = "";
        for(int j=0; j<sortedAlph.size(); j++){
            newLine += sortedAlph[j][i];
        }
        encRowsVec.push_back(newLine);
    }
    //TESTING
    // for(int i=0; i<encRowsVec.size(); i++){       //print the lines
    //     cout << encRowsVec[i] << endl;
    // }
    // cout << endl << endl << endl << endl;
    return encRowsVec;  
};

void determineShift(vector<string> finalForm, string secretMsg){
    string str = finalForm[0];
    map<char,int> mapAlph;
    
    //Create a map, with the key representing each encrypted value 
    //that will display its real value
    int x = 0;
    for(int i=0; i<finalForm.size(); i++){
        for(int j=0; j<finalForm[0].length(); j++){
            char letter = char(finalForm[i][j]);
            if(letter != 'a'){
                mapAlph[letter] += (x+65);
                x++;
            }
        }
    }
    for(int i=0; i<secretMsg.length(); i++){
        char letter = char(secretMsg[i]);
        if(letter == ' '){
            cout << " ";
        }
        else{
            cout << char(mapAlph[letter]);
        }
    }
    cout << endl;
};


int main() {
    int n; 
    string key;
    string secretMessage;

    vector<string> encryptedAlph;
    vector<string> sortedAlph;
    vector<string> finalForm;

    cin >> n;
    while(n--){
        cin >> key;

        string newKey = removeDuplicate(key);
        encryptedAlph = generateAlph(newKey);
        sortedAlph = sortAlph(encryptedAlph);
        finalForm = rowToColumn(sortedAlph);

        cin.ignore();
        getline(cin, secretMessage);
        determineShift(finalForm, secretMessage);
    }
    return 0;
}