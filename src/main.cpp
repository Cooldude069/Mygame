#include<bits/stdc++.h>
#include"libs/conio.h"
#include<string>
#include<chrono>
#include<thread>
#include"libs/mob.h"
#include"libs/shooter.h"
using namespace std;

class Game : public KillerPiller{
    public:
    //Speed is for the speed of the mob as compared to the user.
    Game(int speed){
        mobSpeed = speed;
        generateMap();
        //bool restart will tell if the game has to be restarted or not.
        while(restart){
            //Reassigning default values
            a[0] = 9;
            a[1] = 5;
            asteroids[0] = -1;
            asteroids[1] = -1;
            asteroids[2] = 3;
            generateMap();
        }
    }
    Gun blaster; // Declaring the blaster/Gun from shooter.h
    int mobSpeed; //speed of the mob relative to the user.
    int a[2] = {9 , 5}; // represents the coordinates of the user.
    string direction = "f"; // Tells the direction of the user. f == forward , b == backward , l == left , r == right. (Lowercase is important)
    bool showScore = false;// Tell if the score should be displayed or not.
    int score = 0; // The score of the user
    int alternate = 0; // A temporary integer to modulate the speed of the mob. It changes its from 0 to the mobSpeed by 1 every frame therefore the speed of the mob decreases.
    //As the mob moves only when alternate is 0.
    int asteroids[3] = {-1 , -1 , 3}; //Represents the asteroid, 0th element -> y coordinate , 1st element -> x coordinate , 2nd element -> the powerup  Value. 
    // If power up value is 3, the gun gets equipped to the user.
    int boostCounter = 0;// The user gets 10 shots using the gun, these shots fired are counted using this var.
    int starPrint = 0; // The stars should be printed after a regular interval. Thi variable moves between 0 and 5 and prints the star if equal to 0
    bool gunEnable = false; // Tells if the gun is enabled or not.
    bool fired = false; // If the user has fired a bullet or not.
    bool restart = false;//If the game should be restarted or not.
    void lostScreen(){
        // This screen is showed when the user loses.
        string input;
        char ORANGE_NO_FLASH[] = "\033[1;33m";
        char RED_NO_FLASH[] = "\033[1;31m";
        char NC[] = "\033[0m";
        char GREEN_NO_FLASH[] = "\033[0;32m";
        system("clear");
        cout<<"You Lost.\nYour Score : \n"<<ORANGE_NO_FLASH<<score<<NC<<endl;
        cout<<RED_NO_FLASH<<"\n\nWould You like to continue?(Y/N)\n"<<NC;
        cin>>input;
        if(input == "y" || input == "Y"){
            restart = true;
        }
        else{
            restart = false;
        }
    }
    void generateMap(){
        // Generates the map in each frame.
    	bool shotUsed = false; // If the user has user the gun or not.
        char ORANGE_NO_FLASH[] = "\033[1;33m";
        char RED_NO_FLASH[] = "\033[1;31m";
        char NC[] = "\033[0m";
        char GREEN_NO_FLASH[] = "\033[0;32m";
        while(true){
            //The next 4 if statements so that the user enters the map from the other direction when it leaves the generated map area.
            if(a[0] > 22)
            a[0] = 0;
            else if(a[1] > 79)
            a[1] = 0;
            else if(a[0] < 0)
            a[0] = 22;
            else if(a[1] < 0)
            a[1] = 78;
            else if(a[0] == asteroids[0] && a[1] == asteroids[1]){
                asteroids[0] = -1;// the -1 signifies that the asteroid should be despawned.
                asteroids[1] = -1;
                if(asteroids[2] == 3){ // Checking if the asteroid was a powerup asteroid.
                    asteroids[2] = 3;
                    boostCounter = 10;
                    gunEnable = true;
                }
                score += 1;
            }
            else if((a[0] <= coords[0] + 1 && a[0] >= coords[0] - 1) && (a[1] <= coords[1] + 1 && a[1] >= coords[1] - 1)){
                //User lost.
                lostScreen();
                score = 0;
                return;
            }
            if(!showScore){
                asteroidSpawner(); // Spawns asteroid if not present in the map.
                for(int i=0;i<23;i++){
                    for(int j=0;j<80;j++){
                        if(i == a[0] && j == a[1]){
                            //Printing the user character.
                            if(direction == "f")
                            cout<<ORANGE_NO_FLASH<<"^"<<NC;
                            else if(direction == "b")
                            cout<<ORANGE_NO_FLASH<<"v"<<NC;
                            else if(direction == "l")
                            cout<<ORANGE_NO_FLASH<<"<"<<NC;
                            else if(direction == "r")
                            cout<<ORANGE_NO_FLASH<<">"<<NC;
                        }
                        else if(i == asteroids[0] && j == asteroids[1]){
                            //Printing the asteroid.
                            if(asteroids[2] == 0){
                                cout<<RED_NO_FLASH<<"#"<<NC;
                            }
                            else{
                                cout<<RED_NO_FLASH<<"@"<<NC;
                            }
                        }
                        else if(i == coords[0] && j == coords[1]){
                            //Enemy
                            cout<<"*";
                        }
                        else if(blaster.shootable(j , i , a[1] , a[0] , coords[1] , coords[0] , direction) && boostCounter > 0 && fired){
                            //The shooting animation.
                            cout<<"+";
                            shotUsed = true;

						}
                        else{
                            //Printing the stars alternately.
                            if(starPrint == 0){
                                cout<<GREEN_NO_FLASH<<"."<<NC;
                                starPrint = 1;
                            }
                            else if(starPrint < 5){
                                cout<<" ";
                                starPrint += 1;
                            }
                            else{
                                cout<<" ";
                                starPrint = 0;
                            }
                        }
                    }
                    cout<<"\n";
                }
            }
            else{
                //To display the score.
                for(int i=0;i<40;i++){
                    for(int j=0;j<40;j++){
                        if(i == 19 && j == 17){
                            cout<<"\033[1;33m"<<"Your"<<"\033[0m";
                            j += 3;//Not needed but just felt like keeping the boundaries still.
                        }
                        else if(i == 20 && j == 16){
                            cout<<"\033[1;33m"<<"Score:"<<"\033[0m";
                            j += 5;
                        }
                        else if(i == 21 && j == 17){
                            int temp = score;
                            int digits = 0;
                            while(temp > 0){
                                digits += 1;
                                temp = temp/10;
                            }
                            cout<<"\033[0;32m"<<score<<"\033[0m";
                            j += digits - 1;
                        }
                        else{
                            cout<<" ";
                        }
                    }
                    cout<<"\n";
                }
                showScore = false;
            }
            if(alternate == 0){
                pathfinder();// This algo will find the path for the mob and also move it.
                alternate = 1;
            }
            else if(alternate < mobSpeed + 1)
            alternate += 1;
            else
            alternate = 0;
            //Since the coordinates of the user might have changed in the frame, their counterpart in the KillerPiller class need to be reassigned.
            userCoords[0] = a[0];
            userCoords[1] = a[1];
            if(shotUsed){
                //Since one shot has been used
            	boostCounter -= 1;
            	shotUsed = false;
            	fired = false;
			}
			if(boostCounter <= 0){
                //The user has used all of their shots.
				gunEnable = false;
				boostCounter = 0;
			}
            //Making it sleep for 32 ms , tht is 30 fps.
            std::this_thread::sleep_for(std::chrono::milliseconds(32));
            updatePosition(getKeyPress());
            system("clear");
        }
    }
    char getKeyPress(){
        if(kbhit()){// If user has hit a key or not.
            char a;
            a = getch();// The key the user hit.
            return a;
        }
    }
    void asteroidSpawner(){
        //Spawns the asteroid if not present.
        srand(time(0));
        int randx;
        int randy;
        int powerUp;
        if(asteroids[0] == -1 && asteroids[1] == -1){
            randx = rand()%79;
            randy = rand()%22;

            asteroids[0] = randy;
            asteroids[1] = randx;
            powerUp = rand()%3 + 1;
            if(powerUp == 2 && mobSpeed == 1){
                asteroids[2] = 3;
            }
            asteroids[2] = 3;
        }
    }
    void updatePosition(char input){
        switch(input){
            //Normal moements.
            case 'w':
                a[0] -= 1;
                direction = "f";
                break;
            case 's':
                a[0] += 1;
                direction = "b";
                break;
            case 'd':
                a[1] += 1;
                direction = "r";
                break;
            case 'a':
                a[1] -= 1;
                direction = "l";
                break;
            case 'o':
                showScore = true;
                break;
            case 'e':
            	if(gunEnable){
            		fired = true;
            	}
            	break;
            default:
                break;
        }
    }
};

int main(){
    //Noice. You made it till here.
    int choice;
    int speed = 1;
    cout<<"Enter\n1 -> Easy\n2 -> Medium\n3 -> hard\n";
    cin>>choice;
    if(choice < 4 && choice > 0){
        speed = 4 - choice;
    }
    system("clear");
    Game g(speed);
    return 0;
}
