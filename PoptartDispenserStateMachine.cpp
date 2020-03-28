#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum state { Out_Of_Poptart, No_Credit, Has_Credit, Dispenses_Poptart };
enum stateParameter { No_Of_Poptarts, Credit, Cost_Of_Poptart };
//enum for bases set to their option code
enum bases { Plain_Base = 1, Spicy_Base = 2, Chocolate_Base = 4, Coconut_Base = 8, Fruity_Base = 16}; 
//enum for fillings set to their option code
enum fillings {Chocolate_Filling = 32, Banana_Filling = 64, Strawberry_Filling = 128, 
	Rasberry_Filling = 256, Apple_Filling = 512, Blackberry_Filling = 1024, 
	Maple_Filling = 2048, Marshmallow_Filling = 4096, Cheese_Filling = 8192, 
	CheeseandHam_Filling = 16384, Caramel_Filling =32768, Vanilla_Filling = 65536};

class StateContext;

// State class
class State
{
protected:

	StateContext* CurrentContext;

public:

	State(StateContext* Context)
	{
		CurrentContext = Context;
	}
	virtual ~State(void) {}
	virtual void transition(void) {}
};

// StateContext state
class StateContext
{
protected:

	State* CurrentState = nullptr;
	int stateIndex = 0;
	vector<State*> availableStates;
	vector<int> stateParameters;

public:
	virtual ~StateContext(void);
	virtual void setState(state newState);
	virtual int getStateIndex(void);
	virtual void setStateParam(stateParameter SP, int value);
	virtual int getStateParam(stateParameter SP);
};

// StateContext destructor
StateContext::~StateContext(void)
{
	for (int i = 0; i < this->availableStates.size(); i++) delete this->availableStates[i];
	this->availableStates.clear();
	this->stateParameters.clear();
}

// Sets the current state 
void StateContext::setState(state newState)
{
	this->CurrentState = availableStates[newState]; // Sets current state to the newstate in vector of states
	this->stateIndex = newState; 
	this->CurrentState->transition();
}


int StateContext::getStateIndex(void)
{
	return this->stateIndex;
}

// sets the state parameter of chosen item e.g. Credit
void StateContext::setStateParam(stateParameter SP, int value)
{
	this->stateParameters[SP] = value;
}

// Returns the state parameter of chosen item e.g. Credit
int StateContext::getStateParam(stateParameter SP)
{
	return this->stateParameters[SP];
}

class Transition
{
public:
	virtual bool insertMoney(int) { cout << "Error!" << endl; return false; } //sets insertMoney default as false because it has not happened yet
	virtual bool makeSelection(int) { cout << "Error!" << endl; return false; } //sets makeSelection default as false because it has not happened yet
	virtual bool moneyRejected(void) { cout << "Error!" << endl; return false; } //sets moneyRejected default as false because it has not happened yet
	virtual bool addPoptart(int) { cout << "Error!" << endl; return false; } //sets addPoptart default as false because it has not happened yet
	virtual bool dispense(void) { cout << "Error!" << endl; return false; } //sets dispense default as false because it has not happened yet
};

class PoptartState : public State, public Transition
{
public:
	PoptartState(StateContext* Context) : State(Context) {}
};

