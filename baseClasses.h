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
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>


using std::vector;
using std::queue;

enum GreenCardType
{
  FOLLOWER, ITEM
};

enum FollowerType 
{
  FOOTSOLDIER, ARCHER, CAVALRY, BUSHIDO, SIEGER, ATAKEBUNE, NOT_FOLLOWER
};

enum ItemType
{
  KATANA, SPEAR, BOW, NINJATO, WAKIZASHI, NOT_ITEM
};

enum BlackCardType
{
  PERSONALITY, HOLDING
};

enum PersonalityType
{
  ATTACKER, DEFENDER, CHAMPION, CHANCELLOR, SHOGUN
};

enum HoldingType
{
  MINE, GOLD_MINE, CRYSTAL_MINE, PLAIN, FARMLAND, GIFT_N_FAVOUR, STRONGHOLD 
};
//==========================================|| C A R D ||==========================================

class Card
{
protected:

  const std::string name;
    
  const size_t cost;
    
  bool isTapped;

public:

  Card(const std::string & name , const size_t & cost );
  virtual ~Card() {} 

  void setTapped()   { isTapped = true;  }
  void setUnTapped() { isTapped = false; }

  size_t getCost(void) const { return cost; }
  const std::string getName (void) const { return name; }
  bool checkTapped() const { return isTapped; }

  virtual void print() const = 0;
};

//==========================================|| G R E E N  C A R D ||==========================================

class GreenCard : public Card
{
protected:

  size_t attackBonus;          
  size_t defenceBonus;
  const size_t minHonor;

  const std::string cardText;
  
  const size_t effectBonus; // [Harry] auta a8roizontai sto antistoixo bonus field sto equipment phase (an ginei upgrade)
  const size_t effectCost;  // meta den exoun KAMIA xrhsimothta

  bool attached; // antistoixo ths isDead tou blackCard < Personality

  const enum GreenCardType type;

public:
    
  GreenCard(const std::string & name , const size_t & cost , const size_t & attackBonus ,const size_t & defenceBonus ,const size_t & minHonor ,const std::string & cardText ,const size_t & effectBonus ,const size_t & effectCost , const enum GreenCardType = FOLLOWER);

  enum GreenCardType getGreenCardType() const { return type; }

  size_t getATK() const { return attackBonus;  }
  size_t getDEF() const { return defenceBonus; }
  size_t getMinHonor() const { return minHonor; }
  size_t getEffectCost() const { return effectCost; }

  void attach() { attached = true;  }
  void detach() { attached = false; }

  bool isAttached() const { return attached; }
  virtual void attachToPersonality(Personality *) = 0;

  void upgrade(); 

  virtual void print() const = 0;
  /* Not defined for this class */
  virtual size_t getMaxPerPersonality() const = 0;
  virtual enum FollowerType getFollowerType() const = 0;
  virtual enum ItemType getItemType() const = 0;
};

//==========================================|| F O L L O W E R ||==========================================

class Follower : public GreenCard
{
  const enum FollowerType type;
  const size_t maxPerPerson; // MAX_<GREENCARD>_PER_PERSON gia na mhn xrhsimopoioume tis define'd times pantou sta .cpp
  // [Spiros] Yparxei 8ema an xrhsimopoioume ta defined all the way ? 

public:

  Follower(const std::string & name , const size_t & cost , const size_t & attackBonus ,const size_t & defenceBonus ,const size_t & minHonor ,const std::string & cardText ,const size_t & effectBonus ,const size_t & effectCost , const enum FollowerType , const size_t maxPerPerson);

  enum FollowerType getFollowerType() const { return type; }
  enum ItemType getItemType() const { return NOT_ITEM; }  // this is not supposed to be called, ever

  size_t getMaxPerPersonality() const { return maxPerPerson; }

  void print() const;
  void attachToPersonality(Personality *);
};

//==========================================|| I T E M ||==========================================

class Item : public GreenCard
{
  size_t durability; // not const because it can be decreased till 0 

  const enum ItemType type;
  const size_t maxPerPerson;

public:
    
  Item(const size_t & dur , const std::string & name , const size_t & cost , const size_t & attackBonus ,const size_t & defenceBonus ,const size_t & minHonor ,const std::string & cardText ,const size_t & effectBonus ,const size_t & effectCost , const enum ItemType , const size_t maxPerPerson);

  enum ItemType getItemType() const { return type; }
  enum FollowerType getFollowerType() const { return NOT_FOLLOWER; }  // this is not supposed to be called, ever 
  
  size_t getDurability() const { return durability; }
  size_t getMaxPerPersonality() const { return maxPerPerson; }
  
  void decreaseDurability() { --durability; }
  void attachToPersonality(Personality *);
  
