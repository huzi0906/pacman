# include <iostream>
# include <windows.h>   //For colors
# include <conio.h>     //To get input
# include <ctime>
# include <iomanip>

using namespace std;

typedef enum {black,blue,green,cyan,red,magenta,yellow,white,gray,lightblue,lightgreen,lightcyan,lightred,lightmagenta,lightyellow,lightwhite};

//Functions Prototype
void start();
void gotoxy (int,int);
void setColor(int,int);
void startmenu();
void game();
void printmap(char[][41]);
void ghostmove(int&,int&,int,char&,char[][41]);
void pacmove(int&,int&,int&,char[][41],char);
bool collision(int,int,int&,int&);
void gameover();
void youwin();

void gotoxy (int x, int y)
{
    COORD c;
    c.X = x; c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setColor(int textColor, int bgColor) 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor + (bgColor * 16)));
}
 
int main()
{
	start();
	Sleep(1000);
	//call to startmenu
	startmenu();
	system("CLS");
	return 0;
}

void start()
{
	string over[6]={
	"000000     0     000000        0     0    0    0      0",
	"0     0   0 0   0              00   00   0 0   0 0    0",
	"0     0  00000  0       00000  0 0 0 0  00000  0  0   0",
	"000000   0   0  0       00000  0  0  0  0   0  0   0  0",
	"0        0   0  0              0     0  0   0  0    0 0",
	"0        0   0   000000        0     0  0   0  0      0"
    };
    for (int i=0;i<6;i++)
    {
    	for (int j=0;j<55;j++)
    	{
    		if (over[i][j]=='0')
    	        setColor(yellow,yellow);
    	    cout<<over[i][j];
    	    setColor(white,black);
		}
        cout<<endl;
        Sleep(100);
	}
}

//startmenu
void startmenu()
{
	//To give a start up menu with choices of new-game and quitting and instructions
	char option='\0';
	int flag=0;
	
	//Instructions
	cout<<setw(15)<<"INSTRUCTIONS"<<endl;
	cout<<"1. Use w,a,s,d to move"<<endl;
	cout<<"2. Eat pellets to increase your score"<<endl;
	cout<<"3. Eat all pellets to win the game"<<endl;
	cout<<"4. There are three ghosts"<<endl;
	cout<<"5. Avoid them to be alive"<<endl<<endl;
	
	//Welcome screen
	cout<<setw(15)<<"WELCOME"<<endl;
	cout<<"a) START GAME "<<endl;
	cout<<"b) QUIT "<<endl;
	cout<<"Please enter your choice"<<endl;
	while (flag!=1)
	{
		cin>>option;
		switch (option)
	    {
		case 'a':
		case 'A':
		case '1':
			system("CLS");
			game();
			flag=1;
			break;
		case 'b':
		case 'B':
		case '2':
			flag=1;
			return;
			break;
		default :
			cout<<"Please try again"<<endl;
			break;
	    }  
	}
	return;
}

