#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<dos.h>

void setColor();

int chooseMovie();
int chooseTime();
int chooseNumPpl();
int chooseAge();
int enterCode();

int movieChosen;
int timeChosen;
int numPeople;
int price;

char movieNames[7][65]={"Roblox: The Oofening","Blue-Shifting Documentary: Featuring Morgan Freeman JPegs","1 Hour, 37 Minutes Of The Kahoot Soundtrack","The Fidget Spinner Movie","Minecraft Let's Play Ep.346 - Carbon Tax Update","Bonobo's Bizarre Adventures","Hacking The Russian Gouvernment in 0.5 A-Presses"};

int seatsOccupied[7][5]={0};

int main(void)
{
    char input;
    int i,j;
    setColor(15);

    do
    {
        j=0;
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
            printf("\n\n\tMOVIE THEATRE\n\n");
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
            printf("\n\n\tPRESS ANY KEY");
            Sleep(10);
            j=(j+1)%5;
        }
        input=getch();
        chooseMovie();
    }
    while(input!=27);
    return 0;
}

int chooseMovie()
{
    int i,j;
    int selection=0;
    char input;
    do
    {
        system("cls");
        setColor(3);
        printf("################################################################################\n\n");
        setColor(11);
        printf("   Please select the movie you wish to view\n\n");
        for(i=0;i<7;i++)
        {
            if(i==selection)
            {
                setColor(10);
                printf(" > ");
            }
            else
            {
                setColor(15);
                printf("   ");
            }
            for(j=0;j<65;j++)
            {
                if(movieNames[i][j]!=0)
                {
                    printf("%c",movieNames[i][j]);
                }
                else
                {
                    printf(" ");
                }
            }
            printf("\n");

        }
        setColor(11);
        printf("\n   Arrow Keys - Move Cursor\n   Backspace - Back\n");
        setColor(3);
        printf("\n################################################################################\n");
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
                if(selection<6)
                {
                    selection++;
                }
                break;
            case 13:
                movieChosen=selection;
                selection=0;
                do
                {
                    system("cls");
                    setColor(3);
                    printf("################################################################################\n\n");
                    setColor(11);
                    printf("   You have selected the movie: ");
                    setColor(15);
                    printf("%s\n",movieNames[movieChosen]);
                    setColor(11);
                    printf("   Are you sure you wish to select this movie?\n\n");
                    if(selection==0)
                    {
                        setColor(12);
                        printf(" > No\n");
                        setColor(15);
                        printf("   Yes\n\n");
                    }
                    else
                    {
                        setColor(15);
                        printf("   No\n");
                        setColor(10);
                        printf(" > Yes\n\n");
                    }
                    setColor(11);
                    printf("   Arrow Keys - Move Cursor\n   Enter - Select Option\n\n");
                    setColor(3);
                    printf("################################################################################\n\n");
                    input=getch();
                    switch(input)
                    {
                        case 72:
                            if(selection==1)
                            {
                                selection=0;
                            }
                            break;
                        case 80:
                            if(selection==0)
                            {
                                selection=1;
                            }
                            break;
                    }
                }
                while(input!=13);
                if(selection==1)
                {
                    return 1;
                }
                input=0;
                selection=0;
                break;
        }
    }
    while(input!=8);
    return 0;
}

int chooseTime()
{
    int i;
    int selection=0;
    char input;
    do
    {
        system("cls");
        setColor(3);
        printf("############################################################\n\n");
        setColor(11);
        printf("   Please select the time at which you wish to view your movie\n\n");
        for(i=0;i<7;i++)
        {
            if(i==selection)
            {
                setColor(10);
                printf(" > ");
            }
            else
            {
                setColor(15);
                printf("   ");
            }
            printf("%s\n",movieNames[i]);
        }
        setColor(11);
        printf("\n   Arrow Keys - Move Cursor\n   Backspace - Back\n");
        setColor(3);
        printf("\n############################################################\n");
        input=getch();
        switch(input)
        {

        }
    }
    while(input!=8);
    return 0;
}

int chooseNumPpl()
{

    return 0;
}

int chooseAge()
{

    return 0;
}

int enterCode()
{

    return 0;
}

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
