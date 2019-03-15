#include <vector> //only to be used to contain dispenser states and nothing else
#include <iostream>
#include <string>

using namespace std;

// ---------------------------------------------
// --------------- SPECIFICATION ---------------
// ---------------------------------------------

enum state { Out_Of_Poptarts, No_Credit, Has_Credit, Dispenses_Poptart };	//DO NOT ALTER
enum stateParameter { No_Of_Poptarts, Credit, Cost_Of_Poptart };			//DO NOT ALTER

class StateContext;

//DO NOT ALTER
class State
{
protected:
	StateContext* CurrentContext;

public:
	State(StateContext* Context)
	{
		CurrentContext = Context;
	}
	virtual ~State(void)
	{
	}
};

//DO NOT ALTER
class StateContext
{
protected:
	State* CurrentState = nullptr;
	int stateIndex = 0;
	vector<State*> availableStates;
	vector<int> stateParameters;

public:
	virtual ~StateContext()
	{
		for (int index = 0; index < this->availableStates.size(); index++) delete this->availableStates[index];
		this->availableStates.clear();
		this->stateParameters.clear();
	}

	virtual void setState(state newState)
	{
		this->CurrentState = availableStates[newState];
		this->stateIndex = newState;
	}

	virtual int getStateIndex(void)
	{
		return this->stateIndex;
	}

	virtual void setStateParam(stateParameter SP, int value)
	{
		this->stateParameters[SP] = value;
	}

	virtual int getStateParam(stateParameter SP)
	{
		return this->stateParameters[SP];
	}
};

//DO NOT ALTER
class Transition
{
public:
	virtual bool insertMoney(int) { cout << "Error!" << endl; return false; }
	virtual bool makeSelection(int) { cout << "Error!" << endl; return false; }
	virtual bool moneyRejected(void) { cout << "Error!" << endl; return false; }
	virtual bool addPoptart(int) { cout << "Error!" << endl; return false; }
	virtual bool dispense(void) { cout << "Error!" << endl; return false; }

};


/* Each PoptartState acts as a "mini brain" that deals with the actions
   defined in the Transition class. If the current state cannot deal
   with the given action, it displays a simple error message and returns
   false. */
class PoptartState : public State, public Transition
{
public:
	PoptartState(StateContext* Context) : State(Context) {} //new
};

/*valid transitions: moneyRejected, addPoptart*/
class OutOfPoptarts : public PoptartState
{
public:
	OutOfPoptarts(StateContext* Context) : PoptartState(Context) {} //new
	bool insertMoney(int) { cout << "Error! no poptarts - must insert poptarts first" << endl; return false; } //new
	bool makeSelection(int) { cout << "Error! no poptarts - must insert poptarts first" << endl; return false; } //new
	bool moneyRejected(void); //new
	bool addPoptart(int); //new
	bool dispense(void) { cout << "Error! no poptarts - must insert poptarts first" << endl; return false; } //new
};

/*valid transitions: insertMoney*/
class NoCredit : public PoptartState
{
public:
	NoCredit(StateContext* Context) : PoptartState(Context) {} //new
	bool insertMoney(int); //new
	bool makeSelection(int) { cout << "Error! no credit - must insert credit first" << endl; return false; } //new
	bool moneyRejected(void) { cout << "Error! no credit - must insert credit first" << endl; return false; } //new
	bool addPoptart(int) { cout << "Error! no credit - must insert credit first" << endl; return false; } //new
	bool dispense(void) { cout << "Error! no credit - must insert credit first" << endl; return false; } //new
};

/*valid transitions: insertMoney, makeSelection, moneyRejected*/
class HasCredit : public PoptartState
{
public:
	HasCredit(StateContext* Context) : PoptartState(Context) {} //new
	bool insertMoney(int); //new
	bool makeSelection(int); //new
	bool moneyRejected(void); //new
	bool addPoptart(int) { cout << "Error! machine is stocked - cannot add new poptarts" << endl; return false; } //new
	bool dispense(void) { cout << "Error! must make a selection first" << endl; return false; } //new
};

