# Tarot

In this repository you will find a tarot game, developed to learn machine learning with a game that I like and know well.

## How to play tarot?
Tarot is a card game where the goal is to match a bid, that you choose depending on how confident you are on your hand. You'll play a few mini-rounds (**tricks**) each round. To earn points, you have to win the cards of your opponents by playing cards higher than them. 

### Number of players
Tarot can be played from 3 to 5 players. The player who takes the bid normally plays alone against everyone, except when there are 5 players : the player who takes the bid will call a King, and the player who has this king will play in team with the taker without anyone knowing, not even the taker!
I took the liberty of adding the possibility of playing with 2 players, as it was played in my familly.

### Cards
Like in some other games, **Kings** are higher than **Queens**, which are higher than **Jacks**, which are higher than 10s, etc. But a tarot deck has 78 cards : it adds **Knights** which are between Queens and Jacks, and **Trumps**. There are 22 trumps, they're all higher than basic cards and the trump 21 is the highest of cards. You can play a trump only if you don't have any cards of the played suit (except for the Excuse that you can play almost all the time).

There are 3 trumps that are special, as they lower the bid :
- Trump 21 : the highest of cards
- Trump 1 : the lowest of trumps, can be hard to play without losing it
- Excuse : like a Joker, play it if you don't want to play anything else, but you'll be sure to lose the trick

## Machine learning
The AI tries a random move out of every possible move given its hand, and saves its outcome in a file. Now that the file is not empty, the AI will try to find a similar move given a similar hand, and play it if the outcome is good.