class OutOfPoptart : public PoptartState
{
public:
	OutOfPoptart(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

class NoCredit : public PoptartState
{
public:
	NoCredit(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

class HasCredit : public PoptartState
{
public:
	HasCredit(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

class DispensesPoptart : public PoptartState
{
public:
	DispensesPoptart(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

class Product
{
	friend class Filling;
protected:
	string product_description;
	int itemCost = 0;
	/*virtual Product* ReturnNext(void);
	virtual void RemoveHighestCostItem(Product* HighestItem);*/
public:
	virtual void consume(void);
	virtual int cost(void);
	virtual string description(void);
	/*virtual Product* ReturnHighestCostItem(void);
	virtual void RemoveHighestCostItem(void);*/
};

class Filling : public Product {
protected:
	Product* product;
public:
	void addProduct(Product* customerProduct);
	~Filling(void);
	virtual int cost(void);
	virtual void addDescription(void);
};

// Returns the cost of product + cost offilling
int Filling::cost(void) {
	return this->itemCost + product->cost();
}

void Filling::addProduct(Product* customerProduct) {
	product = customerProduct;
}

Filling::~Filling(void) {
	delete product;
}

void Filling::addDescription(void) {
	cout << product_description << endl;
}

void Product::consume(void) {//********************************???********************************
	//delete this->
}

int Product::cost(void) {
	return itemCost;
}

string Product::description(void) {
	return product_description;
}

class Poptart : public Product
{
public:
	Poptart(void)
	{
		this->product_description = "Poptart";
		this->itemCost = 50;
	}
};

// Plain base class + method sets the description and cost of spicy base
class PlainBase : public Poptart {
public:
	PlainBase(void) {
		this->product_description = "Plain Base";
		this->itemCost = 100;
	}
};

// Spicy base class + method sets the description and cost of spicy base
class SpicyBase : public Poptart {
public:
	SpicyBase(void) {
		this->product_description = "Spicy Base";
		this->itemCost = 150;
	}
};

// Chocolate base class + method sets the description and cost of chocolate base
class ChocolateBase : public Poptart {
public:
	ChocolateBase(void) {
		this->product_description = "Chocolate Base";
		this->itemCost = 200;
	}
};

// Coconut base class + method sets the description and cost of coconut base
class CoconutBase : public Poptart {
public:
	CoconutBase(void) {
		this->product_description = "Coconut Base";
		this->itemCost = 200;
	}
};

// Fruity base class + method sets the description and cost of fruity base
class FruityBase : public Poptart {
public:
	FruityBase(void) {
		this->product_description = "Fruity Base";
		this->itemCost = 200;
	}
};

// Chocolate filling class + method sets the description and cost of chocolate filling
class ChocolateFilling : public Filling {
public:
	ChocolateFilling(void) {
		this->product_description = "Chocolate Filling";
		this->itemCost = 20;
	}
};

// Banana filling class + method sets the description and cost of banana filling
class BananaFilling : public Filling {
public:
	BananaFilling(void) {
		this->product_description = "Banana Filling";
		this->itemCost = 50;
	}
};

// Strawberry filling class + method sets the description and cost of strawberry filling
class StrawberryFilling : public Filling {
public:
	StrawberryFilling(void) {
		this->product_description = "Strawberry Filling";
		this->itemCost = 50;
	}
};

// Rasberry filling class + method sets the description and cost of rasberry filling
class RasberryFilling : public Filling {
public:
	RasberryFilling(void) {
		this->product_description = "Rasberry Filling";
		this->itemCost = 50;
	}
};

// Apple filling class + method sets the description and cost of apple filling
class AppleFilling : public Filling {
public:
	AppleFilling(void) {
		this->product_description = "Apple Filling";
		this->itemCost = 50;
	}
};

// Blackberry filling class + method sets the description and cost of blackberry filling
class BlackberryFilling : public Filling {
public:
	BlackberryFilling(void) {
		this->product_description = "Blackberry Filling";
		this->itemCost = 50;
	}
};

// Maple filling class + method sets the description and cost of maple filling
class MapleFilling : public Filling {
public:
	MapleFilling(void) {
		this->product_description = "Maple Filling";
		this->itemCost = 100;
	}
};

// Marshmallow filling class + method sets the description and cost of marshmallow filling
class MarshmallowFilling : public Filling {
public:
	MarshmallowFilling(void) {
		this->product_description = "Marshmallow Filling";
		this->itemCost = 20;
	}
};

// Cheese filling class + method sets the description and cost of cheese filling
class CheeseFilling : public Filling {
public:
	CheeseFilling(void) {
		this->product_description = "Cheese Filling";
		this->itemCost = 70;
	}
};

// Cheese and ham filling class + method sets the description and cost of cheese and ham filling
class CheeseandHamFilling : public Filling {
public:
	CheeseandHamFilling(void) {
		this->product_description = "Cheese and ham Filling";
		this->itemCost = 100;
	}
};

// Caramel filling class + method sets the description and cost of caramel filling
class CaramelFilling : public Filling {
public:
	CaramelFilling(void) {
		this->product_description = "Caramel Filling";
		this->itemCost = 20;
	}
};

// Vanilla filling class + method sets the description and cost of vanilla filling
class VanillaFilling : public Filling {
public:
	VanillaFilling(void) {
		this->product_description = "Vanilla Filling";
		this->itemCost = 50;
	}
};

class Poptart_Dispenser : public StateContext, public Transition
{
	friend class DispensesPoptart;
	friend class HasCredit;
private:
	PoptartState* PoptartCurrentState = nullptr;
	bool itemDispensed = false;
	//indicates whether a product is there to be retrieved
	Product* DispensedItem = nullptr;
	bool itemRetrieved = false; //indicates whether a product has been retrieved
public:
	Poptart_Dispenser(int inventory_count);
	~Poptart_Dispenser(void);
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
	Product* getProduct(void);
	virtual void setStateParam(stateParameter SP, int value);
	virtual int getStateParam(stateParameter SP);
};

//
Poptart_Dispenser::Poptart_Dispenser(int inventory_count) {
	this->availableStates.push_back(new OutOfPoptart(this));
	this->availableStates.push_back(new NoCredit(this));
	this->availableStates.push_back(new HasCredit(this));
	this->availableStates.push_back(new DispensesPoptart(this));
	this->stateParameters.push_back(0); //No of Poptarts
	this->stateParameters.push_back(0); //Available Credit

	this->setState(Out_Of_Poptart);
	if (inventory_count > 0) {
		this->addPoptart(inventory_count);
	}
}

Poptart_Dispenser::~Poptart_Dispenser(void)
{
	if (!this->itemRetrieved)
	{
		delete this->DispensedItem; // if item has not been retrieved ???
	}
}

bool Poptart_Dispenser::insertMoney(int money)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState; //sets insertMoney to be the current state
	return this->PoptartCurrentState->insertMoney(money);
}

bool Poptart_Dispenser::makeSelection(int option)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState; //sets makeSelection to be the current state
	return this->PoptartCurrentState->makeSelection(option);
}

bool Poptart_Dispenser::moneyRejected(void)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState; //sets moneyRejected to be the current state
	return this->PoptartCurrentState->moneyRejected();
}

bool Poptart_Dispenser::addPoptart(int number)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState; // sets addPoptart to be the current state
	return this->PoptartCurrentState->addPoptart(number);
}

bool Poptart_Dispenser::dispense(void)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState; // sets money dispense to be the current state
	return this->PoptartCurrentState->dispense();
}

Product* Poptart_Dispenser::getProduct(void)
{
	if (this->itemDispensed)
	{
		this->itemDispensed = false; //there is no more item to be dispensed
		this->itemRetrieved = true; //the item has been retrived
		return this->DispensedItem;
	}

	return nullptr;
}

void Poptart_Dispenser::setStateParam(stateParameter SP, int value)
{
	if (SP == Cost_Of_Poptart) return;
	this->stateParameters[SP] = value; //the state parameter is != to value then set it to value
}

int Poptart_Dispenser::getStateParam(stateParameter SP)
{
	if (SP == Cost_Of_Poptart)
	{
		if (DispensedItem == nullptr) return 0; //if there is no dispensed item exit
		return DispensedItem->cost(); //if there is a dispensed item return its cost
	}
	return this->stateParameters[SP];
}

// When in DispensesPoptart state insertMoney must be false because by being in DispensesPoptart state it implies you have already added money 
bool DispensesPoptart::insertMoney(int money) {
	cout << "Error: Money already inserted" << endl;
	return false;
}

// When in DispensesPoptart state makeSelection must be false because by being in DispensesPoptart state it implies you have already made a selection
bool DispensesPoptart::makeSelection(int option) {
	cout << "Error: Selection already made" << endl;;
	return false;
}

// When in DispensesPoptart state moneyRejected must be false because you cant reject money while dispensing
bool DispensesPoptart::moneyRejected(void) {
	cout << "Error: Money already accepted" << endl;
	return false;
}

// When in DispensesPoptart state addPoptart should be false because to be get to dispense there has to be at least a poptart 
bool DispensesPoptart::addPoptart(int number) {
	cout << "Error! Already Got Poptarts!" << endl;
	return false;
}

// When in DispensesPoptart state dispense should be true because the user should be able to dispense the Poptart
bool DispensesPoptart::dispense(void) {

	// Check if there is enough money to dispense the Poptart
	if (this->CurrentContext->getStateParam(Credit) < ((Poptart_Dispenser*)CurrentContext)->DispensedItem->cost())
	{
		this->CurrentContext->setState(Has_Credit); // If there isnt enough money set current state to HasCredit so the user can either add more money (insertMoney) or cancel transaction (moneyRejected)
		return true;
	}

	this->CurrentContext->setStateParam(Credit, this->CurrentContext->getStateParam(Credit) - ((Poptart_Dispenser*)CurrentContext)->DispensedItem->cost()); // Subtract the cost of the Poptart from the Credit
	this->CurrentContext->setStateParam(No_Of_Poptarts, this->CurrentContext->getStateParam(No_Of_Poptarts) - 1); // Subtract the no of Poptarts by 1 because 1 will be dispensed
	((Poptart_Dispenser*)CurrentContext)->DispensedItem->consume();
	((Poptart_Dispenser*)CurrentContext)->itemDispensed = true;
	((Poptart_Dispenser*)CurrentContext)->getProduct();

	// Check if there are any Poptarts left 
	if (this->CurrentContext->getStateParam(No_Of_Poptarts) == 0) { 
		cout << "No more Poptarts" << endl;
		this->CurrentContext->setState(Out_Of_Poptart); // If there aren't any left go to OutOfPoptart state
	}

	// Check if there is stil Credit left
	else if (this->CurrentContext->getStateParam(No_Of_Poptarts) > 0 && this->CurrentContext->getStateParam(Credit) == 0) {
		cout << "No more Credit"  << endl;
		this->CurrentContext->setState(No_Credit); // If there is no Credit left then go to NoCredit state
	}

	// Check if there is stil Credit left
	else if (this->CurrentContext->getStateParam(No_Of_Poptarts) > 0 && this->CurrentContext->getStateParam(Credit) > 0) {
		cout << "You have " << this->CurrentContext->getStateParam(Credit) << " credit left." << endl;
		this->CurrentContext->setState(Has_Credit); // If there is no Credit left then go to NoCredit state
	}
	return true;
}

// When in HasCredit state insertMoney should be true because the user should be able insert more money to add to their existing credit
bool HasCredit::insertMoney(int money) {
	cout << "You inserted: " << money;
	this->CurrentContext->setStateParam(Credit, this->CurrentContext->getStateParam(Credit) + money); //adds inserted money to existing credit
	cout << " Total: " << this->CurrentContext->getStateParam(Credit) << endl;
	return true;
}

// When in HasCredit state makeSelection should be true because user shoould make selection in order advance to DispnsesPoptart state
bool HasCredit::makeSelection(int option) {
	Product* poptart = nullptr;
	Filling* currentFilling;

	// if base code is a subset of option code then create base

	// base   0001	
	// option 1000 &	
	// sum	  0000 NO BASE CHOSEN

	// base   1000	
	// option 1001 &	
	// sum	  1000 CHOCOLATE BASE CHOSEN

	// Bases
	if (option & Plain_Base) {
		poptart = new PlainBase; 
		cout << poptart->description() << endl; 
	}   
	else if (option & Spicy_Base) {
		poptart = new SpicyBase; 
		cout <<poptart->description() << endl;
	}
	else if (option & Chocolate_Base) {
		poptart = new ChocolateBase; 
		cout <<poptart->description() << endl;
	}
	else if (option & Coconut_Base) {
		poptart = new CoconutBase; 
		cout <<poptart->description() << endl;
	}
	else if (option & Fruity_Base) {
		poptart = new FruityBase; 
		cout <<poptart->description() << endl;
	}

	// if filling code is a subset of option code then create filling add it to the Poptart

	// filling   0010 0000 
	// option    1000 0000 &	
	// sum	     0000 0000 NO FILLING CHOSEN

	// filling   0010 0000	
	// option    1010 0000 &	
	// sum	     0010 0000 CHOCOLATE FILLING CHOSEN

	// Fillings
	//multiple if instead of else if because many fillings can be chosen
	if (option & Chocolate_Filling) {
		currentFilling = new ChocolateFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription(); 
	}
	if (option & Banana_Filling) {
		currentFilling = new BananaFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription();
	}
	if (option & Strawberry_Filling) {
		currentFilling = new StrawberryFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription();
	}
	if (option & Rasberry_Filling) {
		currentFilling = new RasberryFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription();
	}
	if (option & Apple_Filling) {
		currentFilling = new AppleFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription();
	}
	if (option & Blackberry_Filling) {
		currentFilling = new BlackberryFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription();
	}
	if (option & Maple_Filling) {
		currentFilling = new MapleFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription();
	}
	if (option & Marshmallow_Filling) {
		currentFilling = new MarshmallowFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription();
	}
	if (option & Cheese_Filling) {
		currentFilling = new CheeseFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription();
	}
	if (option & CheeseandHam_Filling) {
		currentFilling = new CheeseandHamFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription();
	}
	if (option & Caramel_Filling) {
		currentFilling = new CaramelFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription();
	}
	if (option & Vanilla_Filling) {
		currentFilling = new VanillaFilling; 
		currentFilling->addProduct(poptart); 
		poptart = currentFilling; 
		currentFilling->addDescription();
	}
	
	this->CurrentContext->setState(Dispenses_Poptart); // Set DispensesPoptart state to current state because a selection has been made

	((Poptart_Dispenser*)CurrentContext)->DispensedItem = poptart; // Set the poptart to be dispensed

	return true;
}

// WHen in HasCredit state moneyRejected should be true because the user should be allowed to reject money i.e user dosent want to buy a Poptart
bool HasCredit::moneyRejected(void) {
	cout << "Money Rejected" << endl;
	this->CurrentContext->setStateParam(Credit, 0); // Set Credit to 0 because when money gets rejected the next state is NoCredit which implies there is no Credit in the machine
	this->CurrentContext->setState(No_Credit); // Set NoCredit state to current state because there is no more Credit
	return true;
}

// When in HasCredit state addPoptart should be false because being in HasCredit state implies there is already Poptarts in the machine
bool HasCredit::addPoptart(int number) {
	cout << "Error: Poptarts already added" << endl;
	return false;
}

// When in HasCredit state dispense should be false because there is nothing selected to dispense
bool HasCredit::dispense(void) {
	cout << "Error! Make selection to dispense" << endl;
	return false;
}

// When in NoCredit state insertMoney should be true because in order to advance to HasCredit money has to be inserted
bool NoCredit::insertMoney(int money) {
	cout << "You inserted: " << money << endl;
	this->CurrentContext->setStateParam(Credit,money); // Set inserted money into Credit
	this->CurrentContext->setState(Has_Credit); // Change to HasCredit state since money has been inserted
	return true;
}

// When in NoCredit state makeSelection should be false because selection can only be made if there is credit in the machine
bool NoCredit::makeSelection(int option) {
	cout << "Error! No money!" << endl;
	return false;
}

// When in NoCredit state moneyRejected should be false because being in NoCredit state implies there is no credit to reject
bool NoCredit::moneyRejected(void) {
	cout << "Error! No money" << endl;
	return false;
}

// When in NoCredit state addPoptart should be false because being in NoCredit implies there is already Poptart/s in the machine
bool NoCredit::addPoptart(int number) {
	cout << "Error! Already Got Poptarts!" << endl;
	return false;
}

// When in NoCredit state dispense should be false because there is no money or selection made yet to dispense a Poptart
bool NoCredit::dispense(void) {
	cout << "Error! No money!" << endl;
	return false;
}

// When in OutOfPoptart state insertMoney should be false because even if money is inserted there are no Poptarts to buy
bool OutOfPoptart::insertMoney(int money) {
	cout << "Error! No Poptarts left! Ejecting!" << endl;
	return false;
}

// When in OutOfPoptart state makeSelection should be false because there are no Poptarts to select
bool OutOfPoptart::makeSelection(int option) {
	cout << "Error! No Poptarts left! No Selection Made" << endl;
	return false;
}

// When in OutOfPoptart state moneyRejected should be true because if there is change left over after dispensing you should be able to retrieve it
bool OutOfPoptart::moneyRejected(void) {
	cout << "Ejecting!" << endl;
	this->CurrentContext->setStateParam(Credit, 0);
	return true;
}

// When in OutOfPoptart state addPoptart should be true because you should be able to add Poptarts since there are none left
bool OutOfPoptart::addPoptart(int number) {
	this->CurrentContext->setStateParam(No_Of_Poptarts, number);
	this->CurrentContext->setState(No_Credit);
	return true;
}

// When in OutOfPoptart state dispense should be false because there are no Poptarts to dispense
bool OutOfPoptart::dispense(void) {
	cout << "Error! No Poptarts left! Nothing to Dispense!" << endl;
	return false;
}
