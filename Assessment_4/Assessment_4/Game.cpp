#include <iostream>
#include <algorithm>
#include <cstdlib> // For rand and srand
#include <ctime>   // For time
#include "Game.h"
#include "Room.h"
#include "Player.h"
#include "String.h"
#include "Item.h"
#include "BoxOfDonuts.h"
#include "Lamp.h"
#include "Cat.h"
using namespace std;


Game::Game() {
    static const string defaultDescription = "Default Description";
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            rooms[i][j].setDescription(defaultDescription);
            rooms[i][j].setItem(nullptr);
        }
    }
    this->playerRow = 2; // Starting location
    this->playerCol = 2;
    player = new Player();

    BoxOfDonuts* boxOfDonuts = new BoxOfDonuts("Box of Donuts description", 10);
    placeItem(rooms, boxOfDonuts);

    Lamp* lamp = new Lamp("Lamp description", true);
    placeItem(rooms, lamp);

    Cat* cat = new Cat("Cat description", true);
    placeItem(rooms, cat);

}

Game::~Game() {
 
    delete player;
}

void Game::Run() {
    // Game loop
    cout << R"(
                                    /@
                     __        __   /\/
                    /==\      /  \_/\/   
                  /======\    \/\__ \__
                /==/\  /\==\    /\_|__ \
             /==/    ||    \=\ / / / /_/
           /=/    /\ || /\   \=\/ /     
        /===/   /   \||/   \   \===\
      /===/   /_________________ \===\
   /====/   / |                /  \====\
 /====/   /   |  _________    /  \   \===\    THE LEGEND OF 
 /==/   /     | /   /  \ / / /  __________\_____      ______       ___
|===| /       |/   /____/ / /   \   _____ |\   /      \   _ \      \  \
 \==\             /\   / / /     | |  /= \| | |        | | \ \     / _ \
 \===\__    \    /  \ / / /   /  | | /===/  | |        | |  \ \   / / \ \
   \==\ \    \\ /____/   /_\ //  | |_____/| | |        | |   | | / /___\ \
   \===\ \   \\\\\\\/   /////// /|  _____ | | |        | |   | | |  ___  |
     \==\/     \\\\/ / //////   \| |/==/ \| | |        | |   | | | /   \ |
     \==\     _ \\/ / /////    _ | |==/     | |        | |  / /  | |   | |
       \==\  / \ / / ///      /|\| |_____/| | |_____/| | |_/ /   | |   | |
       \==\ /   / / /________/ |/_________|/_________|/_____/   /___\ /___\
         \==\  /               | /==/
         \=\  /________________|/=/    TEXT ADVENTURE GAME
           \==\     _____     /==/ 
          / \===\   \   /   /===/
         / / /\===\  \_/  /===/
        / / /   \====\ /====/
       / / /      \===|===/
       |/_/         \===/
                      =)" << endl;
    String direction;
    while (true) {
        printMap();
        cout << "Enter direction to move (North, East, South, West, or Quit to exit): ";
        direction.ReadFromConsole();

        // Convert the input direction to lowercase for case-insensitive comparison
        direction.ToLower();

        // Move the player based on the input direction
        if (direction == "north") {
            movePlayer(-1, 0); // Move north
        }
        else if (direction == "east") {
            movePlayer(0, 1); // Move east
        }
        else if (direction == "south") {
            movePlayer(1, 0); // Move south
        }
        else if (direction == "west") {
            movePlayer(0, -1); // Move west
        }
        else if (direction == "quit") {
            cout << "Quitting the game." << endl;
            return;
        }
        else {
            cout << "Invalid direction. Please enter North, East, South, West, or Quit." << endl;
        }
    }
}

void Game::printMap() const {
    // Print the ASCII map
    // This is just a simple example, you can replace it with your actual ASCII map
    cout << "-------------------" << endl;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            cout << " _____ ";
        }
        cout << endl;

        // Print the middle part of the rooms (including the player's position if applicable)
        for (int j = 0; j < 5; ++j) {
            if (i == this->playerRow && j == this->playerCol) {
                cout << "|  P  |";
            }
            else {
                cout << "|     |";
            }
        }
        cout << endl;

        // Print the bottom part of the rooms
        for (int j = 0; j < 5; ++j) {
            cout << "|_____|";
        }
        cout << endl;
    }
    cout << "-------------------------" << endl;
}

void Game::movePlayer(int rowOffset, int colOffset) {
    // Calculate the new player position after moving
    int newRow = playerRow + rowOffset;
    int newCol = playerCol + colOffset;

    // Check if the new position is within the grid bounds
    if (newRow >= 0 && newRow < 5 && newCol >= 0 && newCol < 5) {
        // Move the player to the new room
        // You may need additional logic to handle interactions with the room, if any
        // For simplicity, this example just moves the player without interaction
        playerRow = newRow;
        playerCol = newCol;
        printRoomDescription(playerRow, playerCol);
    }
    else {
        cout << "Cannot move in that direction. Please choose another direction." << endl;
    }
}


void Game::placeItem(Room rooms[5][5], Item* item) {
    srand(time(nullptr));

    // Randomly select a room until an empty one is found
    while (true) {
        // Generate random row and column indices
        int row = rand() % 5;
        int col = rand() % 5;

        // Check if the selected room is empty
        if (rooms[row][col].getItem() == nullptr) {
            // Place the item in the room
            rooms[row][col].setItem(item);
            break; // Exit the loop
        }
    }
}



void Game::printRoomDescription(int row, int col) const {
    // Check if the row and column are within bounds
    if (row >= 0 && row < 5 && col >= 0 && col < 5) {
        // Get the room description
        rooms[row][col].Description();

        // Check if there is an item in the room
        Item* item = rooms[row][col].getItem();
        if (item != nullptr) {
            // Output the item description
            cout << "Item Description: ";
            item->Description();

            // Prompt the player to use the item
            cout << "Do you want to use this item? (Yes/No): ";
            String response;
            response.ReadFromConsole();
            response.ToLower();
            if (response == "yes") {
                item->Use();
            }
        }
        else {
            cout << "No item in the room." << endl;
        }
    }
    else {
        cout << "Invalid room coordinates." << endl;
    }
}

void Game::EnterCurrentRoom() {
    Room& currentRoom = rooms[playerRow][playerCol];
    // Enter the current room

    currentRoom.Description();
    Item* item = currentRoom.getItem();
    if (item != nullptr) {
        cout << "Item Description: ";
        item->Description();
    }
    else {
        cout << "No item in the room." << endl;
    }
}

void Game::ClearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

