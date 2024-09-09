#include <iostream>
#include <fstream>
#include <signal.h>
#include <cstring>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

/*
 - TODO
 * make them refrence as much as possible
 * write ShowRecords func
 * save score after exiting
 * show last record in game
 * func to check possibility of ending the game after anychange!
 * make all int to short int
 * func for star in table
*/

using namespace std;

// ENUMS
enum Color
{
    MAIN = 36, // It is green now & easy to change
    NONE = 0,
    GRAY = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    PURPLE = 35,
    CYAN = 36,
    WHITE = 37,
};

struct
{
    bool g_login_status = false;
    char g_username[100];
    char g_password[100];
    int score = 0;
    int max_score = 0;

} User;

struct
{
    int g_game_size = 31;
    int g_auth_size = 60;
    char g_path_users[100] = "users.txt";
    char g_auth_message[100] = "Hello & Welcome to the game";
    Color g_auth_message_color = Color::GREEN;
    int table[9][9] = {-1};

    int selectors[2][2] = {{-1, -1}, {-1, -1}}; // {i,j}
    int selector_checker = 0;                   // 0 first selector's turn | 1 second selector's turn

    char score_message[8] = "Score: ";
    int score_message_length = 8;
    char username_message[7] = "User: ";
    int username_message_length = 7;

    char board_info_message[30] = "'Z' or 'z' >>>> exit the game";
    int board_info_message_length = 30;

    char selector_error_message[100] = "";

} Game;

// CHAR
char vertical_line[1] = {'|'};

// FUNCTIONS
int Random(int start_range = 1, int end_range = 9)
{
    /*
        This function only returns a random number in entered range
        default range is => [1,9]

        e.g.
            start range = 6
            end range = 20

            diff = 14
            rand() % 14 => 0 - 13
            => + start range => 6 - 20
    */

    int random_number;
    random_number = (rand() % ((end_range - start_range) + 1)) + start_range;
    return random_number;
}

int AbsoluteValue(int number)
{
    if (number < 0)
        return -number;
    return number;
}

void MoveCursor(int lines, int columns)
{
    // This function helps to move cursor in terminal

    // for lines
    if (lines > 0)
        // goes up
        cout << "\033[" << lines << "A";
    else if (lines < 0)
        // goes down
        cout << "\033[" << lines << "B";

    // for columns
    if (columns > 0)
        // goes forward
        cout << "\033[" << columns << "C";
    else if (columns < 0)
        // goes backward
        cout << "\033[" << columns << "D";
}

void PositionCursor(int line, int columns)
{
    cout << "\033[" << line << ";" << columns << "f";
}

