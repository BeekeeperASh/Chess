#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <time.h>
#include <cmath>
#include "Connector.hpp"
#include "ConnectorA.hpp"
using namespace sf;
// position startpos moves 
std::string position = "";
int size = 84;
bool e = true;
bool d = true;
int whosemove = 0;

Sprite f[32];

int board[8][8] =
{ -1, -2, -3, -4, -5, -3, -2, -1,
-6, -6, -6, -6, -6, -6, -6, -6,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
6, 6, 6, 6, 6, 6, 6, 6,
1, 2, 3, 4, 5, 3, 2, 1
};

std::string toChessNote(Vector2f p) {
    std::string s = "";
    s += char(p.x / size + 97);
    s += char(56 - p.y / size);
    return s;
}

Vector2f toCoord(char a, char b) {
    int x = int(a) - 97;
    int y = 56 - int(b);
    return Vector2f(x * size, y * size);
}

void move(std::string str) {
    Vector2f oldPos = toCoord(str[0], str[1]);
    Vector2f newPos = toCoord(str[2], str[3]);

    for (int i = 0; i < 32; i++) {
        if (f[i].getPosition() == newPos) f[i].setPosition(-100, -100);
        
    }
    for (int i = 0; i < 32; i++) {
        if (f[i].getPosition() == oldPos) f[i].setPosition(newPos);
    }

    if (str == "e1g1") /*if (position.find("e1") == -1)*/ { move("h1f1"); }
    if (str == "e8g8") /*if (position.find("e8") == -1)*/ { move("h8f8"); }
    if (str == "e1c1") /*if (position.find("e1") == -1)*/ { move("a1d1"); }
    if (str == "e8c8") /*if (position.find("e8") == -1)*/ { move("a8d8"); }

}

