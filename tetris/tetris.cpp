#include <iostream>
#include <stdio.h>
#include <thread>
#include <vector>
#include <time.h>
#include <SFML/Graphics.hpp>

using namespace sf;


const int screenWidth = 80;
const int screenHeight = 30;
const int fieldWidth = 10;
const int fieldHeight = 20;


int shapes[7][4] = {
    1,3,5,7,
    2,4,5,7,
    3,5,4,6,
    3,5,4,7,
    2,3,5,7,
    3,5,7,6,
    2,3,4,5,
};

int field[fieldHeight][fieldWidth] = {0};



struct Point {
    int x;
    int y;
} tetro[4], tetro_[4];

bool check() {
    for (int i=0; i<4; ++i) {
        if (tetro[i].x<0 || tetro[i].x>=fieldWidth || tetro[i].y>=fieldHeight) {
            return false;
        } else if (field[tetro[i].y][tetro[i].x]){
            return false;
        }
        return true;
    }
}


int main () {

    srand(time(0));

    RenderWindow window(VideoMode(320, 480), "The game!");
    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/bg.png");
    t3.loadFromFile("images/frame.png");

    Sprite s(t1);

    int dx = 0;
    bool rotate = false;
    int color = 1;
    float timer=0, delay=0.3;

    Clock clock;

    while (window.isOpen()) {
        float seconds = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += seconds;

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) window.close();
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Up) rotate = true;
                else if (e.key.code == Keyboard::Left) dx = -1;
                else if (e.key.code == Keyboard::Right) dx = 1;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) delay=0.05;

        // move tetromino, tetro_ is for keep original tetromino
        for (int i=0; i<4; ++i) {
            tetro_[i] = tetro[i];
            tetro[i].x += dx;
        }
        
        // Moves make tetro be out of boundary will be ignored
        if (!check()) {
            for (int i=0; i<4; ++i) tetro[i] = tetro_[i];
        }

        // rotate
        if (rotate) {
            Point p = tetro[1]; // pivot
            for (int i=0; i<4; ++i) {
                int _x = tetro[i].y - p.y;
                int _y = tetro[i].x - p.x;
                tetro[i].x = p.x - _x;
                tetro[i].y = p.y + _y;
            }
            if (!check()) for (int i=0; i<4; ++i) tetro[i] = tetro_[i];
        }

        if (timer > delay) {
            for (int i=0; i<4; ++i) {
                tetro_[i] = tetro[i];
                tetro[i].y += 1;
            }
    
            // if fall down to bottom, prepare for another shape with another color
            if (!check()) {
                for (int i=0; i<4; ++i) {
                    field[tetro_[i].y][tetro_[i].x] = color;
                }
                color = 1 + rand()%7; // change color
                int n = rand()%7;
                for (int i=0; i<4; ++i) {
                    tetro[i].x = shapes[n][i] % 2;
                    tetro[i].y = shapes[n][i] / 2;
                }
               
            }
            timer = 0;
        }

        int k = fieldHeight-1;
        for (int i = fieldHeight-1; i>0; --i) {
            int count = 0;
            for (int j=0; j<fieldWidth; ++j) {
                if (field[i][j]) count++;
                field[k][j] = field[i][j];
            }
            if (count < fieldWidth) k--;
        }

        // re-initialize
        dx = 0;
        rotate = false;
        delay = 0.3;

        // draw
        window.clear(Color::White);

        for (int i=0; i<fieldHeight; ++i) {
            for (int j=0; j<fieldWidth; ++j) {
                if (field[i][j] == 0) continue;
                s.setTextureRect(IntRect(field[i][j]*18, 0, 18, 18));
                s.setPosition(j*18, i*18);
                s.move(28, 31);
                window.draw(s);
            }
        }

        for (int i=0; i<4; ++i) {
            s.setTextureRect(IntRect(color*18,0,18,18));
            s.setPosition(tetro[i].x*18,tetro[i].y*18);
            s.move(28,31); //offset
            window.draw(s);
        }
        
        window.display();
    }

    return 0;
}






