# Reinforcement Learning

This code demonstrates how to make a reinforcement learning algorithm from scratch in C++.

# Download, Compile & Run

```
git clone https://github.com/OmarAflak/Reinforcement-Learning-CPP
cd Reinforcement-Learning-CPP
make
./main
```
# Environment

The agent has to go from point A to point B.

    A..........B
    
He has two available commands : `left` and `righ`.

The reward policy is the following :

* if (agent reaches point **B**) { reward=1 }
* else { reward=0 }

# Network

Although the problem is very simple, I used a Neural Network to approach the **Q(s,a)** function.

The network takes the current state of the game as an input, and it outputs the **Q-value** for each possible action.
