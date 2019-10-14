#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <iostream>

struct body_part{
    int x;
    int y;
    char part;
};

struct letter{
    int x;
    char letter;
    bool hidden;
};

const int WIDTH                     = 80;
const int HEIGHT                    = 21;
const body_part HEAD                = {41, 5, 'O'};
const body_part TORSO               = {41, 6, '|'};
const body_part LEFT_ARM            = {40, 6, '/'};
const body_part RIGHT_ARM           = {42, 6, '\\'};
const body_part LEFT_LEG            = {40, 7, '/'};
const body_part RIGHT_LEG           = {42, 7, '\\'};       

std::string get_word(int index){
    std::ifstream wordsfile("hard_words.txt");
    std::string word;
    for(int i = 0; i <= index; i ++){
        std::getline(wordsfile, word);
    }
    return word;
}

void read_screen(char words[HEIGHT][WIDTH]){
    std::ifstream screenfile("begin_screen.txt");
    std::string temp;
    int i = 0;
    while(std::getline(screenfile, temp)){
        strcpy(words[i],temp.c_str());
        i++;
    }
}

void display(char screen[HEIGHT][WIDTH]){
    for(int i = 0; i < HEIGHT; i ++){
        for(int j = 0; j < WIDTH; j ++){
            printf("%c", screen[i][j]);
        }
        printf("\n");
    }
}

void setup(char screen[HEIGHT][WIDTH], std::vector<letter> letters, std::string word){
    int length = word.size();
    int start = (length * 2) - length + 40;

    const char* wordarr = word.c_str();

    for(int i = 0; i < length; i ++){
        letters.push_back({})
    }
}

int find(std::vector<letter> letters, char guess){
    return 0;
}

void take_turn(char screen[HEIGHT][WIDTH], std::vector<letter> letters, char guess){

}


int main(){
    /* seed the random number generator*/
    srand(time(NULL));

    char screen[HEIGHT][WIDTH];

    /* read in the word and screen from the two text files */
    std::string word = get_word(rand() % 200);
    read_screen(screen);

    std::vector<letter> letters;

    setup(screen, letters, word);


    return 0;
}