#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cmath>

#include "../include/matrix.h"
#include "../include/environment.h"

using namespace std;

// params
Matrix X, W1, H, W2, Y, B1, B2, Y2, dJdB1, dJdB2, dJdW1, dJdW2;
double learningRate;

// used to init random weights and biases
double random(double x){
    return (double)(rand() % 10000 + 1)/10000-0.5;
}

// the sigmoid function
double sigmoid(double x){
    return 1/(1+exp(-x));
}

// the derivative of the sigmoid function
double sigmoidePrime(double x){
    return exp(-x)/(pow(1+exp(-x), 2));
}

double stepFunction(double x){
    if(x>0.9){
        return 1.0;
    }
    if(x<0.1){
        return 0.0;
    }
    return x;
}

void init(int inputNeuron, int hiddenNeuron, int outputNeuron, double rate){
    learningRate = rate;

    W1 = Matrix(inputNeuron, hiddenNeuron);
    W2 = Matrix(hiddenNeuron, outputNeuron);
    B1 = Matrix(1, hiddenNeuron);
    B2 = Matrix(1, outputNeuron);

    W1 = W1.applyFunction(random);
    W2 = W2.applyFunction(random);
    B1 = B1.applyFunction(random);
    B2 = B2.applyFunction(random);
}

// forward propagation
Matrix computeOutput(vector<double> input){
    X = Matrix({input}); // row matrix
    H = X.dot(W1).add(B1).applyFunction(sigmoid);
    Y = H.dot(W2).add(B2).applyFunction(sigmoid);
    return Y;
}

// back propagation and params update
void learn(vector<double> expectedOutput){
    Y2 = Matrix({expectedOutput}); // row matrix

    // Loss J = 1/2 (expectedOutput - computedOutput)^2
    // Then, we need to calculate the partial derivative of J with respect to W1,W2,B1,B2

    // compute gradients
    dJdB2 = Y.subtract(Y2).multiply(H.dot(W2).add(B2).applyFunction(sigmoidePrime));
    dJdB1 = dJdB2.dot(W2.transpose()).multiply(X.dot(W1).add(B1).applyFunction(sigmoidePrime));
    dJdW2 = H.transpose().dot(dJdB2);
    dJdW1 = X.transpose().dot(dJdB1);

    // update weights and biases
    W1 = W1.subtract(dJdW1.multiply(learningRate));
    W2 = W2.subtract(dJdW2.multiply(learningRate));
    B1 = B1.subtract(dJdB1.multiply(learningRate));
    B2 = B2.subtract(dJdB2.multiply(learningRate));
}

std::vector<double> toCategorical(int n, int max){
    std::vector<double> v(max, 0);
    v[n] = 1;
    return v;
}

void getMax(const Matrix& row, int* y, int* x, double* value){
    double max=0;
    int ii=0, jj=0;
    for(int i=0 ; i<row.getHeight() ; i++){
        for(int j=0 ; j<row.getWidth() ; j++){
            if(max<row.get(i, j)){
                max = row.get(i, j);
                ii = i;
                jj = j;
            }
        }
    }

    if(x!=NULL){
        *x = jj;
    }
    if(y!=NULL){
        *y = ii;
    }
    if(value!=NULL){
        *value = max;
    }
}

int main(int argc, char *argv[]){
    srand (time(NULL)); // to generate random weights
    Environment game; // init environment

    // init network
    // input : game state (ex: [1,0,0,0,0,0,0,0,0,0] when agent is at the first position)
    // output : action to take
    init(game.length, 10, game.actionsCount, 0.1);

    // q learning var
    double gamma = 0.8;
    double eps = 1.0;
    double epsDecay = 0.95;

    // train
    int episode = 100;
    for (int i=0 ; i<episode ; i++){
        cout << "Episode " << i+1 << "/" << episode << "\t";
        int steps=0, maxIndex=0, action=0, position=0;
        double qsa=0, max=0;
        bool done=false;

        position = game.reset();

        while(!done && steps<300){
            steps++;

            // take random actions sometimes to allow game exploration
            if(((double) rand() / RAND_MAX) < eps){
                action = rand()%game.actionsCount;
            }
            else{
                Matrix actions = computeOutput({toCategorical(position, game.length)});
                getMax(actions, NULL, &action, NULL);
            }

            // take action
            Feedback fb = game.step(action);

            // get max action in next state
            Matrix nextActions = computeOutput({toCategorical(fb.position, game.length)});
            getMax(nextActions, NULL, &maxIndex, &max);
            qsa = fb.reward + gamma*max;

            // update network
            std::vector<double> in(game.actionsCount);
            for(int i=0 ; i<nextActions.getWidth() ; i++){
                in[i] = nextActions.get(0, i);
            }
            in[maxIndex] = qsa;
            learn({in});

            done = fb.done;
            position = fb.position;
            eps *= epsDecay;
        }
        cout << "[" << steps << " steps]" << endl;
    }

    // testing
    cout << "\n\nPlaying game..." << endl;
    usleep(1000000);

    int action, position = game.reset();
    bool done = false;
    while(!done){
        game.render();
        std::cout << "\r";

        Matrix actions = computeOutput({toCategorical(position, game.length)});
        getMax(actions, NULL, &action, NULL);
        Feedback fb = game.step(action);
        done = fb.done;
        position = fb.position;

        usleep(200000);
    }
}
