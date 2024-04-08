#include "knight2.h"

bool snt(int n) {
    if (n < 2) {
        return false; 
    }
    for (int i = 2; i*2 < n; i++) {
        if (n % i == 0) {
            return false; 
        }
    }
    return true; 
}

// * * * * * */begin of class event
int Events:: count() const {
        return num;
    };
int Events::get(int i) const{
        return *(arr+i); 
    };
Events::Events (const string &file_events) {
    ifstream in(file_events);
    in>>num;
    arr = new int[num];
    for (int i = 0; i < num; i++){
        in>>arr[i]; 
    }
};
Events::~Events(){
        delete[] arr;
    }    
//* * * * * * */end of class event

// * * * * /begin of the items/ * * * * * *;
string antidote::itemname(){ return "Antidote"; }
string pd1::itemname(){ return "PhoenixI"; }
string pd2::itemname(){ return "PhoenixII"; }
string pd3::itemname(){ return "PhoenixIII"; }
string pd4::itemname(){ return "PhoenixIV"; }
bool pd1::canUse ( BaseKnight * knight ) { return (knight->HP() <= 0?1:0);}
void pd1::use ( BaseKnight * knight ) { return knight->heal(knight->maxHP());}
ItemType pd1::stuff() {return pdI;}
//pd2: HP < MaxHP/4; phục hồi HP thành MaxHP.
bool pd2::canUse ( BaseKnight * knight ) { return (knight->HP() < (knight->maxHP()/4)?1:0);}
void pd2::use ( BaseKnight * knight ) { return knight->heal(knight->maxHP());}
ItemType pd2::stuff(){return pdII;}
/*pd3: HP < MaxHP/3 (chỉ tính phần nguyên), nếu HP <= 0 thì phục hồi HP thành MaxHP/3 (chỉ lấy phần nguyên), ngược lại tăng thêm 1
lượng bằng MaxHP/4 (chỉ lấy phần nguyên).*/
bool pd3::canUse ( BaseKnight * knight ) { return (knight->HP() < (knight->maxHP()/3)?1:0);}
void pd3::use ( BaseKnight * knight ) {
    if ((knight->HP())<=0) { return knight->heal((knight->maxHP())/3); }
    else { return knight->heal( (knight->HP()) + (knight->maxHP())/4 );}
}
ItemType pd3::stuff() {return pdIII;}
/*HP < MaxHP/2 (chỉ tính phần nguyên), nếu HP <= 0 thì phục hồi HP thành MaxHP/2 (chỉ lấy phần nguyên), ngược lại tăng thêm 1
lượng bằng MaxHP/5 (chỉ lấy phần nguyên).*/
bool pd4::canUse ( BaseKnight * knight ) { return (knight->HP() < (knight->maxHP()/2)?1:0);}
void pd4:: use ( BaseKnight * knight ) {
    if ((knight->HP())<=0) { return knight->heal((knight->maxHP())/2); }
    else { return knight->heal( (knight->HP()) + (knight->maxHP())/5 );}
}
ItemType pd4::stuff() {return pdIV;}
//antidote
bool antidote::canUse ( BaseKnight * knight ) { return knight->teemoR();}
void antidote::use ( BaseKnight * knight ) { return knight->getteemo(false);}
ItemType antidote::stuff(){return poison;}
//* * * *end of the items* * * * *;


