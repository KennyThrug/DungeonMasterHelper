#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <unistd.h>
#include <sstream>
#include <stdio.h>

#define LengthOfHalfRow 50

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
    void addToRow(int rowNumber, std::vector<std::string> strings){
        for(int i = 0; i < strings.size(); i++){
            addToRow(rowNumber,strings[i]);
        }
    }
    /**
     * @brief Outputs all rows of current string. Must be added to row variable first using one of the many functions in this
     * 
     */
    void outputStrings(){
        std::cout << "\x1B[2J\x1B[H";
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
    void clear(){
        output[0].clear();
        output[1].clear();
    }
};

class DateTime{
    public:
    int Year = 0;
    int Month = 0;
    int Day = 0;
    int Hour = 0;
    int Minute = 0;
    int Second = 0;
    void copyTime(DateTime* tim){
        this->Year = tim->Year;
        this->Month = tim->Month;
        this->Day = tim->Day;
        this->Hour = tim->Hour;
        this->Minute = tim->Minute;
        this->Second = tim->Second;
    }
    void fixTime(){
       if(Second >= 60){
           Minute++;
           Second -= 60;
       }
       else if(Second < 0){
           Minute--;
           Second += 60;
       }
       if(Minute >= 60){
           Hour++;
           Minute -= 60;
       }
       else if(Minute < 0){
           Hour--;
           Minute += 60;
       }
       if(Hour >= 24){
           Day++;
           Hour -= 24;
       }
       else if(Hour < 0){
           Day--;
           Hour += 24;
       }
       if(Day > 28){
            if(Month % 2 == 0){
                if(Month == 2){
                    Month++;
                    Day -= 28;
                }
                else{
                    if(Month == 8){
                        if(Day > 31){
                            Month++;
                            Day -= 31;
                        }
                    }
                    else if(Day > 30){
                        Month++;
                        Day -= 30;
                    }
                }
            }
            else{
                if(Month == 8 && Day >= 31){
                    Month++;
                    Day -= 31;
                }
                Month++;
                Day -= 28;
           }
       }
       else if(Day <= 0){
           if(Month % 2 == 1){
               if(Month == 3){
                   Month--;
                   Day = 28;
               }
               else if(Month == 9){
                   Month--;
                   Day = 31;
               }
               else{
                   Month--;
                   Day = 30;
               }
           }
           else{
               Month--;
               Day = 31;
           }
       }
       else if(Day < 0){
           Month--;
           Day += 28;
       }
       if(Month > 12){
           Year++;
           Month-= 12;
       }
       else if(Month <= 0){
           Year--;
           Month += 12;
       }
    }

};

class data{
public:
    struct Initiative{
        std::string name;
        int initiative;
        int health;
        int KAC;
        int EAC;
        bool isPlayer;
    };

    DateTime curTime;
    DateTime lastLongRest;
    std::vector<Initiative> initiativeList;
    TwoRows rows = TwoRows();
    
    std::string getInput(){
        std::string x;
        std::cin >> x;
        return x;
    }
    int getNumInput(){
        int x;
        std::cin >> x;
        return x;
    }
    void addInitiativeToRow(int row){
        rows.addToRow(row,"Initiative:");
        for(int i = 0; i < initiativeList.size(); i++){
            std::string name = "";
            if(initiativeList[i].isPlayer){
                name = "\033[34m";
            }
            name += std::to_string(initiativeList.at(i).initiative) + "-->" + initiativeList[i].name + "\033[0m";
            rows.addToRow(row,name);
        }
    }

    void addTimeToRow(int row){
        curTime.fixTime();
        rows.addToRow(row,"Date:");
        rows.addToRow(row,std::to_string(curTime.Month) + "/" + std::to_string(curTime.Day) + "/" + std::to_string(curTime.Year));
        rows.addToRow(row,std::to_string(curTime.Hour) + ":" + std::to_string(curTime.Minute) + ":" + std::to_string(curTime.Second));
    }

