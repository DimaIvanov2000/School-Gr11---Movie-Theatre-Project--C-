/*

Movie Ticket Purchase Interface Program
Written by Dima Ivanov
Date submitted: Oct 21 , 2017

Color-changing code supplied by online user Ashish Ahuja. Full code can be found at:
https://stackoverflow.com/questions/29574849/how-to-change-text-color-and-console-color-in-codeblocks/29575262#29575262

Code for unlocking free popcorn is ICS3U

Source code explanation best understood if read from top down

*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#include<time.h>

//functions:
void loadMovieInfo(); //loads information about movies including name, description, genres and rating; this exists so that editing movies is easier via the text editor
void randomizeSeats(); //randomizes the occupation of the seats for all times and movies to make it interesting
void showTitle(); //displays the main title
int selectMovie(); //allows user to select a movie, including previews of the film, rating, genre
int selectTime(); //allows the user to select a time to view their movie which has not yet been occupied
int selectNumPeople(); //allows user to select an appropriate number of people to view the movie (seat availability once again)
int selectPersonAge(); //allows user to select the age of an individual person + whether they are a student (if R rated, only few ages available)
int finalCheck(); //a final confirmation from the user that all they input is correct, if this is true, new seats are occupied
void enterCode(); //allows user to enter code for free popcorn
void printReceipt(); //prints the receipt all fancy-like using the variables below
void setColor(); //sets text color for aesthetic purposes (not my own code)
void cprintf(); //sets color and then prints text
void cprinti(); //sets color and then prints an integer
void borderprintf(); //prints text in paragraph format to look nicer

//Variables in use (those which are actively changed):
int movieChosen; // number of the movie user has chosen
int timeChosen; // number of the time user has chosen (not the time itself, that would be a waste of memory
int numPeople; // number of people user has selected to buy tickets for
int numCurrentPerson; // counter remembering the current person the user is inputing the age of
int peopleRecordedAgeRanges[10]; //saves the ages of all the entered attendees so user can change them at will before the final price is calculated
int price; // total price for user, calculated over the course of one "run" through the program
int codeRedeemed=0; //a global variable keepng track of whether the user has redeemed their code or not
int movieSeatsOccupied[7][5]={0}; //list of how many seats are occupied in each combination of movie and time

//Constant variables (those which are not changed and/or are retrieved from the movieInfo.txt file):
char movieNames[7][65]; //string array of movie names to be retrieved from txt document
int movieRatings[7]={0}; //list of movie ratings (/5 stars) to be retrieved from txt document
char movieGenres[7][5][20]={0}; //list of movie genres to be retrieved from txt document
char movieDescriptions[7][500]={0}; //list of movie descriptions to be retrieved from txt document
char movieTimeOptions[5][10]={"4:00pm ","5:30pm ","7:00pm ","8:30pm ","10:00pm"}; //list of strings holding all the time slots offered
char ageRanges[8][25]={"<3 Years","3-13 Years","3-13 Years [Student]","14-17 Years","14-17 Years [Student]","18-64 Years","18-64 Years [Student]","65+ Years"}; //list of strings holding all the age ranges proposed
int ageRangePrices[8]={0,6,4,12,10,12,10,6}; //integer list of prices corresponding to each age range
char movieAgeRestrictionNames[4][8]={"[G]","[PG]","[PG-13]","[R]"}; //list of strings all movie age restriction ratings
int movieAgeRestrictions[7]; //list of movie age ratings to be retrieved from txt document

int main(void)
{
    /*

    general notes about main:
    -there are a lot of nested loops which allows to user to return to previous steps with some of their info stored (say they are of num of people and want to return to time selection)
    -all of the functions allowing the user to select something return 1 if a valid input is entered and 0 if the user goes to the previous step, they contain their own loops in them

    */

    char input; //present in many of the functions below, this variable is just used to store the user's last character input
    int canExit; //keeps track of whether or not to exit from the nested loops below
    //setup:
    loadMovieInfo(); //loads movie info
    randomizeSeats(); //randomizes seat occupations
    //continuously active code:
    while(1)//main loop (ahows title)
    {
        do //small loop showing he title while the user does not escape or enter (27 and 13 in ascii respectively)
        {
            showTitle();
            input=getch(); //you will see this a lot, this takes a character input from the user without pressing enter
        }
        while(input!=27&&input!=13); //while not esc or enter
        if(input==27) // if the input is escape, it prints a "goodbye" and exits the program
        {
            system("cls");
            cprintf(14,"\nThank you for coming!\n");
            setColor(15);
            break;
        }
        canExit=0; //resets this to 0 before diving into all he loops
        while(canExit==0&&selectMovie()==1) //2nd loop (chooses movie)
        {
            while(canExit==0&&selectTime()==1) //3rd loop (chooses time)
            {
                while(canExit==0&&selectNumPeople()==1) //4th loop (choose num of people)
                {
                    numCurrentPerson=1; //counter reset
                    while(numCurrentPerson>0&&numCurrentPerson<=numPeople&&canExit==0) //5th loop taking in the ages of all the people
                    {
                        if(selectPersonAge(numCurrentPerson)==1) //takes in age of current person or returns to previous person
                        {
                            numCurrentPerson++;
                        }
                        else
                        {
                            numCurrentPerson--;
                        }
                        if(numCurrentPerson>numPeople) //if this is true, all the people have been entered
                        {
                            if(finalCheck()==1) //returns to last person if the final prompt is denied, continues if it is not
                            {
                                enterCode(); //returns 1 or 0 based on if the user is successful or not in putting in the right code respectively
                                printReceipt(); //prints reciept, user can press any key to continue
                                codeRedeemed=0; // resets it for next time (precaution)
                                canExit=1; //all loops above stop when this is 1 except the main loop
                            }
                            else
                            {
                                numCurrentPerson=numPeople;//sets the current person number to the last person and continues on for the user's convenience
                            }
                        }
                    }
                }
            }
        }
    }
}

