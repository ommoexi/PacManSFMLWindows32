#include "Mancare.h"
#include "Joc.h"

Mancare::Mancare(Joc* joc) : ObiectJoc(joc, new sf::RectangleShape({1,1})) {

}

void Mancare::coliziune(Player& player) {
	eSters = true;
}