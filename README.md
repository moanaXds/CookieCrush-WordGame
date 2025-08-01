
CookieCrush-WordGame 
--------------------

A fun and addictive 2D word puzzle game  inspired by Candy Crush, built using C++ with OpenGL. Instead of matching candies or cookies, players shoot alphabet tiles upward into a grid and form valid English words to score points before time runs out! 

Game Overview 
-------------

In CookieCrush-WordGame, you control a shooter at the bottom of the screen. Launch random alphabet cookies upward and strategically place them in a 5x15 grid to create valid English words (minimum 3 letters) such as horizontally, vertically, or diagonally. 
Each matched word clears from the board and increases your score based on its length (score += word_length * 10). You only have 120 seconds to make as many words as possible! 

<img width="1916" height="1452" alt="1" src="https://github.com/user-attachments/assets/a1957e7a-e923-47d4-bc0d-d3208c64d78a" />

Terminal Output
---------------

<img width="2752" height="1258" alt="3" src="https://github.com/user-attachments/assets/3b565140-7e79-420d-aaaa-ce1c3d6da0b3" />

It Represent the bursting words.

It also Searches the longest word that Burst.

<img width="1916" height="1452" alt="Screenshot from 2025-07-11 16-28-07" src="https://github.com/user-attachments/assets/3350c118-df53-4398-bb7b-ecf1a8f53ca0" />

Features 
--------
    Real-time dictionary validation with over 370,099 English words 
    Dynamic alphabet launching and collision detection 
    Word matching logic that scans the board for matches
    Texture-based rendering using .bmp images for each letter (A–Z)
    Score tracking and countdown timer
    Written in C++ using OpenGL/GLUT 
     

Repository Contents 
     
    wordshooter.cpp
	
Main game logic and OpenGL rendering code
  
    words_alpha.txt
	
Dictionary file containing 370,099 English words
   
    image-data.bin
	
Binary file containing pre-loaded texture data for alphabets
   
    util.cpp / util.h
	
Utility functions for input handling, drawing, textures, and word search
   
    Board.h / Board.cpp
	
Logic for managing the 5x15 alphabet grid
    
    Makefile
	
Build script for compiling the game on Linux

    install-libraries.sh
	
Script to install required dependencies on Ubuntu

    Project.pdf
	
Project documentation (if available)
    
    CImg.h
	
Image processing library used for texture loading

    *.bmp files

 Gameplay Tips 
--------------
    .Click anywhere on the screen to shoot the current letter.
    .Aim to form valid English words  (3+ letters).
    .Words can be formed horizontally, vertically, or diagonally.
    .Matched letters disappear and new letters drop from above.
    .Score increases based on word length (score += word_length * 10).
     

License 
-------

MIT License – see repository for details. 

Dictionary source: https://github.com/dwyl/english-words   (public domain) 

Textures: Placeholder .bmp files for each letter (can be replaced with custom designs) 
 
Author 
------

Muanna Hamid 
Initial concept and implementation 
 