void loadPosition() {
    int k = 0;
    int check[8][8] =
    { -1, -2, -3, -4, -5, -3, -2, -1,
    -6, -6, -6, -6, -6, -6, -6, -6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    6, 6, 6, 6, 6, 6, 6, 6,
    1, 2, 3, 4, 5, 3, 2, 1
    };
    if (position.length() >= 12) {
        if (position.substr(position.length() - 6, 4) == position.substr(position.length() - 12, 4)) {
            //std::cout << position.length() << std::endl;
            position.erase(position.length() - 6, 6);
            
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int n = board[i][j];
            if (!n) continue;
            int x = abs(n) - 1;
            int y = n > 0 ? 1 : 0;
            f[k].setTextureRect(IntRect(size * x, size * y, size, size));
            f[k].setPosition(size * j, size * i);
            
            k++;
        }
    }
    
    for (int i = 0; i < position.length(); i += 6) {
        Vector2f oldPos = toCoord(position.substr(i, 4)[0], position.substr(i, 4)[1]);
        Vector2f newPos = toCoord(position.substr(i, 4)[2], position.substr(i, 4)[3]);
        int xn = int(newPos.x / size), yn = int(newPos.y / size);
        int xo = int(oldPos.x / size), yo = int(oldPos.y / size);
        
        if (i == position.length() - 6) {
            whosemove = ((position.length() % 12) - 1) * check[yo][xo];
            if (!(xn >= 0 && xn <= 7 && yn >= 0 && yn <= 7) || (check[yn][xn] * check[yo][xo] > 0) || 
                (whosemove < 0)) {
                position.erase(position.length() - 6, 6);
                
                break;
            }
            bool t = false;
            int z1 = yo, z2 = yn;
            int r1 = xo, r2 = xn;
            switch (check[yo][xo]) {
            case 6:
                if (!((((yo - yn) == 2 || (yo - yn) == 1) && xo - xn == 0 && check[yn][xn] == 0) || ((check[yn][xn] != 0) && ((yo - yn) == 1) && ((xn - xo == -1) || (xn - xo == 1))))) {
                    t = true;
                }
                break;
            case 7:
                if (!((((yo - yn) == 1) && xo - xn == 0 && check[yn][xn] == 0) || ((check[yn][xn] != 0) && ((yo - yn) == 1) && ((xn - xo == -1) || (xn - xo == 1))))) t = true; 
                break;
            case -6:
                if (!((((yo - yn) == -2 || (yo - yn) == -1) && xo - xn == 0 && check[yn][xn] == 0) || ((check[yn][xn] != 0) && ((yo - yn) == -1) && ((xn - xo == -1) || (xn - xo == 1))))) {
                    t = true;
                }
                break;
            case -7:
                if (!((((yo - yn) == -1) && xo - xn == 0 && check[yn][xn] == 0) || ((check[yn][xn] != 0) && ((yo - yn) == -1) && ((xn - xo == -1) || (xn - xo == 1))))) t = true;
                break;
            case 1:
                if (!((yo - yn) * (xo - xn) == 0)) {
                    t = true;
                    break;
                }
                while (z1 < yn - 1) {
                    z1++;
                    if (check[z1][xn] != 0) t = true;
                    //std::cout << z1 << " " << check[z1][xn] << std::endl;
                }
                while (z2 < yo - 1) {
                    z2++;
                    if (check[z2][xn] != 0) t = true;
                    //std::cout << z2 << " " << check[z2][xn] << std::endl;
                }
                while (r1 < xn - 1) {
                    r1++;
                    if (check[yn][r1] != 0) t = true;
                }
                while (r2 < xo - 1) {
                    r2++;
                    if (check[yn][r2] != 0) t = true;
                } 
                break;
            case -1:
                if (!((yo - yn) * (xo - xn) == 0)) {
                    t = true;
                    break;
                }
                while (z1 < yn - 1) {
                    z1++;
                    if (check[z1][xn] != 0) t = true;
                    
                }
                while (z2 < yo - 1) {
                    z2++;
                    if (check[z2][xn] != 0) t = true;
                    
                }
                while (r1 < xn - 1) {
                    r1++;
                    if (check[yn][r1] != 0) t = true;
                }
                while (r2 < xo - 1) {
                    r2++;
                    if (check[yn][r2] != 0) t = true;
                }
                break;
            case 2:
                if (!(((yn - yo == 2 || yn - yo == -2) && (xn - xo == 1 || xn - xo == -1)) ||
                    ((xn - xo == 2 || xn - xo == -2) && (yn - yo == 1 || yn - yo == -1)))) t = true;
                break;
            case -2:
                if (!(((yn - yo == 2 || yn - yo == -2) && (xn - xo == 1 || xn - xo == -1)) ||
                    ((xn - xo == 2 || xn - xo == -2) && (yn - yo == 1 || yn - yo == -1)))) t = true;
                break;
            case 3:
                if (!((yn - yo == xn - xo) || (yn - yo == xo - xn))) {
                    t = true;
                    break;
                }
                if (r1 < xn - 1) {
                    while (z1 < yn - 1) {
                        z1++;
                        r1++;
                        if (check[z1][r1] != 0) t = true;
                    }
                }
                else {
                    while (z1 < yn - 1) {
                        z1++;
                        r1--;
                        if (check[z1][r1] != 0) t = true;                     
                    }
                }
                if (r2 < xo - 1) {
                    while (z2 < yo - 1) {
                        z2++;
                        r2++;
                        if (check[z2][r2] != 0) t = true;
                    }
                }
                else {
                    while (z2 < yo - 1) {
                        z2++;
                        r2--;
                        if (check[z2][r2] != 0) t = true;
                    }
                }
                break;
            case -3:
                if (!((yn - yo == xn - xo) || (yn - yo == xo - xn))) {
                    t = true;
                    break;
                }
                if (r1 < xn - 1) {
                    while (z1 < yn - 1) {
                        z1++;
                        r1++;
                        if (check[z1][r1] != 0) t = true;
                    }
                }
                else {
                    while (z1 < yn - 1) {
                        z1++;
                        r1--;
                        if (check[z1][r1] != 0) t = true;
                    }
                }
                if (r2 < xo - 1) {
                    while (z2 < yo - 1) {
                        z2++;
                        r2++;
                        if (check[z2][r2] != 0) t = true;
                    }
                }
                else {
                    while (z2 < yo - 1) {
                        z2++;
                        r2--;
                        if (check[z2][r2] != 0) t = true;
                    }
                }
                break;
            case 4:
                if (!((yo - yn) * (xo - xn) == 0 || (yn - yo == xn - xo) || (yn - yo == xo - xn))) {
                    t = true;
                    break;
                }
                if ((yo - yn) * (xo - xn) == 0) {
                    while (z1 < yn - 1) {
                        z1++;
                        if (check[z1][xn] != 0) t = true;

                    }
                    while (z2 < yo - 1) {
                        z2++;
                        if (check[z2][xn] != 0) t = true;

                    }
                    while (r1 < xn - 1) {
                        r1++;
                        if (check[yn][r1] != 0) t = true;
                    }
                    while (r2 < xo - 1) {
                        r2++;
                        if (check[yn][r2] != 0) t = true;
                    }
                }
                else {
                    if (r1 < xn - 1) {
                        while (z1 < yn - 1) {
                            z1++;
                            r1++;
                            if (check[z1][r1] != 0) t = true;
                        }
                    }
                    else {
                        while (z1 < yn - 1) {
                            z1++;
                            r1--;
                            if (check[z1][r1] != 0) t = true;
                        }
                    }
                    if (r2 < xo - 1) {
                        while (z2 < yo - 1) {
                            z2++;
                            r2++;
                            if (check[z2][r2] != 0) t = true;
                        }
                    }
                    else {
                        while (z2 < yo - 1) {
                            z2++;
                            r2--;
                            if (check[z2][r2] != 0) t = true;
                        }
                    }
                }
                break;
            case -4:
                if (!((yo - yn) * (xo - xn) == 0 || (yn - yo == xn - xo) || (yn - yo == xo - xn))) {
                    t = true;
                    break;
                }
                if ((yo - yn) * (xo - xn) == 0) {
                    while (z1 < yn - 1) {
                        z1++;
                        if (check[z1][xn] != 0) t = true;

                    }
                    while (z2 < yo - 1) {
                        z2++;
                        if (check[z2][xn] != 0) t = true;

                    }
                    while (r1 < xn - 1) {
                        r1++;
                        if (check[yn][r1] != 0) t = true;
                    }
                    while (r2 < xo - 1) {
                        r2++;
                        if (check[yn][r2] != 0) t = true;
                    }
                }
                else {
                    if (r1 < xn - 1) {
                        while (z1 < yn - 1) {
                            z1++;
                            r1++;
                            if (check[z1][r1] != 0) t = true;
                        }
                    }
                    else {
                        while (z1 < yn - 1) {
                            z1++;
                            r1--;
                            if (check[z1][r1] != 0) t = true;
                        }
                    }
                    if (r2 < xo - 1) {
                        while (z2 < yo - 1) {
                            z2++;
                            r2++;
                            if (check[z2][r2] != 0) t = true;
                        }
                    }
                    else {
                        while (z2 < yo - 1) {
                            z2++;
                            r2--;
                            if (check[z2][r2] != 0) t = true;
                        }
                    }
                }
                break;
            case 5:
                if (!((yn - yo == 1 || yn - yo == -1 || yn - yo == 0) && (xn - xo == 1 || xn - xo == -1 || xn - xo == 0))) t = true;
                if ((xn - xo == 2 && check[7][7] == 1 && check[7][5] == 0) || (xn - xo == -2 && check[7][0] == 1 && check[7][3] == 0 && check[7][1] == 0)) {
                    t = false;
                   
                }
                break;
            case -5:
                if (!((yn - yo == 1 || yn - yo == -1 || yn - yo == 0) && (xn - xo == 1 || xn - xo == -1 || xn - xo == 0))) t = true;
                if ((xn - xo == 2 && check[0][7] == -1 && check[0][5] == 0) || (xn - xo == -2 && check[0][0] == -1 && check[0][3] == 0 && check[0][1] == 0)) {
                    t = false;
                    
                }
                break;
            default:
                t = false;
                break;
            }
            if (t) {
                position.erase(position.length() - 6, 6);
                
                break;
            }
        }
        
        

        if (check[yo][xo] == 5) {
            
            if (xn - xo == 2) {
                check[7][7] = 0;
                check[7][5] = 1;
            }
            else if (xn - xo == -2) {
                check[7][0] = 0;
                check[7][3] = 1;
            }
        }
        if (check[yo][xo] == -5) {
            if (xn - xo == 2) {
                check[0][7] = 0;
                check[0][5] = -1;
            }
            else if (xn - xo == -2) {
                check[0][0] = 0;
                check[0][3] = -1;
            }
        }
        int w = i;
        
        if (check[yo][xo] == 7 && yn == 0) {
            for (int i = 0; i < 32; i++) {
                if (f[i].getPosition() == oldPos) {
                    f[i].setTextureRect(IntRect(size * 3, size * 1, size, size));
                    check[yo][xo] = 4;
                    position[w + 4] = 'q';
                }
            }
        }

        if (check[yo][xo] == -7 && yn == 7) {
            for (int i = 0; i < 32; i++) {
                if (f[i].getPosition() == oldPos) {
                    f[i].setTextureRect(IntRect(size * 3, 0, size, size));
                    check[yo][xo] = -4;
                    position[w + 4] = 'q';
                }
            }
        }

        check[yn][xn] = check[yo][xo];
        if (check[yo][xo] == 6 && (yn != 6)) check[yn][xn] = 7;
        if (check[yo][xo] == -6 && (yn != 1)) check[yn][xn] = -7;
        if (oldPos != newPos) check[int(oldPos.y / size)][int(oldPos.x / size)] = 0;
        
        move(position.substr(i, 4));
           
    }

    /*
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << check[i][j] << " ";
        }
        std::cout << '\n';
    } 
    */
        
}

