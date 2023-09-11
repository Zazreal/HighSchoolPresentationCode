#include "pch.h"
#include <iostream>
#include <string>
#include <memory>

class Entity{
private:

protected:
	std::string name = "Unknown";
	double Health = 5;
	double movementSpeed = 1;
	std::string TypeName = "Entity";
	static int EntityCount;
public:
	Entity(){EntityCount++;}
	~Entity() { std::cout << "Destroying Object " << name << "\n"; }
	Entity(const char* name1, double Health1) : name{ name1 }, Health{ Health } {
		EntityCount++;
	}
	Entity(const char* name, double Health, double movementSpeed):
	name(name), Health(Health), movementSpeed(movementSpeed)
	{
		std::cout << "This is an Entity constructor of " << this->name <<std::endl;
		EntityCount++;
	}
	double getHealth() { return Health; }
	void setHealth(double Health) {
		this->Health = Health;
	}
	void setMMS(double movementSpeed) {
		this->movementSpeed = movementSpeed;
	}
	virtual void TakeDamage(double DT) {
		this->Health -= DT;
		printf("%s has taken %.2f damage\n", name.c_str(), DT);
		if (Health <= 0) {
			printf("%s has fallen\n", name.c_str());
		}
		else {
			printf("%s now has %.2f Health\n", name.c_str(), Health);
		}
		std::cout << std::flush;
	}
	void PrintEntityStats() const{
		std::cout << TypeName<<" "<< this->name << "'s stats\n";
		printf("Health = %.2f, movementSpeed = %.2f\n", this->Health, this->movementSpeed);
		std::cout << std::flush;
	}
};




class Warrior: public Entity
{
private:
	double AttackSpeed  = 1;
	double AttackDamage = 1;
	double Defense = 0;
public:
	Warrior() :Entity() { TypeName = "Warrior"; }
	~Warrior() {/* printf("%s Has beeen destroyed\n", name.c_str());*/ }
	Warrior(const char* name, double Health, double movementSpeed, double ATS , double ATD, double DEF) :
		Entity(name, Health, movementSpeed)
	{
		this->AttackDamage = ATD;
		this->AttackSpeed = ATS;
		this->Defense = DEF;
		TypeName = "Warrior";
		std::cout << "This is a Warrior constructor of " << this->name << std::endl;
	}
	double GetDamage() 
	{ 
		return AttackDamage;
	}
	void TakeDamage(double AT) override final {
		this->Health -= (AT - this->Defense) > 0 ? (AT - this->Defense) : 1;
		double DamageTaken = (AT - this->Defense) > 0 ? (AT - this->Defense) : 1;
		printf("%s has taken %.2f damage\n", name.c_str(), DamageTaken);
		if (Health <= 0) {
			printf("%s has fallen\n\n", name.c_str());
			//	Warrior::~Warrior();
		}
		else {
			printf("%s now has %.2f Health\n\n", name.c_str(), Health);
		}
		std::cout << std::flush;
	}
	
	void PrintWarriorStats() const{
		PrintEntityStats();
		printf("AttackDamage = %.2f, AttackSpeed = %.2f, Defence = %.2f\n"
				,this->AttackDamage, this->AttackSpeed, this->Defense);
	}
};
int Entity::EntityCount = 0;

void Fight(Warrior& W1, Warrior& W2) {
	while (true) {
		W1.TakeDamage(W2.GetDamage());
		if (W1.getHealth() <= 0)break;
		W2.TakeDamage(W1.GetDamage());
		if (W2.getHealth() <= 0)break;
	}
}

void Fight(Warrior* W1, Warrior* W2) {
	while (true) {
		W1->TakeDamage(W2->GetDamage());
		if (W1->getHealth() <= 0)break;
		W2->TakeDamage(W1->GetDamage());
		if (W2->getHealth() <= 0)break;
	}
}



int main(){

	Entity* Ent = new Entity("Ent", 1, 1);
	Ent->PrintEntityStats();
	std::unique_ptr<Warrior> Grunt = std::make_unique<Warrior>("Grunt", 30, 1.5, 1, 10, 0);
	std::unique_ptr<Warrior> Boss = std::make_unique<Warrior>("Big Boss", 150, 1, 1, 25, 15);
	Fight(*Grunt,*Boss);
	delete Ent;
}