#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"

// #define DEBUG?? >:)

class Events {
    private:
    int *arr;
    int num;
public:
    int count() const;
    int get(int i) const ;
    Events (const string &file_events);   
    ~Events();

};

class BaseKnight;
//Just put here for BaseItem works ._.


//begin of the items
enum ItemType {poison, pdI, pdII, pdIII, pdIV};
class BaseItem {
public:
    virtual bool canUse ( BaseKnight * knight ) = 0;
    virtual void use ( BaseKnight * knight ) = 0;
    virtual ItemType stuff() = 0;
    virtual string itemname() = 0;
};
class antidote: public BaseItem {
public:
    bool canUse ( BaseKnight * knight );
    void use ( BaseKnight * knight );
    ItemType stuff();
    string itemname();
};
class pd1: public BaseItem {
public:
    bool canUse ( BaseKnight * knight );
    void use ( BaseKnight * knight );
    ItemType stuff();
    string itemname();
};
class pd2: public BaseItem {
public:
    bool canUse ( BaseKnight * knight );
    void use ( BaseKnight * knight );
    ItemType stuff();
    string itemname();
};
class pd3: public BaseItem {
public:
    bool canUse ( BaseKnight * knight );
    void use ( BaseKnight * knight );
    ItemType stuff();
    string itemname();
};
class pd4: public BaseItem {
public:
    bool canUse ( BaseKnight * knight );
    void use ( BaseKnight * knight );
    ItemType stuff();
    string itemname();
}; 
//end of the items


/* * * * * * BAG * * * * */
class BaseBag { 
protected:
    class node {
    public:
    BaseItem *item = NULL;
    node* next = NULL;
};
protected:
    node *head = NULL;  
    BaseKnight *keeper;
    int itemnum = 0;
    int holdmax;      
public:
    BaseBag(BaseKnight *keeper); 
    virtual bool insertFirst(BaseItem * item);
    virtual BaseItem * get(ItemType itemType);
    virtual string toString() const;
    virtual void drop(); 
    virtual string num2str () const;
//    virtual void kickhead(node *&head, BaseItem *&item);
//    virtual void swap(node *&head, node *&temp);
    ~BaseBag();
};
class palabag: public BaseBag{
public: palabag(BaseKnight *keeper);
};
class lancebag: public BaseBag{
public: lancebag(BaseKnight *keeper);
};
class dragonbag: public BaseBag{
public: dragonbag(BaseKnight *keeper);
bool insertFirst(BaseItem * item);
};
class normalbag: public BaseBag{
public: normalbag(BaseKnight *keeper);
};
/* * * * * * BAG * * * * * */

/* * * * EVERYBODY WANT TO BE MY ENEMY * * * */
enum enemytype {MadBear = 1, Bandit = 2, LordLupin = 3, Elf = 4, Troll = 5, Tornbery = 6, 
QueenOfCards = 7, NinaDeRings = 8, DurianGarden = 9, OmegaWeapon = 10, Hades = 11};
class BaseOpponent{
    public:
    static bool killomegaweap;
    static bool killhades;
    protected:
    int i;
    public:
    static BaseOpponent* create(int id, enemytype type);
    virtual enemytype type() = 0;
    virtual void combat(BaseKnight *knight) = 0;
};
class madbear: public BaseOpponent{
    enemytype type();
    void combat(BaseKnight *knight);
};
class bandit: public BaseOpponent{
    enemytype type();
    void combat(BaseKnight *knight);
};
class lordlupin: public BaseOpponent{
    enemytype type();
    void combat(BaseKnight *knight);
};
class elf: public BaseOpponent{
    enemytype type();
    void combat(BaseKnight *Knight);
};
class troll: public BaseOpponent{
    enemytype type();
    void combat(BaseKnight *Knight);
};
class tornbery: public BaseOpponent{
    enemytype type();
    void combat(BaseKnight *knight);
};
class qoc: public BaseOpponent{
    enemytype type();
    void combat(BaseKnight *knight);
};
class ndr: public BaseOpponent{
    enemytype type();
    void combat(BaseKnight *knight);
};
class durian: public BaseOpponent{
    enemytype type();
    void combat(BaseKnight *knight);
};
class omega: public BaseOpponent{
    enemytype type();
    void combat(BaseKnight *knight);
};
class hades: public BaseOpponent{
    enemytype type();
    void combat(BaseKnight *knight);
};
/* * * * END OF THE ENEMY * * * */

/*BEGIN OF KNIGHTS*/
enum KnightType { PALADIN = 0, LANCELOT = 1, DRAGON = 2, NORMAL = 3 };
class BaseKnight {
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    int Antidote;
    BaseBag * bag;
    KnightType knightType;
    bool teemo = 0;
    int resmoney = 0;
public:
    BaseKnight();
    static BaseKnight * create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    string toString() const;
    virtual bool fight(BaseOpponent *enemy);
    virtual int dmg() = 0;
    ~BaseKnight();
public:    
    int HP();
    int maxHP();
    void heal(int hp);
    int lvl();
    void uplvl(int lvl);
    bool teemoR();
    void getteemo(bool dote);
    int money(); //show knight's gil
    void getmoney(int gil); //set the money
    bool setknightbag(BaseItem *item); //add item to the bag
    int getresmoney();  //the residualgil
};
class paladin: public BaseKnight{
public:
    paladin();
    int dmg();
    bool fight(BaseOpponent *enemy);
}; 
class lance: public BaseKnight{
public:
    lance();
    int dmg();
    bool fight(BaseOpponent *enemy);
};
class dragon: public BaseKnight{
public:    
    dragon();
    int dmg();
    bool fight(BaseOpponent *enemy);
};
class normal: public BaseKnight{
public:
    normal();
    int dmg();
    bool fight(BaseOpponent *enemy);
};



class ArmyKnights {
    protected:
    int knightnum;
    BaseKnight **army;
    bool shield = 0;
    bool spear = 0;
    bool hair = 0;
    bool saber = 0;
public:
    ArmyKnights (const string & file_armyknights);
    ~ArmyKnights();
    bool fight(BaseOpponent * opponent);
    bool adventure (Events * events);
    int count() const;
    BaseKnight * lastKnight() const;

    bool hasPaladinShield() const;
    bool hasLancelotSpear() const;
    bool hasGuinevereHair() const;
    bool hasExcaliburSword() const;

    void printInfo() const;
    void printResult(bool win) const;
};
/*END OF KNIGHTS*/

class KnightAdventure {
private:
    ArmyKnights * armyKnights;
    Events * events;

public:
    KnightAdventure();
    ~KnightAdventure(); // TODO:

    void loadArmyKnights(const string &file);
    void loadEvents(const string &file);
    void run();
};

#endif // __KNIGHT2_H__