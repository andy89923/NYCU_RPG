#include "Monster.h"

Monster::Monster(): GameCharacter("", "ms", 0, 0, 0) {
}

Monster::Monster(string nam, int h, int a, int d): GameCharacter(nam, "ms", h, a, d) {
}
Monster::Monster(const Monster* mon) {
    this -> setName(mon -> getName());
    this -> setTag(mon -> getTag());
    this -> setMaxHealth(mon -> getMaxHealth());
    this -> setCurrentHealth(mon -> getCurrentHealth());
    this -> setAttack(mon -> getAttack());
    this -> setDefense(mon -> getDefense());
}

// record use
void Monster::listInfo(ofstream& out) const {
    Object::listInfo(out);
    out << getCurrentHealth() << ' ' << getMaxHealth() << '\n';
    out << getAttack() << ' ' << getDefense() << '\n';
}

// debug use
void Monster::showInfo() const {
    cout << this -> getName() << '\n';

    int hp =  this -> getCurrentHealth();
    cout << "HP: " << hp << ", ";
    // Attack
    int at = this -> getAttack();
    cout << "Attack: " << at << ", ";
    // Defense
    int df = this -> getDefense();
    cout << "Defense: " << df << '\n';
}

// Combat System
bool Monster::triggerEvent(Object* obj) {
    Player *player = dynamic_cast<Player*>(obj);
    if (player == NULL) return false;

    while (1) {
        cout << "\nAttacking " << *this << '\n';
        cout << "C (c) : confirm attack\n";
        cout << "R (r) : retreat\n";

        // c : confirm attack, r : retreat
        string c; cin >> c;
        c[0] = tolower(c[0]);

        if (c != "r" and c != "c") {
            cout << "Invalid Input\n";
            continue;
        }
        if (c == "r") {
            cout << "You run away, what a loser?!\n";
            break;
        }
        if (c == "c") {
            int dg = this -> takeDamage((*player).getAttack());
            cout << "You attack the monster, take " << dg << " damage\n";
            
            if (this -> checkIsDead()) { // monster dead
                cout << "Monster is dead, you got the buff\n";

                if (this -> getName() == "Calculus") {
                    player -> setIsWin(true);
                }
                // Buff rand
                int hp = this -> getAttack() * 0.5;
                int at = this -> getDefense() * 0.8; 
                int df = this -> getAttack() * 0.8;

                Item buff(this -> getName() + "_Buff", hp, at, df);
                cout << buff << '\n';

                buff.triggerEvent(player);
                return true;
            }

            dg = player -> takeDamage(this -> getAttack());
            cout << "Monster attack you, cause " << dg << " damage\n";

            if ((*player).checkIsDead()) { // player dead
                cout << "You are dead~~\n Game Over!\n";
                sleep(10);
                break;
            }

            int r = max(3, rand() % 10);
            cout << "Monster learn your weakness, attack enhance " << r << "\n\n";
            this -> setAttack(this -> getAttack() + r);

            sleep(2);

            cout << *player << '\n';
        }
    }
	return false;
}


ostream& operator<<(ostream& out, const Monster& mon) {
	out << "Monster: " << mon.getName() << '\n';

    int t = rand() % 5;

	// HP
    int hp = mon.getCurrentHealth();
    out << "    HP: " << hp - t << " ~ " << hp - t + 5 << ", ";

    // Attack
    int at = mon.getAttack();
    t = rand() % 5;
    out << "Attack: " <<  at - t << " ~ " << at - t + 5 << ", ";


    // Defense
    int df = mon.getDefense();
    t = rand() % 5;
    out << "Defense: " << df - t << " ~ " << df - t + 5;

	return out;
}