import random

import numpy as np
import matplotlib.pyplot as plt
import math

class Agent:
    def __init__(self, startMoneyPercentage, XorNothing, winRate):
        # since it doesn't matter how much start money is (it only matters
        # its relative value compared to bet) startMoneyPercentage is used.
        # a single bet will be 1, and so if the startMoneyPercentage is 1000
        # then 1000 single bets can be made
        self.startMoney = startMoneyPercentage
        # this is the multiplying factor if you lose. E.g if XorNothing was 2
        # that would mean every time you lose, you bet twice as much
        self.XorNothing = XorNothing
        # a decimal between 0 and 100 to determine how often you win
        self.winRate = winRate
        self.currMoney = startMoneyPercentage

def simulateCoinFlip(agent):
    #E.g if winRate = 70 then 70% of the time it will return True
    if random.randrange(100) < agent.winRate:
        return True
    else :
        return False

def placeBet(moneyPlaced, agent):
    # it will either return +moneyPlaced or -moneyPlaced
    if simulateCoinFlip(agent):
        return moneyPlaced
    else :
        return -moneyPlaced

# use this if you want to find the total bets placed 
# and max current money
def untilBankrupt(agent):
    # useful stats to know
    maxCurrMoney = agent.currMoney
    betsPlaced = 0
    
    # since currMoney is relative to 1
    betAmount = 1
    while (agent.currMoney > 0):
        betsPlaced += 1

        if (betAmount > agent.currMoney):
            # if doing double or nothing and bet is
            # greater than currMoney
            betAmount = 1

        currentBet = placeBet(betAmount, agent)
        agent.currMoney += currentBet
        if (agent.currMoney > maxCurrMoney):
            maxCurrMoney = agent.currMoney
        if (currentBet < 0):
            betAmount *= agent.XorNothing
        else:
            betAmount = 1
    return betsPlaced, maxCurrMoney

# use this if you want an array with all the data points of 
# each bet (good for matplotlib plotting)
def untilBankruptArray(agent):
    allData = []
    # useful stats to know
    betsPlaced = 0
    
    # since currMoney is relative to 1
    betAmount = 1
    while (agent.currMoney > 0):
        allData.append((betsPlaced, agent.currMoney))
        betsPlaced += 1
        
        if (betAmount > agent.currMoney):
            # if doing double or nothing and bet is
            # greater than currMoney
            betAmount = 1

        currentBet = placeBet(betAmount, agent)
#         print(betsPlaced, currentBet)
        agent.currMoney += currentBet
        if (currentBet < 0):
            betAmount *= agent.XorNothing
        else:
            betAmount = 1
    allData.append((betsPlaced, agent.currMoney))
    return allData


# simulate an agent with $100 starting, with Z being 2 (double or nothing),
#  and the win percentage is 50%
agent = Agent(100, 2, 50)
data = untilBankrupt(agent)

print(data)