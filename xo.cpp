#include <iostream>
#include <fstream>

using namespace std;

// global variables
string path = "db.txt";

// for knowing who's first next round
short int next_round_turn;

// file variables
short int global_status = 0;
short int global_rounds[2];
short int global_records[2];
string global_records_array[10];
string global_table[9];
string global_player1[2];
string global_player2[2];

// info data for showing a message in table
string board_info = "";
short int board_info_lines = 0;

// error data for showing errors in table
string board_error = "";
short int board_error_lines = 0;

// working with reply inputs with start command
string tmp;

// GUI size
short int linelength = 70;

// enums
enum Color
{
    MAIN = 32, // It is green now & easy to change
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

enum Message
{
    STARTING,
    HELP,
    COMMAND_ERROR,
    COMMANDS,
};

// declaration
string GlobalTexts(Message msg);

void ClearConsole();

void PrintSpace(short int number);
void PrintText(string str, Color color, bool new_line);
void PrintXO(string str);
void PrintEmptyLine(short int counter);

void TextSplitter(string message, string results[], char seprator);

void StatusUpdater();
void Updater(bool relpy_to_command, bool variables_to_file, bool file_to_variables);
void UpdateTopHeader();
void UpdateMainBoard();
void UpdateBottomHeader(string text, short int parts, Color color, bool newline);

bool CommandChecker(string &command);
void CommandHandler(string &command);
void ErrorHandler();

void FileToVariablesUpdater();
void VariablesToFileUpdater();

bool InputCell();
short int GameChecker();

// main func
int main()
{
    StatusUpdater();
    Updater(false, false, false);
    return 0;
}

// definition
string GlobalTexts(Message msg)
{
    // to access texts in every place in program

    string str;
    switch (msg)
    {
    case Message::STARTING:
        str = "Hello and Welcome to the game|Seems like this is your first time here, wanna see how to play?|Enter 'help' for more information or just simply 'start' the game";
        break;

    case Message::HELP:
        str = "'start'   => the game will be started                            |'reset'   => all data will be deleted & game starts from scratch |'records' => you can see all last 10 saved records (if available)";
        break;

    case Message::COMMAND_ERROR:
        str = " is not a valid command!";
        break;

    case Message::COMMANDS:
        str = " __________Commands___________ $|start|records|help|reset|exit|$ ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ ";
    }
    return str;
}

void ClearConsole()
{
    // just clears the terminal
    system("clear || cls");
}

void PrintSpace(short int number)
{
    // just couts free spaces
    cout << string(number, ' ');
}

void PrintText(string str, Color color = NONE, bool new_line = false)
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

        color codes are saved in
    */

    cout << "\033[1;";
    if (color)
        cout << color;
    cout << "m" << str << "\033[0m";
    if (new_line)
        cout << endl;
}

void PrintXO(string str)
{
    /*
        this function is only used when we wanna show the table
        and we want the color of each user's symbol to be equal to the user's default color

        player1 => blue & ✘
        player2 => red & ◯
    */

    if (str == "✘")
        PrintText(str, Color::BLUE);
    else if (str == "◯")
        PrintText(str, Color::RED);
    else
        PrintText(str, Color::WHITE);
}

void PrintEmptyLine(short int counter = 1)
{
    /*
        just printing a line in game framework
        | { enough spaces depend on size of game } |
    */
    for (short int i = 0; i < counter; i++)
    {
        PrintText("|", Color::MAIN);
        PrintText(string(linelength, ' '));
        PrintText("|", Color::MAIN, true);
    }
}

void TextSplitter(string message, string results[], char seprator = '|')
{

    /*
        `message` is the whole thing that is gonna be shown
        and `seprator` is that specific character between message which helps to seprate parts
        and let us show the message in multiple lines

        e.g.
            Hello & welcome to the game|This is really fun|YOHOOO

            line 1 => Hello & welcome to the game
            line 2 => This is really fun
            line 3 => YOHOOO
    */

    short int index = 0;
    string line = "";

    /*
        for last part we add seprator for ease of seprating last part without more conditions
        bcs this code only saves the part which is ended by the seprator

        e.g:
            if line is "hello|goodbye" => it will be "hello|goodbye|"
    */
    message += seprator;

    for (short int i = 0; i < message.length(); i++)
    {
        if (message[i] != seprator)
            line += message[i];
        else
        {
            results[index] = line;
            index++;
            line = "";
        }
    }
}

