#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

//******************************************************************************************
// CARD SECTION
//******************************************************************************************

class Card
{
public:
	Card(char suit, int value);
	char getSuit();
	void setSuit(char s);
	int getValue();
	string getValueDisplay();
	void setValue(int v);
	void display();

private:
	char suit;
	int value;
};

Card::Card(char s = 'S', int v = 1)
{
	suit = s;
	value = v;
}

char Card::getSuit()
{
	return suit;
}

void Card::setSuit(char s)
{
	if (s == 'H' || s == 'C' || s == 'D' || s == 'S')
	{
		suit = s;
	}

}

int Card::getValue()
{
	return value;
}

//Valid Card Values are 1-13 | 1 is an Ace | 11,12,13 are Jack, Queen, King
string Card::getValueDisplay()
{
	string valueDisplay;
	if (value < 11 && value >1)
		valueDisplay = to_string(value);
	else if (value == 1)
		valueDisplay = "A";
	else if (value == 11)
		valueDisplay = "J";
	else if (value == 12)
		valueDisplay = "Q";
	else if (value == 13)
		valueDisplay = "K";
	else
		valueDisplay = "A";
	return valueDisplay;
}

void Card::setValue(int v)
{
	if (v > 0 && v <= 13)
	{
		value = v;
	}
}

void Card::display()
{
	cout << getValueDisplay() << suit << endl;
}


TEST_CASE("Card tests")
{
	Card c1('C', 9);
	Card c2('D', 12);

	REQUIRE(c1.getSuit()=='C');
	REQUIRE(c1.getValue() == 9);
	REQUIRE(c1.getValueDisplay() == "9");

	REQUIRE(c2.getSuit() == 'D');
	REQUIRE(c2.getValue() == 12);
	REQUIRE(c2.getValueDisplay() == "Q");

	SECTION("Modifying a Card")
	{
		c1.setSuit('H');
		REQUIRE(c1.getSuit() == 'H');
		c1.setValue(5);
		REQUIRE(c1.getValue() == 5);
	}

	SECTION("Modifying Another Card")
	{
		c2.setSuit('S');
		REQUIRE(c2.getSuit() == 'S');
		c2.setValue(0);
		REQUIRE(c2.getValue() == 12);
	}

	Card c3;
	REQUIRE(c3.getSuit() == 'S');
	REQUIRE(c3.getValue() == 1);
	REQUIRE(c3.getValueDisplay() == "A");
}

//******************************************************************************************
// Deck SECTION
//******************************************************************************************

class Deck
{
public:
	Deck();
	Card getCard(int index);
	int getSize();
	void shuffle();
	void display();
	vector<Card> dealCards(int numCards);

private:
	vector<Card> deck;
	Card switcher;
};

//Valid Card Values are 1-13 | 1 is an Ace | 11,12,13 are Jack, Queen, King
Deck::Deck()
	:switcher(3, 1)
{
	for (int i = 1; i <= 13; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (j == 0)
			{
				Card c('C', i);
				deck.push_back(c);
			}
			else if (j == 1)
			{
				Card c('D', i);
				deck.push_back(c);
			}
			else if (j == 2)
			{
				Card c('H', i);
				deck.push_back(c);
			}
			else {
				Card c('S', i);
				deck.push_back(c);
			}
		}
	}
}

Card Deck::getCard(int index)
{
	return deck[index];
}

int Deck::getSize()
{
	return deck.size();
}

void Deck::shuffle()
{
	srand(time(0));
	for (int i = 0; i < deck.size(); i++)
	{
		int random = rand() % deck.size();
		switcher = deck[i];
		deck[i] = deck[random];
		deck[random] = switcher;
	}
}

void Deck::display()
{
	for (int i = 0; i < deck.size(); i++)
	{
		deck[i].display();
	}
}

vector<Card> Deck::dealCards(int numCards)
{
	vector<Card> cardsDealt;
	for (int i = 0; i < numCards; i++)
	{
		cardsDealt.push_back(deck[deck.size() - 1 ]);
		deck.pop_back();
	}
	return cardsDealt;
}

TEST_CASE("Deck tests")
{
	Deck d1;
	REQUIRE(d1.getSize() == 52);
	d1.shuffle();
	REQUIRE(d1.getSize() == 52);

	int numSpades = 0;
	int numDiamonds = 0;
	int numClubs = 0;
	int numHearts = 0;

	for (int i = 0; i < d1.getSize(); i++)
	{
		if (d1.getCard(i).getSuit() == 'S')
		{
			numSpades++;
		}
		else if (d1.getCard(i).getSuit() == 'D')
		{
			numDiamonds++;
		}
		else if (d1.getCard(i).getSuit() == 'C')
		{
			numClubs++;
		}
		else if (d1.getCard(i).getSuit() == 'H')
		{
			numHearts++;
		}
	}

	REQUIRE(numSpades == 13);
	REQUIRE(numDiamonds == 13);
	REQUIRE(numClubs == 13);
	REQUIRE(numHearts == 13);

	vector<Card> hand = d1.dealCards(5);
	REQUIRE(hand.size() == 5);
	REQUIRE(d1.getSize() == 47);

	Card c1 = d1.getCard(d1.getSize() - 1);
	Card c2 = d1.getCard(d1.getSize() - 2);
	Card c3 = d1.getCard(d1.getSize() - 3);
	vector<Card> hand2 = d1.dealCards(3);

	REQUIRE(hand2[0].getSuit() == c1.getSuit());
	REQUIRE(hand2[0].getValue() == c1.getValue());
	REQUIRE(hand2[1].getSuit() == c2.getSuit());
	REQUIRE(hand2[1].getValue() == c2.getValue());
	REQUIRE(hand2[2].getSuit() == c3.getSuit());
	REQUIRE(hand2[2].getValue() == c3.getValue());
}

