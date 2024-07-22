#include <iostream>
#include <string>
#include <utility>
#include <cassert>
#include <fstream>
#include <vector>

using namespace std;

vector<string> convertProcesses(int p) {
    vector<string> pID;
    int counter=0;
    for (int i=65; i<=90; i++) {
        char letter = char(i);
        for (int j=0; j<=9; j++) {
            char number_char = '0'+j;
            string result;
            result+=letter;
            result+=number_char;
            pID.push_back(result);
            counter++;
            if (counter==p) {
                return pID;
            }
        }
    }
    return pID;
}