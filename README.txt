Fall 2015: CMPM 170, Summer project
A "pong-like" with nonstandard gameplay element.

Author     : Chaiz Tuimoloau 
Game       : Mr. Pong
Language   : C++
Library    : SFML 2.3.1
Development: Microsoft Visual C++ 2010

This project was developed alongside several tutorials, primarily
for getting started with C++ development in Microsoft's Visual Studio
IDE with the SFML libraries. The code was started using SFML's own
"Pong" example game for reference. Specifically, SFML's method of
implementing the main game loop, and utilizing SFML Pong's method of 
drawing objects, collision detection, AI, and radial angle generation
are virtually identical, but the implementation seemed fundamental 
enough to build off of.

Special thanks to these pages (visited many times):
http://www.gamefromscratch.com/page/Game-From-Scratch-CPP-Edition-Part-1.aspx
http://www.sfml-dev.org/documentation/2.0/classsf_1_1Clock.php
http://stackoverflow.com/

==== MR. PONG MECHANICS ====
(detailing mostly special mechanics that differ from
SFML's Pong example, or nonstandard Pong mechanics...)

- Scoreboard added
- Collision detection slightly more precise than SFML Pong
- Ball angle is adjusted to precisely match angle of paddle hit
     (A linear transition dependant on how far from the center of the paddle)
- Sound effects added for different events (ball bouncing, scoring, etc...)
- Upon scoring, the player scored on will serve the ball (a random angle)
- Ball speeds up upon each paddle return, maxes speed around 12+ hits
- NPC Mr. Pong spawns on set timers activated after 1st score is made
- Mr. Pong randomly travels within borders of the screen for a while, goes and
      comes back consistently
- Hitting Mr. Pong will set the ball to a very fast speed (faster than max)
      and shoots it off in a completely random direction, and destroys Mr. Pong
      for the time being.
    

All sprite and sound instances are entirely home made.