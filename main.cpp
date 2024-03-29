//CPSC-25
//Jacob Butler
//Professor Kanemoto
//
// We ran into a string-handling issue when we ran our code for the first time, which threw a wrench in our program. As we continued to run the debugger, we found that the program was accessing a deck value more than 52 cards and would run into an overflow error. In order to fix this error, we implemented the function resetDeck that shuffles the deck once the program tries to reach above 52 cards in the deck. Once we got the program running again, we ran into a different issue. When we were implementing our function to track our 3-of-a-kind and 4-of-a-kind, we weren't getting the results we were expecting. When we looked at the lines, we discovered that we were comparing against incorrect count values. Despite fixing that issue, we ran into yet another problem. When we would run the program, our hand wouldn’t change with each new iteration of our main for-loop. Upon further inspection, we discovered that the initial vector playerHand was being pushed onto, making it an enormous size, fitting every hand we generated through out program.  The tempPlayerCards vector was also only copying the first 5 values in our playerHand vector, rather than the newly drawn cards. The scoring for-loop was also then only looking at the first 5 values of the playerHand vector. In order to conserve on memory and fix the issue, we cleared both the PlayerCards and tempPlayerCards at the end of each iteration of our main for-loop.
#include <iostream>
#include <map>
#include <vector>

using namespace std;



class Card { // Keeps a card object
public:
  Card();
  Card(string suit, string rank, int value);
  void set_rank(string rank);
  void set_suit(string suit);
  void set_value(int value);
  string get_rank();
  string get_suit();
  int get_value();
  void print_card();
  Card copy();

private:
  string suit;
  string rank;
  int value;
};

Card::Card() {
  suit = "club";
  rank = "A";
  value = 11;
}

Card::Card(string suit, string rank, int value) {
  this->suit = suit;
  this->rank = rank;
  this->value = value;
}

void Card::set_rank(string rank) { this->rank = rank; }
void Card::set_suit(string suit) { this->suit = suit; }
void Card::set_value(int value) { this->value = value; }
string Card::get_rank() { return rank; }
string Card::get_suit() { return suit; }
int Card::get_value() { return value; }
void Card::print_card() {
  cout << rank << " of " << suit << " with value " << value << endl;
}
Card Card::copy() {
  Card newCard = Card(suit, rank, value);
  return newCard;
}
const string SUITS[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
const string RANKS[] = {"2", "3",  "4",    "5",     "6",    "7",  "8",
                        "9", "10", "Jack", "Queen", "King", "Ace"};
const int VALUES[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

// int DECK[52];
Card deck[52];
int currentCardIndex = 0;
/*
defines the arrays and variables related to a deck of playing cards through the
suits, ranks, the current card index, and the deck itself.
*/

void initializeDeck() {
  int deckIndex = 0;
  for (int suitIndex = 0; suitIndex < 4; suitIndex++) {
    for (int rankIndex = 0; rankIndex < 13; rankIndex++) {
      // Card(string suit, string rank, int value);
      deck[deckIndex++] =
          Card(SUITS[suitIndex], RANKS[rankIndex], VALUES[rankIndex]);
    }
  }
}

void printDeck() {
  for (int i = 0; i < 52; i++)
    deck[i].print_card();
}

void shuffleDeck() {
  for (int i = 0; i < 52; i++) {
    int index = rand() % 52;
    Card temp = deck[i]; // these next three lines are a swap function
    deck[i] = deck[index];
    deck[index] = temp;
  }
}

Card dealCard() { return deck[currentCardIndex++]; }

int dealInitialPlayerCards() {
  Card card1 = dealCard();
  Card card2 = dealCard();
  cout << "Your cards: ";
  card1.print_card();
  card2.print_card();
  // cout << "Your cards: " << RANKS[card1 % 13] << " of " << SUITS[card1 / 13]
  //<< " and " << RANKS[card2 % 13] << " of " << SUITS[card2 / 13] << endl;
  return card1.get_value() + card2.get_value();
  // return cardValue(card1) + cardValue(card2);
}

int playerTurn(int playerTotal) {
  while (true) {
    cout << "Your total is " << playerTotal << ". Do you want to hit or stand?"
         << endl;
    string action;
    getline(cin, action);
    if (action == "hit") {
      Card newCard = dealCard();
      // playerTotal += cardValue(newCard);
      playerTotal += newCard.get_value();
      cout << "You drew a ";
      newCard.print_card();
      // cout << "You drew a " << RANKS[newCard % 13] << " of "
      //<< SUITS[newCard / 13] << endl;
      if (playerTotal > 21) {
        break;
      }
    } else if (action == "stand") {
      break;
    } else {
      cout << "Invalid action. Please type 'hit' or 'stand'." << endl;
    }
  }
  return playerTotal;
}

void resetDeck(){
  shuffleDeck();
  currentCardIndex = 0;
}


int main() {
  // srand((time(0)));
  srand((time(0)));

  initializeDeck();
  // printDeck();
  shuffleDeck();
  // printDeck();

  // deal user 5 cards
  vector<Card> playerCards;
  vector<Card> tempPlayerCards;
  int NUM_CARDS = 5;
  int pairCount = 0;
  int threeCount = 0;
  int fourCount = 0;

  initializeDeck();
  // printDeck();
  shuffleDeck();

  //run this part 1000 times
  for (int d = 0; d < 1000; d++) {
    cout << "Round " << d << endl;
    
    // deal player 5 cards
    for (int i = 0; i < NUM_CARDS; i++) {
      playerCards.push_back(dealCard());
    }
  
    // make temp player hand so we can change the cards later
    for (int i = 0; i < NUM_CARDS; i++) {
      tempPlayerCards.push_back(playerCards.at(i).copy());
    }
    
    // count the hand score
    for (int i = 0; i < NUM_CARDS; i++) {
      int count = 0;
      for (int j = i + 1; j < NUM_CARDS; j++) {
        if (tempPlayerCards[i].get_value() == tempPlayerCards[j].get_value()) {
          if(tempPlayerCards[i].get_value() != 0) {
            count++;
            tempPlayerCards[j].set_value(0);
          }
        }//end if
      }//inner for loop
      if(count == 1)
        pairCount++;
      else if(count == 2)
        threeCount++;
      else if(count == 3)
        fourCount++;
    }//outer for loop
  
    //check our results
    for (int i = 0; i < NUM_CARDS; i++) {
      playerCards.at(i).print_card();
    }

    // reset the deck 
    if (currentCardIndex >= 52) {
      resetDeck();
    }

    // reset the player/temp hand
    playerCards.clear();
    tempPlayerCards.clear();
  }

  cout << "num of pairs " << pairCount << endl;
  cout << "the percent of pairs " << (pairCount/1000.0) * 100 << endl;
  cout << "num of triples " << threeCount << endl;
  cout << "the percent of threes of a kind " << (static_cast<double>(threeCount)/1000.0) * 100 << endl;
  cout << "num of fours of a kind " << fourCount << endl;
  cout << "the percent of fours of a kind " << (static_cast<double>(fourCount)/1000.0) * 100 << endl;
}//end main

/*
int playerTotal = dealInitialPlayerCards();
cout << "The playerTotal is " << playerTotal << endl;
//int dealerTotal = dealInitialDealerCards();

playerTotal = playerTurn(playerTotal);
if (playerTotal > 21) {
  cout << "You busted! Dealer wins." << endl;
  return 0;
}
*/
