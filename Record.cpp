#include "Record.h"
#include <unistd.h>  // sleep

Record::Record() {
}

void Record::savePlayer(Player* now, ofstream& out) {
	now -> listInfo(out);

	out << now -> getMaxHealth() << ' ' << now -> getCurrentHealth() << ' ';
	out << now -> getAttack() << ' ' << now -> getDefense() << '\n';

	Room* cur = now -> getCurrentRoom();
	Room* prv = now -> getPreviousRoom();

	out << cur -> getIndex() << ' ';
	out << prv -> getIndex() << '\n';

	vector<Item> inventory = (*now).getInventory();
	out << inventory.size() << '\n';
	for (auto i : inventory) {
		i.listInfo(out);
	}
}

void Record::loadPlayer(Player* now, ifstream& in, vector<Room>& rooms) {
	string tag, nam; in >> tag >> nam;
	
	int mh, ch, at, df;
	in >> mh >> ch >> at >> df;

	*now = *(new Player(nam, mh, at, df));
	now -> setCurrentHealth(ch);

	int curRoomid; in >> curRoomid;
	int prvRoomid; in >> prvRoomid;

	now -> setCurrentRoom(&rooms[curRoomid - 1]);
	now -> setPreviousRoom(&rooms[prvRoomid - 1]);

	vector<Item> inventory;
	int t; in >> t;
	while (t--) {
		in >> tag >> nam;
		int h, a, d; in >> h >> a >> d;
		inventory.push_back(Item(nam, h, a, d));
	}
	now -> setInventory(inventory);
}

    
void Record::saveRooms(vector<Room>& v, ofstream& out) {
	out << v.size() << '\n';
	for (auto i : v) {
		out << (i.getIsExit() ? 1 : 0) << '\n';
		
		// connect room
		out << (i.getUpRoom()    == NULL ? -1 : (*(i.getUpRoom())).getIndex()) << ' ';
		out << (i.getDownRoom()  == NULL ? -1 : (*(i.getDownRoom())).getIndex()) << ' ';
		out << (i.getLeftRoom()  == NULL ? -1 : (*(i.getLeftRoom())).getIndex()) << ' ';
		out << (i.getRightRoom() == NULL ? -1 : (*(i.getRightRoom())).getIndex()) << '\n';
	
		vector<Object*> v = i.getObjects();
		out << v.size() << '\n';
		for (auto j : v) {
			// list all the objects in that room
			// Object virtual function
			j -> listInfo(out);
		}
	}
}

void Record::loadRooms(vector<Room>& v, ifstream& in) {
	int n; in >> n;
	for (int i = 0; i < n; i++) {
		v.push_back(Room());
		v[i].setIndex(i + 1);
	}
	for (int i = 0; i < n; i++) {
		int isExit; in >> isExit;
		v[i].setIsExit((isExit == 1 ? true : false));

		int up, dw, lf, rg; in >> up >> dw >> lf >> rg;
		if (up != -1) v[i].setUpRoom(&v[up - 1]);
		if (dw != -1) v[i].setDownRoom(&v[dw - 1]);
		if (lf != -1) v[i].setLeftRoom(&v[lf - 1]);
		if (rg != -1) v[i].setRightRoom(&v[rg - 1]); 
		

		int objsize; in >> objsize;
		vector<Object*> objs; objs.clear();
		while (objsize--) {
			string tag, nam; in >> tag >> nam;

			if (tag == "ms") {
				int cth, mxh, att, def;
				in >> cth >> mxh >> att >> def;
				objs.push_back(new Monster(nam, mxh, att, def));
				
				Monster* mos = dynamic_cast<Monster*>(objs.back());
				(*mos).setCurrentHealth(cth);
			}
			if (tag == "np") {
				in.ignore();
				string scr; getline(in, scr);
				int sz; in >> sz;

				vector<Item> comm; comm.clear();
				for (int j = 1; j <= sz; j++) {
					string itnam, ittag; in >> ittag >> itnam;
					int h, a, d; in >> h >> a >> d;
					comm.push_back(Item(itnam, h, a, d));
				}
				objs.push_back(new NPC(nam, scr, comm));
			}
			if (tag == "it") {
				int h, a, d; in >> h >> a >> d;
				objs.push_back(new Item(nam, h, a, d));
			}
		}
		v[i].setObjects(objs);
	}
}

void Record::loadFromFile(string& nam, Player* now, vector<Room>& v) {
	std::ifstream file_in;
	
	cout << "Map ........ ";
	file_in.open(nam + "_map.txt");
	loadRooms(v, file_in);
	file_in.close();
	sleep(1);

	cout << "Finished\n";


	cout << "User ....... ";

	file_in.open(nam + "_usr.txt");
	loadPlayer(now, file_in, v);
	file_in.close();
	sleep(1);

	cout << "Finished\n";
}

// save record
void Record::saveToFile(Player* now, vector<Room>& v) {
	string nam = (*now).getName();
	std::ofstream out;

	out.open(nam + "_map.txt");
	this -> saveRooms(v, out);
	out.close();

	out.open(nam + "_usr.txt");
	this -> savePlayer(now, out);
	out.close();

	cout << "\nRecord Saved\n";
}

bool Record::checkFile(string nam) {
	std::ifstream file_in;

	string file_name = nam + "_usr.txt";
	file_in.open(file_name);
	if (!file_in.good()) return false;
	file_in.close();

	file_name = nam + "_map.txt";
	file_in.open(file_name);
	if (!file_in.good()) return false;
	file_in.close();
	
	return true;
}