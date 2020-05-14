#include "Room.h"

Room::Room() {
	isExit = false;
	upRoom = downRoom = leftRoom = rightRoom = NULL;
	objects.clear();
	index = -1;
}

Room::Room(bool isExit, int index, vector<Object*> v) {
	upRoom = downRoom = leftRoom = rightRoom = NULL;
	this -> isExit = isExit;
	this -> index = index;
	this -> objects = v;
}

/*pop out the specific object, used when the interaction is done*/
bool Room::popObject(Object* obj) {
	bool ok = false;
	vector<Object*> tmp; tmp.clear();
	for (auto i : objects) {
		if (obj == i) {
			ok = true;
			continue;
		}
		tmp.push_back(i);
	}
	this -> setObjects(tmp);

	return ok;
}

void Room::pushObject(Object* obj) {
	(this -> objects).push_back(obj);
}

void Room::setUpRoom(Room* now) {
	this -> upRoom = now;
}

void Room::setDownRoom(Room* now) {
	this -> downRoom = now;
}

void Room::setLeftRoom(Room* now) {
	this -> leftRoom = now;
}

void Room::setRightRoom(Room* now) {
	this -> rightRoom = now;
}

void Room::setIsExit(bool bl) {
	this -> isExit = bl;
}

void Room::setIndex(int index) {
	this -> index = index;
}

void Room::setObjects(vector<Object*> v) {
	this -> objects = v;
}

bool Room::getIsExit() const {
	return this -> isExit;
}

int Room::getIndex() const {
	return this -> index;
}

vector<Object*> Room::getObjects() const {
	return this -> objects;
}

Room* Room::getUpRoom() const {
	return this -> upRoom;
}

Room* Room::getDownRoom() const {
	return this -> downRoom;
}

Room* Room::getLeftRoom() const {
	return this -> leftRoom;
}

Room* Room::getRightRoom() const {
	return this -> rightRoom;
}

void Room::listAttack() const {
	bool ok = false;
	for (auto i : objects) {
		Monster* monptr = dynamic_cast<Monster*>(i);
		NPC* npcptr = dynamic_cast<NPC*>(i);

		if (monptr != NULL) {
			ok = 1;
			cout << *monptr << '\n';
		}
		if (npcptr != NULL) {
			ok = 1;
			cout << *npcptr << '\n';
		}
	}
	if (!ok) cout << "There is no target can attack\n";
}

void Room::listCommunicate() const {
	bool ok = false;
	for (auto i : objects) {
		NPC* npcptr = dynamic_cast<NPC*>(i);

		if (npcptr != NULL) {
			ok = 1;
			cout << *npcptr << '\n';
		}
	}
	if (!ok) cout << "No one can communicate with!\n";
}

ostream& operator<<(ostream& out, const Room& rom) {
	out << "Room Info:\n";
	if (rom.getIsExit()) out << "There is a Exit~~\n";

	vector<Object*> v = rom.getObjects();
	if (v.size())
		for (auto i : v) {
			Monster* monptr = dynamic_cast<Monster*>(i);
			NPC* npcptr = dynamic_cast<NPC*>(i);
			Item* itmptr = dynamic_cast<Item*>(i);

			if (monptr != NULL) out << *monptr << '\n';
			if (npcptr != NULL) out << *npcptr << '\n';
			if (itmptr != NULL) out << *itmptr << '\n';
		}
	else
		out << "Nothing in this room!";

	return out;
}
