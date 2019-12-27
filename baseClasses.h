/* baseClasses.h */

// I love camelCase // :|
/* Declarations */ // also works as an index :)
class Game;
class Player;
class Card;
class GreenCard;
class BlackCard;

class Personality;
class Holding;
class Mine;
class GoldMine;
class CrystalMine;
class StrongHold;

class Follower;
class Item;


#include <cstddef> // size_t
#include <queue>
#include <vector>

using std::vector;
using std::queue;

//==========================================|| C A R D ||==========================================

class Card
{
  const std::string name;
    
  const size_t cost;
    
  bool isTapped;

public:

  Card(/* args */);
  ~Card();
};

// Kalutera na mhn valoume ta panta ola const gt mporei na exoume 8emata meta 

//==========================================|| G R E E N  C A R D ||==========================================

class GreenCard : public Card
{
  const size_t attackBonus;              /* I think bonuses are e.g  0,2 x attack . That's why they're floats */
  const size_t defenceBonus;
  const size_t minHonor;

  const std::string cardText;
  
  const size_t effectBonus;
  const size_t effectCost;

public:
    
  GreenCard(/* args */);
  ~GreenCard();
};

//==========================================|| B L A C K  C A R D ||==========================================

class BlackCard : public Card
{
  bool isRevealed;

public:
    
  BlackCard(/* args */); 
  ~BlackCard();
};

//==========================================|| F O L L O W E R ||==========================================

class Follower : public GreenCard
{
public:
    
  Follower(/* args */);
  ~Follower();
};

//==========================================|| I T E M ||==========================================

class Item : public GreenCard
{
  const size_t durability;

public:
    
  Item(/* args */);
  ~Item();
};

//==========================================|| P E R S O N A L I T Y ||==========================================

class Personality : public BlackCard
{
  const size_t attack;
  const size_t defence;
  const size_t honor;

  bool isDead;
  
  vector <Follower *> * followers;
  vector <Item *>     * items;

public:
    
  Personality(/* args */);
  ~Personality();
};

//==========================================|| H O L D I N G ||==========================================

class Holding : public BlackCard
{
  const size_t harvestValue;     

public:
    
  Holding(/* args */);
  ~Holding();
};

//==========================================|| M I N E ||==========================================

class Mine : public Holding
{
  Holding * upperHolding;

public:
    
  Mine(/* args */);
  ~Mine();
};

//==========================================|| C R Y S T A L  M I N E ||==========================================
// ligo malakia pou exw mia oloidia class me allo onoma enos member alla nta3 ugeia
class CrystalMine : public Holding
{
  Holding * subHolding;

public:
    
  CrystalMine(/* args */);
  ~CrystalMine();
};

//==========================================|| G O L D  M I N E ||==========================================

class GoldMine : public Holding
{
  Holding * upperHolding;
  Holding * subHolding;

public:
    
  GoldMine(/* args */);  
  ~GoldMine();
};

//==========================================|| S T R O N G H O L D ||==========================================

class StrongHold : public Holding
{
  const size_t initHonor;
  const size_t initDefence;
/*  const size_t initMoney; == harvest apo base class */
public:
    
  StrongHold(/* args */);
  ~StrongHold();
};

//==========================================|| T Y P E D E F S ||==========================================

typedef Item Katana;
typedef Item Spear;
typedef Item Bow;
typedef Item Ninjato;
typedef Item Wakizashi;

typedef Follower Footsoldier;
typedef Follower Archer;
typedef Follower Cavalry;
typedef Follower Bushido;
typedef Follower Sieger;
typedef Follower Atakebune;

typedef Personality Attacker;
typedef Personality Defender;
typedef Personality Champion;
typedef Personality Chancellor;
typedef Personality Shogun;

typedef Holding Plain;
typedef Holding Farmland;
typedef Holding Gift_n_Favour;

//==========================================|| P L A Y E R ||==========================================

class Player
{
  const std::string userName;

  const size_t honor;

  StrongHold *strongHold;

  queue <GreenCard *> * fateDeck;
  queue <BlackCard *> * dynastyDeck;

  vector <GreenCard *>   * hand;
  vector <Holding *>     * holdings;
  vector <Personality *> * army;
  vector <BlackCard *>   * provinces;

public:

  Player(/* args */);
  ~Player();

  const std::string& getUserName() const { return userName; }
  vector <BlackCard *> * getProvinces() { return provinces; }
};

//==========================================|| G A M E ||==========================================

class Game
{
  vector <Player *> * players;

  size_t checkWinningCondition(void);

  void startingPhase (Player *);
  void equipmentPhase(Player *);
  void battlePhase   (Player *);
  void economyPhase  (Player *);
  void finalPhase    (Player *);

public:

  Game(size_t numPlayers, size_t maxGreenCards, size_t maxBlackCards, size_t maxHand /*might need more, you're up*/);
  ~Game();

  void initGameBoard(/* args */);
  void printGameStats(void);
  void gameplay(void);
};

//==========================================|| E N D  O F  F I L E ||==========================================