/* * * BEGIN implementation of class BaseBag * * */
BaseBag::BaseBag (BaseKnight *keeper){
    this->head = NULL;  
    this->keeper = keeper;
    this->itemnum = 0;
    this->holdmax;
} 
bool BaseBag::insertFirst(BaseItem * item){
    if (itemnum == holdmax) return false;
    node *newitem = new node();
    newitem->item = item;
    newitem->next = head;
    head = newitem;
//    kickhead(head, item);
    itemnum++;
    return true;
}
BaseItem* BaseBag::get(ItemType itemType){
// trỏ đến vật phẩm sử dụng được, sử dụng hàm use ở đây, get là sử dụng luôn   
    if (itemnum == 0) return NULL;
// chia làm giải độc và hồi máu
    if (itemType == poison) {
        node *temp = head;
        while (temp!=NULL && temp->item->stuff()!=itemType){ temp = temp -> next; }
        if (temp == NULL) return NULL;
        else {
            swap(head->item, temp->item);
            temp = head;
            head = head->next;
            temp->item->use(keeper);
            itemnum--;
            delete temp->item;
            delete temp;
            return NULL;
        } 
    }   
    else {
        node *temp = head;
        while (temp!=NULL && temp->item->canUse(keeper) == false){ temp = temp -> next; }
        if (temp == NULL) return NULL;
        else {
            swap(head->item, temp->item);
            temp = head;
            head = head->next;
            temp->item->use(keeper);
            itemnum--;
            delete temp->item;
            delete temp;
            return NULL;
        } 
    }  
}
string BaseBag::num2str() const{
    ostringstream num;
    num<<itemnum;
    return num.str(); 
}
string BaseBag::toString() const{
    if (itemnum==0) return "Bag[count=0;]";
    string ans = "Bag[count=" + num2str() + ";";
    node *temp = head;
    while (temp!=NULL) {
        ans = ans + temp->item->itemname() + ",";
        temp = temp->next;
    }
    ans[ans.length() - 1] = ']';
    return ans;
}
void BaseBag::drop(){
    if (itemnum == 0) return;
    node *lost = head;
    head = head->next;
    delete lost->item;
    delete lost;
    itemnum--;
}
BaseBag::~BaseBag(){
    while (itemnum!=0){
        node *del = head;
        head = head->next;
        delete del->item;
        delete del;
        itemnum--;
    }
}
palabag::palabag(BaseKnight *keeper): BaseBag(keeper) {this->holdmax = -1;}
lancebag::lancebag(BaseKnight *keeper): BaseBag(keeper) {this->holdmax = 16;}
dragonbag::dragonbag(BaseKnight *keeper): BaseBag(keeper) {this->holdmax = 14;}
normalbag::normalbag(BaseKnight *keeper): BaseBag(keeper) {this->holdmax = 19;}
bool dragonbag::insertFirst(BaseItem * item){
    if (itemnum == holdmax || item->stuff() == poison) return false;
//    kickhead(head, item);
    node *newitem = new node();
    newitem->item = item;
    newitem->next = head;
    head = newitem;
    itemnum++;
    return true;
}
/* * * END implementation of class BaseBag * * */

