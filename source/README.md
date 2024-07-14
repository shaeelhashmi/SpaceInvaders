# Space-Shooter:
## Summary:
This is a space shooter game made using the C++ framework SFML, implementing the concepts of OOP and utilizing the C++ Standard Template Library for easier data control.
## GameMechanics
* ### Asteroids :
  Asteroids will be falling on the user throughout the entire game which the user can dodge or destroy to gain points, if there is a contact with with the asteroid the users 
  health will decrease by a single heart
  Along with this there will also be a special kind of an asteroids that will make the health of the user zero on impact
* ### Power ups :
    * #### Health Regen:
      Whenever the user picks up this power ups his health is replenished by two hearts
    * #### Speed Multiplier:
      This power up increases the speed of the users spaceship.
    * #### Revive:
      This power up once picked up by the user will revive the user once he dies the powerup is stored inside the running the program so, multiple revives can be picked up and stored by the user
* ### Boss Fights:
  The game consists of 4 boss fights, each with its own uniqueness. The boss fights occur every five levels and stop once the user has defeated all four bosses.
  * #### Boss Fight 1:
    * The boss shoots projectiles at the player.
    * The player must avoid the projectiles and shoot the boss to reduce its health.
    * This boss doesn't follow the player but will teleport to the players position after some time
  * #### Boss Fight 2:
    * This boss has similar features as of boss 1 with the added feature of following the players movement
  * #### Boss Fight 3:
    * Same as boss Fight this boss will also have asteroids falling along with asteroids falling with it
    * If the user collides with the projectiles of this boss his health will be lost by 2 hearts
  * #### Boss Fight 4:
    * This is the last fight this boss will also have the support of a minion that will come and attack the user while moving horizantally along the screen.
## Collabrators:
* [Mujtaba](https://github.com/Mujtaba0150)
* [Sarim Shakeel](https://github.com/sarimshakeel)
* [Shaeel Hashmi](https://github.com/shaeelhashmi)
## Roles:
* <b><u>Shaeel Hashmi</u></b> - All features of Boss fights, HighScore screen, Spaceship control and Revive Powerup.
## Getting Started:
### Prerequistes:
* A C++ compiler like g++
* SFML installed and linked to the C++ compiler
Make sure SFML is installed and linked on your compiler.
### Video Demonstration:
Watch this video to see the game in action. Video link [here](https://www.linkedin.com/feed/update/urn:li:activity:7202682563788689408/)
### Installation:
* You can install the zip file of the project from [here](https://github.com/shaeelhashmi/Spaceshipgame-SFML)
* If you have git installed type
```
git clone https://github.com/shaeelhashmi/Spaceshipgame-SFML
```
Then run 
```
cd Spaceshipgame-SFML
```
### Execution:
* Execute the mainmenu.cpp file.
* If you are using any linux distribution follow these steps:
   * Go the project directory.
   * In the terminal right the following commands:
       * ```
         g++ mainmenu.cpp -o myprog -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio.
         ```
       * ```
         ./myprog
         ```
* Play the game and enjoy     
       
                 