void UpdateTopHeader()
{
    string header_text = "XO Game | Made by ";
    string heart_text = "♥️";

    short int header_text_length = header_text.length() + 1; // 1 is heart length
    short int spaces = (linelength - header_text_length) / 2;

    PrintText(string(linelength + 2, '*'), Color::MAIN, true);
    PrintText("|", Color::MAIN);
    PrintSpace(spaces);
    PrintText(header_text, Color::WHITE);
    PrintText(heart_text, Color::RED);

    // if it was odd we add another space
    if ((linelength - header_text_length) % 2)
        spaces++;

    PrintSpace(spaces);
    PrintText("|", Color::MAIN, true);
    PrintText(string(linelength + 2, '*'), Color::MAIN, true);
    PrintEmptyLine();
}

void UpdateMainBoard()
{
    // main board's data are from globla variables and not file
    // global variables and file are synced

    // table here means the XO game table not the big table
    short int table_length = 13;

    // the distance of the left vertical line to the game table
    short int left_spaces = 5;

    // ------------line 1------------
    PrintText("|", Color::MAIN);
    PrintSpace(left_spaces);
    PrintText(".───────────.", Color::PURPLE);
    string round = "Rounds => " + to_string(global_rounds[1]) + "/" + to_string(global_rounds[0]);

    short int round_length = round.length();
    short int spaces = linelength - (table_length + left_spaces + round_length);

    PrintSpace(spaces / 2);
    PrintText(round, Color::YELLOW);
    if (spaces % 2)
        spaces++;
    PrintSpace(spaces / 2);
    PrintText("|", Color::MAIN, true);
    //----------end line 1----------

    // ------------line 2------------
    PrintText("|", Color::MAIN);
    PrintSpace(left_spaces);
    PrintText("│ ", Color::PURPLE);
    PrintXO(global_table[0]);
    PrintText(" │ ", Color::PURPLE);
    PrintXO(global_table[1]);
    PrintText(" │ ", Color::PURPLE);
    PrintXO(global_table[2]);
    PrintText(" │", Color::PURPLE);

    spaces = linelength - (table_length + left_spaces);
    if (spaces % 2)
        spaces++;
    PrintSpace(spaces);
    PrintText("|", Color::MAIN, true);
    //----------end line 2----------

    // ------------line 3------------
    PrintText("|", Color::MAIN);
    PrintSpace(left_spaces);
    PrintText("│───────────│", Color::PURPLE);

    string player1 = global_player1[0] + " (" + global_player1[1] + ")";
    string versus = " ✦✦✦ "; // length = 5
    string player2 = global_player2[0] + " (" + global_player2[1] + ")";
    short int players_length = player1.length() + player2.length() + 5;
    spaces = linelength - (table_length + left_spaces + players_length);
    PrintSpace(spaces / 2);
    PrintText(player1, Color::BLUE);
    PrintText(versus, Color::WHITE);
    PrintText(player2, Color::RED);
    if (spaces % 2)
        spaces++;
    PrintSpace(spaces / 2);
    PrintText("|", Color::MAIN, true);
    //----------end line 3----------

    // ------------line 4------------
    PrintText("|", Color::MAIN);
    PrintSpace(left_spaces);
    PrintText("│ ", Color::PURPLE);
    PrintXO(global_table[3]);
    PrintText(" │ ", Color::PURPLE);
    PrintXO(global_table[4]);
    PrintText(" │ ", Color::PURPLE);
    PrintXO(global_table[5]);
    PrintText(" │", Color::PURPLE);

    spaces = linelength - (table_length + left_spaces);
    if (spaces % 2)
        spaces++;
    PrintSpace(spaces);
    PrintText("|", Color::MAIN, true);
    //----------end line 4----------

    // ------------line 5------------
    string commands[3];
    TextSplitter(GlobalTexts(Message::COMMANDS), commands, '$');
    short int commands_length = commands[0].length();

    PrintText("|", Color::MAIN);
    PrintSpace(left_spaces);
    PrintText("│───────────│", Color::PURPLE);
    spaces = linelength - (table_length + left_spaces + commands_length);
    PrintSpace(spaces / 2);
    PrintText(commands[0], Color::GRAY);
    if (spaces % 2)
        PrintSpace((spaces + 1) / 2);
    else
        PrintSpace(spaces / 2);
    PrintText("|", Color::MAIN, true);
    //----------end line 5----------

    // ------------line 6------------
    PrintText("|", Color::MAIN);
    PrintSpace(left_spaces);
    PrintText("│ ", Color::PURPLE);
    PrintXO(global_table[6]);
    PrintText(" │ ", Color::PURPLE);
    PrintXO(global_table[7]);
    PrintText(" │ ", Color::PURPLE);
    PrintXO(global_table[8]);
    PrintText(" │", Color::PURPLE);

    PrintSpace(spaces / 2);
    PrintText(commands[1], Color::GRAY);
    if (spaces % 2)
        PrintSpace((spaces + 1) / 2);
    else
        PrintSpace(spaces / 2);
    PrintText("|", Color::MAIN, true);
    //----------end line 6----------

    // ------------line 7------------
    PrintText("|", Color::MAIN);
    PrintSpace(left_spaces);
    PrintText("*───────────*", Color::PURPLE);
    PrintSpace(spaces / 2);
    PrintText(commands[2], Color::GRAY);
    if (spaces % 2)
        PrintSpace((spaces + 1) / 2);
    else
        PrintSpace(spaces / 2);
    PrintText("|", Color::MAIN, true);
    PrintText("|" + string(linelength, '_') + "|", Color::MAIN, true);
    //----------end line 7----------

    PrintEmptyLine();
}