/* * * BEGIN implementation of class BaseKnight * * */
BaseKnight* BaseKnight::create(int id, int maxhp, int level, int gil, int Antidote, int phoenixdownI){
BaseKnight *crep;
    if (snt(maxhp)) {
        crep = new paladin;
        crep -> knightType = PALADIN;
        crep->bag = new palabag(crep);
    }

    else if (maxhp == 888) {
        crep = new lance;
        crep -> knightType = LANCELOT; 
        crep->bag = new lancebag(crep);
    } 

    else if (maxhp==345 || maxhp==354|| maxhp==435 || maxhp==453 || maxhp==534 || maxhp==543) {
        crep = new dragon;
        crep -> knightType = DRAGON;  
        crep->bag = new dragonbag(crep);  
    } 

    else {
        crep = new normal;
        crep -> knightType = NORMAL;
        crep->bag = new normalbag(crep);
    } 

    crep->id = id;
    crep->maxhp = maxhp;
    crep->hp = maxhp;
    crep->level = level;
    crep->gil = gil;
    while (phoenixdownI!=0){
        pd1 *healitem = new pd1();
        if (crep->bag->insertFirst(healitem)==false) delete healitem;
        phoenixdownI--;
    }
    while (Antidote!=0){
        antidote *nondote = new antidote();
        if (crep->bag->insertFirst(nondote)==false) delete nondote;
        Antidote--;
    }
    return crep;
}
int BaseKnight::HP() {return hp;}
int BaseKnight::maxHP() {return maxhp;}
void BaseKnight::heal(int hp){
    this->hp = hp;
    if (this->hp > this->maxhp) { this->hp = this->maxhp; }
}
bool BaseKnight::teemoR(){return teemo;}
void BaseKnight::getteemo(bool dote){this->teemo = dote;}
BaseKnight::BaseKnight(){};
string BaseKnight::toString() const {
    string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    // inefficient version, students can change these code
    //      but the format output must be the same
    string s("");
    s += "[Knight:id:" + to_string(id) 
        + ",hp:" + to_string(hp) 
        + ",maxhp:" + to_string(maxhp)
        + ",level:" + to_string(level)
        + ",gil:" + to_string(gil)
        + "," + bag->toString()
        + ",knight_type:" + typeString[knightType]
        + "]";
    return s;
}
bool BaseKnight::fight(BaseOpponent *enemy){
    if (this->teemo){  
        this->bag->get(poison);
        if (this->teemo){
            this->hp -= 10;
            this->bag->drop();
            this->bag->drop();
            this->bag->drop();
            this->teemo = false;
        }
    }
    this->bag->get(pdI);
    if (this->hp <= 0 && this->gil >= 100){
        this->hp = this->maxhp/2;
        this->gil -= 100;
    }
    return (this->hp>0?1:0);
//check hp then heal
}
int BaseKnight::lvl() { return this->level;}
void BaseKnight::uplvl(int lvl) {
    if (lvl > 10) this->level = 10;
    else this->level = lvl;
}
int BaseKnight::money(){ return gil;}
void BaseKnight::getmoney(int gil){
    if (gil > 999) {
        this->resmoney = gil - 999;
        this->gil = 999;
    }
    else this->gil = gil;
}
int BaseKnight::getresmoney(){
    int temp;
    temp = this->resmoney;
    this->resmoney = 0;
    return temp;
}
bool BaseKnight::setknightbag(BaseItem *item){
    return bag->insertFirst(item);
}
BaseKnight::~BaseKnight(){
    delete bag;
}
//knight::knight
paladin::paladin(){}
lance::lance(){}
dragon::dragon(){}
normal::normal(){}
// virtual int dmg() = 0;
int paladin::dmg(){return this->hp * this->level * 0.06;}
int lance::dmg(){return this->hp * this->level * 0.05;}
int dragon::dmg(){return this->hp * this->level * 0.075;}
int normal::dmg(){return 0;}
bool paladin::fight(BaseOpponent *enemy){
    if (enemy->type() == MadBear) this->getmoney(this->money() + 100);
    else if (enemy->type() == Bandit) this->getmoney(this->money() + 150);
    else if (enemy->type() == LordLupin) this->getmoney(this->money() + 450);
    else if (enemy->type() == Elf) this->getmoney(this->money() + 750);
    else if (enemy->type() == Troll) this->getmoney(this->money() + 800);
    else if (enemy->type() == QueenOfCards) {
        int keepmoney = this->money();
        enemy->combat(this);
        if (this->money() < keepmoney) this->gil = keepmoney;
    }
    else if (enemy->type() == NinaDeRings) hp = hp<(maxhp/3)?(hp += maxhp/5):hp;
    else if (enemy->type() == Hades && this->level >= 8){
        BaseOpponent::killhades = true;
    }
    else enemy->combat(this);
    return BaseKnight::fight(enemy);    
}
bool lance::fight(BaseOpponent *enemy){
    if (enemy->type() == MadBear) this->getmoney(this->money() + 100);
    else if (enemy->type() == Bandit) this->getmoney(this->money() + 150);
    else if (enemy->type() == LordLupin) this->getmoney(this->money() + 450);
    else if (enemy->type() == Elf) this->getmoney(this->money() + 750);
    else if (enemy->type() == Troll) this->getmoney(this->money() + 800);
    else enemy->combat(this);
    return BaseKnight::fight(enemy);
}
bool dragon::fight(BaseOpponent *enemy){
    if (enemy->type() == OmegaWeapon) {
        if (BaseOpponent::killomegaweap == true) return true;
        level = 10;
        gil = 999;
        BaseOpponent::killomegaweap = true;
    }
    else enemy->combat(this);
    teemo = false;
    return BaseKnight::fight(enemy);
}
bool normal::fight(BaseOpponent *enemy){
    enemy->combat(this);
    return BaseKnight::fight(enemy);
}
/* * * END implementation of class BaseKnight * * */

