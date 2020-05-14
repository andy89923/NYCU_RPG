#include "Player.h"

#include <iostream>
using namespace std;

Player::Player() {
}

Player::Player(string nam, int hp, int at, int de): GameCharacter(nam, "py", hp, at, de) {
	this -> currentRoom = NULL;
	this -> previousRoom = NULL;
	this -> isWin = false;
}

void Player::addItem(Item it) {
	this -> increaseStates(it.getHealth(), it.getAttack(), it.getDefense());
	this -> inventory.push_back(it);
}

void Player::increaseStates(int h, int a, int d) {
	this -> setCurrentHealth(min(getCurrentHealth() + h, getMaxHealth()));
	this -> setAttack(getAttack() + a);
	this -> setDefense(getDefense() + d);
}

void Player::changeRoom(Room* now) {
	this -> previousRoom = this -> currentRoom;
	this -> currentRoom = now;
}

bool Player::triggerEvent(Object* obj) {
	return false;
}

void Player::setPreviousRoom(Room* rom) {
	this -> previousRoom = rom;
}

void Player::setCurrentRoom(Room* now) {
	this -> setPreviousRoom(this -> currentRoom);
	this -> currentRoom = now;
}

void Player::setInventory(vector<Item> v) {
	for (auto i : v) {
		i.triggerEvent(this);
	}
	this -> inventory = v;
}

Room* Player::getCurrentRoom() const {
	return this -> currentRoom;
}

Room* Player::getPreviousRoom() const {
	return this -> previousRoom;
}

vector<Item> Player::getInventory() const {
	return this -> inventory;
}

void Player::setIsWin(bool b) {
	this -> isWin = b;
}

bool Player::getIsWin() const {
	return this -> isWin;
}

ostream& operator<<(ostream& out, const Player& p) {

    cout << p.getName() << "'s Status:\n";
    // HP
    out << "HP: " << p.getCurrentHealth() << '/';
    out << p.getMaxHealth() << '\n';
    // Attack
    out << "Attack: " << p.getAttack() << '\n';
    // Defense  
    out << "Defense: " << p.getDefense();

    return out;
}