// comments need:

void UpdateBottomHeader(string text = board_info, short int parts = board_info_lines, Color color = GRAY, bool newline = true)
{
    /*
        bottom header is for showing messages and talk with guys in game
    */
    if (parts)
    {
        string messages[parts] = {};

        if (parts == 1)
            messages[0] = text;
        else
            TextSplitter(text, messages);

        for (short int i = 0; i < parts; i++)
        {
            string message = messages[i];
            short int message_length = message.length();
            short int spaces = (linelength - message_length);
            PrintText("|", Color::MAIN);
            PrintSpace(spaces / 2);
            PrintText(message, color);
            if (spaces % 2)
                spaces++;
            PrintSpace(spaces / 2);
            PrintText("|", Color::MAIN, true);
        }

        if (newline)
            PrintEmptyLine();
    }
}

bool CommandChecker(string &command)
{
    /*
        just checks if input is a valid command
        exit command is checked in Updater
    */

    short int n = 4;
    string commands[n] = {
        "start",
        "reset",
        "records",
        "help",
    };

    for (short int i = 0; i < n; i++)
        if (command == commands[i])
            return true;

    return false;
}

void ErrorHandler()
{
    if (board_error_lines)
    {
        UpdateBottomHeader(board_error, board_error_lines, RED);
        board_error_lines = 0;
    }
    else
        PrintEmptyLine(2);
}

void Updater(bool relpy_to_command = false, bool variables_to_file = false, bool file_to_variables = false)
{

    ClearConsole();
    UpdateTopHeader();
    UpdateMainBoard();
    UpdateBottomHeader();
    ErrorHandler();

    // sync condition
    if (variables_to_file)
        VariablesToFileUpdater();
    if (file_to_variables)
        FileToVariablesUpdater();

    // get new command
    PrintText("❯❯❯❯❯❯❯❯❯❯❯ ", Color::CYAN);
    string command;
    cin >> command;

    // check new command
    if (command != "exit")
    {
        if (relpy_to_command)
        {
            // the value of input goes in to global tmp variable & will be checked if is needed
            tmp = command;
        }
        else
        {
            if (CommandChecker(command))
                CommandHandler(command);
            else
            {
                board_error = command + GlobalTexts(Message::COMMAND_ERROR);
                board_error_lines = 1;
                Updater();
            }
        }
    }
    else
    {
        // save all changes before closing the game
        VariablesToFileUpdater();
        PrintText("See u later", WHITE, true);
        exit(0);
    }
}