//******************************************************************************************
// PLAYER SECTION
//******************************************************************************************
class Player
{
public:
	Player();
	void startGame();
	void sortHand();
	void setHand(const vector<Card> &newHand);
	Card highCard();
	bool isPair();
	bool is2Pair();
	bool is3K();
	bool is4K();
	bool isFlush();
	bool isStraight();
	bool isFullHouse();
	bool isRoyalFlush();

private:
	Deck deck;
	vector<Card>hand;
	vector<Card>exCards;
};

Player::Player()
{
	vector<Card> hand;
}

void Player::startGame()
{
	deck.shuffle();
	hand = deck.dealCards(5);
}

void Player::sortHand()
{
	for (int j = 0; j < hand.size(); j++)
	{
		for (int i = 0; i < hand.size() - 1; i++)
		{
			if (hand[i].getValue() > hand[i + 1].getValue())
			{
				swap(hand[i], hand[i + 1]);
			}
		}
	}
}

void Player::setHand(const vector<Card> &newHand)
{
	int i = 0;
	for (const Card &tempCard : newHand)
	{
		hand[i] = tempCard;
		i++;
	}
}

Card Player::highCard()
{
	return hand[hand.size() - 1];
}
bool Player::isPair()
{
	bool tf = false;
	for (int i = 0; i < hand.size() - 1; i++)
	{
		for (int j = i + 1; j < hand.size(); j++)
		{
			if (hand[i].getValue() == hand[j].getValue())
			{
				tf = true;
				break;
			}
		}
	}
	return tf;
}

bool Player::is2Pair()
{
	bool tf = false;
	int count = 0;
	for (int i = 0; i < hand.size() - 1; i++)
	{
		for (int j = i + 1; j < hand.size(); j++)
		{
			if (hand[i].getValue() == hand[j].getValue())
			{
				count++;
				if (count == 2)
				{
					tf = true;
					break;
				}
			}
		}
	}
	return tf;
}

bool Player::is3K()
{
	bool tf = false;
	if (hand[0].getValue() == hand[1].getValue() && hand[0].getValue() == hand[2].getValue())
		tf = true;
	else if (hand[1].getValue() == hand[2].getValue() && hand[1].getValue() == hand[3].getValue())
		tf = true;
	else if (hand[2].getValue() == hand[3].getValue() && hand[2].getValue() == hand[4].getValue())
		tf = true;

	return tf;
}

bool Player::is4K()
{
	bool tf = false;
	int b1, b2;
	b1 = hand[0].getValue();
	b2 = hand[1].getValue();
	if (b1 == b2 && b1 == hand[2].getValue() && b1 == hand[3].getValue())
		tf = true;
	else if (b2 == hand[2].getValue() && b2 == hand[3].getValue() && b2 == hand[4].getValue())
		tf = true;

	return tf;
}

bool Player::isFlush()
{
	int count = 0;
	bool tf = false;
	for (int i = 1; i < hand.size(); i++)
	{
		if (hand[0].getSuit() == hand[i].getSuit())
			count++;
		else
			break;
	}
	if (count == 4)
		tf = true;

	return tf;
}

bool Player::isStraight()
{
	bool tf = false;
	int count = 0;
	if (hand[0].getValue() == 1 && hand[1].getValue() == 2)
	{
		for (int i = 2; i < hand.size(); i++)
		{
			if (hand[i].getValue() == hand[i - 1].getValue() + 1)
			{
				count++;
			}
			else
				break;
		}
		if (count == 3)
			tf = true;
	}
	else if (hand[0].getValue() == 1 && hand[4].getValue() == 13)
	{
		if (hand[1].getValue() == 10 && hand[2].getValue() == 11 && hand[3].getValue() == 12)
			tf = true;
	}
	else
	{
		for (int i = 1; i < hand.size(); i++)
		{
			if (hand[i].getValue() == hand[i - 1].getValue() + 1)
				count++;
			else
				break;
		}
		if (count == 4)
			tf = true;
	}
	return tf;
}

bool Player::isFullHouse()
{
	bool tf = false;
	if (hand[0].getValue() == hand[1].getValue() && hand[2].getValue() == hand[3].getValue() && hand[3].getValue() == hand[4].getValue())
		tf = true;
	else if (hand[0].getValue() == hand[1].getValue() && hand[0].getValue() == hand[2].getValue() && hand[3].getValue() == hand[4].getValue())
		tf = true;

	return tf;
}

bool Player::isRoyalFlush()
{
	bool tf = false;
	if (isFlush() && isStraight() && hand[0].getValue() == 1 && hand[4].getValue() == 13)
		tf = true;
	return tf;
}

TEST_CASE("Player Tests")
{
	Player testPlayer;
	testPlayer.startGame();

	vector<Card> testHand{ {'H',3}, {'S',13}, {'H',4}, {'C',3}, {'D',4} };
	testPlayer.setHand( testHand );

	SECTION("PAIR TESTS")
	{
		
	}

}