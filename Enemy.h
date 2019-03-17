
#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
public:
    
    float pos[3];

    //Vec saiba[3];
    Enemy () {
     
     srand(time(0));
     /*for (int i = 0; i < 1; i++) {
        saiba[i][0] = 800;
        saiba[i][1] = (rand() % 500 + 1);
        saiba[i][2] = 0.0;
     }*/
     pos[0] = 0.0;
     pos[1] = 0.0;
     pos[2] = 0.0;
    }
}Enemy;


#endif