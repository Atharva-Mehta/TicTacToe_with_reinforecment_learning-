#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <utility>
#include <vector>
#include <bits/stdc++.h>
#include <time.h>
#include <fstream>

using namespace std;

class Random {
    string agent_name;
public:
    char ID;

    Random(char agent_ID) {
        ID = agent_ID;
        agent_name = "Random Agent";
    }
    int get_move(string state) {
    int move = 0;
        do {
            move = rand() % 9;
        } while(state.at(move) != '-');
        return move;
    }
    void calculate(char result) {
        return;
    }
    void reset() {
        return;
    }
};

class SimpleAgent {
    string agent_name;
public:
    char ID;

    SimpleAgent(char agent_ID) {
        ID = agent_ID;
        agent_name = "Simple Agent";
    }

    int get_move(string state) {
    int move = 0;
        for (size_t i=0; i<state.length(); i++) {
            if (state[i] == '-')
                move = i;
        }
        return move;
    }

    void calculate(char result) {
        return;
    }

    void reset() {
        return;
    }
};

class Q_Agent {
public:
    string agent_name;
    map<string, vector<double>> states;
    double alpha = 0.7;
    double discount = 0.8;
    double init = 0.6;
    double epsilon;
    const double min_epsilon = 0.2;
    const double max_epsilon = 1;
    double epsilon_decay_rate = 0.01;
    vector<pair<string, int>> recorded_states;

public:
    char ID;

    Q_Agent(char agent_ID) {
        ID = agent_ID;
        agent_name = "Q-Agent";
        epsilon = 1;
    }

    void reset() {
        recorded_states.clear();
    }

    void calculate(const double reward) {
        reverse(recorded_states.begin(), recorded_states.end());
        double maximum = 0.0;
        bool first = true;

        for (auto recorded_state: recorded_states) {
          if (first) {
              states[recorded_state.first][recorded_state.second] = reward;
              first = false;
          }
          else {
              double epsilon_threshold = static_cast<double>((rand() % 100))/100;
              if (epsilon_threshold > epsilon) {
                first = true;
              }
              else {
                states[recorded_state.first][recorded_state.second] = (1 - alpha) * states[recorded_state.first][recorded_state.second] + alpha * (discount * maximum);
              }
          }
          maximum = *max_element(states[recorded_state.first].begin(), states[recorded_state.first].end());
        }
       epsilon = min_epsilon + (max_epsilon - min_epsilon) * exp(-epsilon_decay_rate * 2);
    }

    vector<double> get_q(const string state) {
        if (!(states.find(state) == states.end()))
            return states[state];

        else {
            vector<double> res;
            for (size_t i = 0; i < 9; i++) {
                if (state.at(i) == '-')
                    res.push_back(init);
                else
                    res.push_back(-10000000.0);
            }
            states[state] = res;
            return res;
        }
    }

    int get_move(const string state) {
        vector<double> q_vals = get_q(state);
        double max_q = *max_element(q_vals.begin(), q_vals.end());
        vector<int> max_val_indices;

        for (size_t i=0; i<q_vals.size(); i++) {
            if (q_vals[i] == max_q) {
                max_val_indices.push_back(i);
            }
        }
        int move = max_val_indices.at(rand() % max_val_indices.size());

        pair<string, int> recorded_state (state, move);
        recorded_states.push_back(recorded_state);
        return move;
    }
};

class TicTacToe {
private:
    string state;

public:
    TicTacToe() {
        state = "---------";
    }

    void reset_game() {
        state = "---------";
    }

    char game_over() {
        for (size_t i=0; i<state.length(); i++) {
            if (state[i] == '-') {
                return 'K';
            }
        }
        int checks[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7} , {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};
        for (int i=0; i<8; i++) {
            string test = string(1, state[checks[i][0]]) + string(1, state[checks[i][1]]) + string(1, state[checks[i][2]]);
            if (test == "XXX") {
                return 'X';
            } else if (test == "OOO") {
                return 'O';
            }
        }
        return 'T';
    }

    bool play_move(int move, char ID) {
        if (state[move] != '-') {
            return false;
        }
        state[move] = ID;
        return true;
    }

    double get_reward(char result, char ID) {
        if (result == 'T') {
            return 0.5;
        } else if (result == ID) {
            return 1;
        } else {
            return 0;
        }
    }

    vector<long> train(Q_Agent &a_1, Q_Agent &a_2, int episodes) {
        vector<long> win_ties;
        if (a_1.ID == a_2.ID) {
            return win_ties;
        }
        long a_1_wins = 0;
        long a_1_ties = 0;
        for (int iter=0; iter<episodes; iter++) {
            char result = 'K';
            char turn = 'X';
            for (int j=0; j<9; j++) {
                if (a_1.ID == turn) {
                    int move = a_1.get_move(state);
                    play_move(move, a_1.ID);
                    turn = 'O';
                    result = game_over();
                    if (result != 'K') {
                        break;
                    }
                } else {
                    int move = a_2.get_move(state);
                    play_move(move, a_2.ID);
                    turn = 'X';
                    result = game_over();
                    if(result != 'K') {
                        break;
                    }
                }
            }
            if (result != 'K') {
                a_1.calculate(get_reward(result, a_1.ID));
                a_2.calculate(get_reward(result, a_2.ID));
                a_1.reset();
                a_2.reset();
                reset_game();

                if (result == a_1.ID) {
                    a_1_wins += 1;
                }
                else if (result == 'T') {
                    a_1_ties += 1;
                }
            }
        }
        win_ties.push_back(a_1_wins);
        win_ties.push_back(a_1_ties);
        return win_ties;
    }
};



int main()
{
    srand(time(0));
    Q_Agent p1('X');
    Q_Agent p2('O');
    Random p3 ('X');
    Random p4 ('O');
    TicTacToe t;
    vector<double> wins;
    vector<double> ties;
    vector<double> losses;
    vector<int> count;
    int batch = 128;
    double LR, DF, DR;
    cout<<"Enter the learning Rate: ";
    cin>>LR;
    cout<<"\n";
    cout<<"Enter the Discount Factor: ";
    cin>>DF;
    cout<<"\n";
    cout<<"Enter the Exploration Decay rate : ";
    cin>>DR;
    cout<<"\n";
    fstream fio;
    fio.open("QvsQ.txt", ios::trunc | ios::out | ios::in);
    p1.alpha=LR;
    p2.alpha=LR;
    p1.discount=DR;
    p2.discount=DR;
    for(size_t i=0;i<500;i++){
        //cout<<(i+1)<<endl;
        vector<long> res = t.train(p1, p2, batch);
    	wins.push_back(static_cast<double>(res[0]) / batch);
    	ties.push_back(static_cast<double>(res[1]) / batch);
    	losses.push_back((batch - res[0] - res[1]) / batch);
    	count.push_back(batch * i);
    }

    for (size_t j = 0; j < wins.size(); j++) {
            fio << wins[j] << endl;
            cout << wins[j] << endl;
    }


}