void MakeCinHidden(bool enable)
{
    /*
        This function makes program to cin in hidden way
        and inputs will be hidden

        true  => makes it hidden
        false => makes it visible

        should be called again after hiding with false parameter
    */

    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (enable)
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void ClearConsole()
{
    // just clears the terminal
    system("clear || cls");
}

void TerminateSignal(int s)
{
    /*
        This function only works when program is going to be terminated by Ctrl+C or Ctrl+Z
        and before terminating first we clear the console for make it clean because in some points
        we are changing cursor place in terminal and if terminating happens in this situation
        after terminating terminal will look so ugly /:
    */

    MakeCinHidden(false);
    ClearConsole();
    exit(0);
}

void PrintSpace(short int number)
{
    // just couts free spaces
    for (short int i = 0; i < number; i++)
        cout << ' ';
}

void CharArray(char result[], short int number, char character)
{
    // get a char array and full it with the defined character
    for (short int i = 0; i < number; i++)
        result[i] = character;

    result[number] = '\0';
}

void Print(char str[], Color color = NONE, bool new_line = false)
{
    /*
        just printing a text with a specific color (optional) & also endl (optional)
        the form is smth like this:
            \033[{codes}m{text}\33[0m

            codes are this:

                    foreground  background
            black        30         40
            red          31         41
            green        32         42
            yellow       33         43
            blue         34         44
            magenta      35         45
            cyan         36         46
            white        37         47
            bold/bright       1  (often a brighter shade of the same colour)

        but here we only use foregrounds not backgrounds

        color codes are saved in Color enum
    */

    cout << "\033[1;";
    if (color)
        cout << color;
    cout << "m" << str << "\033[0m";
    if (new_line)
        cout << endl;
}

void EmptyLine(short int counter = 1, short int line_length = Game.g_game_size)
{
    /*
        printing a line with enough spaces depending on size of game frame
    */

    for (short int i = 0; i < counter; i++)
    {
        Print(vertical_line, Color::MAIN);
        PrintSpace(line_length);
        Print(vertical_line, Color::MAIN, true);
    }
}

bool isFileEmpty(fstream &file)
{
    /*
        true  => file is empty
        false => file has some content
    */

    file.seekg(0, ios::end);
    short int length = file.tellg();
    file.seekg(0);

    if (length)
        return false;
    return true;
}

bool UsernamePasswordValidity()
{
    for (int i = 0; i < User.g_username[i]; i++)
    {
        int ascii = (int)User.g_username[i];

        if (!((65 <= ascii && ascii <= 90) || (97 <= ascii && ascii <= 122)))
        {
            strcpy(Game.g_auth_message, "Username should be only letters (aA-zZ)");
            Game.g_auth_message_color = Color::RED;
            return false;
        }
    }

    for (int i = 0; i < User.g_password[i]; i++)
    {
        int ascii = (int)User.g_password[i];

        if (!((65 <= ascii && ascii <= 90) || (97 <= ascii && ascii <= 122) || (48 <= ascii && ascii <= 57)))
        {
            strcpy(Game.g_auth_message, "Passwords should be contain letters & numbers");
            Game.g_auth_message_color = Color::RED;
            return false;
        }
    }

    return true;
}

void CheckUserInFile()
{

    /*
        checks username and password there are 3 scenarios:
            1. The username is there but password doesn't match so it's a login error (error)
            2. If the username and password are in the file, the user will be logged in (sign-in)
            3. The username is new so data will be saved and user will be logged in (sign-up)
    */

    fstream file;
    file.open(Game.g_path_users, ios::in | ios::out | ios::app);

    if (isFileEmpty(file))
    {
        file << User.g_username << vertical_line << User.g_password << vertical_line << 0 << vertical_line << "\n";
        User.g_login_status = true;
    }
    else
    {
        char line[200];
        while (file.getline(line, 200))
        {
            char tmp[3][100];
            short int index = 0;
            short int tmp_index = 0; // false: tmp_username | true: tmp_password

            for (short int i = 0; line[i]; i++)
            {
                if (line[i] != '|')
                    tmp[tmp_index][index++] = line[i];
                else
                {
                    tmp[tmp_index][index] = '\0';
                    tmp_index++;

                    index = 0;

                    if (tmp_index == 3)
                        break;
                }
            }

            if (strcmp(User.g_username, tmp[0]) == 0)
            {
                // the username is found

                // close stream connection it anyway
                file.close();

                if (strcmp(User.g_password, tmp[1]) == 0)
                {
                    // password matches
                    User.g_login_status = true;
                    User.max_score = ((tmp[2][0] - 48) * 10) + (tmp[2][1] - 48);
                }
                else
                {
                    // password doesn't match
                    strcpy(Game.g_auth_message, "Passowrd is wrong!");
                    Game.g_auth_message_color = Color::RED;
                }

                return;
            }
        }

        // it's a new username and should be saved
        file.clear();
        file << User.g_username << vertical_line << User.g_password << vertical_line << 0 << vertical_line << "\n";
        User.g_login_status = true;

        file.close();
    }
}

void Auth()
{
    /*
        responsible for getting username and password
        and sign-in or sign-up the user

        if user is already logged in nothing happens
    */

    while (User.g_login_status == false)
    {
        int g_auth_message_length = strlen(Game.g_auth_message);
        int spaces;

        char star_line[Game.g_auth_size + 2];
        CharArray(star_line, Game.g_auth_size + 2, '*');
        Print(star_line, Color::MAIN, true);

        EmptyLine(1, Game.g_auth_size);

        // top message
        Print(vertical_line, Color::MAIN);
        spaces = (Game.g_auth_size - g_auth_message_length) / 2;
        PrintSpace(spaces);
        Print(Game.g_auth_message, Game.g_auth_message_color);

        if (g_auth_message_length % 2)
            spaces++;

        PrintSpace(spaces);
        Print(vertical_line, Color::MAIN, true);

        // newline
        EmptyLine(1, Game.g_auth_size);

        char username_array[100] = "Enter username: ";
        char password_array[100] = "Enter password: ";

        // size of the largest text here is important so that both lines will be placed in one column
        int username_array_length = strlen(username_array);
        spaces = (Game.g_auth_size - username_array_length) / 3;

        // username line
        Print(vertical_line, Color::MAIN);
        PrintSpace(spaces);
        Print(username_array, Color::WHITE);

        PrintSpace(Game.g_auth_size - spaces - username_array_length);

        Print(vertical_line, Color::MAIN, true);

        // password line
        Print(vertical_line, Color::MAIN);
        PrintSpace(spaces);
        Print(password_array, Color::WHITE);

        PrintSpace(Game.g_auth_size - spaces - username_array_length);

        Print(vertical_line, Color::MAIN, true);

        EmptyLine(1, Game.g_auth_size);

        Print(star_line, Color::MAIN, true);

        // goes up two line
        // goes forward as long as username_string's length
        MoveCursor(4, strlen(username_array) + spaces + 1);

        // get username
        cin >> User.g_username;

        // goes forward again based on password_string's length
        MoveCursor(0, strlen(password_array) + spaces + 1);

        // enable invisibility
        MakeCinHidden(true);
        cin >> User.g_password;

        // disable it
        MakeCinHidden(false);

        // first checking username & password validity then checking for account
        if (UsernamePasswordValidity())
            CheckUserInFile();
        ClearConsole();
    }
}

void CreateTable()
{
    int numbers[81];

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            Game.table[i][j] = Random(1, 9);
        }
    }

    // star
    Game.table[Random(0, 8)][Random(0, 8)] = 10;

    Game.selectors[0][0] = 0;
    Game.selectors[0][1] = 0;
    Game.selector_checker = 0;
}