void loadMovieInfo()
{
    /*

    general notes about load movie info:
    -format of the file for it to work is very very specific
    -counters do not have specific names for the sake of not writing a ton of code, the use of the counters is in the context of the code

    */

    int i=0,j=0,k=0; //misc counters
    int c; // keeps track of the last character retrieved from the file

    FILE*info=fopen("movieInfo.txt","r"); // opens file in read mode
    //47
    do //for this loop, i is used to keep track of the movie number
    {
        c=fgetc(info); //this retrieves the next character in the file
        if(c!=47) //47 is a slash, used to know if the end of the file has been reached
        {
            for(j=0;j<65;j++) //preliminarily sets all characters in the movie name to 0 to prevent glitches, j is used in many places as a misc counter
            {
                movieNames[i][j]=0;
            }
            j=0; //resets counter
            while(c!=10) //takes characters and places them in he movie name while the character retrieved is not enter (10 in the case of file IO)
            {
                movieNames[i][j]=c;
                j++;
                c=fgetc(info);
            }
            for(j=2;j>=0;j--) //takes 3 characters (expected to be numbers) and uses their values to calculate the movie's rating
            {
                movieRatings[i]+=(fgetc(info)-48)*(int)pow(10,(double)j);
            }
            c=fgetc(info); //throws away an enter
            movieAgeRestrictions[i]=fgetc(info)-48; //takes in a character (expected number) and uses it determine the age rating
            c=fgetc(info); //throws away an enter
            j=0; //resets counter
            do //a nested while loop in a while loop which adds characters to the movie genre name, switches genre name slots if the character is a slash (47) and stops when the character is an enter (10)
            {
                c=fgetc(info);
                k=0;
                while(c!=10&&c!=47)
                {
                    movieGenres[i][j][k]=c;
                    k++;
                    c=fgetc(info);
                }
                j++;
            }
            while(c!=10);
            j=0; //resets counter
            c=fgetc(info); //gets a character to start
            while(c!=10) //puts characters in the movie's description while there is no enter (assumed that the description is less than 500 characters)
            {
                movieDescriptions[i][j]=c;
                j++;
                c=fgetc(info);
            }
            c=fgetc(info);
            i++;
        }
    }
    while(c!=47); //end of the first do while loop, program ends when instead of a new enter, there is a slash. If you look in the file, it says /END simply for readability purposes
    fclose(info); //closes file
}