/* * * * * * * BEGIN OF CLASS ENEMY * * * * * * */
bool BaseOpponent::killhades = 0;
bool BaseOpponent::killomegaweap = 0;
BaseOpponent* BaseOpponent::create(int i, enemytype type){
BaseOpponent *enemy;    
    if (type == MadBear) enemy = new madbear; 
    else if (type == Bandit) enemy = new bandit;
    else if (type == LordLupin) enemy = new lordlupin;
    else if (type == Elf) enemy = new elf;
    else if (type == Troll) enemy = new troll;
    else if (type == Tornbery) enemy = new tornbery;
    else if (type == QueenOfCards) enemy = new qoc;
    else if (type == NinaDeRings) enemy = new ndr;
    else if (type == DurianGarden) enemy = new durian;
    else if (type == OmegaWeapon) enemy = new omega;
    else if (type == Hades) enemy = new hades;
    else return NULL;
    enemy->i = i;
    return enemy;    
}
//enemytype type();
enemytype madbear::type() { return MadBear;}
enemytype bandit::type() { return Bandit;}
enemytype lordlupin::type() { return LordLupin;}
enemytype elf::type() { return Elf;}
enemytype troll::type() { return Troll;}
enemytype tornbery::type() { return Tornbery;}
enemytype qoc::type() { return QueenOfCards;}
enemytype ndr::type() { return NinaDeRings;}
enemytype durian::type() { return DurianGarden;}
enemytype omega::type() { return OmegaWeapon;}
enemytype hades::type() { return Hades;}
//void combat(BaseKnight *knight);
void madbear::combat(BaseKnight *knight){
    int levelO = (this->i + 1)%10 + 1;
    if (knight->lvl() >= levelO) knight->getmoney(knight->money()+100);
    else knight->heal(knight->HP() - 10 * (levelO - knight->lvl()));
}
void bandit::combat(BaseKnight *knight){
    int levelO = (this->i + 2)%10 + 1;
    if (knight->lvl() >= levelO) knight->getmoney(knight->money()+150);
    else knight->heal(knight->HP() - 15 * (levelO - knight->lvl()));    
}
void lordlupin::combat(BaseKnight *knight){
    int levelO = (this->i + 3)%10 + 1;
    if (knight->lvl() >= levelO) knight->getmoney(knight->money()+450);
    else knight->heal(knight->HP() - 45 * (levelO - knight->lvl()));
}
void elf::combat(BaseKnight *knight){
    int levelO = (this->i + 4)%10 + 1;
    if (knight->lvl() >= levelO) knight->getmoney(knight->money()+750);
    else knight->heal(knight->HP() - 75 * (levelO - knight->lvl()));
}
void troll::combat(BaseKnight *knight){
    int levelO = (this->i + 5)%10 + 1;
    if (knight->lvl() >= levelO) knight->getmoney(knight->money()+800);
    else knight->heal(knight->HP() - 95 * (levelO - knight->lvl()));
}
void tornbery::combat(BaseKnight *knight){
    int levelO = (this->i + 6)%10 + 1;
    if (knight->lvl() >= levelO) knight->uplvl(knight->lvl()+1);
    else knight->getteemo(true);
}
void qoc::combat(BaseKnight *knight){
    int levelO = (this->i + 7)%10 + 1;
    if (knight->lvl() >= levelO) knight->getmoney(knight->money()*2);
    else knight->getmoney(knight->money()/2);
}
void ndr::combat(BaseKnight *knight){
    if (knight->money() >= 50){
        if (knight->HP() < (knight->maxHP()/3) ){
            knight->heal(knight->HP() + (knight->maxHP()/5) );
            knight->getmoney(knight->money() - 50);
        } 
    }
}
void durian::combat(BaseKnight *knight){
    knight->heal(knight->maxHP());
}
void omega::combat(BaseKnight *knight){
    if (BaseOpponent::killomegaweap == true) return;
    if (knight->lvl() == 10 && knight->HP() == knight->maxHP()){
        knight->uplvl(10);
        knight->getmoney(999);
        BaseOpponent::killomegaweap = true;
    }
    else knight->heal(0);
}
void hades::combat(BaseKnight *knight){
    if (BaseOpponent::killhades == true) return;
    if (knight->lvl() == 10) BaseOpponent::killhades = true;
    else knight->heal(0);
}
/* * * * * * * END OF CLASS ENEMY * * * * * * */