void GetScore(char result[2])
{
    if (User.score > 9)
    {
        result[0] = (User.score / 10) + 48;
        result[1] = (User.score % 10) + 48;
    }
    else
    {
        result[0] = '0';
        result[1] = User.score + 48;
    }
}

void ShowTable()
{
    char line_border[Game.g_game_size];
    CharArray(line_border, Game.g_game_size + 2, '*');
    Print(line_border, Color::MAIN, true);

    EmptyLine(1);

    int spaces = Game.g_game_size - (Game.score_message_length + Game.username_message_length + strlen(User.g_username) + 3); // 2 is for score & 3 is spaces between score and username

    Print("|", Color::MAIN);
    PrintSpace(spaces / 2);
    Print(Game.score_message, Color::GRAY);

    char score_array[2];
    GetScore(score_array);
    Print(score_array, Color::RED);

    PrintSpace(3);
    Print(Game.username_message, Color::GRAY);
    Print(User.g_username, Color::RED);

    if (spaces % 2)
        spaces++;

    PrintSpace(spaces / 2);
    Print("|", Color::MAIN, true);

    EmptyLine(1, 31);

    for (int i = 0; i < 9; i++)
    {
        Print("|  ", Color::MAIN);
        for (int j = 0; j < 9; j++)
        {
            int item = Game.table[i][j];
            char res[1];
            if (item != 10 && item)
                res[0] = item + 48;
            else if (item)
                res[0] = '*';
            else
                res[0] = 'X';
            res[1] = '\0';

            // first selector
            if (Game.selectors[0][0] == i && Game.selectors[0][1] == j)
            {
                Print("(", Color::PURPLE);
                Print(res, Color::YELLOW);
                Print(")", Color::PURPLE);
            }

            // second selector
            else if (Game.selectors[1][0] == i && Game.selectors[1][1] == j)
            {
                Print("(", Color::PURPLE);
                Print(res, Color::CYAN);
                Print(")", Color::PURPLE);
            }

            else
            {
                PrintSpace(1);
                Print(res, Color::WHITE);
                PrintSpace(1);
            }
        }

        Print("  |", Color::MAIN, true);
    }

    EmptyLine(1, 31);

    // message
    Print("|", Color::MAIN);
    PrintSpace(1);
    Print(Game.board_info_message, Color::BLUE);
    PrintSpace(1);
    Print("|", Color::MAIN, true);

    EmptyLine(1, 31);

    if (strcmp(Game.selector_error_message, "") != 0)
    {
        spaces = Game.g_game_size - strlen(Game.selector_error_message);

        Print("|", Color::MAIN);
        PrintSpace(spaces / 2);
        Print(Game.selector_error_message, Color::RED);

        if (spaces % 2)
            spaces++;

        PrintSpace(spaces / 2);
        Print("|", Color::MAIN, true);

        strcpy(Game.selector_error_message, "");
    }
    else
        EmptyLine(1, 31);
    EmptyLine(1, 31);

    CharArray(line_border, Game.g_game_size + 2, '*');
    Print(line_border, Color::MAIN, true);
}