int hardlevel = 0;

void menu(RenderWindow& window) {
    Texture menuBackground, zai, bend;
    
    zai.loadFromFile("images/zai.png");
    bend.loadFromFile("images/bend.jpg");
    menuBackground.loadFromFile("images/chessimage.png");
    Sprite menuBg(menuBackground), zait(zai), bendt(bend);
    bool isMenu = 1;
    int menuNum = 0;

    menuBg.setPosition(0, 0);
    zait.setPosition(191, 20);
    bendt.setPosition(191, 275);

    while (isMenu)
    {

        hardlevel = 0;
        window.clear(Color(129, 181, 221));

        if (IntRect(191, 20, 300, 235).contains(Mouse::getPosition(window))) { hardlevel = 2; }
        if (IntRect(191, 275, 300, 300).contains(Mouse::getPosition(window))) {  hardlevel = 1; }
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            if (hardlevel == 0) {
                window.close();
                isMenu = false;
            }
            else {
                isMenu = false;
            }
        }

        window.draw(menuBg);
        window.draw(zait);
        window.draw(bendt);

        window.display();
    }
}

int main()
{
    RenderWindow window(VideoMode(682, 682), "The Chess");
    menu(window);
    wchar_t a[] = L"stockfish_15_x64_avx2.exe";
    ConnectToEngine(a);
    wchar_t b[] = L"stockfish-3-32-ja.exe";
    wchar_t c[] = L"stockfish.exe";
    if (hardlevel == 1) {
        ConnectToEngineA(b);
    }
    else {
        ConnectToEngineA(c);
    }
   
    Texture t1, t2;
    t1.loadFromFile("images//Figures_Chess.png");
    t2.loadFromFile("images//BoardChess.png");
    Sprite s(t1);
    Sprite sBoard(t2);

    SoundBuffer buffer;
    buffer.loadFromFile("sounds//movesound.ogg");
    Sound sound;
    sound.setBuffer(buffer);
    

    for (int i = 0; i < 32; i++) f[i].setTexture(t1);
    loadPosition();

    int povtorpos = 0;
    bool isMove = false;
    float dx = 0, dy = 0;
    Vector2f oldPos, newPos;
    std::string str;
    int n = 0;

    while (window.isOpen())
    {
        Vector2i posm = Mouse::getPosition(window);

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed && e) {
                if (Keyboard::isKeyPressed(Keyboard::Space)) {
                    d = true;
                   
                    if (position.length() % 12 == 0) {
                        str = getNextMove(position);
                    }
                    else {
                        str = getNextMoveA(position);
                    }
                    
                    if (str[5] != ' ') str[5] = ' ';
                    if (position.length() > 24) if (str == position.substr(position.length() - 24, 6)) {
                        povtorpos++;
                        
                    }
                    else {
                        povtorpos = 0;
                    }
                    
                    if (str == "(none ") {
                        if (position.length() % 12 == 0) {
                            f[28].setColor(Color::Red);
                        }
                        else {
                            
                            f[4].setColor(Color::Red);
                        }
                        e = false;
                    }
                    if (str == "error " || povtorpos == 6) {
                        
                        f[4].setColor(Color::Red);
                        f[28].setColor(Color::Red);

                        e = false;
                        
                    }
                    
                    //std::cout << "*******" << str << "*******" << std::endl;
                    //std::cout << "*******" << position.length() % 12 << "*******" << std::endl;
                    if (position.length() > 6) if (position.substr(position.length() - 12, 6) == str) d = false;
                    if (str != "error" && d) {
                        oldPos = toCoord(str[0], str[1]);
                        newPos = toCoord(str[2], str[3]);
                        for (int i = 0; i < 32; i++) if (f[i].getPosition() == oldPos) n = i;
                        sound.play();
                        for (int k = 0; k < 50; k++) {
                            Vector2f p = newPos - oldPos;
                            f[n].move(p.x / 50, p.y / 50);
                            window.draw(sBoard);
                            for (int i = 0; i < 32; i++) window.draw(f[i]); window.draw(f[n]);
                            window.display();
                        }
                        //move(str); 
                        //f[n].setPosition(newPos);
                        //position += str + " ";
                        position += str;
                        loadPosition();

                    }
                   
                    //std::cout << position.length() << std::endl;
                    //std::cout << position << std::endl;
                }
                
                if (event.key.code == Keyboard::BackSpace && position.length() > 0) {
                    position.erase(position.length() - 6, 6);
                    loadPosition();
                    
                }
            }
            if (event.type == Event::MouseButtonPressed && e)
                for (int i = 0; i < 32; i++)
                    if (f[i].getGlobalBounds().contains(posm.x, posm.y)) {
                        isMove = true;
                        n = i;
                        dx = posm.x - f[i].getPosition().x;
                        dy = posm.y - f[i].getPosition().y;
                        oldPos = f[i].getPosition();
                    }
            
            if (event.type == Event::MouseButtonReleased && e)
                if (event.key.code == Mouse::Left) {
                    isMove = false;
                    Vector2f p = f[n].getPosition() + Vector2f(size / 2, size / 2);
                    Vector2f newPos = Vector2f(size * int(p.x / size), size * int(p.y / size));
                    
                    str = toChessNote(oldPos) + toChessNote(newPos);
                    sound.play();
                    //move(str);
                    position += str + "  ";
                    //std::cout << position.length() << std::endl;
                    
                    
                    if (position.length() >= 12) {
                        if (position.substr(position.length() - 6, 4) == position.substr(position.length() - 12, 4)) {
                            //std::cout << position.length() << std::endl;
                            position.erase(position.length() - 6, 5);
                            
                        }
                    }

                    if (position.length() > 30) if (position.substr(position.length() - 6, 6) == position.substr(position.length() - 30, 6)) {
                        povtorpos++;
                        //std::cout << povtorpos << std::endl;
                    }
                    else {
                        povtorpos = 0;
                    }
                   
                    loadPosition();
                    
                    /*if (getNextMove(position) == "(none ") {
                        if (position.length() % 12 == 0) {
                            f[28].setColor(Color::Red);
                        }
                        else {

                            f[4].setColor(Color::Red);
                        }
                    }*/
                    
                    //std::cout << position << std::endl;

                    //f[n].setPosition(newPos);
                }

        }
        
        if (isMove) f[n].setPosition(posm.x - dx, posm.y - dy);

        window.clear();
        window.draw(sBoard);
        for (int i = 0; i < 32; i++) window.draw(f[i]);
        window.display();
    }
    CloseConnection();
    return 0;
}
