#include "Dungeon.h"
#include "Player.h"
#include <iostream>
#include <cctype>
#include <ctime>
using namespace std;

Dungeon::Dungeon() {
}

void Dungeon::createPlayer(string& nam) {
	this -> player = Player(nam, 100, 10, 0);
	this -> player.setCurrentRoom(&rooms[0]);
	this -> player.setPreviousRoom(&rooms[0]);
}

vector<Monster*> loadsMonster() {
	vector<Monster*> v;
	v.clear();

	std::ifstream file_in;
	file_in.open("Monsters.txt");
	if (!file_in.good()) {
		cout << "Monster file loads failed\n";
		exit(0);
	}

	string nam;
	int h, a, d;
	while (file_in >> nam) {
		file_in >> h >> a >> d;
		v.push_back(new Monster(nam, h, a, d));
	}
	file_in.close();
	return v;
}

vector<NPC*> loadsNpc() {
	vector<NPC*> v;
	v.clear();

	std::ifstream file_in;
	file_in.open("NPCs.txt");
	if (!file_in.good()) {
		cout << "NPC file loads failed\n";
		exit(0);
	}

	vector<Item> commodity; 
	string nam, scr, itnam;
	int n, h, a, d;
	while (file_in >> nam) {
		file_in.ignore();
		getline(file_in, scr);

		file_in >> n;
		commodity.clear();
		while (n--) {
			file_in >> itnam >> h >> a >> d;
			commodity.push_back(Item(itnam, h, a, d));
		}
		v.push_back(new NPC(nam, scr, commodity));
	}
	file_in.close();
	return v;
}

void Dungeon::createMap() {
	std::ifstream file_in;
	file_in.open("map.txt");
	if (!file_in.good()) {
		cout << "Map file loads failed\n";
		exit(0);
	}
	int maxRoom; file_in >> maxRoom;
	for (int i = 0; i < maxRoom; i++) {
		rooms.push_back(Room());
		rooms[i].setIndex(i + 1);
	}

	int x;
	for (int i = 0; i < maxRoom; i++) {
        file_in >> x; if (x != -1) rooms[i].setUpRoom(&rooms[x]);;  // up
        file_in >> x; if (x != -1) rooms[i].setDownRoom(&rooms[x]); // down
        file_in >> x; if (x != -1) rooms[i].setLeftRoom(&rooms[x]); // left
        file_in >> x; if (x != -1) rooms[i].setRightRoom(&rooms[x]); // right
	}
	file_in.close();

	rooms[maxRoom - 1].pushObject(monsters[0]); // Boss 
	rooms[maxRoom - 1].setIsExit(true);

	int ptrNpc = 0;
	for (int i = 1; i < maxRoom; i += rand() % 3) {
		rooms[i].pushObject(new NPC(npcs[ptrNpc++]));
		
		if (ptrNpc == npcs.size()) ptrNpc = 3;
	}
}

void Dungeon::handleMovement() {
	Room* rm = player.getCurrentRoom();
	Room* pm = player.getPreviousRoom();

	Room* up = (*rm).getUpRoom();
	Room* dw = (*rm).getDownRoom();
	Room* lf = (*rm).getLeftRoom();
	Room* rg = (*rm).getRightRoom();

	if (up != NULL) cout << "U (u) : Go up room\n";
	if (dw != NULL) cout << "D (d) : Go down room\n";
	if (lf != NULL) cout << "L (l) : Go left room\n";
	if (rg != NULL) cout << "R (r) : Go right room\n";

	bool isExit = (*rm).getIsExit(); 
	if (isExit) cout << "F (f) : Finished the game\n";

	string ope; cin >> ope;
	ope[0] = tolower(ope[0]);
	if (ope == "u" && up != NULL) player.setCurrentRoom(up);
	if (ope == "d" && dw != NULL) player.setCurrentRoom(dw);
	if (ope == "l" && lf != NULL) player.setCurrentRoom(lf);
	if (ope == "r" && rg != NULL) player.setCurrentRoom(rg);
	if (ope == "f" && isExit && player.getIsWin()) {
		cout << "Congratulation!, You gratuate from NYCU\n";
		exit(0);
	}
	int r = rand(), tmp = RAND_MAX / 3;
	if (r < tmp && pm != player.getCurrentRoom()) {
		player.getCurrentRoom() -> pushObject(new Monster(monsters[rand() % (monsters.size() - 1) + 1]));
	}
	r = rand(), tmp = RAND_MAX / 5;
	if (r < tmp && pm != player.getCurrentRoom()) {
		player.getCurrentRoom() -> pushObject(new NPC(npcs[rand() % (npcs.size() - 2) + 2]));
	}
}