void CommandHandler(string &command)
{
    if (command == "help")
    {
        board_info = GlobalTexts(Message::HELP);
        board_info_lines = 3;

        Updater();
    }

    else if (command == "reset")
    {
        fstream file;
        file.open(path, std::ofstream::out | std::ofstream::trunc);
        file << "0\n3|0\n10|0\n1|2|3|4|5|6|7|8|9\nPlayer1|0\nPlayer2|0\n";
        file.close();

        FileToVariablesUpdater();

        board_info = GlobalTexts(Message::STARTING);
        board_info_lines = 3;

        Updater();
    }

    else if (command == "records")
    {
        if (global_records[1])
        {
            board_info = "last recorded games|";
            board_info_lines = 1;

            for (short int i = 0; i < global_records[1]; i++)
            {
                string table_items[10];
                TextSplitter(global_records_array[i], table_items);

                // spaces with these characters have problems so this way works
                for (short int j = 0; j < 9; j++)
                {
                    if (table_items[j] == "✘")
                        table_items[j] = "x";
                    else if (table_items[j] == "◯")
                        table_items[j] = "o";
                }

                board_info += "Winner : " + table_items[9] + " -> ";

                for (short int j = 0; j < 9; j++)
                    board_info += table_items[j] + "-";
                board_info.pop_back();
                board_info += "|";
                board_info_lines++;
            }
            board_info.pop_back();
        }
        else
        {

            board_info = "There is no record!|How about starting a new game?";
            board_info_lines = 2;
        }

        Updater();
    }

    else // start command
    {
        if (global_status == 0)
        {
            board_error = "Make sure it's a valid name|don't make me confused and broken ok?";
            board_error_lines = 2;

            board_info = "Player1 how should I call u?";
            board_info_lines = 1;
            Updater(true);
            global_player1[0] = tmp;

            // show error again
            // only make it 2 bcs every time after showing error it will be changed to 0
            // but the text still remains
            board_error_lines = 2;

            board_info = "Player2 how about u?";
            Updater(true);
            global_player2[0] = tmp;

            // new message
            board_info = "Well, let's get started, Just enter something to start";
            board_info_lines = 1;

            global_status = 21;
            next_round_turn = 22;

            Updater(true, true);

            string command = "start";
            CommandHandler(command);
        }

        else if (global_status == 1)
        {
            board_info = "Wow wanna start a new game?|make me sure that you are ready & enter something (0-0)";
            board_info_lines = 2;
            Updater(true);

            global_status = 22;
            next_round_turn = 21;
            string command = "start";
            CommandHandler(command);
        }

        else
        {
            short int game_result = GameChecker();
            while (game_result == 0)
            {
                if (global_status == 21)
                    board_info = global_player1[0];
                else
                    board_info = global_player2[0];

                board_info += "! Choose your place";
                board_info_lines = 1;

                Updater(true);

                while (!InputCell())
                {
                    board_error = "This number is not a right place!|Try with another one /:";
                    board_error_lines = 2;
                    Updater(true);
                }

                // change turn
                if (global_status == 21)
                    global_status = 22;
                else
                    global_status = 21;

                game_result = GameChecker();
            }

            if (game_result == -1)
            {
                board_info = "This round ended in a tie!!!|Let's start a new round to see who wins this round at the end|If you are ready just enter somthing!";
                board_info_lines = 3;

                for (short int i = 0; i < 9; i++)
                    global_table[i] = to_string(i + 1);

                Updater(true);

                // swap turns
                if (global_status != next_round_turn)
                {
                    global_status = global_status + next_round_turn;
                    next_round_turn = global_status - next_round_turn;
                    global_status -= next_round_turn;
                }
                else
                {
                    if (next_round_turn == 21)
                        next_round_turn = 22;
                    else
                        next_round_turn = 21;
                }

                string command = "start";
                CommandHandler(command);
            }
            else
            {
                // the round has ended

                string round_winner;
                if (game_result == 1)
                {
                    round_winner = global_player1[0];
                    global_player1[1] = to_string(stoi(global_player1[1]) + 1);
                }
                else // game_result is 2
                {
                    round_winner = global_player2[0];
                    global_player2[1] = to_string(stoi(global_player2[1]) + 1);
                }

                // make record ready to add to the records
                string record = "";
                for (short int i = 0; i < 9; i++)
                {
                    record += global_table[i] + "|";

                    // clear table
                    global_table[i] = to_string(i + 1);
                }
                record += round_winner;

                // add the round details to records if there are no limit for saving an other
                if (global_records[1] < global_records[0])
                    // also ++ bcs we recorded a new one
                    global_records_array[global_records[1]++] = record;
                else
                {
                    // The specified space is filled and every time the first recorded item is deleted and a new item is added to the end of the array
                    for (short int i = 1; i < global_records[1]; i++)
                        global_records_array[i - 1] = global_records_array[i];
                    global_records_array[global_records[1] - 1] = record;
                }

                // complete message
                board_info = round_winner + " has won the round!!!";
                board_info_lines = 1;

                // check if game is over
                global_rounds[1]++;
                if (global_rounds[0] == global_rounds[1])
                {
                    string game_winner;
                    // this is the end of the game (not just a round)
                    if (stoi(global_player1[1]) > stoi(global_player2[1]))
                        game_winner = global_player1[0];
                    else
                        game_winner = global_player2[0];

                    // check if the round winner is the same with game winner
                    if (round_winner != game_winner)
                        board_info += "|But ";
                    else
                        board_info += "|Also ";

                    board_info += "the game winner issss : " + game_winner;
                    board_info += "|If you want to play the game again, just 'start'";
                    board_info_lines += 2;

                    // reset everything
                    fstream file;
                    file.open(path);
                    global_status = 1;
                    file << "1\n" + to_string(global_rounds[0]) + "|0\n" + to_string(global_records[0]) + "|" + to_string(global_records[1]) + "\n1|2|3|4|5|6|7|8|9\n" + global_player1[0] + "|0\n" + global_player2[0] + "|0\n";

                    for (short int i = 0; i < global_records[1]; i++)
                        file << global_records_array[i] << "\n";
                    file.close();
                    // end reset

                    Updater(false, false, true);
                }
                else
                {
                    // The game is still in progress & only round has ended
                    board_info += "|Ok so let's start the next round guys...|Just enter something to make me sure you're ready!";
                    board_info_lines += 2;

                    Updater(true, true);

                    // turns
                    if (global_status == 21)
                    {
                        global_status = 22;
                        next_round_turn = 21;
                    }
                    else
                    {
                        global_status = 21;
                        next_round_turn = 22;
                    }

                    string command = "start";
                    CommandHandler(command);
                }
            }
        }
    }
}