char my_getch()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

int GetKeyboard()
{

    /*
        only returns:
            0 => error / exit
            1 => top
            2 => right
            3 => down
            4 => left
            5 => enter
            6 => records
    */

    char n = my_getch();

    if (n == 'w' || n == 'W')
        return 1;
    else if (n == 'd' || n == 'D')
        return 2;
    else if (n == 's' || n == 'S')
        return 3;
    else if (n == 'a' || n == 'A')
        return 4;
    else if (n == 10)
        return 5;
    else if (n == 'x' || n == 'X')
        return 6;
    else if (n == 'z' || n == 'Z')
    {
        ClearConsole();
        Print("Have a nice day!", Color::PURPLE, true);
        exit(0);
    }

    return 0;
}

void CheckTwoSelectedCells()
{
    int fi = Game.selectors[0][0];
    int fj = Game.selectors[0][1];
    int si = Game.selectors[1][0];
    int sj = Game.selectors[1][1];

    int diff_i = AbsoluteValue(fi - si);
    int diff_j = AbsoluteValue(fj - sj);

    if (Game.table[fi][fj] == 0 || Game.table[si][sj] == 0)
        strcpy(Game.selector_error_message, "WRONG CELL DETECTED!");
    else if (fi == si && fj == sj)
        strcpy(Game.selector_error_message, "SAME CELLS? KIDDING?");
    else if (diff_i == 1 && diff_j == 1)
        strcpy(Game.selector_error_message, "DIAGONAL CELLS!?");
    else if ((diff_i == 0 || diff_i == 1) && (diff_j == 0 || diff_j == 1))
    {

        if ((Game.table[fi][fj] == Game.table[si][sj]) || ((Game.table[fi][fj] + Game.table[si][sj]) == 10))
        {
            Game.table[fi][fj] = 0;
            Game.table[si][sj] = 0;
            User.score++;
        }
        else
            strcpy(Game.selector_error_message, "NO CHANCE FOR SELECTED CELLS!");
    }
    else
    {
        // far

        if (diff_i == 0 && diff_j > 1)
        {
            bool checker = true;
            int first_tmp, second_tmp;

            if (fj > sj)
            {
                first_tmp = sj + 1;
                second_tmp = fj;
            }
            else
            {
                first_tmp = fj + 1;
                second_tmp = sj;
            }

            for (int jcell = first_tmp; jcell < second_tmp; jcell++)
            {
                if (Game.table[fi][jcell] != 0)
                {
                    checker = false;
                    break;
                }
            }

            if (checker && ((Game.table[fi][fj] == Game.table[si][sj]) || ((Game.table[fi][fj] + Game.table[si][sj]) == 10)))
            {
                Game.table[fi][fj] = 0;
                Game.table[si][sj] = 0;
                User.score++;
            }
            else
                strcpy(Game.selector_error_message, "NO CHANCE FOR SELECTED CELLS!");
        }

        else if (diff_j == 0 && diff_i > 1)
        {
            bool checker = true;
            int first_tmp, second_tmp;

            if (fi > si)
            {
                first_tmp = si + 1;
                second_tmp = fi;
            }
            else
            {
                first_tmp = fi + 1;
                second_tmp = si;
            }

            for (int icell = first_tmp; icell < second_tmp; icell++)
            {
                if (Game.table[icell][fj] != 0)
                {
                    checker = false;
                    break;
                }
            }

            if (checker && ((Game.table[fi][fj] == Game.table[si][sj]) || ((Game.table[fi][fj] + Game.table[si][sj]) == 10)))
            {
                Game.table[fi][fj] = 0;
                Game.table[si][sj] = 0;
                User.score++;
            }
            else
                strcpy(Game.selector_error_message, "NO CHANCE FOR SELECTED CELLS!");
        }

        else
            strcpy(Game.selector_error_message, "NO CHANCE FOR SELECTED CELLS!");
    }
};