//main game
void game()
{
	int score=0;
	char move=' ';
	// Array used as map
	char map[23][41]={
		                {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'},
		                {'0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0'},
		                {'0',' ','.',' ','0','0','0','0','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0'},
		                {'0',' ','.',' ','0','0','0','0','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0'},
		                {'0',' ','.',' ','0','0','0','0','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0'},
		                {'0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0'},
		                {'0',' ','.',' ','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0'},
		                {'0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0',' ','.',' ','.',' ','.',' ','0',' ','.',' ','.',' ','.',' ','0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0'},
		                {'0','0','0','0','0','0','0','0','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0','0','0','0','0','0','0','0','0'},
		                {' ',' ',' ',' ',' ',' ',' ',' ','0',' ','.',' ','0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0',' ','.',' ','0',' ',' ',' ',' ',' ',' ',' ',' '},
		                {'0','0','0','0','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','_','_','_','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0','0','0','0','0'},
		                {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','.',' ','.',' ','.',' ','0',' ',' ',' ',' ',' ',' ',' ','0',' ','.',' ','.',' ','.',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
					    {'0','0','0','0','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0','0','0','0','0'},
					    {' ',' ',' ',' ',' ',' ',' ',' ','0',' ','.',' ','0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0',' ','.',' ','0',' ',' ',' ',' ',' ',' ',' ',' '},
					    {'0','0','0','0','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0','0','0','0','0'},
					    {'0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0'},
		                {'0',' ','.',' ','0','0','0','0','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0','0','0','0','0',' ','.',' ','0'},
		                {'0',' ','.',' ','.',' ','.',' ','0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0',' ','.',' ','.',' ','.',' ','0'},
		                {'0','0','0','0','0',' ','.',' ','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0'},
		                {'0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0',' ','.',' ','.',' ','.',' ','0',' ','.',' ','.',' ','.',' ','0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0'},
		                {'0',' ','.',' ','0','0','0','0','0','0','0','0','0','0','0','0','0',' ','.',' ','0',' ','.',' ','0','0','0','0','0','0','0','0','0','0','0','0','0',' ','.',' ','0'},
		                {'0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','.',' ','0'},
		                {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'}
					  };
	srand(time(0)); //to get random values everytime
	int end=0;
	int i=17,j=20; //intial position of pacman
	int gx1=11,gy1=20,g1=0; //intial position of ghost 1
	int gx2=11,gy2=18,g2=0; //intial position of ghost 2
	int gx3=11,gy3=22,g3=0; //intial position of ghost 3
	char temp1='\0',temp2='\0',temp3='\0';				  
	while (true) //so that game always keep running
	{
		map[i][j]=char(258); // character for pacman
		map[gx1][gy1]=char(220); //character for ghosts
		map[gx2][gy2]=char(220); //character for ghosts
		map[gx3][gy3]=char(220); //character for ghosts
		cout<<"Score is: "<<score<<endl;

        printmap(map); //function to print map
		move = _getch(); //to get input
		
		g1=(rand()%4)+1; // generating random number for ghost movement
		g2=(rand()%4)+1; // generating random number for ghost movement
		g3=(rand()%4)+1; // generating random number for ghost movement
		
		ghostmove(gx1,gy1,g1,temp1,map); //movement of ghost 1
		ghostmove(gx2,gy2,g2,temp2,map); //movement of ghost 2
		ghostmove(gx3,gy3,g3,temp3,map); //movement of ghost 3

        pacmove(i,j,score,map,move); //movement of pacman
        
		system("CLS"); //screen clearing
		
		if (move==char(27)) // break when esc
			return;
				
		//to see if collision
	  	if (collision(i,j,gx1,gy1) || collision(i,j,gx2,gy2) || collision(i,j,gx3,gy3))
		{
			end=1;
			break;
		}
		
		//if all palletes are eaten
		if (score==198)
		    end=2;
		if (end==2)
		    break;
	}
	
	Sleep(1000);
	
	// Calling gameover function
	if (end==1)
	{
		gotoxy(0,10);
		gameover();	
	}
	// Calling youwin function
	else if (end==2)
	{
		gotoxy(0,10);
		youwin();
	}
	
	Sleep(1000);
	gotoxy(20,18);
	cout<<"Your score is : "<<score;
	
	Sleep(5000);			
}

//for printing map again and again
void printmap(char map[][41])
{
	int x=1;
		for (int k=0;k<23;k++)
		{
			for (int l=0;l<41;l++)
			{
				if (map[k][l]=='0')
				    setColor(cyan,cyan);
				else if (map[k][l]==' ')
				    setColor(yellow,white);
				else if (map[k][l]=='.')
				    setColor(yellow,white);
				else if (map[k][l]==char(258))
				    setColor(yellow,white);
				else if (map[k][l]==char(220))
				{
					x++;
					if (x%3==1)
					    setColor(red,white);
					else if (x%3==2)
					    setColor(blue,white);
					else if (x%3==0)
					    setColor(green,white);
				}
				else if (map[k][l]=='_')
				    setColor(cyan,white);
				cout<<map[k][l];
				setColor(white,black);
			}
			cout<<endl;
	    }
	    setColor(white,black);	
}

//for ghosts movement
void ghostmove(int &gx, int &gy, int g, char &temp, char map[][41])
{
	    //ghost out from home
	    if (gx==11 && gy==20)
		{
			map[gx][gy]=' ';
		    gx-=2;	
		}
		
		//ghost teleportation
		if (gx==11 && gy==40)
		{
			map[gx][gy]=' ';
			gx=11;gy=2;
		}
		if (gx==11 && gy==0)
		{
			map[gx][gy]=' ';
			gx=11;gy=38;
		}
		
		//movement of ghost
		switch (g)
		{
			case 1:
				if (map[gx-1][gy]!='0' && map[gx-1][gy]!='_' && map[gx-1][gy]!=char(220))
				{
					temp=map[gx-1][gy];
					gx--;
					map[gx+1][gy]=temp;
				}
				break;
			case 2:
				if (map[gx+1][gy]!='0' && map[gx+1][gy]!='_' && map[gx+1][gy]!=char(220))
				{
					temp=map[gx+1][gy];
					gx++;
					map[gx-1][gy]=temp;
				}
				break;
			case 3:
				if (map[gx][gy+2]!='0' && map[gx][gy+2]!='_' && map[gx][gy+2]!=char(220))
				{
					temp=map[gx][gy+2];
					gy+=2;
					map[gx][gy-2]=temp;
				}
				break;
			case 4:
				if (map[gx][gy-2]!='0' && map[gx][gy-2]!='_' && map[gx][gy-2]!=char(220))
				{
					temp=map[gx][gy-2];
					gy-=2;
					map[gx][gy+2]=temp;
				}
				break;
		}
}

//for pacman movement
void pacmove(int &i, int &j,int &score, char map[][41], char move)
{
	    //pacman teleportation
	    if (i==11 && j==40)
		{
			map[i][j]=' ';
			i=11;j=2;
		}
		if (i==11 && j==0)
		{
			map[i][j]=' ';
			i=11;j=38;
		}
		
	    //pacman movement
		switch (move)
		{
			case 'w':
			case 'W':
				if (map[i-1][j]!='0' && map[i-1][j]!='_')
				{
					i--;
					if (map[i][j]=='.')
					    score++;
					map[i+1][j]=' ';
				}	
				break;
			case 'a':
			case 'A':
				if (map[i][j-2]!='0' && map[i][j-2]!='_')
				{
					j-=2;
					if (map[i][j]=='.')
					    score++;
					map[i][j+2]=' ';
				}
				break;
			case 'd':
			case 'D':
				if (map[i][j+2]!='0' && map[i][j+2]!='_')
				{
					j+=2;
					if (map[i][j]=='.')
					    score++;
					map[i][j-2]=' ';
				}
				break;
			case 's':
			case 'S':
				if (map[i+1][j]!='0' && map[i+1][j]!='_')
				{
					i++;
					if (map[i][j]=='.')
					    score++;
					map[i-1][j]=' ';
				}
				break;
		}
}

//To check for collision
bool collision(int i, int j, int &gx, int &gy)
{
	if (( i==gx ||  i==(gx+1) || i==(gx-1) ) && ( j==gy || j==(gy+2) || j==(gy-2) ))
	    return 1;
	return 0; 
}

//To display gameover screen
void gameover()
{
	string over[6]={
	" 00000     0    0     0 0000000      00000  0     0 0000000 000000 ",
	"0     0   0 0   00   00 0           0     0 0     0 0       0    0 ",
	"0        00000  0 0 0 0 0000000     0     0  0   0  0000000 000000 ",
	"0   000  0   0  0  0  0 0000000     0     0  0   0  0000000 0 0    ",
	"0     0  0   0  0     0 0           0     0   0 0   0       0  0   ",
	" 00000   0   0  0     0 0000000      00000     0    0000000 0   0  "
    };
    for (int i=0;i<6;i++)
    {
    	for (int j=0;j<67;j++)
    	{
    		if (over[i][j]=='0')
    	        setColor(red,red);
    	    cout<<over[i][j];
    	    setColor(white,black);
		}
        cout<<endl;
        Sleep(100);
	}
}

//To display you win screen
void youwin()
{
	string win[6]={
	"0     0  00000  0     0     0     0 0000000 0      0",
	" 0   0  0     0 0     0     0     0    0    0 0    0",
	"  0 0   0     0 0     0     0  0  0    0    0  0   0",
	"   0    0     0 0     0     0 0 0 0    0    0   0  0",
 	"   0    0     0 0     0     00   00    0    0    0 0",
	"   0     00000   00000      0     0 0000000 0      0"
    };
    for (int i=0;i<6;i++)
    {
    	for (int j=0;j<52;j++)
    	{
    		if (win[i][j]=='0')
    	        setColor(green,green);
    	    cout<<win[i][j];
    	    setColor(white,black);
		}
        cout<<endl;
        Sleep(100);
	}
}   