void randomizeSeats()
{
    int i,j; //misc counters

    srand((unsigned int) time(NULL)); //sets up randomness
    for(i=0;i<7;i++) //cycles through movies
    {
        for(j=0;j<5;j++) //cycles through times
        {
            movieSeatsOccupied[i][j]=rand()%1000; //anywhere from 0 to 999 seats are occupied
        }
    }
}

void showTitle()
{
    /*

    Notes about set title:
    -system("cls"); is used A LOT, as well as in all the other code. This merely clears the screen for aesthetic purposes
    -cprintf used A LOT. sets color and then prints a string
    -cprinti used later in the code, sets a color and then prints an integer. This is usually used to make selected options stand out amongst other text in the interface
    */


    int i,j=0; //misc counters, used all over the place
    while(!kbhit()) //loops while there is no keyboard input
    {
        system("cls");
        for(i=0;i<30;i++) //uses the counter "j" to make "bulbs light up" in a regular, linearily-changing fashion
        {
            if((i-j)%5==0)
            {
                cprintf(14,"O");
            }
            else
            {
                cprintf(6,"O");
            }
        }
        cprintf(15,"\n\n\tDMIRTY THEATRE\n\n");
        for(i=0;i<30;i++) //repeats bulbs lighting up on the bottom except going the other way to simulate a rotating feel
        {
            if((i+j)%5==0)
            {
                cprintf(14,"O");
            }
            else
            {
                cprintf(6,"O");
            }
        }
        cprintf(15,"\n\n      ENTER - CONTINUE\n         ESC - QUIT");
        Sleep(10); //10-millisecond pause
        j=(j+1)%5; //increments counter, if it is 5+, resets it to 0
    }
}

