#pragma once
#include <SFML/Graphics.hpp>
enum class TIPURI_OBIECTE {
	nimic= -1,
	perete = 0,
	usa = 1,
};
class Joc;
class Player;
class Monstru;
class ObiectJoc {
private:
	sf::Shape* _forma; // pointer unic , fiecare obiect in parte are propria lui forma
protected:
	Joc* _joc;
public:
	TIPURI_OBIECTE tipObiect = TIPURI_OBIECTE::nimic;
	bool eSters = false;
	virtual ~ObiectJoc() {
		delete _forma;
	}

	ObiectJoc(Joc* joc, sf::Shape* forma);

	sf::Shape& forma() {
		return *_forma;
	}

	virtual void desenare();
	virtual void update() {}
	virtual void updateCandEFreeze() {};

	virtual void coliziune(Player& player){}
	virtual void coliziune(Monstru& monstru){}
};