  void print() const;
};

//==========================================|| B L A C K  C A R D ||==========================================

class BlackCard : public Card
{
protected:

  bool isRevealed;

  const enum BlackCardType type;

public:
    
  BlackCard(const std::string & name , const size_t & cost , const enum BlackCardType = PERSONALITY); 

  enum BlackCardType getBlackCardType() const { return type; }

  void setRevealed() { isRevealed = true;  }
  void setHidden()   { isRevealed = false; }

  bool checkRevealed() { return isRevealed; }

  virtual void print() const = 0;
  virtual void attachToPlayer(Player *) = 0;
};

//==========================================|| P E R S O N A L I T Y ||==========================================

class Personality : public BlackCard
{
  const size_t attack;
  const size_t defence;
  
  size_t honor; // honor can be decreased till 0

  bool isDead;
  
  vector <Follower *> * followers;
  vector <Item *>     * items;

  const enum PersonalityType type;

public:
    
  Personality(const std::string & name , const size_t & cost , const size_t & attack ,const size_t & defence , const size_t & honor , const enum PersonalityType );
  ~Personality();

  enum PersonalityType getPersonalityType() const { return type; }

  size_t getATK() const { return attack;  }
  size_t getDEF() const { return defence; }
  size_t getHonor() const { return honor; }

  void die() { isDead = true; } // this is nice
  void kys() { isDead = true; } // this is nicer
  void decreaseHonor() { --honor; }

  bool checkIfDead() const { return isDead; }

  vector <Follower *> * getFollowers() { return followers; }
  vector <Item *>     * getItems() { return items; }

  void print() const;
  void attachToPlayer(Player *);

  void cleanup(); // removes detached items + followers
};

//==========================================|| H O L D I N G ||==========================================

class Holding : public BlackCard
{
protected:

  size_t harvestValue; // Not const , because during Mine linkage it grows 

  const enum HoldingType type;

public:
    
  Holding(const std::string & name , const size_t & cost , const size_t & harvestValue , const enum HoldingType type);

  enum HoldingType getHoldingType() const { return type; }

  size_t getHarvestValue() const { return harvestValue; }

  void increaseHarvestValueBy (size_t points) { harvestValue += points; }

  virtual void print() const; // isws den xreiazontai prints sta mines :shrug: (alla mallon xreiazontai)

  void attachToPlayer(Player *);
};

//==========================================|| M I N E ||==========================================

class Mine : public Holding
{
  GoldMine * upperHolding;

public:
    
  Mine(const std::string & name = "MINE", const size_t & cost = 5 , const size_t & harvestValue = 3);
  ~Mine();

  void print() const;

  void attachToPlayer(Player *);

  GoldMine * getUpperHolding (void) const { return upperHolding ; }

  void setUpperHolding (GoldMine * glM) { upperHolding = glM ; }

};

//==========================================|| C R Y S T A L  M I N E ||==========================================
// ligo malakia pou exw mia oloidia class me allo onoma enos member alla nta3 ugeia
class CrystalMine : public Holding
{
  GoldMine * subHolding;

public:
    
  CrystalMine(const std::string & name = "CRYSTAL_MINE" , const size_t & cost = 12 , const size_t & harvestValue = 6);
  ~CrystalMine();

  void print() const; 

  void attachToPlayer(Player *);

  GoldMine * getSubHolding (void) const { return subHolding ; }

  void setSubHolding (GoldMine * glM) { subHolding = glM ; }

};

//==========================================|| G O L D  M I N E ||==========================================

class GoldMine : public Holding
{
  CrystalMine * upperHolding;
  Mine * subHolding;

public:
    
  GoldMine(const std::string & name = "GOLD_MINE", const size_t & cost = 7 , const size_t & harvestValue = 5);  
  ~GoldMine();

  void print() const;

  void attachToPlayer(Player *);

  CrystalMine * getUpperHolding (void) const { return upperHolding ; }
  Mine * getSubHolding (void) const { return subHolding ; }

  void setUpperHolding (CrystalMine * crM) { upperHolding = crM ; }
  void setSubHolding (Mine * M) { subHolding = M ; }
};

//==========================================|| S T R O N G H O L D ||==========================================

class StrongHold : public Holding
{
  const size_t initHonor;
  const size_t initDefence;
/*  const size_t initMoney; == harvest apo base class */
public:
    
  StrongHold();

  const size_t & getInitHonor() { return initHonor; }

  size_t getInitDEF() const { return initDefence; }

  void print() const;
};

//==========================================|| P R O V I N C E ||==========================================
class Province
{
  bool isBroken; // init as false

  BlackCard *card;

public:

  Province(BlackCard *);
  ~Province();

  bool checkBroken() const { return isBroken; }
  void setBroken() { isBroken = true; }

