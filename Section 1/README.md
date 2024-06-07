# Section 1: Overview of Game Hacking

Welcome to Introduction to Game Hacking!

Before we dive into what game hacking is, we need to understand what a game is and how it functions. In this section, we'll explore the different types of games and provide an overview of how they operate. Then, we'll delve into the motivations behind game hacking and the various methods used to hack games.

## Games

_"A game is a description of strategic interaction that includes the constraints on the actions that the players can take and the players’ interests, but does not specify the actions that the players do take."_  

... Oh, not that one.

A "game" can be defined in many ways, but for this course, we're interested in a specific category: `video games`. These are games played on electronic devices equipped with screens and speakers for output, and buttons or touch controls for input. Video games offer an interactive experience that combines entertainment, challenge, and sometimes storytelling. For the rest of the course by "game" we shall be referring to a "video game".

## Types of games

![List of Games](https://i.imgflip.com/yj9e9.jpg)

There are many classifications of video games. On the basis of platforms you can list the following types: Windows, Mobile, PlayStation, Nintendo, ~~Mac~~, Linux games. There are also many classifications on the basis of genres but what we are interested in is the following classification: **Offline games and Online games**.

### Online Games

These games typically follow a Client-Server model. There is a server somewhere in the world that has a database of all player credentials and the player has to download a Client of the game and register with the server to get some identification, which is then used to later connect to the server to enable gameplay. 

Even during the gameplay, a lot of the player attributes are stored on the server side and are constantly updated by communicating with the server either after every Game Tick or when an input button is pressed by the player. For this reason, you can't just attach Cheat Engine to Fortnite and expect to be invincible, because the player health is calculated and stored on the server and not on the client. 

![client-server](https://imgur.com/0bAvSpo.png)

The exact mechanism of how the server actually handles the requests and what does the client do while it waits for the server's response is quite complex and varies from game to game. You can read [this link](https://www.cnblogs.com/cuizhf/p/3417195.html) to understand how the communication is handled in Fast-paced Multiplayer games.

### Offline Games

Offline games rely entirely on the device they're played on. All computations, graphics rendering, and logic processing occur locally. This means the game's performance and capabilities are directly tied to the hardware of the device. For this reason, we can freely modify any and all player attributes in any way we want. But, usually offline games are stripped before release to improve the performance which makes it difficult to decompile and understand the Game Logic.

## Game Hacking

Now that we know how does a game work, let us understand what we mean by game hacking.

_"Game hacking" refers to modifying the working of a game by exploiting certain functionalities or actively changing the game's flow._

There are a variety of ways this can be achieved. The most common methods include:

1. **Memory Editing**: modifying the memory values where the game keeps its status information
2. **Botting**: Developing an automated script to perform actions without direct player input.
3. **Code Injection**: modification of the game's executable code while it is running
4. **Network Traffic Forgery**: editing packets to modify outbound network traffic

We shall be exploring the bottom two methods in this course.

## Why hack games?

There can be a variety of reasons for this:

For offline games:
 * To make the game easier to play
 * To skip a corrupted part of the game
 * For fun

For online games:
 * Gain an advantage over other players
 * Attack the game infrastructure
 * For fun

We are of course interested in the educational motivation of gaining a deeper understanding of the game's functioning by directly manipulating the game flow. 

## How to hack games?

We’ll be doing these hacks hands-on by trying them out on the game PwnAdventure3, created by Vector35. This game is specifically designed for practicing game hacking techniques. Refer to [`Docker Server`]() for instructions to set up the game client and server.

Come back to this guide after you have downloaded the game and successfully connected to the server.

## Assignment

Welcome back! By now you should have installed the game client and server and should be able to play the game.

The assignment of this week is to make a character with your name and Escape the Lost Cave and come out on the open area of the island. Then log out of the server and log back in. Just before selecting your character take a screenshot of the character selection menu. It should show your player name and the background should not be of the cave. Here is an example:
![player on island](https://imgur.com/6m6ds5f.png)

Submit this assignment [here]().