#include "NPC.h"

NPC::NPC(): GameCharacter("", "np", 10, 10, 1){
}

NPC::NPC(string nam, string scr, vector<Item> v): GameCharacter(nam, "np", 10, 10, 1) {
	this -> script = scr;
	this -> commodity = v;
}

NPC::NPC(const NPC* now) {
	this -> setName(now -> getName());
    this -> setTag(now -> getTag());
    this -> setMaxHealth(now -> getMaxHealth());
    this -> setCurrentHealth(now -> getCurrentHealth());
    this -> setAttack(now -> getAttack());
    this -> setDefense(now -> getDefense());
    this -> setCommodity(now -> getCommodity());
}

void NPC::listCommodity() {
	cout << getScript() << '\n';
	cout << "Number of Items(s): " << commodity.size() << '\n';
	
	for (auto i : commodity) {
		cout << i.getName() << '\n';
	}
	cout << '\n';
}

bool NPC::triggerEvent(Object* obj) {
	Player* player = dynamic_cast<Player*>(obj);
	if (player == NULL) return false;

	cout << "Enter the item you want(You can only pick one item from each NPC): ";
	string nam; cin >> nam;

	vector<Item> tmp; tmp.clear();
	bool ok = false;
	for (auto i : commodity) {
		if (i.getName() == nam) {
			player -> addItem(i);
			cout << "Pick up " << nam << '\n';
			ok = 1;
			continue;		
		}
		tmp.push_back(i);
	}
	if (ok) {
		this -> setCommodity(tmp);
		return (this -> commodity.size() == 0);
	}

	cout << "None such item\n";
	return false;
}

void NPC::setScript(string src) {
	this -> script = src;
}

void NPC::setCommodity(vector<Item> v) {
	this -> commodity = v;
}

string NPC::getScript() const {
	return this -> script;
}

vector<Item> NPC::getCommodity() const {
	return this -> commodity;
}

// record use only
void NPC::listInfo(ofstream& out) const {
	Object::listInfo(out);
	out << script << '\n' << commodity.size() << '\n';
	for (auto i : commodity) {
		cout << "    ";
		i.listInfo(out);
	}
}

ostream& operator<<(ostream& out, const NPC& npc) {
	
	out << "NPC: " << npc.getName();

	return out;
}

