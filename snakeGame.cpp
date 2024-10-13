#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <string>

using namespace std;

class Snake {
private:
    const int width = 60;
    const int height = 20;
    bool gameOver;
    int score;
    int speedLevel;
    
    struct Position {
        int x, y;
    };
    
    Position head;
    vector<Position> tail;
    Position fruit;
    enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
    Direction dir;

public:
    Snake() {
        setup();
    }

    void setup() {
        gameOver = false;
        dir = STOP;
        head.x = width / 2;
        head.y = height / 2;
        fruit.x = rand() % (width - 2) + 1;
        fruit.y = rand() % (height - 2) + 1;
        score = 0;
        speedLevel = 1;
        tail.clear();  
    }


    void draw() {
        system("cls"); 

        string buffer;

        buffer += "\n";
        buffer += "  +-------------------+\n";
        buffer += "  |    SNAKE GAME     |\n";
        buffer += "  +-------------------+\n\n";

        buffer.append(width, '#');
        buffer.append("\n");

        for (int i = 1; i < height-1; i++) {
            buffer += '#'; 
            for (int j = 1; j < width - 1; j++) {
                if (j == head.x && i == head.y) {
                    buffer += '@'; 
                } else if (j == fruit.x && i == fruit.y) {
                    buffer += '*'; 
                } else {
                    bool printTail = false;
                    for (const Position& segment : tail) {
                        if (segment.x == j && segment.y == i) {
                            buffer += 'o'; 
                            printTail = true;
                            break;
                        }
                    }
                    if (!printTail) {
                        buffer += ' '; 
                    }
                }
            }
            buffer += '#'; 
            buffer += '\n'; 
        }

        buffer.append(width, '#');
        buffer.append("\n");

        buffer += "Score: " + to_string(score) + " | Length: " + to_string(tail.size() + 1) + " | Level: " + to_string(speedLevel) + "\n";
        buffer += "Controls: W,A,S,D to move, X to quit\n";


        cout << buffer;
    }



    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a':
                    if (dir != RIGHT) dir = LEFT;
                    break;
                case 'd':
                    if (dir != LEFT) dir = RIGHT;
                    break;
                case 'w':
                    if (dir != DOWN) dir = UP;
                    break;
                case 's':
                    if (dir != UP) dir = DOWN;
                    break;
                case 'x':
                    gameOver = true;
                    break;
            }
        }
    }

    void run() {
        if (dir == STOP) return;

        Position prev = head;

        switch (dir) {
            case LEFT:  head.x--; break;
            case RIGHT: head.x++; break;
            case UP:    head.y--; break;
            case DOWN:  head.y++; break;
        }

        if (!tail.empty()) {
            Position prevSegment = prev;
            for (Position& segment : tail) {
                Position temp = segment;
                segment = prevSegment;
                prevSegment = temp;
            }
        }

        if (head.x <= 0 || head.x >= width - 1 || head.y <= 0 || head.y >= height - 1)
            gameOver = true;

        for (const Position& segment : tail) {
            if (head.x == segment.x && head.y == segment.y)
                gameOver = true;
        }

        if (head.x == fruit.x && head.y == fruit.y) {
            score += 10;

            Position newSegment;
            if (tail.empty()) {
                newSegment = prev; 
            } else {
                newSegment = tail.back();  
            }
            tail.push_back(newSegment);

            do {
                fruit.x = rand() % (width - 2) + 1;
                fruit.y = rand() % (height - 2) + 1;
            } while (isPositionOccupied(fruit));  

            if (score % 50 == 0 && speedLevel < 5)
                speedLevel++;
        }
    }

    bool isPositionOccupied(const Position& pos) {
        if (pos.x == head.x && pos.y == head.y) return true;
        for (const Position& segment : tail) {
            if (pos.x == segment.x && pos.y == segment.y) return true;
        }
        return false;
    }

    bool isGameOver() const {
        return gameOver;
    }

    int getSpeedLevel() const {
        return speedLevel;
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    
    while (true) {
        Snake snake;
        while (!snake.isGameOver()) {
            snake.draw();
            snake.input();
            snake.run();
            Sleep(200 / snake.getSpeedLevel());
        }
        
        cout << "\nGame Over!" << endl;
        cout << "Press 'R' to restart or any other key to exit" << endl;
        
        char choice = _getch();
        if (choice != 'r') {
            break; 
        }
    }
    
    return 0;
}