    void addCharacterToInitiative(){
        rows.clear();
        std::string x = questionare("Name: (Type [Exit] to Leave");
        if(x == "Exit"){
            runCombat();
        }
        else{
            Initiative init;
            init.name = x;
            rows.addToRow(0,"1.) Is a Player");
            if(questionareInt("2.) Is an NPC") == 1){ //IsPlayer
                init.isPlayer = true;
                init.initiative = questionareInt("Initiative?");
            }
            else{ //Is Not Player
                init.isPlayer = false;
                init.initiative = questionareInt("Initiative?");
                init.KAC = questionareInt("KAC?");
                init.EAC = questionareInt("EAC?");
                init.health = questionareInt("Health?");
            }
            initiativeList.push_back(init);
            addCharacterToInitiative();
        }
    }

    void runCombat(){
        rows.clear();
        rows.addToRow(0,std::vector<std::string>({
            "1.) Next Turn",
            "2.) Previous Turn",
            "3.) Add To initiative",
            "4.) Add Effect",
            "5.) Return"
        }));
        addInitiativeToRow(1);
        rows.outputStrings();
        addTimeToRow(1);
        int x = getNumInput();
        switch(x){
        case 3:
            addCharacterToInitiative();
            break;
        case 5:
            showDefault();
            break;
        default:
            runCombat();
            break;
        }
    }
    void clearInitiative(){
        initiativeList.clear();
    }

//Everything Below Sets Messages

    std::string questionare(std::string question){
        rows.addToRow(0,question);
        rows.outputStrings();
        std::string x = getInput();
        rows.addToRow(1,x);
        rows.outputStrings();
        return x;
    }
    
    int questionareInt(std::string question){
        rows.addToRow(0,question);
        rows.outputStrings();
        int x = getNumInput();
        rows.addToRow(1,std::to_string(x));
        rows.outputStrings();
        return x;
    }

    void addTime(){
        rows.clear();
        rows.addToRow(0,std::vector<std::string>({
        "1.) Seconds",
        "2.) Minutes",
        "3.) Hours",
        "4.) Days",
        }));
        addTimeToRow(1);
        rows.outputStrings();
        int x = getNumInput();
        rows.clear();
        switch (x)
        {
        case (1):
            curTime.Second += questionareInt("How Many Seconds?");
            break;
        case (2):
            curTime.Minute += questionareInt("How Many Minutes?");
            break;
        case (3):
            curTime.Hour += questionareInt("How Many Hours?");
            break;
        case (4):
            curTime.Day += questionareInt("How Many Days?");
        default:
            break;
        }
        showDefault();
    }

    //Message that Sets Time
    void setTime(){
        rows.clear();
        curTime.Year = questionareInt("Year?");
        curTime.Month = questionareInt("Month?");
        curTime.Day = questionareInt("Day");
        curTime.Hour = questionareInt("Hour?");
        curTime.Minute = questionareInt("Minute?");
        curTime.Second = questionareInt("Second?");
        showDefault();
    }
    //Shows first message
    void showDefault(){
        rows.clear();
        rows.addToRow(0,std::vector<std::string>({
        "1.) Run Combat",
        "2.) Travel Distance",
        "3.) Long Rest",
        "4.) Add Time",
        "5.) 10 minutes",
        "6.) Set Time",
        "7.) Fix Time",
        "8.) Clear Initiative"}));

        addInitiativeToRow(1);
        addTimeToRow(1);
        rows.outputStrings();
        int x = getNumInput();
        switch (x){
        case 1:
            runCombat();
            break;
        case 4:
            addTime();
            break;
        case 5:
            curTime.Minute += 10;
            curTime.fixTime();
        case 6:
            setTime();
            break;
        case 7:
            curTime.fixTime();
            break;
        case 8:
            clearInitiative();
            break;
        default:
            showDefault();
            break;
        };
        rows.outputStrings();
    }
};



int main(){
    data dt;
    dt.showDefault();
}