// void Dungeon::handleEvent(Object* obj) {
// }
// void Dungeon::chooseAction(vector<Object*> v) {
// }

bool Dungeon::handleAttack(string& nam) {
	Room* rom = player.getCurrentRoom();

	for (auto i : (*rom).getObjects()) {
		Monster* monptr = dynamic_cast<Monster*>(i);
		if (monptr == NULL) continue;

		if ((*i).getName() == nam) {
			Object* now = monptr;

			if ((*now).triggerEvent(&player))
				(*rom).popObject(i);
			return true;
		}
	}
	cout << "No such Name\n";
	return false;
}

bool Dungeon::handleCommunicate(string& nam) {
	Room* rom = player.getCurrentRoom();

	for (auto i : (*rom).getObjects()) {
		NPC* npcptr = dynamic_cast<NPC*>(i);
		if (npcptr == NULL) continue;

		if ((*i).getName() == nam) {
			npcptr -> listCommodity();

			if ((*npcptr).triggerEvent(&player))
				(*rom).popObject(i);
			return true;
		}
	}
	cout << "No such Name\n";
	return false;
}


void Dungeon::startGame() {
	srand(time(NULL));

	string nam;
	Record rec;

	// Source
	cout << "Loading source... ";
	this -> monsters = loadsMonster();
	this -> npcs = loadsNpc();
	cout << "Finished\n";
	sleep(1);

	
	cout << "Enter your name: "; cin >> nam;
	if (rec.checkFile(nam)) {
		cout << "\nFound previous records, loading....\n";
		rec.loadFromFile(nam, &player, rooms);
		sleep(1);
	}
	else {
		cout << "\nNo before record(s), creating..... ";
		this -> createMap();
		this -> createPlayer(nam);
		cout << "Finished\n";
		sleep(1);
	}
	this -> runDungeon();
}

bool Dungeon::checkGameLogic() {
	if ((this -> player).checkIsDead()) return false;

	return true;
}

void listOperations() {
	cout << "\nOperations:\n";
	cout << "L (l) : List the item(s) you have\n";
	cout << "M (m) : Move\n";
	cout << "A (a) : Attack\n";
	cout << "C (c) : communiate with someone\n";
	cout << "E (e) : Exit the game and save records\n";
}

bool isOneOfOperation(string& s) {
	if (s.length() != 1) return false;

	if (s == "e") return true;
	if (s == "l") return true;
	if (s == "m") return true;
	if (s == "a") return true;
	if (s == "c") return true;

	return false;
}

void Dungeon::runDungeon() {
	string ope;
	while (checkGameLogic()) {
		cout << "\n\n--------------------------\n\n";

		cout << player << "\n\n";
		cout << *(player.getCurrentRoom()) << '\n';
		
		listOperations();

		cin >> ope;
		ope[0] = tolower(ope[0]);
		while (!isOneOfOperation(ope)) {
			cout << "Invalid input(s), One char only\n";
			cin >> ope;
		}
		// Exit
		if (ope == "e") {
			Record rec;
			rec.saveToFile(&player, rooms);
			break;
		}
		// List
		if (ope == "l") {
			vector<Item> v = player.getInventory();
			for (auto i : v) cout << i << '\n';
			if (v.size() == 0) cout << "You have nothing~\n";
		}
		// Move
		if (ope == "m") this -> handleMovement();
		// Attack
		if (ope == "a") {
			(*(player.getCurrentRoom())).listAttack();
			string s;
			do {
				cout << "Enter the target you want to attack(Enter \"e\" to exit): ";
				cin >> s;
				if (s == "e") break;
			} while (!(this -> handleAttack(s)));
		}
		// Communicate
		if (ope == "c") {
			(*(player.getCurrentRoom())).listCommunicate();
			string s;
			do {
				cout << "Enter the target you want to talk to(Enter \"e\" to exit): ";
				cin >> s;
				if (s == "e") break;
			} while (!(this -> handleCommunicate(s)));
		}
		cout << "Loading...\n";
		sleep(2);
	}
}