/* * * BEGIN implementation of class ArmyKnights * * */
void ArmyKnights::printInfo() const {
    cout << "No. knights: " << this->count();
    if (this->count() > 0) {
        BaseKnight * lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
        << ";LancelotSpear:" << this->hasLancelotSpear()
        << ";GuinevereHair:" << this->hasGuinevereHair()
        << ";ExcaliburSword:" << this->hasExcaliburSword()
        << endl
        << string(50, '-') << endl;
}

void ArmyKnights::printResult(bool win) const {
    cout << (win ? "WIN" : "LOSE") << endl;
}
ArmyKnights::ArmyKnights (const string & file_armyknights){
    shield = 0; spear = 0; hair = 0; saber = 0;
    ifstream in(file_armyknights);
    in>>knightnum;
    army = new BaseKnight* [knightnum];
    for (int i = 0; i < knightnum; i++){
        int hp, lvl, pd, gil, antidote;
        in>>hp>>lvl>>pd>>gil>>antidote;
        army[i] = BaseKnight::create(i+1,hp,lvl,gil,antidote,pd);
    }
}
ArmyKnights::~ArmyKnights(){  
    for (int i = 0; i < knightnum; i++){
        delete army[i];
    }
    delete[] army;
}
bool ArmyKnights::fight(BaseOpponent * opponent){
        BaseKnight *knightcombat = lastKnight(); //gán last knight
        if (knightcombat->fight(opponent)){ //truyền gil dư
            for (int i = knightnum - 1; i >= 0; i--){ 
                int resgil = knightcombat->getresmoney(); //số gil dư
                if (resgil == 0 || i == 0) break; //hết gil dư
                else army[i-1]->getmoney(army[i-1]->money() + resgil); //truyền gil dư 
            } 
        }
        else {
        delete knightcombat; //nếu thua
        knightnum--; //tiếp tục vòng lặp
        }
    delete opponent;
    return knightnum > 0? 1 : 0;    
}
bool ArmyKnights::adventure (Events * events){
    for (int i = 0; i < events->count(); i++){
        int eventnum = events->get(i);
//* * * * * * combat with crep
        if (0 < eventnum && eventnum <= 11){
            this->fight(BaseOpponent::create(i,enemytype(eventnum)));
            if (knightnum == 0) {
                this->printInfo();
                return false;
            }    
            if (BaseOpponent::killhades) this->shield = true;
            this->printInfo();
            continue;
        }
//* * * * * * take important weap for final combat
        else if (eventnum == 95) {
            this->shield = true;
            this->printInfo();
            continue;
        }
        else if (eventnum == 96) {
            this->spear = true;
            this->printInfo();
            continue;
        }
        else if (eventnum == 97) {
            this->hair = true;
            this->printInfo();
            continue;
        }
        else if (eventnum == 98){
            if (this->shield && this->spear && this->hair)
            this->saber = true;
            this->printInfo();
            continue;
        }
//* * * * * * take some stuff
        else if (eventnum == 112){
            pd2 *phoe2 = new pd2;
            for (int j = knightnum - 1; j >= 0; j-- ){
                if (army[j]->setknightbag(phoe2)) break;
                if (j==0) delete phoe2;
            }
            this->printInfo();
            continue;
        }
        else if (eventnum == 113){
            pd3 *phoe3 = new pd3;
            for (int j = knightnum - 1; j >= 0; j-- ){
                if (army[j]->setknightbag(phoe3)) break;
                if (j==0) delete phoe3;
            }
            this->printInfo();
            continue;
        }
        else if (eventnum == 114){
            pd4 *phoe4 = new pd4;
            for (int j = knightnum - 1; j >= 0; j-- ){
                if (army[j]->setknightbag(phoe4)) break;
                if (j==0) delete phoe4;
            }
            this->printInfo();
            continue;
        }
// * * * * * * * combat final boss
        else if (eventnum == 99){
//if we have saber
            if (saber){
                this->printInfo();
                return true;
            }
//if we dont owned enough things
            if ( shield == 0 || spear == 0 || hair == 0){
                for(int i = 0; i < knightnum ; i++) delete army[i];
                knightnum = 0;
                this->printInfo();
                return false;
            }
//if we have only 3 things
            if (shield && spear && hair){
                int bosshp = 5000;
                int zerodmg = 0; //number of normal knight
                while (knightnum){
                   
                    BaseKnight *myturn = lastKnight();
                    bosshp = bosshp - myturn->dmg();
                    if (myturn->dmg()==0) {
                        zerodmg++;
                        knightnum--;
                        continue;
                    }
                    if (bosshp <= 0){
                        knightnum += zerodmg;
                        this->printInfo();
                        delete myturn;
                        return true;
                    }
                    if (bosshp > 0){
                        delete myturn;
                        knightnum--;
                        if (this->knightnum == 0) { return false; } //why u don't run??? :((                       
                    }
                }       
                this->printInfo();
                return false;         
            }// end of the 3 things combat case
        
        }//end of combat final boss

    }//biggest for loop
    return true;
}//end of this function
int ArmyKnights::count() const             { return this->knightnum;}
BaseKnight* ArmyKnights::lastKnight() const{ return this->army[knightnum-1];}
bool ArmyKnights::hasPaladinShield() const { return this->shield;}
bool ArmyKnights::hasLancelotSpear() const { return this->spear;}
bool ArmyKnights::hasGuinevereHair() const { return this->hair;}
bool ArmyKnights::hasExcaliburSword() const{ return this->saber;}

/* * * END implementation of class ArmyKnights * * */

/* * * BEGIN implementation of class KnightAdventure * * */
KnightAdventure::KnightAdventure() {
    armyKnights = nullptr;
    events = nullptr;
}
KnightAdventure::~KnightAdventure(){
    delete armyKnights;
    delete events;
}

void KnightAdventure::loadArmyKnights(const string &file) { armyKnights = new ArmyKnights(file);}
void KnightAdventure::loadEvents(const string &file) { events = new Events(file);}
void KnightAdventure::run(){
    BaseOpponent::killhades = 0;
    BaseOpponent::killomegaweap = 0;
    armyKnights->printResult(armyKnights->adventure(events));
}
/* * * END implementation of class KnightAdventure * * */