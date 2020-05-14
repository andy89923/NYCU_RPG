
#include "Object.h"

Object::Object() {
	this -> name = "NULL";
	this -> name = "NULL";
}

Object::Object(string nam, string tag) {
	this -> name = nam;
	this -> tag = tag;
}

void Object::setName(string nam) {
	this -> name = nam;
}

void Object::setTag(string tag) {
	this -> tag = tag;
}

string Object::getName() const {
	return this -> name;
}

string Object::getTag() const {
	return this -> tag;
}

void Object::listInfo(ofstream& out) const {
	out << getTag() << ' ' << getName() << '\n';
}

ostream& operator<<(ostream& out, const Object& obj) {
	out << obj.getName();
	return out;
}