//
// Created by Scott on 12/07/2020.
//

#ifndef ONGRID_SETTING_H
#define ONGRID_SETTING_H


#include <stdlib.h>
#include <time.h>

class Setting {
public:
    static void init() {
        srand(time(NULL));
    }

    static int getRandomNumber(int min, int max) {
        return rand() % max + min;
    }

private:

};

#endif //ONGRID_SETTING_H
