# C++ SDL2 Asteroids Game

## Creating a custom Asteroids game using SDL2

## GIF
![](/Screenshots-Videos/UPDATE 11 - gameplay.gif)

### UPDATE 11 (19/11/20)
- ADDED collectable powerups first pass (sheild regen) - items pulse, move through space and have a limited lifetime for collection.
- ADDED many visual improvements - visual indicators for collecting items and when damaged
- FEATURE blackholes now have a transition enlarge phase when entered - also items get destroyed when entering (more todo)
- ADDED smooth sheild bar animation upon increase/decrease.
- NEW and additional SOUNDS
- NEW background image
- code cleanup

### UPDATE 10 (18/9/20)
- ADDED StateMachine (Full Code structure change) - easier editing game states going forward
- Code tidy, more forward declarations. Less #includes (removed unused/ not needed)

### UPDATE 9 (22/8/20)
- ADDED particles upon destroying asteroids (velocity and direction dependant on asteroids)
- ADDED thrust particles
- CHANGED / lowered overall scale of game - playing area greatly increased due to this.
- CHANGED how asteroid speed and direction is calculated. No guaranteed slow asteroid created from destroying slow parent asteroid.
- ADDED sounds - courtesy of mine and my gf's talented vocal chords :)
- Min and Max asteroid speed increases per level.

### UPDATE 8 (11/8/20)
- ADDED control - Hold LSHIFT or RSHIFT to limit your rotation for higher accuracy shooting.
- FIXED size bug for scoreboard on smaller resolution displays.
- ADDED initial skeleton code for powerup class system (upcoming upgrades)

### UPDATE 7 (20/7/20)
- ADDED enterable Blackhole to transition to each new level. (see screenshot + playable exe).
- CODE clear up: moved various variables/ code scopes out of game update and into relevant classes.
- CODE added missed 'const' to methods and arguments.
- VISUAL addition of sheild icon next to sheild bar.
- BUG added screen ratio checks for text textures (rare circumstances meant they were rendered to big on some screens).

### UPDATE 6 (17/7/20)
- ADDED sheild bar - default lose 25% each collision (if you loose all sheild and get hit = gameover).
- ADDED player sprite flashes during collision cooldown after being hit.
- CHANGED background asteroid to more basic image (no need to use standard detailed image due to smaller size).
- CHANGED Player image slightly (fill with 70% opacity red) + better resolution image.
- FIXED score rect now displays corrently as score increases (no more contraction).
- ADDED Screenshots

### UPDATE 5 (16/7/20)
- NEW Game over screen (displays, score and can replay the game through the main window).
- NEW asteroid speed calculations.
- ADDED press 'i' to view collision circles. (please report any strange sizes wit ha pic and your resolution settings).
- Split asteroids inhert speed from parent asteroid when created (with offset split angle).
- New asteroid images.

### UPDATE 5
- Split Asteroids velocity now calculated based on existing parent asteroid velocity + offset (more natural physics).
- Added angle acceleration (better precision when tapping).
- Default endless mode.
- Bugfix: bullet can no longer destroy more than 1 asteroid in certain circumstances).
- Code: Tidy up and fixed memory leak.

### UPDATE 4
- Added ship thrust visualisation.
- Added wave system (currently 5 waves).
- HUD Display and delay when wave complete.
- Code cleanup.
- Added standalone playable game folder (exe, dll + assets included).
- New splash screen.
- Screenshot folder.

### UPDATE 3
- Added high score.txt that keeps track of your highest 3 scores.
- Added Score display to screen.
- Added Spawning protection around player when spawning new asteroids. 

### UPDATE 2
- Game auto grabs screen resolution and uses 70% height and scales the game occordingly.
- Tidied up code and updated classes/methods to intergrate the scaling algorythmn.
- Added quick respawn of asteroids after destroying them all (Player pos checks and level progression to add).

### UPDATE 1
- Added collision for bullets, player and asteroids (3sec delay before activation at level start).
- Change inplemetation of 'CreateAsteroid' function.
- Larger asteroids break into 2 smaller chunks upon bullet collision.

### Initial Commit
- Ship math and controls complete.
- Bullets complete. (delay based on level to be implemented).
- Asteroid creation and math complete.

### ONGOING PROJECT
#### Upcoming
- Collision - DONE
- Score system - intially DONE
- Add high score in game (replace console implementation)
- Wave/levels - intially DONE added blackholes (v7)
- Lives/ Shield - Sheild 1st pass DONE
- Sounds - DONE reworked (v11)
- Powerups - done (1st pass v11)
- Menus
- move entity updates and collisions into releveant child classes