int selectMovie()
{
    /*
        notes for select movie:

        -this function follows a basic structure which 3 other functions will also follow including selectTime, selectNumPeople, and selectPersonAge:
            loop 1 (asks the user a question to be answered)
            {
                clear screen and display stuff
                take input
                switch statement dealing with input
                {
                    arrow keys move cursor
                    upon pressing enter, start loop 2 (confirm the user's selection) this loop has an exra condition of input not being enter
                    {
                        clear screen ad display stuff
                        take input
                        switch()
                        {
                            arrow keys move cursor
                            returns 1 (success) if selection is 1 (yes)
                        }
                    }
                    here, loop 2 stops if input is enter because it would have returned by now if the user had selected yes, therefore they selected no
                    loop 1 restarts after this
                }
                loop 1 stops if the user presses esc or backspace, returning a value of 0 (fail)
            }

        -Please keep this structure in mind as this code will not be commented as in-depthly from here on out
    */

    int i,j; //misc counters
    int selection=0; //this variable is used throughout the code in various functions toremember which option the user is on (as scanf is not used)
    char input; //remembers last character input
    do //main loop
    {
        //Display phase (prints stuff):
        system("cls");
        cprintf(3,"###################################################################################################################\n\n"); //prints the screen border
        cprintf(11,"   Please choose the movie you wish to view:\n\n");
        for(i=0;i<7;i++) //prints all the options in colors depending on hich is selected currently
        {
            if(i==selection)
            {
                cprintf(10," > ");
            }
            else
            {
                cprintf(15,"   ");
            }
            printf("%s ",movieNames[i]);
            cprintf(10+((movieAgeRestrictions[i]+1)/2)*4-(6*(movieAgeRestrictions[i]/3)),movieAgeRestrictionNames[movieAgeRestrictions[i]]); //an algorythm which determines a color based on the integer value corresponding to the age restriction
            printf("\n");
        }
        cprintf(11,"\n   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
        cprintf(3,"###################################################################################################################\n\n");
        //Input phase (takes in and uses input):
        input=getch();
        switch(input)
        {
            case 72: //up key - appropriately moves cursor
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80: //down key - appropriately moves cursor
                if(selection<6)
                {
                    selection++;
                }
                break;
            case 13: //enter
                movieChosen=selection; //stores selection moviechosen variable
                selection=0; //resets selection for second use
                do //a nested loop
                {
                    //Second display phase:
                    system("cls");
                    cprintf(3,"###################################################################################################################\n\n   ");
                    cprintf(15,movieNames[movieChosen]);
                    cprintf(14,"\n\n   Rating: ");
                    for(i=0;i<5;i++) //converts integer rating (0-100) into printed characters representing stars.
                    {
                        if(movieRatings[movieChosen]-(i*20)>=16)
                        {
                            printf("%c ",219);
                        }
                        else if(movieRatings[movieChosen]-(i*20)>=12)
                        {
                            printf("%c ",178);
                        }
                        else if(movieRatings[movieChosen]-(i*20)>=8)
                        {
                            printf("%c ",177);
                        }
                        else if(movieRatings[movieChosen]-(i*20)>=4)
                        {
                            printf("%c ",176);
                        }
                        else
                        {
                            printf("  ");
                        }
                    }
                    printf("(%i%%)\n\n   ",movieRatings[movieChosen]); //prints integer rating
                    cprintf(11,"Genre:");
                    for(i=0;i<5;i++) //prints a genre if he first character of the genre array is not null (0)
                    {
                        if(movieGenres[movieChosen][i][0]!=0)
                        {
                            if(i>0)
                            {
                                printf(" /");
                            }
                            printf(" %s",movieGenres[movieChosen][i]);
                        }
                    }
                    printf("\n\n   Description:\n   ");
                    borderprintf(3,60,movieDescriptions[movieChosen]); //prints description in paragraph form, 60 characters/line
                    if(movieAgeRestrictions[movieChosen]==3) //warns user of R rating
                    {
                        cprintf(12,"\n\n  This movie is rated R");
                    }
                    cprintf(11,"\n\n   Are you sure you wish to select this movie?\n\n");
                    if(selection==0)//prints selections (yes or no in this case so an if statement suffices)
                    {
                        cprintf(12," > No\n");
                        cprintf(15,"   Yes\n\n");
                    }
                    else
                    {
                        cprintf(15,"   No\n");
                        cprintf(10," > Yes\n\n");
                    }
                    cprintf(11,"   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
                    cprintf(3,"###################################################################################################################\n\n");
                    //Second input phase:
                    input=getch();
                    switch(input)
                    {
                        case 72: //up
                            if(selection>0)
                            {
                                selection--;
                            }
                            break;
                        case 80: //down
                            if(selection<1)
                            {
                                selection++;
                            }
                            break;
                        case 13: //enter
                            if(selection==1)
                            {
                                return 1;
                            }
                            break;
                    }
                }
                while(input!=8&&input!=27&&input!=13);
                selection=0; //reset selection
                input=0; //reset input so only one loop is broken from
                break;
        }
    }
    while(input!=27&&input!=8);
    return 0;
}

int selectTime()
{
    //variables same as usual
    int i,j;
    int selection=0;
    char input;

    do
    {
        //display
        system("cls");
        cprintf(3,"###################################################################################################################\n\n   ");
        cprintf(11,"Please select a time to view you movie:\n\n   ");
        for(i=0;i<5;i++) //carefully prints time options with how many seats are left and color denoting if any at all seats are left
        {
            if(1000-movieSeatsOccupied[movieChosen][i]==0&&selection==i)
            {
                cprintf(12," > ");
            }
            else if(1000-movieSeatsOccupied[movieChosen][i]==0)
            {
                cprintf(12,"   ");
            }
            else if(selection==i)
            {
                cprintf(10," > ");
            }
            else
            {
                cprintf(15,"   ");
            }
            printf("%s  [%i Seats Remaining]\n   ",movieTimeOptions[i],1000-movieSeatsOccupied[movieChosen][i]); //1000-seatsOccupied used to get the number of seats empty
        }
        cprintf(11,"\n   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
        cprintf(3,"###################################################################################################################\n\n   ");
        input=getch();
        switch(input)
        {
            case 72: //up
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80: //down
                if(selection<4)
                {
                    selection++;
                }
                break;
            case 13: //enter
                if(1000-movieSeatsOccupied[movieChosen][selection]!=0)
                {
                    timeChosen=selection;
                    selection=0;
                    do
                    {
                        //dislpay
                        system("cls");
                        cprintf(3,"###################################################################################################################\n\n   ");
                        cprintf(11,"You have selected ");
                        cprintf(15,movieTimeOptions[timeChosen]);
                        cprintf(11," for your viewing");
                        if(1000-movieSeatsOccupied[movieChosen][timeChosen]<=10) //warns if there are less than 10 seats left
                        {
                            cprintf(12,"\n   WARNING: Only ");
                            cprinti(14,1000-movieSeatsOccupied[movieChosen][timeChosen]);
                            cprintf(12," seats remaining for this movie and time");
                        }
                        cprintf(11,"\n   Please confirm this selection:\n\n");
                        if(selection==0)
                        {
                            cprintf(12," > No\n");
                            cprintf(15,"   Yes\n\n");
                        }
                        else
                        {
                            cprintf(15,"   No\n");
                            cprintf(10," > Yes\n\n");
                        }
                        cprintf(11,"   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
                        cprintf(3,"###################################################################################################################\n\n   ");
                        //input
                        input=getch();
                        switch(input)
                        {
                            case 72: //up
                                if(selection>0)
                                {
                                    selection--;
                                }
                                break;
                            case 80: //down
                                if(selection<1)
                                {
                                    selection++;
                                }
                                break;
                            case 13: //enter
                                if(selection==1)
                                {
                                    return 1;
                                }
                                break;
                        }
                    }
                    while(input!=27&&input!=8&&input!=13);
                    selection=0;
                    input=0;
                }
                break;
        }
    }
    while(input!=27&&input!=8);
    return 0;
}

int selectNumPeople()
{
     //variables same as usual
    int i,j;
    char input;
    int selection=0;

    do
    {
        //display
        system("cls");
        cprintf(3,"###################################################################################################################\n\n   ");
        cprintf(11,"Please select the number of people to participate in the viewing:\n\n");
        for(i=0;i<10;i++)//prints all possible numbers of people that can enter per ticket (max=10) with colors showing invalid options
        {
            if(1000-movieSeatsOccupied[movieChosen][timeChosen]<i+1&&selection==i)
            {
                cprintf(12," > ");
            }
            else if(1000-movieSeatsOccupied[movieChosen][timeChosen]<i+1)
            {
                cprintf(12,"   ");
            }
            else if(selection==i)
            {
                cprintf(10," > ");
            }
            else
            {
                cprintf(15,"   ");
            }
            printf("%i\n",i+1);
        }
        cprintf(11,"\n   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
        cprintf(3,"###################################################################################################################\n\n   ");
        //input
        input=getch();
        switch(input)
        {
            case 72: //up
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80: //down
                if(selection<1000-movieSeatsOccupied[movieChosen][timeChosen]-1)
                {
                    selection++;
                }
                break;
            case 13: //enter
                if(1000-movieSeatsOccupied[movieChosen][timeChosen]>=selection+1) //if the selection is invalid, nothing happens, there should be no way that an invalid option is selected but this is an easy precaution
                {
                    numPeople=selection+1;
                    selection=0;
                    do
                    {
                        //display
                        system("cls");
                        cprintf(3,"###################################################################################################################\n\n   ");
                        cprintf(11,"You have selected for ");
                        setColor(15);
                        printf("%i",numPeople);
                        cprintf(11," people to participate in this viewing\n   Please confirm this selection:\n\n");
                        if(selection==0)
                        {
                            cprintf(12," > No\n");
                            cprintf(15,"   Yes\n\n");
                        }
                        else
                        {
                            cprintf(15,"   No\n");
                            cprintf(10," > Yes\n\n");
                        }
                        cprintf(11,"\n   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
                        cprintf(3,"###################################################################################################################\n\n   ");
                        //input
                        input=getch();
                        switch(input)
                        {
                            case 72: //up
                                if(selection>0)
                                {
                                    selection--;
                                }
                                break;
                            case 80: //down
                                if(selection<1)
                                {
                                    selection++;
                                }
                                break;
                            case 13: //enter
                                if(selection==1)
                                {
                                    return 1;
                                }
                                break;
                        }
                    }
                    while(input!=27&&input!=8&&input!=13);
                    selection=0;
                    input=0;
                }
                break;
        }
    }
    while(input!=27&&input!=8);
    return 0;
}

int selectPersonAge(int pNum)
{
     //variables same as usual
    int i,j;
    char input;
    int selection=5*(movieAgeRestrictions[movieChosen]/3);

    do
    {
        //display
        system("cls");
        cprintf(3,"###################################################################################################################\n\n   ");
        cprintf(11,"Please select the age range of person ");
        setColor(15);
        printf("%i",pNum);
        cprintf(11,":\n\n");
        for(i=0;i<8;i++) //displays valid and invalid age options based on R rating
        {
            if(i<5*(movieAgeRestrictions[movieChosen]/3))
            {
                cprintf(12,"   ");
            }
            else if(selection==i)
            {
                cprintf(10," > ");
            }
            else
            {
                cprintf(15,"   ");
            }
            printf("%s\n",ageRanges[i]);
        }
        cprintf(11,"\n   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
        cprintf(3,"###################################################################################################################\n\n   ");
        //input
        input=getch();
        switch(input)
        {
            case 72: //up
                if(selection>5*(movieAgeRestrictions[movieChosen]/3)) //algorythm determines minimum selection value if R rating is present
                {
                    selection--;
                }
                break;
            case 80: //down
                if(selection<7)
                {
                    selection++;
                }
                break;
            case 13: //enter
                peopleRecordedAgeRanges[pNum-1]=selection;
                selection=0;
                do
                {
                    //display
                    system("cls");
                    cprintf(3,"###################################################################################################################\n\n   ");
                    cprintf(11,"You have selected ");
                    cprintf(15,ageRanges[peopleRecordedAgeRanges[pNum-1]]);
                    cprintf(11," for person ");
                    setColor(15);
                    printf("%i",pNum);
                    cprintf(11,"\n   Please confirm this selection:\n\n");
                    if(selection==0)
                    {
                        cprintf(12," > No\n");
                        cprintf(15,"   Yes\n\n");
                    }
                    else
                    {
                        cprintf(15,"   No\n");
                        cprintf(10," > Yes\n\n");
                    }
                    cprintf(11,"\n   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
                    cprintf(3,"###################################################################################################################\n\n   ");
                    //input
                    input=getch();
                    switch(input)
                    {
                        case 72: //up
                            if(selection>0)
                            {
                                selection--;
                            }
                            break;
                        case 80: //down
                            if(selection<1)
                            {
                                selection++;
                            }
                            break;
                        case 13: //enter
                            if(selection==1)
                            {
                                return 1;
                            }
                            break;
                    }
                }
                while(input!=27&&input!=8&&input!=13);
                selection=5*(movieAgeRestrictions[movieChosen]/3); //resets selection depending on the presence of an R rating
                input=0;
                break;
        }
    }
    while(input!=27&&input!=8);
    return 0;
}

int finalCheck()
{
     //variables same as usual
    int i,j;
    int selection=0;
    char input;

    do //this function only contains 1 loop which is very similar in structure to loop 2 as described in selectMovie()
    {
        //display
        system("cls");
        cprintf(3,"###################################################################################################################\n\n   ");
        //All of the variables are used to print out, in essence, a preview of the final ticket
        cprintf(11,"You have selected the following option for your viewing:\n   Movie: ");
        cprintf(15,movieNames[movieChosen]);
        cprintf(11,"\n   Time: ");
        cprintf(15,movieTimeOptions[timeChosen]);
        cprintf(11,"\n   Number of people viewing: ");
        cprinti(15,numPeople);
        cprintf(11," of the following respective age ranges:");
        for(i=0;i<numPeople;i++)
        {
            if(i%5==0)
            {
                printf("\n");
            }
            cprintf(15,"   ");
            printf("%s",ageRanges[peopleRecordedAgeRanges[i]]);
        }
        cprintf(13,"\n\n   Confirm this selection to complete your transaction:\n\n");
        if(selection==0)
        {
            cprintf(12," > No\n");
            cprintf(15,"   Yes\n\n");
        }
        else
        {
            cprintf(15,"   No\n");
            cprintf(10," > Yes\n\n");
        }
        cprintf(11,"\n   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
        cprintf(3,"###################################################################################################################\n\n   ");
        //input
        input=getch();
        switch(input)
        {
            case 72:
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80:
                if(selection<1)
                {
                    selection++;
                }
                break;
            case 13:
                if(selection==1)
                {
                    movieSeatsOccupied[movieChosen][timeChosen]+=numPeople; //IMPORTANT: adds new seats occupied
                    return 1;
                }
                break;
        }
    }
    while(input!=27&&input!=8&&input!=13);
    return 0;
}

void enterCode()
{
    int i,j; //misc counters
    char input;
    //These Variables are used to basically remake the scanf function with the feature of having  character limit and to only take in letter and numbers, no wired characters:
    int placeInString; //remembers you place in the string you are typing
    char fullInput[6]; //remembers full dtring you are typing
    fullInput[5]=0; //sets last element of the string above to 0 so strcmp finds the string's end and works

    for(i=0;i<3;i++) //gives you 3 tries to enter code correctly
    {
        placeInString=0; //resets place in string
        for(j=0;j<5;j++) //resets full string
        {
            fullInput[j]=0;
        }
        do
        {
            //display
            system("cls");
            cprintf(3,"###################################################################################################################\n\n   ");
            cprintf(11,"Enter the code found in the code header for free popcorn [You have ");
            cprinti(15,3-i); //number of tries left
            cprintf(11," tries left]:\n\n   ");
            setColor(15);
            for(j=0;j<5;j++) //prints string
            {
                printf("%c",fullInput[j]);
            }
            cprintf(11,"\n\n   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
            cprintf(3,"###################################################################################################################\n\n   ");
            //input
            input=getch();
            if(input==8) //if the input is a backspace, place instring returns to previous one and deletes that character
            {
                if(placeInString>0)
                {
                    placeInString--;
                    fullInput[placeInString]=0;
                }
            }
            else if((input>=65&&input<=90)||(input>=48&&input<=57)||(input>=97&&input<=122)) //these are he ascii character ranges corresponding to capitals, lowercases, and numbers
            {
                if(placeInString<5)
                {
                    fullInput[placeInString]=input;
                    placeInString++;
                }
            }
        }
        while(input!=27&&input!=13); //loop breaks if escape or enter are entered
        if(input==27) //entire for loop breaks if escape is hit (you don't want to redeem your code)
        {
            codeRedeemed=0;
            return 0;
        }
        if(strcmp(fullInput,"ICS3U")==0) //if you succeed it gives you a message, sets codeRedeemed to 1 and returns 1
        {
            system("cls");
            cprintf(3,"###################################################################################################################\n\n   ");
            cprintf(10,"Code redeemed :D\n\n");
            cprintf(3,"###################################################################################################################\n\n   ");
            getch();
            codeRedeemed=1;
            return 1;
        }
    } //Upon the end of the for loop, it prints a message that the code was not redeemed successfully and then returns 0
    system("cls");
    cprintf(3,"###################################################################################################################\n\n   ");
    cprintf(12,"Sorry, you were not able to redeem your code   :(\n\n");
    cprintf(3,"###################################################################################################################\n\n   ");
    getch();
    codeRedeemed=0;
    return 0;
}

void printReceipt()
{
    int i; //misc counter
    char numberWords[10][10]={"ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE","TEN"}; //an array of all the numbers 1-10 in words for fanciness purposes
    int calculatedPrice=0; //calculated price (exclusive to this function)
    for(i=0;i<numPeople;i++) //price is calculated here based on the ages of people and what prices correspond with that
    {
        calculatedPrice+=ageRangePrices[peopleRecordedAgeRanges[i]];
    }
    for(i=0;i<3;i++) // fancy little fake loading screen, useful if this ticket was physical
    {
        system("cls");
        cprintf(3,"###################################################################################################################\n\n\n\n###################################################################################################################\n\n   ");
        Sleep(500);
        system("cls");
        cprintf(3,"###################################################################################################################\n\n   ");
        cprintf(14,"Printing Receipt...\n\n");
        cprintf(3,"###################################################################################################################\n\n   ");
        Sleep(500);
    }
    //The final receipt is printed in all its glory!
    system("cls");
    cprintf(3,"###################################################################################################################\n\n   ");
    cprintf(11,"Vaild Oct 21\n\n   ");
    cprintf(14,"DMITRY THEATRE\n");
    cprintf(15,"   ADMIT ");
    cprintf(15,numberWords[numPeople-1]);
    printf("\n\n   ");
    cprintf(15,movieNames[movieChosen]);
    printf(" ");
    cprintf(10+((movieAgeRestrictions[movieChosen]+1)/2)*4-(6*(movieAgeRestrictions[movieChosen]/3)),movieAgeRestrictionNames[movieAgeRestrictions[movieChosen]]); //same algorythm which controls color based on age rating
    printf("\n   ");
    cprintf(15,movieTimeOptions[timeChosen]);
    cprintf(11,"\n\n   Cost: $");
    cprinti(15,calculatedPrice);
    if(codeRedeemed==1) //if the code was redeemed, free popcorn!!!!
    {
        cprintf(10,"\n\n   FREE POPCORN");
    }
    printf("\n\n");
    cprintf(3,"###################################################################################################################\n\n   ");
    getch();
}

//borrowed code for changing color of individual characters in the console
//full credit may be found above
void setColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
      wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
      SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}
// end of borrowed code

void cprintf(int c,char*s) //saves a lot of lines of code in display, arguements are color then string
{
    //self-explanatory
    setColor(c);
    printf(s);
}

void cprinti(int c,int i) //saves some of lines of code in display, arguements are color then integer
{
    //self-explanatory
    setColor(c);
    printf("%i",i);
}

void borderprintf(int indent,int size,char*s) //arguements are indent size, line size, and the string itself
{
    int i=0; //misc counters
    int j;

    while(s[i]!=0)//stops when a 0 is hit (end)
    {
        printf("%c",s[i]); //prints the character
        if((i+1)%size==0)
        {
            if(s[i]!=32) //prints dash if needbe
            {
                printf("-");
            }
            printf("\n"); //prints enter
            for(j=0;j<indent;j++)//prints indent
            {
                printf(" ");
            }
        }
        i++; //counter incremented
    }
}
