/*
Author: AngelaJiang
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "a4.h"

#define PRINT_DEBUG 1

// Implement the Hand and other functions in here
// create a hand type
Hand* createHand() {
  Hand* newHand = (Hand *)malloc(sizeof(Hand));
  memset(newHand, '\0', sizeof(Hand));
  newHand -> num_cards_in_hand = 0;
  newHand -> firstCard = NULL;
  return newHand;
}
// add card to hand
void addCardToHand(Card *card, Hand *hand) {
  CardNode *newCard = (CardNode*)malloc(sizeof(CardNode));
  newCard -> thisCard = card;
  // if there is no cards in hand
  if (hand -> num_cards_in_hand == 0) {
    newCard -> nextCard = NULL;
    newCard -> prevCard = NULL;
    hand -> firstCard = newCard;
    hand -> num_cards_in_hand += 1;
  } else {
    newCard -> nextCard = hand -> firstCard;
    newCard -> prevCard = NULL;
    hand -> firstCard -> prevCard = newCard;
    hand -> firstCard = newCard;
    hand -> num_cards_in_hand += 1;
  }
}
// justify whether the hand is empty
int isHandEmpty(Hand* hand) {
  if (hand -> num_cards_in_hand <= -1) {
    return 1;
  } else {
    return 0;
  }
}
// remove card from hand
Card* removeCardFromHand(Card *card, Hand *hand) {
  if (hand->firstCard == NULL) {
    return NULL;
  }
  if (hand ->num_cards_in_hand == 1) {
    free(hand->firstCard);
    hand->firstCard = NULL;
    hand->num_cards_in_hand = 0;
    return card;
  }
  CardNode *temp = hand->firstCard;
  while (temp != NULL) {
    if (temp->thisCard == card) {
      if (temp->prevCard == NULL) {
        temp->nextCard->prevCard = NULL;
        hand->firstCard = temp->nextCard;
      } else if (temp->nextCard == NULL) {
        temp->prevCard->nextCard = NULL;
      } else {
        temp->prevCard->nextCard = temp->nextCard;
        temp->nextCard->prevCard = temp->prevCard;
      }
      hand->num_cards_in_hand -= 1;
      free(temp);
      return card;
    }
    temp = temp->nextCard;
  }
  free(temp);
  return card;
}
// destroy the hand
void destroyHand(Hand* hand) {
  while (hand->firstCard != NULL) {
    CardNode * curNode = hand->firstCard;
    hand->firstCard = hand->firstCard->nextCard;
    free(curNode->thisCard);
    free(curNode);
  }
  free(hand);
}
// justify whether the move is legal
int isLegalMove(Hand *hand, Card *leadCard, Card *playedCard) {
  int havePlayedCard = 0;  // to decide if there is playedCard in hand
  int haveLeadCardSuit = 0;  // to decide if there is suit of lead card
  CardNode * curCard = hand->firstCard;
  while (curCard != NULL) {
    if (curCard->thisCard -> suit == playedCard -> suit
        && curCard -> thisCard -> name == playedCard -> name)
      {  havePlayedCard = 1;
        if (playedCard -> suit == leadCard->suit) {
          return 1;
          // if there is playedCard in hand and it
  // has same suit with lead card, it is legal
        }
      }
    if (curCard->thisCard -> suit == leadCard->suit) {
      haveLeadCardSuit = 1;
    }
    curCard = curCard->nextCard;
  }

  if (havePlayedCard == 1 && (haveLeadCardSuit == 0
      || leadCard->suit == playedCard-> suit)) {
    return 1;
  } else {
    return 0;
    }
}
// decide who won
int whoWon(Card *leadCard, Card *followedCard, Suit trump) {
  if (leadCard->suit == followedCard->suit) {
    if (leadCard->name >  followedCard->name) {
      return 1;
    } else {
      return 0;
    }
  } else if (followedCard->suit == trump) {
    return 0;
  } else {
    return 1;
  }
}
// shuffle the deck
void shuffle(Deck* aDeck) {
  srand((unsigned)time(0));
  int ran_num;
  Card *temp;
  for (int i = 0; i <= aDeck->topCard; i++) {
    ran_num = rand() % (aDeck->topCard + 1);
    temp = aDeck->cards[i];
    aDeck->cards[i] = aDeck->cards[ran_num];
    aDeck->cards[ran_num] = temp;
}
}
// make a deal
void deal(Deck *aDeck, Hand *p1hand, Hand *p2hand) {
  for (int i = 0; i < NUM_CARDS_IN_HAND; i++) {
    addCardToHand(popCardFromDeck(aDeck), p1hand);
    addCardToHand(popCardFromDeck(aDeck), p2hand);
  }
}

void shuffleHand(Hand* hand) {
  int NUMCARDS = hand->num_cards_in_hand;
  int map[NUMCARDS];
  for (int i = 0; i < NUMCARDS; i++) {
    map[i] = i;
  }
  srand((unsigned)time(0));
  for (int j = 0; j < NUMCARDS; j++) {
    int ran_num;
    int temp;
    ran_num = rand() % (NUMCARDS);
    temp = map[j];
    map[j] = map[ran_num];
    map[ran_num] = temp;
  }
  Card * handArray[NUMCARDS];
  for (int iter = 0; iter < NUMCARDS; iter++) {
  if (map[iter] != 0) {
          CardNode * curNode = hand->firstCard;
          for (int i = 1; i < map[iter]; i++) {
            curNode = curNode->nextCard;
          }
          handArray[iter] = curNode->thisCard;
  } else {
    handArray[iter] = hand->firstCard->thisCard;
  }
  }
  CardNode * res = hand->firstCard;
  for (int j = 0; j < NUMCARDS; j++) {
        res->thisCard->suit = handArray[j]->suit;
        res->thisCard->name = handArray[j]->name;
        res = res->nextCard;
  }
}