  BlackCard * getCard() { return card; }
  void setCard(BlackCard * bc) { card = bc ; }
 
  void print() const;

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

  StrongHold *strongHold;

  const size_t honor;
  
  size_t activeProvinces;

  queue <GreenCard *> * fateDeck;
  queue <BlackCard *> * dynastyDeck;

  vector <GreenCard *>   * hand;
  vector <Holding *>     * holdings;
  vector <Personality *> * army;

  vector <Province *>    * provinces;

public:

  Player(const std::string & userName );
  ~Player();

  size_t getHonor() const { return honor; }
  size_t getProvincesNum() const { return activeProvinces; }

  size_t getCurrMoney();

  void decreaseProvinceNum() { --activeProvinces; }
  void increaseProvinceNum() { ++activeProvinces; }

  const std::string& getUserName() const { return userName; }
  
  queue <GreenCard *>    * getFateDeck() { return fateDeck; }
  queue <BlackCard *>    * getDynastyDeck() { return dynastyDeck; }
  vector <GreenCard *>   * getHand() { return hand; }
  vector <Province *>    * getProvinces() { return provinces; }
  vector <Personality *> * getArmy() { return army; }
  vector <Holding *>     * getHoldings() { return holdings; }

  void setFateDeck(queue<GreenCard *> * fDeck) { fateDeck = fDeck; }
  void setDynastyDeck(queue<BlackCard *> * dDeck) { dynastyDeck = dDeck; }

  StrongHold * getStrongHold() { return strongHold; }

  void print() const;
  void printHoldings() const;
  void printArmy() const;
  void printArena() const;
  void printAvailableArmy() const;
  void printHand() const;
  void printProvinces() const;
  void printStatistics() const;

  void cleanup(); // Removes 1) dead personalities from army 2) detached GreenCards

  BlackCard * drawBlackCard (void);
  GreenCard * drawFateCard (void);

  bool makePurchase (size_t cost ); // Tap Holdings until you cover the needed cost
  // Returns true if the purchase is successfull
};  

//==========================================|| G A M E ||==========================================

class Game
{
  std::shared_ptr< vector < std::shared_ptr <Player > > > players;
 
  size_t checkWinningCondition();

  void startingPhase (std::shared_ptr <Player > );
  void equipmentPhase(std::shared_ptr <Player > );
  void battlePhase   (std::shared_ptr <Player > );
  void economyPhase  (std::shared_ptr <Player > );
  void finalPhase    (std::shared_ptr <Player > );

public:

  Game(size_t numPlayers, size_t maxGreenCards, size_t maxBlackCards, size_t maxHand /*might need more, you're up*/);

  void initGameBoard(std::shared_ptr< vector < std::shared_ptr <Player >  > > players , size_t numPlayers , size_t maxGreenCards , size_t maxBlackCards , size_t maxHand);
  void printGameStatistics() const;
  void gameplay();
};

//===================================|| T E M P L A T E  F U N C T I O N ||===================================

// D stands for Deck's Cards. D can be GreenCard or BlackCard
// E stands for Enum. 
template <class D , typename E>
// Pushes <total> times , <D type> objects in the given <deck>
void pushNtimes(std::queue< D *> * deck , std::unordered_map<std::string , vector<size_t> >::iterator j ,const E type  , size_t total , size_t * times ) {

  size_t temp = total;
  while (temp--)
  {
    if (j->second.size() == 6) {  // Follower
      deck->push ((D *)new Follower (j->first , j->second[0] , j->second[1] , j->second[2] , j->second[3] , "Random Text" , j->second[4] , j->second[5] , (const enum FollowerType)type , total));
    }
    else if (j->second.size() == 7) {  // Item
      deck->push ((D *)new Item (j->second[6] , j->first , j->second[0] , j->second[1] , j->second[2] , j->second[3] , "Random Text" , j->second[4] , j->second[5] , (const enum ItemType)type , total));
    }
    else if (j->second.size() == 4) {  // Personality
      deck->push ((D *)new Personality (j->first , j->second[0] , j->second[1] , j->second[2] , j->second[3] , (const enum PersonalityType)type));
    }
    else if (j->second.size() == 2) {  // Holding
        if (j->first == "MINE")
          deck->push ((D *)new Mine ());  
        else if (j->first == "GOLD_MINE")
          deck->push ((D *)new GoldMine ());
        else if (j->first == "CRYSTAL_MINE")
          deck->push ((D *)new CrystalMine ());    
        else 
          deck->push ((D *)new Holding (j->first , j->second[0] , j->second[1] , (const enum HoldingType)type));
    }
    (*times)++;
  }
}


//==========================================|| E N D  O F  F I L E ||==========================================