#include "GameCharacter.h"

GameCharacter::GameCharacter() {

}

GameCharacter::GameCharacter(string name, string tag, int h, int a, int d): Object(name, tag) {
	this -> name = name;

	this -> maxHealth = h;
	this -> currentHealth = h;
	this -> attack = a;
	this -> defense = d;
}

bool GameCharacter::checkIsDead() {
	return (this -> currentHealth <= 0);
}

int GameCharacter::takeDamage(int t) {
	float def = rand() / RAND_MAX;
	int damageCause = min(max(rand() % 5, t - getDefense() + (int)(def * getDefense() * 0.2)), getCurrentHealth());
	this -> currentHealth -= damageCause;
	return damageCause;
}

void GameCharacter::setMaxHealth(int h) {
	this -> maxHealth = h;
}

void GameCharacter::setCurrentHealth(int h) {
	this -> currentHealth = min(h, this -> maxHealth);
}

void GameCharacter::setAttack(int a) {
	this -> attack = a;
}

void GameCharacter::setDefense(int d) {
	this -> defense = d;
}

int GameCharacter::getMaxHealth() const {
	return this -> maxHealth;
}

int GameCharacter::getCurrentHealth() const {
	return this -> currentHealth;
}

int GameCharacter::getAttack() const {
	return this -> attack;
}

int GameCharacter::getDefense() const {
	return this -> defense;
}