/*valid tansitions: dispense*/
class DispensesPoptart : public PoptartState
{
public:
	DispensesPoptart(StateContext* Context) : PoptartState(Context) {} //new
	bool insertMoney(int) { cout << "Error! selection made - must dispense items first" << endl; return false; } //new
	bool makeSelection(int) { cout << "Error! selection made - must dispense items first" << endl; return false; } //new
	bool moneyRejected(void) { cout << "Error! selection made - must dispense items first" << endl; return false; } //new
	bool addPoptart(int) { cout << "Error! selection made - must dispense items first" << endl; return false; } //new
	bool dispense(void); //new
};

//NOTE: ReturnHighestCostItem not implemented!
class Product
{
protected:
	string product_description;
	int itemCost = 0;

public:
	virtual void consume(void)
	{
		cout << "Consuming product..." << endl;
		delete this;
	}
	virtual int cost(void) { return this->itemCost; }
	virtual string description(void) { return product_description; }
	virtual Product* ReturnHighestCostItem(void) { return nullptr; }	// NOT IMPLEMENTED
};

class Base : public Product
{
};

class Filling : public Product
{
private:	//all private attributes are new
	Product* ProductBase = nullptr;

public:
	virtual void fillProduct(Product* NewBase);
	virtual int cost(void);
	virtual string description(void);
	Product* ReturnHighestCostItem(void) { return nullptr; }	// NOT IMPLEMENTED
};


class PlainBase : public Base
{
public:
	PlainBase(void)
	{
		this->product_description = "plain base";
		this->itemCost = 100;
	}
};

class SpicyBase : public Base
{
public:
	SpicyBase(void)
	{
		this->product_description = "spicy base";
		this->itemCost = 150;
	}
};

class ChocoBase : public Base
{
public:
	ChocoBase(void)
	{
		this->product_description = "chocolate base";
		this->itemCost = 200;
	}
};

class CoconutBase : public Base
{
public:
	CoconutBase(void)
	{
		this->product_description = "coconut base";
		this->itemCost = 200;
	}
};

class FruityBase : public Base
{
public:
	FruityBase(void)
	{
		this->product_description = "fruity base";
		this->itemCost = 200;
	}
};


class ChocoFilling : public Filling
{
public:
	ChocoFilling(void)
	{
		this->product_description = "chocolate";
		this->itemCost = 20;
	}
};

class BananaFilling : public Filling
{
public:
	BananaFilling(void)
	{
		this->product_description = "banana";
		this->itemCost = 50;
	}
};

class StrawberryFilling : public Filling
{
public:
	StrawberryFilling(void)
	{
		this->product_description = "strawberry";
		this->itemCost = 50;
	}
};

class RaspberryFilling : public Filling
{
public:
	RaspberryFilling(void)
	{
		this->product_description = "raspberry";
		this->itemCost = 50;
	}
};

class AppleFilling : public Filling
{
public:
	AppleFilling(void)
	{
		this->product_description = "apple";
		this->itemCost = 50;
	}
};

class BlackberryFilling : public Filling
{
public:
	BlackberryFilling(void)
	{
		this->product_description = "blackberry";
		this->itemCost = 50;
	}
};

class MapleFilling : public Filling
{
public:
	MapleFilling(void)
	{
		this->product_description = "maple";
		this->itemCost = 100;
	}
};

class MarshmallowFilling : public Filling
{
public:
	MarshmallowFilling(void)
	{
		this->product_description = "marshmallow";
		this->itemCost = 20;
	}
};

class CheeseFilling : public Filling
{
public:
	CheeseFilling(void)
	{
		this->product_description = "cheese";
		this->itemCost = 70;
	}
};

class CheeseAndHamFilling : public Filling
{
public:
	CheeseAndHamFilling(void)
	{
		this->product_description = "cheese & ham";
		this->itemCost = 100;
	}
};

class CaramelFilling : public Filling
{
public:
	CaramelFilling(void)
	{
		this->product_description = "caramel";
		this->itemCost = 20;
	}
};

class VanillaFilling : public Filling
{
public:
	VanillaFilling(void)
	{
		this->product_description = "vanilla";
		this->itemCost = 50;
	}
};


