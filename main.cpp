//Projet made by Cauã Ribas Devitte

#include <iostream>
#include <fstream>
#include <conio.h> //getch()
#include <windows.h>
#include <ctime>
#include <string>
#include <sstream> //Library used to save and load game

using namespace std;

struct PLAYER {
    int x,y;
    int fov = 4;
    bool flag = true;//Bool that alows characters to move or not
    double timeSaved;

    void coord (int li, int co){
        x=li;
        y=co;
    }
};

struct ARQUIVO {
    char* nome;
    int x, y;
    int** m;

    void tam(int li, int co){
        x=li;
        y=co;
    }

    void carrega (){
        m = new int*[x];
        for (int i=0; i<x; i++){
            m[i]=new int[y];
        }

        ifstream mapa;
        mapa.open(nome);
        char c;

        ///Puts map on a 2D array
        for(int i=0; i<x;i++){
            for(int j=0; j<y;j++){
                    mapa>>c;
                    m[i][j]=c-48;
            }
        }
    }

};

void posicaoxy( int column, int line ){
        COORD coord;
        coord.X = column;
        coord.Y = line;
        SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
}

double relogio(clock_t inicio){

    clock_t soma=33.33;

    posicaoxy(47,3);

    double time = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;

    return time;
}

void mostrarCursor(bool showFlag){
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void colorir (int cor){

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(out, cor);
}

void Save(PLAYER &P1, PLAYER &P2){

    cout << "Saving game... " << endl;
    Sleep(500);
    system("cls");

    ofstream saveFile ("Save.txt");

    saveFile << P1.x << ",";
    saveFile << P1.y << ",";
    saveFile << P2.x << ",";
    saveFile << P2.y << ",";

    saveFile.close();
}

void Load(PLAYER &P1, PLAYER &P2){

    ifstream loadFile;
    loadFile.open("Save.txt", ifstream::in);

    cout << "Loading file..." ;
    Sleep(500);
    system("cls");

    string Player1X, Player1Y, Player2X, Player2Y, STimeSaved;//Variables to store player positions as strings
    string line;

    if(loadFile.is_open()){
        while(getline(loadFile, line)){
            stringstream ss(line);

            getline(ss, Player1X, ',');
            P1.x = stoi(Player1X);//Variables string to integers

            getline(ss, Player1Y, ',');
            P1.y = stoi(Player1Y);//Variables string to integers

            getline(ss, Player2X, ',');
            P2.x = stoi(Player2X);//Variables string to integers

            getline(ss, Player2Y, ',');
            P2.y = stoi(Player2Y);//Variables string to integers
        }
        loadFile.close();
    }
}

void HUD(){
    cout << "\n\n\n\n\n\n\n\n\n\n\n";
    cout << " " << char(177) << " ---> Teleport\n";
    cout << " " << char(157) << " ---> FOV reducer\n";
    cout << " " << char(207) << " ---> Motion blocker\n";
}

void MostrarMapa (PLAYER &P1, ARQUIVO mapa){
    posicaoxy(0,0);
    for (int i = P1.x - P1.fov; i < P1.x + P1.fov; i++){
        for (int j = P1.y - P1.fov; j < P1.y + P1.fov; j++){
            if(i < 0 || j < 0 || i >= mapa.x || j >= mapa.y)//No negative indexes on 2D array
                cout<<" ";
            else {
                switch(mapa.m[i][j]){
                case 0:
                    cout<<" ";
                break;
                case 1:
                    cout<< char(219);
                break;
                case 2:
                    cout<< char(1);
                break;
                case 3:
                    cout << char(177);
                break;
                case 4:
                    cout << char(157);
                break;
                case 5:
                    cout << char(207);
                break;
                }//switch
            }//else
        }
        cout<<"\n";
    }
    HUD();
}

void MonstrarMapaTransposto (PLAYER &P2, ARQUIVO mapa){
    posicaoxy(0,0);
    for (int i = P2.y - P2.fov ; i < P2.y + P2.fov; i++){
        cout << "\t\t\t\t\t\t\t\t\t\t\t\t";
        for (int j = P2.x - P2.fov ; j < P2.x + P2.fov ; j++){
            if(i < 0 || j < 0 || i >= mapa.x || j>= mapa.y)//No negative indexes on 2D array
                cout<<" ";
            else {
                switch(mapa.m[j][i]){
                case 0:
                    cout<<" ";
                break;
                case 1:
                    cout<< char(219);
                break;
                case 2:
                    cout<< char(120);
                break;
                case 3:
                    cout << char(177);
                break;
                case 4:
                    cout << char(157);
                break;
                case 5:
                    cout << char(207);
                break;
                }//switch
            }//else
        }
        cout<<"\n";
    }
}

void Movimentacao (PLAYER &P1, PLAYER &P2, ARQUIVO &mapa){

    clock_t soma=33.33;
    clock_t inicio = clock();

    if(kbhit()){//Code keeps updating even if there is no key being pressed
        char tecla=getch();
        switch (tecla){//Player moviment switch
            case 'w':
                if(P1.flag == true){//Bool that alows characters to move or not
                    if(mapa.m[P1.x-1][P1.y]==0){
                        mapa.m[P1.x][P1.y]=0;
                        mapa.m[P1.x-1][P1.y]=2;
                        P1.x=P1.x-1;
                    }
                    if(mapa.m[P1.x-1][P1.y]==3){//Teleport
                        mapa.m[P1.x][P1.y]=0;
                        mapa.m[P1.x-1][P1.y]=3;
                        P1.x = 1 + (rand() % 18);
                        P1.y = 1 + (rand() % 18);
                        if(mapa.m[P1.x][P1.y] == 1 || mapa.m[P1.x][P1.y] == 3)//Preventing player to teleport inside walls or other portal
                            P1.x++;
                    }
                    if(mapa.m[P1.x-1][P1.y]==4){//FOV reducer
                        mapa.m[P1.x][P1.y]=0;
                        mapa.m[P1.x-1][P1.y]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        system("cls");

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;
                            P2.fov = 2;

                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                loop = false;
                                P2.fov = 4;
                            }
                        }
                        system("cls");
                    }
                    if(mapa.m[P1.x-1][P1.y]==5){//Motion blocker
                        mapa.m[P1.x][P1.y]=0;
                        mapa.m[P1.x-1][P1.y]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;

                            P2.flag = false;
                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                P2.flag = true;
                                loop = false;
                            }
                        }
                    }
                }//end if
            break;
        case 's':
            if(P1.flag == true){//Bool that alows characters to move or not
                    if(mapa.m[P1.x+1][P1.y]==0){
                    mapa.m[P1.x][P1.y]=0;
                    mapa.m[P1.x+1][P1.y]=2;
                    P1.x=P1.x+1;
                }
                if(mapa.m[P1.x+1][P1.y]==3){//Teleport
                    mapa.m[P1.x][P1.y]=0;
                    mapa.m[P1.x+1][P1.y]=3;
                    P1.x = 1 + (rand() % 18);
                    P1.y = 1 + (rand() % 18);
                    if(mapa.m[P1.x][P1.y] == 1 || mapa.m[P1.x][P1.y] == 3)//Preventing player to teleport inside walls or other portal
                        P1.x++;
                }
                if(mapa.m[P1.x+1][P1.y]==4){//FOV reducer
                    mapa.m[P1.x][P1.y]=0;
                    mapa.m[P1.x+1][P1.y]=0;

                    clock_t inicio = clock();//Start time
                    double tempo;
                    double delay = 5;
                    bool loop = true;

                    system("cls");

                    while (loop){
                        tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;
                        P2.fov = 2;

                        cout << relogio(P1.timeSaved);

                        MostrarMapa(P1, mapa);
                        MonstrarMapaTransposto(P2, mapa);
                        Movimentacao(P1, P2, mapa);

                        if (tempo >= delay){
                            loop = false;
                            P2.fov = 4;
                        }
                    }
                    system("cls");
                }
                if(mapa.m[P1.x+1][P1.y]==5){//Motion blocker
                        mapa.m[P1.x][P1.y]=0;
                        mapa.m[P1.x+1][P1.y]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;

                            P2.flag = false;
                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                P2.flag = true;
                                loop = false;
                            }
                        }
                    }
                }
            break;
        case 'a':
            if(P1.flag == true){//Bool that alows characters to move or not
                if(mapa.m[P1.x][P1.y-1]==0){
                    mapa.m[P1.x][P1.y]=0;
                    mapa.m[P1.x][P1.y-1]=2;
                    P1.y=P1.y-1;
                }
                if(mapa.m[P1.x][P1.y-1]==3){//Teleport
                    mapa.m[P1.x][P1.y]=0;
                    mapa.m[P1.x][P1.y-1]=3;
                    P1.x = 1 + (rand() % 18);
                    P1.y = 1 + (rand() % 18);
                    if(mapa.m[P1.x][P1.y] == 1 || mapa.m[P1.x][P1.y] == 3)//Preventing player to teleport inside walls or other portal
                        P1.x++;
                }
                if(mapa.m[P1.x][P1.y-1]==4){//FOV reducer
                    mapa.m[P1.x][P1.y]=0;
                    mapa.m[P1.x][P1.y-1]=0;

                    clock_t inicio = clock();//Start time
                    double tempo;
                    double delay = 5;
                    bool loop = true;

                    system("cls");

                    while (loop){
                        tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;
                        P2.fov = 2;

                        cout << relogio(P1.timeSaved);

                        MostrarMapa(P1, mapa);
                        MonstrarMapaTransposto(P2, mapa);
                        Movimentacao(P1, P2, mapa);

                        if (tempo >= delay){
                            loop = false;
                            P2.fov = 4;
                        }
                    }
                    system("cls");
                }
                if(mapa.m[P1.x][P1.y-1]==5){//Motion blocker
                        mapa.m[P1.x][P1.y]=0;
                        mapa.m[P1.x][P1.y-1]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;

                            P2.flag = false;
                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                P2.flag = true;
                                loop = false;
                            }
                        }
                    }
                }
            break;
        case 'd':
            if(P1.flag == true){//Bool that alows characters to move or not
                if(mapa.m[P1.x][P1.y+1]==0){
                    mapa.m[P1.x][P1.y]=0;
                    mapa.m[P1.x][P1.y+1]=2;
                    P1.y=P1.y+1;
                }
                if(mapa.m[P1.x][P1.y+1]==3){//Teleport
                    mapa.m[P1.x][P1.y]=0;
                    mapa.m[P1.x][P1.y+1]=3;
                    P1.x = 1 + (rand() % 18);
                    P1.y = 1 + (rand() % 18);
                    if(mapa.m[P1.x][P1.y] == 1 || mapa.m[P1.x][P1.y] == 3)//Preventing player to teleport inside walls or other portalC
                        P1.x++;
                }
                if(mapa.m[P1.x][P1.y+1]==4){//FOV reducer
                    mapa.m[P1.x][P1.y]=0;
                    mapa.m[P1.x][P1.y+1]=0;

                    clock_t inicio = clock();//Start time
                    double tempo;
                    double delay = 5;
                    bool loop = true;

                    system("cls");

                    while (loop){
                        tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;
                        P2.fov = 2;

                        cout << relogio(P1.timeSaved);

                        MostrarMapa(P1, mapa);
                        MonstrarMapaTransposto(P2, mapa);
                        Movimentacao(P1, P2, mapa);

                        if (tempo >= delay){
                            loop = false;
                            P2.fov = 4;
                        }
                    }
                    system("cls");
                }
                if(mapa.m[P1.x][P1.y+1]==5){//Motion blocker
                        mapa.m[P1.x][P1.y]=0;
                        mapa.m[P1.x][P1.y+1]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;

                            P2.flag = false;
                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                P2.flag = true;
                                loop = false;
                            }
                        }
                    }
                }
                break;
            case 75://KEY-LEFT
                if(P2.flag == true){//Bool that alows characters to move or not
                    if(mapa.m[P2.x-1][P2.y]==0){
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x-1][P2.y]=2;
                        P2.x=P2.x-1;
                    }
                    if(mapa.m[P2.x-1][P2.y]==3){//Teleport
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x-1][P2.y]=3;
                        P2.x = 1 + (rand() % 18);
                        P2.y = 1 + (rand() % 18);
                        if(mapa.m[P2.x][P2.y] == 1 || mapa.m[P2.x][P2.y] == 3)//Preventing player to teleport inside walls or other portal
                            P2.x++;
                    }
                    if(mapa.m[P2.x-1][P2.y]==4){//FOV reducer
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x-1][P2.y]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        system("cls");

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;
                            P1.fov = 2;

                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                loop = false;
                                P1.fov = 4;
                            }
                        }
                        system("cls");
                    }
                    if(mapa.m[P2.x-1][P2.y]==5){//Motion blocker
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x-1][P2.y]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;


                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;

                            P1.flag = false;
                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                P1.flag = true;
                                loop = false;
                            }
                        }
                    }
                }
                break;
            case 77://KEY-RIGHT
                if(P2.flag == true){//Bool that alows characters to move or not
                    if(mapa.m[P2.x+1][P2.y]==0){
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x+1][P2.y]=2;
                        P2.x=P2.x+1;
                    }
                    if(mapa.m[P2.x+1][P2.y]==3){//Teleport
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x+1][P2.y]=3;
                        P2.x = 1 + (rand() % 18);
                        P2.y = 1 + (rand() % 18);
                        if(mapa.m[P2.x][P2.y] == 1 || mapa.m[P2.x][P2.y] == 3)//Preventing player to teleport inside walls or other portal
                            P2.x++;
                    }
                    if(mapa.m[P2.x+1][P2.y]==4){//FOV reducer
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x+1][P2.y]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        system("cls");

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;
                            P1.fov = 2;

                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                loop = false;
                                P1.fov = 4;
                            }
                        }
                        system("cls");
                    }
                    if(mapa.m[P2.x+1][P2.y]==5){//Motion blocker
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x+1][P2.y]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;

                            P1.flag = false;
                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                P1.flag = true;
                                loop = false;
                            }
                        }
                    }
                }
                break;
            case 72://KEY-UP
                if(P2.flag == true){//Bool that alows characters to move or not
                    if(mapa.m[P2.x][P2.y-1]==0){
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x][P2.y-1]=2;
                        P2.y=P2.y-1;
                    }
                    if(mapa.m[P2.x][P2.y-1]==3){//Teleport
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x][P2.y-1]=3;
                        P2.x = 1 + (rand() % 18);
                        P2.y = 1 + (rand() % 18);
                        if(mapa.m[P2.x][P2.y] == 1 || mapa.m[P2.x][P2.y] == 3)//Preventing player to teleport inside walls or other portal
                            P2.x++;
                    }
                    if(mapa.m[P2.x][P2.y-1]==4){//FOV reducer
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x][P2.y-1]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        system("cls");

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;
                            P1.fov = 2;

                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                loop = false;
                                P1.fov = 4;
                            }
                        }
                        system("cls");
                    }
                    if(mapa.m[P2.x][P2.y-1]==5){//Motion blocker
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x][P2.y-1]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;

                            P1.flag = false;
                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                P1.flag = true;
                                loop = false;
                            }
                        }
                    }
                }
                break;
            case 80://KEY-DOWN
                if(P2.flag == true){//Bool that alows characters to move or not
                    if(mapa.m[P2.x][P2.y+1]==0){
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x][P2.y+1]=2;
                        P2.y=P2.y+1;
                    }
                    if(mapa.m[P2.x][P2.y+1]==3){//Teleport
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x][P2.y+1]=3;
                        P2.x = 1 + (rand() % 18);
                        P2.y = 1 + (rand() % 18);
                        if(mapa.m[P2.x][P2.y] == 1 || mapa.m[P2.x][P2.y] == 3)//Preventing player to teleport inside walls or other portal
                            P2.x++;
                    }
                    if(mapa.m[P2.x][P2.y+1]==4){//FOV reducer
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x][P2.y+1]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        system("cls");

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;
                            P1.fov = 2;

                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa);
                            MonstrarMapaTransposto(P2, mapa);
                            Movimentacao(P1, P2, mapa);

                            if (tempo >= delay){
                                loop = false;
                                P1.fov = 4;
                            }
                        }
                        system("cls");
                    }
                    if(mapa.m[P2.x][P2.y+1]==5){//Motion blocker
                        mapa.m[P2.x][P2.y]=0;
                        mapa.m[P2.x][P2.y+1]=0;

                        clock_t inicio = clock();//Start time
                        double tempo;
                        double delay = 5;
                        bool loop = true;

                        while (loop){
                            tempo = (clock() - inicio + soma) / (double) CLOCKS_PER_SEC;

                            P1.flag = false;
                            cout << relogio(P1.timeSaved);

                            MostrarMapa(P1, mapa); //shows the perspective of player 1
                            MonstrarMapaTransposto(P2, mapa); //shows the perspective of player 2
                            Movimentacao(P1, P2, mapa); //Moviment function

                            if (tempo >= delay){
                                P1.flag = true;
                                loop = false;
                            }
                        }
                    }
                }
            break;
        case 27://ESC
            system("cls");

            int menuOp2;

            //system("Color 02");

            cout << "[1]-Continue\n";
            cout << "[2]-Load Game\n";
            cout << "[3]-Save Game\n";
            cout << "[4]-Rankings\n";//did not finish rankings lol
            cout << "[5]-Leave\n";

            cin >> menuOp2;

            if(menuOp2 == 1){
                system("cls");
            }
            else if(menuOp2 == 2){
                system("cls");
                Load(P1, P2);
                break;
            }
            else if(menuOp2 == 3){
                system("cls");
                Save(P1, P2);
                break;
            }
            else if(menuOp2 == 4){
                system("cls");
                //LeaderBoard(P1);
                system("pause");
                system("cls");
                break;
            }
            else{
                exit(0);
            }
            //system("pause");
            break;
        }//end switch
    }//end if
}

