#include <iostream>
#include <string>
#include <vector>
#include <array>

#define LengthOfHalfRow 30

class TwoRows{
    std::array<std::vector<std::string>,2> output;
public:
    //If string is longer than HalfRow, then will cut it up on different rows
    void addToRow(int rowNumber, std::string str){
        if(str.size() > LengthOfHalfRow){
            (output[rowNumber]).push_back(str.substr(0,LengthOfHalfRow));
            addToRow(rowNumber,str.substr(LengthOfHalfRow,str.size()-LengthOfHalfRow));
        }
        else{
            output[rowNumber].push_back(str);
        }
    }
    void outputStrings(){
        int numRows = (output[0].size() > output[1].size()) ? output[0].size() : output[1].size();
        //Number of Characters this row
        int numChars = 0;
        for(int i = 0; i < numRows; i++){
            if(i < output[0].size()){
                std::cout << output[0][i];
                numChars += output[0][i].size();
            }
            if(i < output[1].size()){
                for(int j = numChars; j < LengthOfHalfRow; j++){
                    std::cout << " ";
                }
                std::cout << " | ";
                std::cout << output[1][i];
            }
            std::cout << std::endl;
            numChars = 0;
        }
    }
};


int main(){
    TwoRows rows = TwoRows();
    rows.addToRow(0,"Hello World");
    rows.addToRow(0,"Test Statement");
    rows.addToRow(1,"I have no clue");
    rows.addToRow(1,"This should be too long for the thing to handle and be put into multiple lines");
    rows.outputStrings();
}