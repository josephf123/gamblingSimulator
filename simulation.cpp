#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include <math.h>
using namespace std;

class Agent {       
    public:
        unsigned long long int startMoney;     
        int ZorNothing;      // Could be double or nothing, triple or nothing etc. This will be used as the bet progression.
                                // If equal to zero, fibonacci, if equal to 1, single bets, if equal to startMoney, all in.
        double winRate;
        unsigned long long int currMoney;
        unsigned long long int maxMoney[2];        // [Amount of bets needed to get to maxMoney, the maxMoney value]
        unsigned long long int betsPlaced;         // Total amount of bets placed before termination
        unsigned long long int prevBetAmount;   // This is only for fibonacci sequence as prevBetAmount is needed
        unsigned long long int currBetAmount;  // Starts off as 1, then depending on ZorNothing, will progress
        string whenToStop;
        bool hasAchievedGoal;

        bool simulateCoinFlip();

        bool placeBet(unsigned long long int betAmount);

        void agentGamble();

        void agentGambleScenario();
        Agent(unsigned long long int startM, int betting, double winR, string stoppingTime) {
            startMoney = startM;
            ZorNothing = betting;
            winRate = winR;
            prevBetAmount = 1;
            currBetAmount = 1;
            whenToStop = stoppingTime;
            currMoney = startM;
            hasAchievedGoal = true;
        }
        void resetVals(){
            currMoney = startMoney;
            maxMoney[0] = 0;
            maxMoney[1] = 0;
            prevBetAmount = 1;
            currBetAmount = 1;
            hasAchievedGoal = true;
            betsPlaced = 0;
        }
};


bool Agent::simulateCoinFlip() {
    if (rand() % 100 < winRate) {
        return true;
    } else {
        return false;
    }
}

bool Agent::placeBet(unsigned long long int betAmount) {
    ++betsPlaced;
    unsigned long long int winLossAmount = betAmount;
    // it will either return +moneyPlaced or -moneyPlaced
    if (!simulateCoinFlip()){
        // This means that they lost the bet
        currMoney -= winLossAmount;
        return false;
    } else {
        currMoney += winLossAmount;
        return true;
    }
}


void Agent::agentGamble() {

    if (currBetAmount > currMoney) {
        // For fibonacci                
        if (ZorNothing == 0) {
            unsigned long long int tmp = currBetAmount;
            // This is the formula for 2 places back on the fibonacci sequence
            currBetAmount = currBetAmount - prevBetAmount;
            // This is the formula for 3 places back on the fibonacci sequence
            prevBetAmount = 2 * prevBetAmount - tmp;
        } else {
            // if doing Z or nothing and bet is
            // greater than currMoney, reset betAmount
            currBetAmount = 1;
        }

    }

    bool didWin;
    // This means that the agent will gamble all their money every time
    if (ZorNothing == startMoney) {
        didWin = placeBet(currMoney);
    } else {
        didWin = placeBet(currBetAmount);
    }


    if (currMoney > maxMoney[1]) {
        maxMoney[0] = betsPlaced;
        maxMoney[1] = currMoney;
    }

    if (didWin == false) {
        // If the agent lost the bet, do something depending on zor
        if (ZorNothing == 0) {
            // This means fibonacci
            long int tmp = prevBetAmount;
            prevBetAmount = currBetAmount;
            currBetAmount += tmp;
        } else {
            currBetAmount *= ZorNothing;
        }

    } else {
        currBetAmount = 1;
    }
}


void Agent::agentGambleScenario() {
    // When the agent loses all their money
    if (whenToStop == "bankrupt") {
        // Keep betting until the agent has no money left
        while (currMoney > 0) {
            // This means that the updates of the gamble
            // will be set in the agent (change in money etc.)
            agentGamble();

        }

    }
    // When the agent doubles their money
    else if (whenToStop == "doubled") {
        // Either the agent stops when their current money is double what they started with 
        // or they run out of money
        while (currMoney < 2 * startMoney && currMoney != 0) {
            agentGamble();

        }
        if (currMoney == 0) {
            // std::cout << "failed\n";
            hasAchievedGoal = false;
        } else {
            // std::cout << "success\n";
            hasAchievedGoal = true;
        }

    }
    else {
        // std::cout << "pick a valid whenToStop\n";
    }
}


// This will calculate the probability of doubling your money for each x value.
// Each x values represents how many times you can lose in a row while still having money
// E.g if x = 5 that means you have 2^5 dollars.
void calculateProbabilityOfDoublingForEachX(int startX, int endX, int ZorNothing, int repeats, double probOfWin) {
    std::ofstream MyFile("dataset_test/probability_of_doubling_with" + to_string(ZorNothing) + "_from_"
    + to_string(startX) + "_to_" + to_string(endX) + "_with_" + to_string(repeats) + "_repeats_"
    + to_string(probOfWin) + "_probOfWin");
    MyFile << "x_val,probability\n";
    for(;startX <= endX; startX++) {
        long int totalWins = 0;
        for (int i=0; i < repeats; i++) {
            unsigned long long int money = pow(2, startX);
            Agent myAgent(money,ZorNothing,50.0, "doubled");
            myAgent.agentGambleScenario();
            if (myAgent.hasAchievedGoal) {
                totalWins++;
            }
            myAgent.resetVals();
        }
        double percentage_won = double(totalWins)/double(repeats);
        // std::cout << totalWins << "," << repeats <<"\n";
        MyFile << startX << "," << percentage_won << "\n";
    }
}

void calcDiffProbOfWinProbOfDoublForEach(int startX, int endX, int ZorNothing,int repeats, double startProbWin, double endProbWin) {
    for (; startProbWin <= endProbWin; startProbWin += 10.0) {
        calculateProbabilityOfDoublingForEachX(startX, endX, ZorNothing,repeats, startProbWin);
    }
}


int main (void) {
    // Create a random seed
    srand( (unsigned)time(NULL) );

    // calculateProbabilityOfDoublingForEachX(1, 15, 2, 100000);
    calcDiffProbOfWinProbOfDoublForEach(1,15,2,800000,30,70);
    return 0;
}


// Variable input
    // 1. startM. start money
    // 2. betting. ZorNothing (if 0, it will be fibonacci)
    // 3. winR. the win rate
    // 4. stoppingTime. When the agent should stop betting. 
    // It can either be when the money is doubled or until bankrupt


// This is an OOP approach. All the data is always inside the Agent.