short int GameChecker()
{
    /*
        considers winning modes and can return 4 different values ​​according to the result

        -1 => game has ended with a draw
        0  => game must continue and the result is not yet known
        1  => player1 is the winner of the round  => ✘
        2  => player2 is the winner of the round  => ◯
    */

    short int index = 0;
    for (short int i = 0; i < 9; i++)
        if (global_table[i] == "✘" || global_table[i] == "◯")
            index++;

    // winning happens only if there are more than 4 moves in the game
    if (index > 4)
    {

        if ((global_table[0] == global_table[1] && global_table[1] == global_table[2]))
        {
            if (global_table[0] == "✘")
                return 1;
            return 2;
        }
        else if ((global_table[3] == global_table[4] && global_table[4] == global_table[5]))
        {
            if (global_table[3] == "✘")
                return 1;
            return 2;
        }
        else if ((global_table[6] == global_table[7] && global_table[7] == global_table[8]))
        {
            if (global_table[6] == "✘")
                return 1;
            return 2;
        }

        else if ((global_table[0] == global_table[3] && global_table[3] == global_table[6]))
        {
            if (global_table[0] == "✘")
                return 1;
            return 2;
        }
        else if ((global_table[1] == global_table[4] && global_table[4] == global_table[7]))
        {
            if (global_table[1] == "✘")
                return 1;
            return 2;
        }
        else if ((global_table[2] == global_table[5] && global_table[5] == global_table[8]))
        {
            if (global_table[2] == "✘")
                return 1;
            return 2;
        }

        else if ((global_table[0] == global_table[4] && global_table[4] == global_table[8]))
        {
            if (global_table[0] == "✘")
                return 1;
            return 2;
        }
        else if ((global_table[2] == global_table[4] && global_table[4] == global_table[6]))
        {
            if (global_table[2] == "✘")
                return 1;
            return 2;
        }

        else
        {
            if (index == 9)
                // the game has ended with a draw
                return -1;
        }
    }

    // no one has won the game yet
    return 0;
}

bool InputCell()
{

    /*
        checks the correctness of the entered cell and if it is correct,
        puts the value in the desired cell

        if everything goes well, it returns true
    */

    try
    {
        short int cell = stoi(tmp);
        if (0 < cell && cell < 10)
        {
            if (global_table[cell - 1] != "✘" && global_table[cell - 1] != "◯")
            {
                if (global_status == 21)
                    global_table[cell - 1] = "✘";
                else
                    global_table[cell - 1] = "◯";

                return true;
            }
        }
        return false;
    }

    catch (const std::invalid_argument &e)
    {
        // if tmp is like "oij33oij" or anything else it'll give us core dumped message
        return false;
    }
}

