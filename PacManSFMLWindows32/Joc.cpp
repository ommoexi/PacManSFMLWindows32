#include "Joc.h"
#include "ObiectJoc.h"
#include "Player.h"
#include "Monstru.h"
Joc::Joc() {
	srand(time(0));
	font.loadFromFile("data\\fonts\\arial.ttf");
	meniuPrincipal = new MeniuPrincipal(this);
	meniuPrincipal->activeaza();
	hud = new Hud(this);
	_ecran->setFramerateLimit(frameratePeSecunda);

}
void Joc::adaugaMonstru(Monstru* monstru) {
	obiecte.push_back(monstru);
	monstrii.push_back(monstru);
}

void Joc::creareMonstrii() {
	Monstru* albastru = new Monstru(this, Monstru::TIP_MONSTRU::albastru);
	albastru->setareCoordonate(14, 14);
	Monstru* roz = new Monstru(this, Monstru::TIP_MONSTRU::roz);
	roz->setareCoordonate(14, 12);
	Monstru* galben = new Monstru(this, Monstru::TIP_MONSTRU::galben);
	galben->setareCoordonate(14, 16);
	Monstru* rosu = new Monstru(this, Monstru::TIP_MONSTRU::rosu);
	rosu->setareCoordonate(11, 12);
	adaugaMonstru(albastru);
	adaugaMonstru(roz);
	adaugaMonstru(galben);
	adaugaMonstru(rosu);
	albastru->secundePoarta = 2.f;
	roz->secundePoarta = 4.f;
	galben->secundePoarta = 6.f;

	for (auto& monstru : monstrii) {
		monstru->setareVelocitateNormala(100 + (_nivel - 1) * 10);
	}
}

Joc::~Joc() {
	delete _ecran;
	sfarsit();
	delete meniuPrincipal;
	delete hud;
}
void Joc::init() {
	sf::Clock ceas;
	while (_ecran->isOpen())
	{
		sf::Event event;
		while (_ecran->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				_ecran->close();
		}
		float timpDeLaUltimulFrameTotal = ceas.restart().asSeconds();
		// sa zicem ca jocul da freeze si trece o secunda, daca nu facem acest loop
		// atunci oricare obiect care se misca folosind timpul, se va teleporta fara a verifica coliziuni
		while (timpDeLaUltimulFrameTotal >= 0) {
			if (timpDeLaUltimulFrameTotal <= _milisecundePeFrame) {
				timpDeLaUltimulFrame = timpDeLaUltimulFrameTotal;
				timpDeLaUltimulFrameTotal = -1;
			}
			else {
				timpDeLaUltimulFrameTotal -= _milisecundePeFrame;
				timpDeLaUltimulFrame = _milisecundePeFrame;
				update();
			}
		}
		update();
		desenare();
	}
}

void Joc::nivelNou() {
	_nivel++;
	incepeNivelNou = false;
	start();
}

void Joc::reincarcareEntitati() {
	for (int i = 0; i < obiecte.size();) {
		ObiectJoc* obiect = obiecte[i];
		// folosesc dynamic casting deoarece nu stiu tipul obiectul, dorind sa sterg doar monstrii si player-ul
		Monstru* monstru = dynamic_cast<Monstru*>(obiect);
		Player* player = dynamic_cast<Player*>(obiect);
		// dynamic_cast returneaza null daca convertirea nu e valida , iar delete cu null este ok
		delete monstru;
		delete player;
		if (monstru || player) {
			obiecte.erase(obiecte.begin() + i);
		}
		else {
			i++;
		}
	}
	monstrii.clear();
	player = new Player(this);
	obiecte.push_back(player);
	creareMonstrii();
	eGameOver = false;
}

void Joc::update() {
	if (eGameOver) {
		if (hud->vieti() > 0) {
			reincarcareEntitati();
			hud->decrementareViata();
		}
		else {
			sfarsit();
			meniuPrincipal->activeaza();
		}
	}
	if (meniuPrincipal->esteActiv()) {
		meniuPrincipal->update();
	}
	else if (eFreeze == false) {
		if (incepeNivelNou) {
			nivelNou();
		}
	/*	if (harta->nrDeMancaruri == 0) {
			nivelNou();
		}*/
		for (int i = 0; i < obiecte.size();) {
			ObiectJoc* obiect = obiecte[i];
			if (obiect->eSters == true) {
				delete obiect;
				obiecte.erase(obiecte.begin() + i);
			}
			else {
				obiect->update();
				i++;
			}
		}
	}
	else {
		for (int i = 0; i < obiecte.size();) {
			ObiectJoc* obiect = obiecte[i];
			if (obiect->eSters == false) {
				obiect->updateCandEFreeze();
				i++;
			}
		}
	}

}
void Joc::desenare() {
	_ecran->clear();
	if (meniuPrincipal->esteActiv()) {
		meniuPrincipal->desenare();
	}
	else {
		for (int i = 0; i < obiecte.size(); i++) {
			obiecte[i]->desenare();
		}
		hud->desenare();
	}
	_ecran->display();
}


void Joc::start() {
	for (auto& obiect : obiecte) {
		delete obiect;
	}
	obiecte.clear();
	monstrii.clear();
	eGameOver = false;
	meniuPrincipal->dezactiveaza();
	harta = new Harta(this);
	obiecte.push_back(harta);
	player = new Player(this);
	obiecte.push_back(player);
	creareMonstrii();
}
void Joc::sfarsit() {
	for (auto& obiect : obiecte) {
		delete obiect;
	}
	obiecte.clear();
	monstrii.clear();
	hud->resetare();
	_nivel = 1;
	eGameOver = false;
}