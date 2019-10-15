#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>

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


/* this function returns a random word from the text file */
std::string get_word(int index){
    std::ifstream wordsfile("hard_words.txt");
    std::string word;
    for(int i = 0; i <= index; i ++){
        std::getline(wordsfile, word);
    }
    return word;
}

/* this function reads in the screen from a text file */
void read_screen(char words[HEIGHT][WIDTH]){
    std::ifstream screenfile("begin_screen.txt");
    std::string temp;
    int i = 0;
    while(std::getline(screenfile, temp)){
        strcpy(words[i],temp.c_str());
        i++;
    }
}

/* this function updates the window and tells ncurses to refresh */
void display(char screen[HEIGHT][WIDTH]){
    for(int i = 0; i < HEIGHT; i ++){
        for(int j = 0; j < WIDTH - 1; j ++){
            mvaddch(i, j, screen[i][j]);
        }
    }
    refresh();
}

/* this function sets up the screen and any vectors holding information */
void setup(char screen[HEIGHT][WIDTH], std::vector<letter> &letters, std::string word,
            std::vector<letter> &alphabet, std::vector<body_part> &body_parts){
    int length = word.size();
    int start = (WIDTH/2) - ((length * 2) - length);

    const char* wordarr = word.c_str();

    for(int i = 0; i < length; i ++){
        letters.push_back({start, wordarr[i], true});
        start = start + 2;
        screen[15][letters[i].x] = '_';
    }

    /* auto generate the alphabet into a vector */
    start = 14;
    for(int i = 65; i < 91; i ++){
        letter new_letter = {start, (char)i, false};
        alphabet.push_back(new_letter);
        screen[18][start] = alphabet[alphabet.size() - 1].letter;
        start = start + 2;
    }

    /* set up body parts vector */
    body_parts.push_back({41, 5, 'O'});
    body_parts.push_back({41, 6, '|'});
    body_parts.push_back({40, 6, '/'});
    body_parts.push_back({42, 6, '\\'});
    body_parts.push_back({40, 7, '/'});
    body_parts.push_back({42, 7, '\\'});
}

/* this function checks to see if a letter has been guessed, and if not,
    sets that letter to hidden 
    1 = checked before
    0 = not guessed before
    -1 = not in alphabet*/
int check_hidden_alphabet(std::vector<letter> &alphabet, char guess){
    for(int i = 0; i < alphabet.size(); i++){
        if(toupper(guess) == alphabet[i].letter){
            if(!alphabet[i].hidden){
                alphabet[i].hidden = true;
                return 0;   // has never been guessed
            }else{
                return 1;   // has been guessed before
            }
        }
    }
    return -1;  // not in alphabet
}

/* this function checks to see if a letter has been guess, then returns 
    whether or not it exists in the guessing word
    1 = found
    0 = not foud
    -1 = guessed previously or not in alphabet */
int reveal(std::vector<letter> &letters, char guess, std::vector<letter> &alphabet){
    /* check if already guessed and hide if not*/
    int checked = check_hidden_alphabet(alphabet, guess);

    if(checked == 0){
        int found = 0;
        for(int i = 0; i < letters.size(); i ++){
            //printf("guess: %c, letters[i]: %c\n", guess, letters[i].letter);
            if(guess == letters[i].letter){
                letters[i].hidden = false;
                found = 1;
            }
        }
        return found;
    }
    return -1;
}

void update(char screen[HEIGHT][WIDTH], std::vector<letter> &letters, char guess,
                std::vector<letter> &alphabet, std::vector<body_part> &body_parts,
                int* num_wrong){
    int found = reveal(letters, guess, alphabet);

    if(found == 0){
        *num_wrong = *num_wrong + 1;
    }
    if(*num_wrong > 5){
        return;
    }

    for(int i = 0; i < *num_wrong; i ++){
        screen[body_parts[i].y][body_parts[i].x] = body_parts[i].part;
    }

    for(int i = 0; i < letters.size(); i ++){
        if(!letters[i].hidden){
            screen[15][letters[i].x] = letters[i].letter;
        }else{
            screen[15][letters[i].x] = '_';
        }
    }  

    for(int i = 0; i < alphabet.size(); i ++){
        if(alphabet[i].hidden){
            screen[18][alphabet[i].x] = ' ';
        }else{
            screen[18][alphabet[i].x] = alphabet[i].letter;
        }
    }
}

bool check_win(std::vector<letter> &letters){
    for(int i = 0; i < letters.size(); i ++){
        if(letters[i].hidden){
            return false;
        }
    }
    return true;
}

int main(){
    initscr();
    noecho();
    curs_set(0);
    /* seed the random number generator*/
    srand(time(NULL));

    char screen[HEIGHT][WIDTH];

    /* read in the word and screen from the two text files */
    std::string word = get_word(rand() % 200);
    read_screen(screen);

    std::vector<letter> letters;
    std::vector<letter> alphabet;
    std::vector<body_part> body_parts;
    int num_wrong = 0;

    setup(screen, letters, word, alphabet, body_parts);
    display(screen);

    char guess;
    bool won = false;
    while(guess != 27 && num_wrong < 6 && won == false){
        guess = getch();
        update(screen, letters, guess, alphabet, body_parts, &num_wrong);
        display(screen);
        won = check_win(letters);
    }

    if(won){
        mvprintw(11, 30, "nice job");
    }else{
        std::string text = "You suck. The word was: " + word;
        mvprintw(11, 22, text.c_str());
    }
    getch();
    endwin();

    return 0;
}