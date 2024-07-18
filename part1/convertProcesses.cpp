#include <iostream>
#include <string>
#include <utility>
#include <cassert>
#include <fstream>
#include <vector>

std::vector<std::string> convertProcesses(int p) {
    std::vector<std::string> pID;
    int counter=0;
    for (int i=65; i<=90; i++) {
        char letter = static_cast<char>(i);
        for (int j=0; i<=9; j++) {
            char number_char = '0'+j;
            std::string result;
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