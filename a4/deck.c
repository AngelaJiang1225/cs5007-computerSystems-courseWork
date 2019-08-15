#include <stdio.h>
#include <stdlib.h>

#include "a4.h"

#define PRINT_DEBUG 1

// Put your deck functions in here
Deck* createDeck() {
  // create deck
  Deck* newDeck = (Deck*)malloc(sizeof(Deck));
  newDeck->topCard = -1;
  return newDeck;
}
Card* createCard(Suit s, Name name) {
  // create card
  Card* newCard = (Card*)malloc(sizeof(Card));
  newCard->name = name;
  newCard->suit = s;
  return newCard;
}
void destroyCard(Card* card) {
  free(card);
}
Deck* populateDeck() {
  // populate deck of 24 cards
  Deck *newDeck = createDeck();
  for (int s = HEARTS; s <= DIAMONDS; s++) {
  for (int n = NINE; n <= ACE; n++) {
      pushCardToDeck(createCard(s, n), newDeck);
}
  }
}
Deck* pushCardToDeck(Card* newCard, Deck* deck) {
  // push cards into deck
  if (deck->topCard >= NUM_CARDS_IN_DECK-1) {
    return deck;
  } else {
    deck->topCard++;
    deck->cards[deck->topCard] = newCard;
    return deck;
  }
}

Card* peekAtTopCard(Deck* deck) {
  // peek and return the top card
  if (isDeckEmpty(deck)) {
    return NULL;
  }
  return deck->cards[deck->topCard];
  }
Card* popCardFromDeck(Deck* deck) {
  // pop card from deck
  if (deck->topCard == -1) {
    return NULL;
  }
  Card *card = deck->cards[deck->topCard];
  deck->cards[deck->topCard] = NULL;
  deck->topCard--;
  return card;
}
int isDeckEmpty(Deck* deck) {
  // if deck is empty return 1 else 0
  if (deck->topCard <= -1) {
    return 1;
  } else {
    return 0;
  }
}
void destroyDeck(Deck* deck) {
  // destroy deck
  for (int i = 0; i <= deck->topCard; i++) {
    destroyCard(deck->cards[i]);
  }
  free(deck);
}
