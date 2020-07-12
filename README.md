# C++ SDL2 Asteroids Game

## Creating a custom Asteroids game using SDL2

### UPDATE 3
- Added high score.txt that keeps track of your highest 3 scores
- Added Score display to screen
- Added Spawning protection around player when spawning new asteroids. 

### UPDATE 2
- Game auto grabs screen resolution and uses 70% height and scales the game occordingly.
- Tidied up code and updated classes/methods to intergrate the scaling algorythmn.
- Added quick respawn of asteroids after destroying them all (Player pos checks and level progression to add)

### UPDATE 1
- Added collision for bullets, player and asteroids (3sec delay before activation at level start)
- Change inplemetation of 'CreateAsteroid' function
- Larger asteroids break into 2 smaller chunks upon bullet collision

### Initial Commit
- Ship math and controls complete.
- Bullets complete. (delay based on level to be implemented)
- Asteroid creation and math complete.

### ONGOING PROJECT
#### Upcoming
- Collision - DONE
- Score system - intially DONE
- Wave/levels
- Menus