void StatusUpdater()
{
    /*
    file detail:
        line 0 => 0 {zero from scratch} 1 {players are defined and ready to start} 21 {2 means in the middle of game | 1 means it's player one's turn}
    */

    fstream file;
    file.open(path, ios::in | ios::out | ios::app);
    file.seekg(0, ios::end);
    short int length = file.tellg();
    file.seekg(0);
    file.close();

    if (length)
    {

        FileToVariablesUpdater();

        if (global_status == 0)
        {
            board_info = GlobalTexts(Message::STARTING);
            board_info_lines = 3;
        }
        else if (global_status == 1)
        {
            board_info = "Hey, Looks like you are interested to start?|Just 'start' the game";
            board_info_lines = 2;
        }
        else
        {
            // it's 21 or 22
            board_info = "Hey what's up guys, WOW see the table!|Seems like you are interested to finish the job (:|Just 'start' to do it";
            board_info_lines = 3;
        }
    }
    else
    {
        // file is new & this is first time
        file.open(path);
        string default_txt = "0\n3|0\n10|0\n1|2|3|4|5|6|7|8|9\nPlayer1|0\nPlayer2|0\n";
        file << default_txt;
        file.close();

        FileToVariablesUpdater();

        board_info = GlobalTexts(Message::STARTING);
        board_info_lines = 3;
    }
}

void FileToVariablesUpdater()
{
    /*

    This function only updates global variables with values in the file
    for ease of using in program.
    Whenever this function is called, variables will be updated.

    file detail:
        line 0 => 0 {zero from scratch} 1 {players are defined and ready to start} 21 {2 means in the middle of game | 1 means it's player one's turn}
        line 1 => rounds [all rounds | current round]
        line 2 => records [all possible records to save|number of records till yet]
        line 3 => table [1|2|3|...|9]
        line 4 => player1 => [name|rounds]
        line 5 => player2 => [name|rounds]
        line 6 to n => records => [1|2|3..|9|{winner player number like 1 or 2}]
    */

    fstream file;
    file.open(path, ios::in | ios::out | ios::app);

    string line;

    // line0 (status)
    // we update global status variable here
    getline(file, line);
    global_status = stoi(line);

    // line1 (rounds)
    getline(file, line);
    string tmp_array[2];
    TextSplitter(line, tmp_array);
    global_rounds[0] = stoi(tmp_array[0]);
    global_rounds[1] = stoi(tmp_array[1]);

    // line2 (records)
    getline(file, line);
    TextSplitter(line, tmp_array);
    global_records[0] = stoi(tmp_array[0]);
    global_records[1] = stoi(tmp_array[1]);

    // line3 (table)
    getline(file, line);
    TextSplitter(line, global_table);

    // line4 (player1)
    getline(file, line);
    TextSplitter(line, global_player1);

    // line5 (player2)
    getline(file, line);
    TextSplitter(line, global_player2);

    // line6 to n (records)
    if (global_records[1])
    {
        for (short int i = 0; i < global_records[1]; i++)
        {
            getline(file, line);
            global_records_array[i] = line;
        }
    }

    file.close();
}

void VariablesToFileUpdater()
{
    fstream file;
    file.open(path, ios::out | ios::trunc);

    // line1 (status)
    file << global_status << "\n";

    // line2 (rounds)
    file << global_rounds[0] << "|" << global_rounds[1] << "\n";

    // line3 (records)
    file << global_records[0] << "|" << global_records[1] << "\n";

    // line4 (table)
    for (short int i = 0; i < 8; i++)
        file << global_table[i] << "|";
    file << global_table[8] << "\n";

    // line5 line6 (player1 player2)
    file << global_player1[0] << "|" << global_player1[1] << "\n";
    file << global_player2[0] << "|" << global_player2[1] << "\n";

    // for records
    for (short int i = 0; i < global_records[1]; i++)
    {
        // we assume items are like => "1|2|3...|9|1|{winner name}"
        file << global_records_array[i] << "\n";
    }

    file.close();
}
