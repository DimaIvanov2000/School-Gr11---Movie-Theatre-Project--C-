/*

Movie Ticket Purchase Interface Program
Written by Dima Ivanov
Date submitted: Oct 20/21 , 2017

Color-changing code supplied by online user Ashish Ahuja. Full code can be found at:
https://stackoverflow.com/questions/29574849/how-to-change-text-color-and-console-color-in-codeblocks/29575262#29575262

Code for unlocking free popcorn is ICS3U

*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#include<time.h>

//functions:
void loadMovieInfo(); //loads information about movies including name, description, genres and rating; this exists so that editing movies is easier via the text editor
void showTitle(); //displays the main title
int selectMovie(); //allows user to select a movie, including previews of the film and seat availability
int selectTime(); //allows the user to select a time to view their movie which has not yet been occupied
int selectNumPeople(); //allows user to select an appropriate number of people to view the movie
int selectPersonAge(); //allows user to select the age of an individual person + whether they are a student
int finalCheck();
void enterCode(); //allows user to enter code for free popcorn
void printReceipt(); //prints the receipt all fancy-like using the variables below
void setColor(); //sets text color for aesthetic purposes
void cprintf(); //sets color and then prints text
void cprinti(); //sets color and then prints an integer
void borderprintf(); //prints text in paragraph format to look nicer

int movieChosen; // number of the movie user has chosen
int timeChosen; // number of the time user has chosen (not the time itself, that would be a waste of memory
int numPeople; // number of people user has selected to buy tickets for
int numCurrentPerson; // counter remembering the current person the user is inputing the age of
int peopleRecordedAgeRanges[10];
int price; // total price for user, calculated over the course of one "run" through the program
int codeRedeemed;

char movieNames[7][65]; //string array of movie names to be retrieved from txt document
int movieRatings[7]={0};
char movieGenres[7][5][20]={0};
char movieDescriptions[7][500]={0};
int movieSeatsOccupied[7][5]={0};
char movieTimeOptions[5][10]={"4:00pm ","5:30pm ","7:00pm ","8:30pm ","10:00pm"};
char ageRanges[6][25]={"<3 Years","3-13 Years","3-13 Years [Student]","14-64 Years","14-64 Years [Student]","65+ Years"};

int main(void)
{
    char input;
    int canExit;
    loadMovieInfo();
    while(1)
    {
        do
        {
            showTitle();
            input=getch();
        }
        while(input!=27&&input!=13);
        if(input==27)
        {
            system("cls");
            cprintf(14,"\nThank you for coming!\n");
            setColor(15);
            break;
        }
        canExit=0;
        while(canExit==0&&selectMovie()==1)
        {
            while(canExit==0&&selectTime()==1)
            {
                while(canExit==0&&selectNumPeople()==1)
                {
                    numCurrentPerson=1;
                    while(numCurrentPerson>0&&numCurrentPerson<=numPeople&&canExit==0)
                    {
                        if(selectPersonAge(numCurrentPerson)==1)
                        {
                            numCurrentPerson++;
                        }
                        else
                        {
                            numCurrentPerson--;
                        }
                        if(numCurrentPerson>numPeople)
                        {
                            while(finalCheck()==1)
                            {
                                enterCode();
                                printReceipt();
                                canExit=1;
                                break;
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
    int i=0,j=0,k=0;
    int c;

    FILE*info=fopen("movieInfo.txt","r");
    //47
    do
    {
        c=fgetc(info);
        if(c!=47)
        {
            for(j=0;j<65;j++)
            {
                movieNames[i][j]=32;
            }
            j=0;
            while(c!=10)
            {
                movieNames[i][j]=c;
                j++;
                c=fgetc(info);
            }
            movieNames[i][j]=0;
            for(j=2;j>=0;j--)
            {
                movieRatings[i]+=(fgetc(info)-48)*(int)pow(10,(double)j);
            }
            c=fgetc(info);
            j=0;
            do
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
            j=0;
            c=fgetc(info);
            while(c!=10)
            {
                movieDescriptions[i][j]=c;
                j++;
                c=fgetc(info);
            }
            c=fgetc(info);
            i++;
        }
    }
    while(c!=47);
    fclose(info);

//    for(i=0;i<7;i++)
//    {
//        printf("%i\n\n",movieRatings[i]);
//    }
}

void showTitle()
{
    int i,j=0;
    while(!kbhit())
    {
        system("cls");
        for(i=0;i<30;i++)
        {
            if((i-j)%5==0)
            {
                setColor(14);
            }
            else
            {
                setColor(6);
            }
            printf("O");
        }
        setColor(15);
        printf("\n\n\tDMIRTY THEATRE\n\n");
        for(i=0;i<30;i++)
        {
             if((i+j)%5==0)
            {
                setColor(14);
            }
            else
            {
                setColor(6);
            }
            printf("O");
        }
        setColor(15);
        printf("\n\n      ENTER - CONTINUE\n         ESC - QUIT");
        Sleep(10);
        j=(j+1)%5;
    }
}

int selectMovie()
{
    int i,j;
    int selection=0;
    char input;
    do
    {
        system("cls");
        cprintf(3,"###################################################################################################################\n\n");
        cprintf(11,"   Please choose the movie you wish to view:\n\n");
        for(i=0;i<7;i++)
        {
            if(i==selection)
            {
                cprintf(10," > ");
            }
            else
            {
                cprintf(15,"   ");
            }
            printf("%s\n",movieNames[i]);
        }
        cprintf(11,"\n   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
        cprintf(3,"###################################################################################################################\n\n");
        input=getch();
        switch(input)
        {
            case 72: //up key
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80: //down key
                if(selection<6)
                {
                    selection++;
                }
                break;
            case 13: //enter
                movieChosen=selection;
                selection=0;
                do
                {
                    system("cls");
                    cprintf(3,"###################################################################################################################\n\n   ");
                    cprintf(15,movieNames[movieChosen]);
                    cprintf(14,"\n\n   Rating: ");
                    for(i=0;i<5;i++)
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
                    printf("(%i%%)\n\n   ",movieRatings[movieChosen]);
                    cprintf(11,"Genre:");
                    for(i=0;i<5;i++)
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
                    borderprintf(3,60,movieDescriptions[movieChosen]);
                    printf("\n\n   Are you sure you wish to select this movie?\n\n");
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
                    cprintf(3,"###################################################################################################################\n\n");
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
                                return 1;
                            }
                            break;
                    }
                }
                while(input!=8&&input!=27&&input!=13);
                selection=0;
                input=0;
                break;
        }
    }
    while(input!=27&&input!=8);
    return 0;
}

int selectTime()
{
    int i,j;
    int selection=0;
    char input;
    do
    {
        system("cls");
        cprintf(3,"###################################################################################################################\n\n   ");
        cprintf(11,"Please select a time to view you movie:\n\n   ");
        for(i=0;i<5;i++)
        {
            if(selection==i)
            {
                cprintf(10," > ");
            }
            else
            {
                cprintf(15,"   ");
            }
            printf("%s\n   ",movieTimeOptions[i]);
        }
        cprintf(11,"\n   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
        cprintf(3,"###################################################################################################################\n\n   ");
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
                if(selection<4)
                {
                    selection++;
                }
                break;
            case 13:
                timeChosen=selection;
                selection=0;
                do
                {
                    system("cls");
                    cprintf(3,"###################################################################################################################\n\n   ");
                    cprintf(11,"You have selected ");
                    cprintf(15,movieTimeOptions[timeChosen]);
                    cprintf(11," for your viewing");
                    if(movieSeatsOccupied[movieChosen][timeChosen]<=10)
                    {
                        cprintf(12,"\n   WARNING: Only ");
                        setColor(14);
                        printf("%i",1000-movieSeatsOccupied[movieChosen][timeChosen]);
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
                                return 1;
                            }
                            break;
                    }
                }
                while(input!=27&&input!=8&&input!=13);
                selection=0;
                input=0;
                break;
        }
    }
    while(input!=27&&input!=8);
    return 0;
}

int selectNumPeople()
{
    int i,j;
    char input;
    int selection=0;
    do
    {
        system("cls");
        cprintf(3,"###################################################################################################################\n\n   ");
        cprintf(11,"Please select the number of people to participate in the viewing:\n\n");
        for(i=0;i<10;i++)
        {
            if(selection==i)
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
        input=getch();
        switch(input)
        {
            case 72: //up key
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80: //down key
                if(selection<9)
                {
                    selection++;
                }
                break;
            case 13: //enter
                numPeople=selection+1;
                selection=0;
                do
                {
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
                                return 1;
                            }
                            break;
                    }
                }
                while(input!=27&&input!=8&&input!=13);
                selection=0;
                input=0;
                break;
        }
    }
    while(input!=27&&input!=8);
    return 0;
}

int selectPersonAge(int pNum)
{
    int i,j;
    char input;
    int selection=0;
    do
    {
        system("cls");
        cprintf(3,"###################################################################################################################\n\n   ");
        cprintf(11,"Please select the age range of person ");
        setColor(15);
        printf("%i",pNum);
        cprintf(11,":\n\n");
        for(i=0;i<6;i++)
        {
            if(selection==i)
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
        input=getch();
        switch(input)
        {
            case 72: //up key
                if(selection>0)
                {
                    selection--;
                }
                break;
            case 80: //down key
                if(selection<5)
                {
                    selection++;
                }
                break;
            case 13:
                peopleRecordedAgeRanges[pNum-1]=selection;
                selection=0;
                do
                {
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
                                return 1;
                            }
                            break;
                    }
                }
                while(input!=27&&input!=8&&input!=13);
                selection=0;
                input=0;
                break;
        }
    }
    while(input!=27&&input!=8);
    return 0;
}

int finalCheck()
{
    int i,j;
    int selection=0;
    char input;
    do
    {
        system("cls");
        cprintf(3,"###################################################################################################################\n\n   ");
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
    int i,j;
    char input;
    int placeInString;
    char fullInput[6];
    fullInput[5]=0;
    for(i=0;i<3;i++)
    {
        placeInString=0;
        for(j=0;j<5;j++)
        {
            fullInput[j]=0;
        }
        do
        {
            system("cls");
            cprintf(3,"###################################################################################################################\n\n   ");
            cprintf(11,"Enter the code found in the code header for free popcorn [You have ");
            cprinti(15,3-i);
            cprintf(11," tries left]:\n\n   ");
            setColor(15);
            for(j=0;j<5;j++)
            {
                printf("%c",fullInput[j]);
            }
            cprintf(11,"\n\n   Arrow Keys - Move Cursor\n   Enter - Select Option\n   Backspace/Esc - Back\n\n");
            cprintf(3,"###################################################################################################################\n\n   ");
            input=getch();
            if(input==8)
            {
                if(placeInString>0)
                {
                    placeInString--;
                    fullInput[placeInString]=0;
                }
            }
            else if((input>=65&&input<=90)||(input>=48&&input<=57)||(input>=97&&input<=122))
            {
                if(placeInString<5)
                {
                    fullInput[placeInString]=input;
                    placeInString++;
                }
            }
        }
        while(input!=27&&input!=13);
        if(input==27)
        {
            codeRedeemed=0;
            return 0;
        }
        if(strcmp(fullInput,"ICS3U")==0)
        {
            system("cls");
            cprintf(3,"###################################################################################################################\n\n   ");
            cprintf(10,"Code redeemed :D\n\n");
            cprintf(3,"###################################################################################################################\n\n   ");
            getch();
            codeRedeemed=1;
            return 1;
        }
    }
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
    int i;
    char numberWords[10][10]={"ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE","TEN"};
    for(i=0;i<3;i++)
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
    system("cls");
    cprintf(3,"###################################################################################################################\n\n   ");
    cprintf(14,"   DMITRY THEATRE\n");
    cprintf(15,"   ADMIT ");
    cprintf(15,numberWords[numPeople-1]);
    //cprintf
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

void cprintf(int c,char*s)
{
    setColor(c);
    printf(s);
}

void cprinti(int c,int i)
{
    setColor(c);
    printf("%i",i);
}

void borderprintf(int indent,int size,char*s)
{
    int i=0;
    int j;
    while(s[i]!=0)
    {
        printf("%c",s[i]);
        if((i+1)%size==0)
        {
            if(s[i]!=32)
            {
                printf("-");
            }
            printf("\n");
            for(j=0;j<indent;j++)
            {
                printf(" ");
            }
        }
        i++;
    }
}
