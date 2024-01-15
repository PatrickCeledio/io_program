// Patrick Celedio
// main.c 
// A program that demonstrates the input-output functionality of C.
// It can take in a string, save it, display it, save it to a .txt file, and 
// even load more strings from .txt files....... WOW.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <io.h>

// // Windows IP Helper API
// #include <iphlpapi.h>

// // Windows Socket API
// #include <winsock2.h>

#define DEBUG 0

/*
*
* Debug helper functions
*
*/

void debugCheckMenuUserInput(int menuChoice){
    if (DEBUG == 1){
        printf("\nDEBUG: User entered %d \n", menuChoice);
    }

    return;
}

void debugCheckMenuUserInputType(int menuChoice){
    if (DEBUG == 1){
        // If menuChoice is a digit is true, then continue, else try again.
        if (isdigit(menuChoice)){
            printf("DEBUG: You entered a char.\n");
        }else{
            printf("DEBUG: You entered a number.\n");
        }
    }
}


/*
*
*  Global variables
*
*/

char userInput[100] = "";



/*
*
* Helper functions
*
*/ 

void caesarCipher(char* text, int shift) {
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            text[i] = (c - base + shift) % 26 + base;
        }
    }
}

void displayLocalDir(){
    struct dirent *entry;
    DIR *dp;

    // Open the local file dir
    dp = opendir(".");

    if (dp == NULL){
        perror("Error opening directory");
        return;
    }else{
        printf("These are the .txt files found in the local directory.\n\n");

        // List the .txt files 
        while((entry = readdir(dp)) != NULL){
            if (strstr(entry->d_name, ".txt") != NULL){
                printf("-> %s\n", entry->d_name);
            }
        }

        closedir(dp);
    }

}

void getUserInput(){
    // Prompt user
    printf("\nUser selected Input.\n\nPlease enter some words; it could be anything.\n");
    printf("Don't make it complicated... Listening...\n\n");

    // char tmp[100];
    // fgets(tmp, 100, stdin);
    // memcpy(userInput, tmp, sizeof(userInput));
    // printf("DEBUG: tmp is %s\n", tmp);

    // Clear the FUCKING BUFFER
    while ((getchar()) != '\n');
    // Get user input
    fgets(userInput, 100, stdin);

    return ;
}



//
// This clears the buffer for userInput
//
void clearUserInputBuffer(char* userInput){
    // Clear string buffer via consuming stray characters until /n encountered
    scanf(" %[^\n]", userInput);
    
    return;
}

//
// This clears the console
//
void clearConsole(){
    printf("\033[2J\033[1;1H");
}

//
// Self-explanatory; user has to hit enter to proceed
//
void UserPressEnter(){
    while(getchar() != '\n');{
        printf("\nPress enter to continue...\n");
    }
}

