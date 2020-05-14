#include "Item.h"

Item::Item(): Object("", "it"){
}

Item::Item(string nam, int h, int a, int d): Object(nam, "it") {
	this -> health = h;
	this -> attack = a;
	this -> defense = d;
}

// record use only
void Item::listInfo(ofstream& out) const {
	Object::listInfo(out);
	out << health << ' ' << attack << ' ' << defense << '\n';
}

// Pick up Item
bool Item::triggerEvent(Object* obj) {
	Player *player = dynamic_cast<Player*>(obj);
    if (player == NULL) return false;

    (*player).addItem(*this);

	return true;
}

int Item::getHealth() const {
	return this -> health;
}

int Item::getAttack() const {
	return this -> attack;
}

int Item::getDefense() const {
	return this -> defense;
}

void Item::setHealth(int h) {
	this -> health = h;
}

void Item::setAttack(int a) {
	this -> attack = a;
}

void Item::setDefense(int d) {
	this -> defense = d;
}

ostream& operator<<(ostream& out, const Item& t) {
	out << "Item: " << t.getName() << " : ";
	out << t.getHealth() << ' ' << t.getAttack() << ' ' << t.getDefense();
	out << "  (health/attack/defense)";
	
	return out;
}