void ShowRecords()
{
}

void Movement()
{
    int movement = GetKeyboard();
    if (movement)
    {
        if (movement == 1 && Game.selectors[Game.selector_checker][0] != 0) // goes up
            Game.selectors[Game.selector_checker][0]--;
        else if (movement == 2 && Game.selectors[Game.selector_checker][1] != 8)
            Game.selectors[Game.selector_checker][1]++;
        else if (movement == 3 && Game.selectors[Game.selector_checker][0] != 8)
            Game.selectors[Game.selector_checker][0]++;
        else if (movement == 4 && Game.selectors[Game.selector_checker][1] != 0)
            Game.selectors[Game.selector_checker][1]--;
        else if (movement == 5)
        {
            if (Game.selector_checker == 1)
            {
                CheckTwoSelectedCells();

                Game.selector_checker = 0;

                Game.selectors[0][0] = 0;
                Game.selectors[0][1] = 0;

                Game.selectors[1][0] = -1;
                Game.selectors[1][1] = -1;
            }
            else
            {
                Game.selector_checker++;

                if (Game.selectors[0][0] != 0 && Game.selectors[0][1] != 0)
                {
                    Game.selectors[1][0] = 0;
                    Game.selectors[1][1] = 0;
                }
                else
                {
                    Game.selectors[1][0] = 0;
                    Game.selectors[1][1] = 1;
                }
            }
        }

        else if (movement == 6)
        {
            // show records table
            ClearConsole();
            ShowRecords();
            my_getch();
        }

        else
            strcpy(Game.selector_error_message, "WANNA GO OUT OF MATRIX?");
    }
    else
        strcpy(Game.selector_error_message, "ENTER A CORRECT MOVEMENT!");
}

void Updater()
{
    ClearConsole();
    Auth();
    ShowTable();
    Movement();
    Updater();
}

int main()
{
    // do it once to create random numbers at the same time without any problem
    srand((unsigned)time(0));

    /*
        Since we sometimes change the cursor in the program, if we decide to terminate it while the program is running
        the appearance of terminal is not gonna be interesting after doing it, so we use these signals to handle it
        and clear the terminal before terminating program
    */
    signal(SIGINT, TerminateSignal);
    signal(SIGTSTP, TerminateSignal);

    CreateTable();
    Updater();

    return 0;
}