void showMenu(int menuChoice){

    // Switch case for possibilities 1-5
    switch(menuChoice){
        case 1:
            debugCheckMenuUserInput(menuChoice);
            debugCheckMenuUserInputType(menuChoice);
            clearConsole();

            

            /*
            *    No more memory allocation, yo
            */

            // // Memory is allocated for x
            // userInput = malloc(sizeof(char) * 100);

            // // Set string to null aka \0
            // memset(userInput, '\0', sizeof(userInput));

            getUserInput();

            // Debug statement to check the value of userInput
            if (DEBUG == 1){
                printf("Debug: userInput contains %s\n", userInput);            
            }

            UserPressEnter();
            
            break;
        case 2:
            debugCheckMenuUserInput(menuChoice);
            debugCheckMenuUserInputType(menuChoice);
            clearConsole();


            printf("\nUser selected Output.\n");

            // Is String is blank or null or full of '\0'? 
            // True == No string saved
            // False == User entered "..."
            if (DEBUG == 1){
                // Debug statement to check the value of userInput
                printf("Debug: userInput contains %s\n", userInput);

                if (strcmp(userInput, "") == 1 || userInput == ""){
                    printf("You have no string saved.\n");
                }else{
                    printf("User last entered %s\n", userInput);
                }
            }

            printf("Currently the saved input is: %s\n", userInput);
            UserPressEnter();

            break;
        case 3:
            debugCheckMenuUserInput(menuChoice);
            debugCheckMenuUserInputType(menuChoice);
            clearConsole();

            printf("User selected Save to .txt.\n");

            printf("Enter a desired filename to save the stored input (e.g. user_Input.txt)\n: ");
            while((getchar()) != '\n');
            char filename[100];
            scanf("%s", filename);

            while((getchar()) != '\n');

            strcat(filename, ".txt");

            FILE* userInputFile = fopen(filename, "w");

            if (userInputFile == NULL){
                printf("ERROR: File does not exist for writing.");
                exit(1);
            }else{
                if (DEBUG == 1){
                    printf("DEBUG: filename is %s\n", filename);
                }
                fprintf(userInputFile, "%s", userInput);
                fclose(userInputFile);
                printf("User input has been saved into a .txt file in your local directory.");
            }

            UserPressEnter();


            break;
        case 4:
            debugCheckMenuUserInput(menuChoice);
            debugCheckMenuUserInputType(menuChoice);
            clearConsole();

            printf("User selected Load .txt\n");

            displayLocalDir();

            char local_file[100];
            printf("\nEnter the name of the .txt file you want to load: ");
            scanf("%s", local_file);

            FILE* open_File = fopen(local_file, "r");

            if (open_File == NULL){
                perror("ERROR: File does not exist.");
                break;
            }else{
                char ch;

                printf("Contents of %s:\n ", local_file);
                while((ch = fgetc(open_File)) != EOF){
                    putchar(ch);
                }

                fclose(open_File);
            }
            


            UserPressEnter();

            break;
        case 5:
            debugCheckMenuUserInput(menuChoice);
            debugCheckMenuUserInputType(menuChoice);
            clearConsole();
            printf("User selected Caesar Cipher.\n");

            displayLocalDir();

            char scramble_File[100];
            printf("\nEnter the name of the .txt file you want to load and scramble: ");
            scanf("%s", scramble_File);

            FILE* opened_Scrambled_File = fopen(scramble_File, "r+");

            if (opened_Scrambled_File == NULL){
                perror("ERROR: File does not exist.");
                break;
            }else{
                char buffer[1000]; // Adjust the buffer size as needed
                char *result;
                char ch;
                printf("\nFile exist... starting scramble...\n");
                
                 // Reset the file pointer to the beginning
                fseek(opened_Scrambled_File, 0, SEEK_SET);
                if (fgets(buffer, sizeof(buffer), opened_Scrambled_File) != NULL) {
                    caesarCipher(buffer, 3); // Scramble text
                    fseek(opened_Scrambled_File, 0, SEEK_SET); // Reset to start of file
                    fputs(buffer, opened_Scrambled_File); // Overwrite content
                    fflush(opened_Scrambled_File); // Flush changes
                    // If scrambled text is shorter, truncate the file
                    ftruncate(fileno(opened_Scrambled_File), strlen(buffer));
                }

                fclose(opened_Scrambled_File); // Close and reopen for reading
                opened_Scrambled_File = fopen(scramble_File, "r");
                if (opened_Scrambled_File != NULL) {
                    printf("Contents of %s:\n ", scramble_File);
                    while((ch = fgetc(opened_Scrambled_File)) != EOF){
                        putchar(ch);
                    }
                    fclose(opened_Scrambled_File);
                } else {
                    perror("ERROR: Unable to reopen file.");
                }
            }

            UserPressEnter();

            break;
        case 6:
            debugCheckMenuUserInput(menuChoice);
            debugCheckMenuUserInputType(menuChoice);

            clearConsole();

            printf("C IO Program: Patrick Celedio - Oct 2023\n\n");

            UserPressEnter();


            break;
        case 7:
            clearConsole();
            printf("\nProgram exit. Have a nice day.\n");
            exit(0);
        case 8:
            debugCheckMenuUserInput(menuChoice);
            debugCheckMenuUserInputType(menuChoice);

            clearConsole();

            printf("Display IP Address (Will only work on Windows for now)\n\n");

            UserPressEnter();
            break;
        case 9:
            debugCheckMenuUserInput(menuChoice);
            debugCheckMenuUserInputType(menuChoice);

            clearConsole();
            printf("Steamboat Willie\n\n");

            puts(
                "........................::.:-=-:.............................::::-----==--::::::----=:....\n"
                "----:-::::::::::::::::::::.:-=--::::............................:--====+=::-----==-===....\n"
                "::::::::::::::::::::::::::.:=+--:::::::-:::::::::::--=-=-=====-========*+=------=--=+=--::\n"
                ".....:...:..::......::::::.:=+--::::::::::::::::----========++*##*+====+==--------=====---\n"
                ".....................:..::.:=+--::::::::::::::::::::--------+@@@@@@#======-=======-------:\n"
                "::::::::::::::::::.:::::::.:=+----::::::::::----------------*@@@@@@@#=======--::::::::::::\n"
                "::---------------:::::::::.:=+::-+----------------:---------=#-.-+#@@+--------==------::::\n"
                "--------------------------:-==::=+=====-::::-----::-=*#%%#*+-......-@@*-------------------\n"
                "--::::::::---=--=--=-:.:::::::--=-----:::::::::::--+@@@@@@+..:....=%@@#-------::::::::::.:\n"
                "............:-:::--:..::::::::::-:..............-:...+%@+.......:-+**+:::::::::::::::::::-\n"
                "........... .:::::-..:::::-:--:::=:::::::::::::::--===--:--....=**##*+--------------------\n"
                "......... ....:-:::-.-:::=-=-:-::-+-----=+=----==::-.    .=@#=%@@@@@@@@*==================\n"
                "...............:=:::-:::+==+::-:::=----=@@@%=-=%-+@-     .*@@@@@@@@@@@@@-::::::-----::-:::\n"
                ":::.......:::--==...-::-+--=:::.-::=:---+%@@+.--=@+.    :#%##%@@@@@@@@@@-::.::--::....:...\n"
                "..::::---=====-=:..::::+---=:=..:::*%%@%#=-:.......    .:.....:%@@@@@@#-..:::--:......-:..\n"
                "==++====---::::-...-:::-=---:+-..-:=@@@@@@=-:.....   ..:+:.....*:----:....::-:......:-----\n"
                "+===--::....:--=...-::-::-+---=.:::-@@@@@@@%##*=:..-++#%=... .-:.........::--:.....:-::---\n"
                "--=---::::-==++=...-::++-:-:--=::=::%@@@%+-==+*#:..:..::....::...........::=-:.....-::::::\n"
                "::=-::::::=++++=...-::*@@@@%+#--::-:+=---------=#+==+**++*#*.............::==--::::.-:::::\n"
                "..---:::::-++++=...-::*@@@@@@@+=:-=-+=++***##%@@@@@@@@@@@@%-.............:::==--:....::-::\n"
                "..----::::-++++=:::-::*@@@@@@@@@@@@@@%%%###**@@@@@@@@@@@@#-:::............:::=+=-:::....-:\n"
                "..---:::::-+--::::--::+@@@@@*===..-:=------+@@@@@@@@@@@@@%=::.:............:::-====---::--\n"
                "..----::::=::::--:::-:-**-:+=::-:.-:=-----=+@@@@@@@@@@@@@%*-........::......:::::-===+=+=+\n"
                "..-----:::-+==++=...-::+=:-+-=-::--:+--===+..-+*##%%%#*=:.:-:......:::.::::::::::::::-----\n"
                "..------:::**+++*:..-::--:==--+--=::+-----+:...:...:.:....-:::::::::::::---------------=--\n"
                "..--------:+*+++++...-::--+-==-.:-:--::::::=:............-=--------:::::::----=---:-----==\n"
                "..--------:+*+++++-..:-::-+===.:-::---..:::-::.....:......=-::--:--:::::::::-----:::------\n"
                ".:---------+++++++*-..-:::-==:::::--:--:::::-@@*-:-::::-%@*-::-::--::::::::::::--::::-----\n"
                ".:---------=+++++++*:..-:::::::::-=----:::::+@@=::::::::-#@@#*=-:--::::::::::::---::::----\n"
                ".:---------=+++++++**-----::::::--::::::::::-#@@=-::::::::+@%=-=+++++=-::-::::-:-----:----\n"
                ".:---------=+++++++*+:::=:------::::::--..:::=-*#-:-::::::=@@-:::----:-=+**=::------------\n"
                "::------:::=++++++**:::==::::::::::---=............=-----:*%=--:-------::--+#=------------\n"
                "::-------::-++++++*=:::+---:::::::--:--:::....::...==--=-....:::::-=---------%-----===----\n"
                "::-------::-++++*+-=--==-----::-----------==--=---==-=-=:..........-=------=**--==-======-\n"
                "::----------++*+=-=--===-----------------=--------====--===-:.....:-==-=****=--===========\n"
                ":-======----++===========---------=---===--------===+==--=--==----=+======================\n"
                "=+============================-=====================+=============+++===========+=++==++==\n"
                "++===============++=============================+++++==========+=++*+++++=======+++++++++=\n"
                "*++++==========++==================+======+++++++++++++=+======++++*++++++++++++++++++++++\n"

            );

            UserPressEnter();
            break;
        case 10:
debugCheckMenuUserInput(menuChoice);
            debugCheckMenuUserInputType(menuChoice);

            clearConsole();
            printf("ChatGPT 4: Can you generate me ascii art of a cup of coffee; How about very detailed; with an ascii background too\n\n");

            puts(
                "                            (                     \n"
                "           )     (                                \n"
                "       ___...(-------)-....___                    \n"
                "       .-""       )    (         ""-.             \n"
                "   .-'``'|-._             )         _.-|          \n"
                "   /  .--.|   `""---...........---""`   |         \n"
                "   ' /    j    .             .             .  l   \n"
                "   `-._   `._.'           /          .'.-'  /     \n"
                "        ```--. . , ; .--`           /    .-'      \n"
                "               |   |            `..-'   /         \n"
                "               |   |               `'  /          \n"
                "               |   |                  (           \n"
                "               |   |                  `.          \n"
                "               |   |                     )        \n"
                "               |   |              `..-'  )        \n"
                "               |   |                  `.(         \n"
                "               |   |                     )        \n"
                "               |   |                      `.      \n"
                "               |   |                        )     \n"
                "               |   |                  `..-'  )    \n"
                "               |   |                  `(/         \n"
                "               |   |                     )        \n"
                "               |   |                     (        \n"
                "               |   |                      `.      \n"
                "               |   |                        )     \n"
                "               |   |                  `..-'  )    \n"
                "               |   |                  `(/         \n"
                "               |   |                     )        \n"
                "               |   |                     (        \n"
                "               |   |                      `.      \n"
                "               |   |                        )     \n"
                "               |   |                  `..-'  )    \n"
                "               |   |                  `(/         \n"
                "               |   |                     )        \n"
                "               |   |                     (        \n"
                "               |   |                      `.      \n"
                "               |   |                        )     \n"
                "               |   |                  `..-'  )    \n"
                "               |   |                  `(/         \n"
                "               |   |                     )        \n"
                "               |   |                     (        \n"
                "               |   |                      `.      \n"
                "               |   |                        )     \n"
                "               |   |                  `..-'  )    \n"
                "               |   |                  `(/         \n"
                "               |   |                     )        \n"
                "               |   |                     (        \n"
                "               |   |                      `.      \n"
                "               |   |                        )     \n"
                "               |   |                  `..-'  )    \n"
                "               |   |                  `(/         \n"
                "               |   |                     )        \n"
                "               |   |                     (        \n"
                "               |   |                      `.      \n"
                "               |   |                        )     \n"
                "               |   |                  `..-'  )    \n"
                "               j   l                  `(/         \n"
                "               `-._)                   )          \n"
                "                   `---..___...---`               \n"
                "                                                  \n"      

            );

            UserPressEnter();
        
            break;
        case 11: 
            debugCheckMenuUserInputType(menuChoice);
            clearConsole();

            printf(""\n);

            printf("1/8/24: I have an internet outage at my home this week. This sucks ass.\n");
            printf("1/9/24: Again.\n");
            printf("1/10/24: It's fixed.\n");
            printf("1/11/24: Lol"\n);
            printf("1/11/24: Lol"\n);
            printf("Another one."\n);
            printf("This is getting ridiculous."\n);


            UserPressEnter();
            break;
        default:
            printf("\nYou entered an invalid selection. Try again. \n");
            break;
    }

}

int main(int argc, char* argv[]){

    int run = 1;
    int menuChoice = -1;

    char nothing[100];

    while(run){
        clearConsole();

        printf("Input-Output Menu\n1. Input\n2. Output\n3. Save to .txt\n4. Load .txt\n5. Scramble\n6. Credits\n7. Quit\n8. Something\n9. Steamboat Willie\n10. AI Coffee\n");
        printf("Listening for input...\nEnter a number based on what you want to select: ");
        
        // Scan for user input; expecting a int. 
        // It will be stored in the memory address of int menuChoice, hence the ampersand
        scanf("%d", &menuChoice);
        showMenu(menuChoice);

        // Clear io buffer before next refresh
        while ((getchar()) != '\n');
    }

    return 0;
}