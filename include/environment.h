#ifndef ENVIRONMENT
#define ENVIRONMENT

#include <string>
#include <iostream>

struct Feedback {
    int position;
    double reward;
    bool done;

    Feedback(int position, double reward, bool done){
        this->position = position;
        this->reward = reward;
        this->done = done;
    }
};

struct Environment {
    const double actions[2] = {0, 1}; // left, right
    const int actionsCount = 2;
    int position;
    int length;

    Environment(){
        length = 10;
        position = 0;
    }

    Feedback step(double action){
        if(action==actions[0]){
            if(position>0){
                position--;
            }
        }

        if(action==actions[1]){
            if(position<length){
                position++;
            }
        }

        if(position==length)
            return Feedback(position, 1, true);
        else
            return Feedback(position, 0, false);
    }

    int reset(){
        position = 0;
        return 0;
    }

    void render(){
        std::cout << "[";
        for(int i=0 ; i<length ; i++){
            std::cout << (i==position?"X":"_");
        }
        std::cout << "]" << std::flush;
    }
};

#endif