// --------- BUILDER PATTERN (p117) ---------
/* No explicit director class - the Poptart_Dispenser already contains a "getProduct()"
   method and so acts as the director.
   Additionally, products are built according to the decorator pattern, where there
   will be many layers for each base / filling the product has (slide 117). */

/* abstract builder - cannot be used to create products
   NOTE: must manually call clear() to de-allocate memory if required */
class ProductBuilder
{
public:
	virtual void clear(void) {}
	virtual void removeReference(void) {}
	virtual void buildBase(int) {}
	virtual void buildFillings(int) {}
	virtual Product* getProduct(void) { return nullptr; }
};

/* concrete builder - can be used to create a poptart. All logic for handling
   the item codes is contained within this class. */
class PoptartBuilder
{
private:
	Product* BuiltProduct = nullptr;

public:
	void clear(void);
	void removeReference(void);
	void buildBase(int itemcode);		// clears the current product, and creates a single base based off of the item code
	void buildFillings(int itemcode);	
	Product* getProduct(void);
};


class Poptart_Dispenser : public StateContext, public Transition
{
	friend class HasCredit;
	friend class DispensesPoptart;

private: //added all private members
	bool itemReadyToDispense = false;
	Product* ItemToDispense = nullptr;
	bool itemRetrieved = false;
	PoptartBuilder* PBuilder = nullptr;

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


// ----------------------------------------------
// --------------- IMPLEMENTATION ---------------
// ----------------------------------------------

// ---------- POPTART BUILDER ----------

// Clears out any already built poptart 
void PoptartBuilder::clear(void)
{
	delete this->BuiltProduct;
	this->BuiltProduct = nullptr;
}

void PoptartBuilder::removeReference(void)
{
	this->BuiltProduct = nullptr;
}

void PoptartBuilder::buildBase(int itemcode)
{
	//codes hardcoded - could implement a "ProductDetails" class containing a map
	//of bases and fillings to their code.

	clear();	//when we build a base, we start from scratch. So remove any lingering product details.

	//treat code as a binary number, where each bit corresponds to a specific base
	//or filling - use bitwise AND operations to test whether an item with a
	//given code exists in the 'itemcode'.
	if ((itemcode & 1) != 0)			//code is '0000 0001' (1), so base = plain
		BuiltProduct = new PlainBase();

	else if ((itemcode & 2) != 0)		//code is '0000 0010' (2), so base = spicy
		BuiltProduct = new SpicyBase();
	
	else if ((itemcode & 4) != 0)		//code is '0000 0100' (4), so base = chocolate
		BuiltProduct = new ChocoBase();
	
	else if ((itemcode & 8) != 0)		//code is '0000 1000' (8), so base = coconut
		BuiltProduct = new CoconutBase();
	
	else if ((itemcode & 16) != 0)	//code is '0001 0000' (16), so base = fruity
		BuiltProduct = new FruityBase();
	
}

void PoptartBuilder::buildFillings(int itemcode)
{
	//Uses the decorator pattern to add new fillings to the product - the
	//program will check each code individually 

	//this is so inefficient - since we're not allowed to use vectors, could
	//create a new enum containing all bases and fillings and replace the
	//hard-coded values with (2 ** ENUM_VALUE) instead.

	if ((itemcode & 32) != 0)		//code is 32, so add choco filling
	{
		Filling* NewFilling = new ChocoFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
	if ((itemcode & 64) != 0)		//banana
	{
		Filling* NewFilling = new BananaFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
	if ((itemcode & 128) != 0)		//strawberry
	{
		Filling* NewFilling = new StrawberryFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
	if ((itemcode & 256) != 0)		//raspberry
	{
		Filling* NewFilling = new RaspberryFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
	if ((itemcode & 512) != 0)		//apple
	{
		Filling* NewFilling = new AppleFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
	if ((itemcode & 1024) != 0)		//blackberry
	{
		Filling* NewFilling = new BlackberryFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
	if ((itemcode & 2048) != 0)		//maple
	{
		Filling* NewFilling = new MapleFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
	if ((itemcode & 4096) != 0)		//marshmallow
	{
		Filling* NewFilling = new MarshmallowFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
	if ((itemcode & 8192) != 0)		//cheese
	{
		Filling* NewFilling = new CheeseFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
	if ((itemcode & 16384) != 0)	//cheese and ham
	{
		Filling* NewFilling = new CheeseAndHamFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
	if ((itemcode & 32768) != 0)	//caramel
	{
		Filling* NewFilling = new CaramelFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
	if ((itemcode & 65536) != 0)	//vanilla
	{
		Filling* NewFilling = new VanillaFilling();
		NewFilling->fillProduct(this->BuiltProduct);
		this->BuiltProduct = NewFilling;
	}
}

Product* PoptartBuilder::getProduct(void)
{
	return BuiltProduct;
}

// ---------- POPTART DISPENSER ----------

Poptart_Dispenser::Poptart_Dispenser(int inventory_count)
{
	//Set all of the possible states for the FSM
	this->availableStates.push_back(new OutOfPoptarts(this));
	this->availableStates.push_back(new NoCredit(this));
	this->availableStates.push_back(new HasCredit(this));
	this->availableStates.push_back(new DispensesPoptart(this));
	//Set parameters
	this->stateParameters.push_back(0);			//number of poptarts
	this->stateParameters.push_back(0);			//current credit
	this->stateParameters.push_back(0);			//cost of poptart

	//Set initial state
	this->setState(Out_Of_Poptarts);
	//Settings inventory like this updates the FSM state as well as the state parameter
	if (inventory_count > 0)
	{
		this->addPoptart(inventory_count);
	}

	this->PBuilder = new PoptartBuilder();
}

Poptart_Dispenser::~Poptart_Dispenser(void)
{
	delete ItemToDispense;
	delete PBuilder;
}

bool Poptart_Dispenser::insertMoney(int money)
{
	return ((PoptartState*)this->CurrentState)->insertMoney(money);
}

bool Poptart_Dispenser::makeSelection(int option)
{
	return ((PoptartState*)this->CurrentState)->makeSelection(option);
}

bool Poptart_Dispenser::moneyRejected(void)
{
	return ((PoptartState*)this->CurrentState)->moneyRejected();
}

bool Poptart_Dispenser::addPoptart(int number)
{
	return ((PoptartState*)this->CurrentState)->addPoptart(number);
}

bool Poptart_Dispenser::dispense(void)
{
	return ((PoptartState*)this->CurrentState)->dispense();
}

Product* Poptart_Dispenser::getProduct(void)
{
	Product* OutputProduct = nullptr;

	if (itemReadyToDispense == true && itemRetrieved == false)
	{
		OutputProduct = ItemToDispense;
		ItemToDispense = nullptr;
		itemReadyToDispense = false;
		itemRetrieved = true;
	}

	return OutputProduct;
}

void Poptart_Dispenser::setStateParam(stateParameter SP, int value)
{
	this->stateParameters[SP] = value;
}

int Poptart_Dispenser::getStateParam(stateParameter SP)
{
	return this->stateParameters[SP];
}

// ---------- OUT OF POPTARTS STATE ----------

bool OutOfPoptarts::moneyRejected(void)
{
	//Perform action:
	cout << "Ejecting money" << endl;
	this->CurrentContext->setStateParam(Credit, 0); //remove credit from machine
	
	//Transition to new state:
	this->CurrentContext->setState(Out_Of_Poptarts);

	//Indicate successfull transition
	return true;
}

bool OutOfPoptarts::addPoptart(int number)
{
	//Perform action: (note - can simply set 'No_Of_Poptarts' to 'number', as
	// there will always be 0 poptarts currently in the system.
	cout << "Adding " << number << " poptarts" << endl;
	this->CurrentContext->setStateParam(No_Of_Poptarts, number);

	//Transition to new state:
	this->CurrentContext->setState(No_Credit);

	//Indicate successfull transition
	return true;
}

// ---------- NO CREDIT STATE ----------

bool NoCredit::insertMoney(int money)
{
	//Perform action:
	cout << "No money in the system - ";
	cout << "Adding " << money << " credit" << endl;
	this->CurrentContext->setStateParam(Credit, money);

	//Transition to new state:
	this->CurrentContext->setState(Has_Credit);

	//Indicate successfull transition
	return true;
}

// ---------- HAS CREDIT STATE ----------

bool HasCredit::insertMoney(int money)
{
	//Perform action:
	cout << "Adding " << money << " more credit - ";
	money += this->CurrentContext->getStateParam(Credit);
	this->CurrentContext->setStateParam(Credit, money);
	cout << "total credit = " << this->CurrentContext->getStateParam(Credit) << endl;

	//Transition to new state:
	this->CurrentContext->setState(Has_Credit);

	//Indicate successfull transition
	return true;
}

bool HasCredit::makeSelection(int option)
{
	//Perform action:
	cout << "Selecting option '" << option << "'" << endl;
	PoptartBuilder* PBuilder = ((Poptart_Dispenser*)CurrentContext)->PBuilder;

	//Use builder class to create the requested poptart
	PBuilder->buildBase(option);
	PBuilder->buildFillings(option);

	//Transition to new state:
	this->CurrentContext->setState(Dispenses_Poptart);

	//Indicate successfull transition
	return true;
}

bool HasCredit::moneyRejected(void)
{
	//Perform action:
	cout << "Ejecting money" << endl;
	this->CurrentContext->setStateParam(Credit, 0); //remove credit from machine

	//Transition to new state:
	this->CurrentContext->setState(No_Credit);

	//Indicate successfull transition
	return true;
}

// ---------- DISPENSE POPTART STATE ----------

bool DispensesPoptart::dispense(void)
{
	//Perform action:
	cout << "Dispensing poptart..." << endl;

	//get a reference to the dispenser
	Poptart_Dispenser* Dispenser = (Poptart_Dispenser*)CurrentContext;	

	//check if there is a pre-existing item in the machine - if there is, vapourise it!
	if (Dispenser->itemReadyToDispense == true) delete Dispenser->ItemToDispense;

	//put the new poptart into the machine waiting to be collected - it's the user's job to collect it manually
	Dispenser->ItemToDispense = Dispenser->PBuilder->getProduct();
	CurrentContext->setStateParam(Cost_Of_Poptart, Dispenser->ItemToDispense->cost());
	Dispenser->itemReadyToDispense = true;
	Dispenser->itemRetrieved = false;
	Dispenser->PBuilder->removeReference();		//responsibility for the product is no longer with the builder, but with the dispenser

	//check that the user has enough credit
	int numPoptarts = CurrentContext->getStateParam(No_Of_Poptarts);
	int userCredit = CurrentContext->getStateParam(Credit);
	int poptartCost = CurrentContext->getStateParam(Cost_Of_Poptart);
	if (userCredit < poptartCost)
	{
		//delete the poptart we just created
		cout << "Error: not enough credit for selected item" << endl;
		Dispenser->ItemToDispense->consume();
		Dispenser->ItemToDispense = nullptr;
		Dispenser->itemReadyToDispense = false;

		//Transition to new state:
		this->CurrentContext->setState(Has_Credit);
	}
	else
	{
		//remove 1 poptart and deduct credit
		CurrentContext->setStateParam(No_Of_Poptarts, numPoptarts - 1);
		CurrentContext->setStateParam(Credit, userCredit - poptartCost);

		//Transition to new state:
		if (this->CurrentContext->getStateParam(No_Of_Poptarts) == 0)
			this->CurrentContext->setState(Out_Of_Poptarts);

		else if (this->CurrentContext->getStateParam(Credit) == 0)
			this->CurrentContext->setState(No_Credit);

		else
			this->CurrentContext->setState(Has_Credit);
	}

	//Indicate successfull transition
	return true;
}


// ---------- PRODUCT ----------

void Filling::fillProduct(Product* NewBase)
{
	if (this->ProductBase != nullptr) delete this->ProductBase;
	this->ProductBase = NewBase;
}

int Filling::cost(void)
{
	int totalCost = this->itemCost;
	if (this->ProductBase != nullptr)
		totalCost += ProductBase->cost();

	return totalCost;
}

string Filling::description(void)
{
	string desc = "";

	if (this->ProductBase != nullptr)
	{
		desc = this->ProductBase->description();
		desc += " with " + product_description;
	}
	else
	{
		desc = product_description;
	}

	return desc;
}