int main(){

    srand (time(NULL));

    mostrarCursor(false);
    PLAYER P1, P2;//struct player 1 e 2

    P1.x = 12;
    P1.y = 12;

    P2.x = 1;
    P2.y = 1;

    P1.coord(P1.x, P1.x);
    P2.coord(P2.x,P2.y);

    cout << "[1]-New Game\n";
    cout << "[2]-Load Game\n";
    cout << "[3]-Save Game\n";

    int menuOp;
    cin >> menuOp;
    system ("cls");

    if(menuOp == 1){
        //continue;
    }
    else if(menuOp == 2){
        system("cls");
        Load(P1, P2);
        Sleep(500);
    }
    else if(menuOp == 3){
        system("cls");
        Save(P1, P2);
        Sleep(500);
    }
    else{
        cout << "Invalid command\n" << endl;
    }

    ARQUIVO mapa;
    mapa.nome="maze.txt";
    mapa.tam(20,20);
    mapa.carrega();

    bool loop = true;
    while (loop){

        clock_t inicio;
        inicio = clock(); // start time
        //cout << "\t\t\t\t\t   Game time";
        cout << relogio(P1.timeSaved);

        MostrarMapa(P1, mapa);
        MonstrarMapaTransposto(P2, mapa);
        Movimentacao(P1, P2, mapa);

        if(abs(P1.x - P2.x) < P2.fov && abs(P1.y - P2.y) < P2.fov)//Collision detector
            loop = false;
    }
    //Deleting 2D after game over
    if(loop == false){
        for(int i = 0; i < mapa.x; i++){
            delete mapa.m[i];
        }
        delete mapa.m;
    }

    system("cls");
    cout << "You won the game!!! \n" << endl;
    cout << relogio(P1.timeSaved) << "\n";
    system("pause");

    return 0;
}
