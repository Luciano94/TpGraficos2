#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <ctime>
#ifdef _DEBUG
	#include "vld.h"
#endif // _DEBUG

#define WINWIDTH 800;
#define WINHEIGHT 600;
#define PSPEED 300;
#define BSPEED 1000;
#define LIFES 3;

void movement(sf::Sprite &player, float speed) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		player.move(-speed, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		player.move(speed, 0);
}

void closeEvent(sf::Event &event, sf::Window &window) {
	if (event.type == sf::Event::Closed ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
}

void asteroidUpdate(sf::Sprite asteroids[], float winHeight, sf::Texture asteroidT, float &aSpeed, float elapsed) {
	for (int i = 0; i < 10; i++) {
		aSpeed = (rand() % 500);
		if (asteroids[i].getPosition().y >(winHeight + asteroidT.getSize().y)) {
			asteroids[i].setPosition(rand() % 600, 0);
		}
		else asteroids[i].move(0, aSpeed * elapsed);
	}
}

void bulletShot(float playerX, float playerY, sf::Sprite &bullet, float bulletSpeed, bool &bulletFlag, sf::Sound &sound) {
	if (!bulletFlag) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			bullet.setPosition(playerX, playerY);
			sound.play();
			bulletFlag = true;
		}
	}
	else {
		bullet.move(0, -bulletSpeed);
		if (bullet.getPosition().y < 0) {
			bullet.setPosition(-8, -8);
			bulletFlag = false;
		}
	}
}

void ColPlayerAsteroid(sf::Sprite &player, int &plifes, sf::Sprite asteroids[], sf::Sound &sound) {
	sf::Texture asteroidT = *asteroids[0].getTexture();
	for (int i = 0; i < 10; i++) {
		if (player.getPosition().x >= asteroids[i].getPosition().x
			&& player.getPosition().x <= (asteroids[i].getPosition().x + asteroidT.getSize().x)
			&& player.getPosition().y <= (asteroids[i].getPosition().y + asteroidT.getSize().y)) {
			sound.play();
			asteroids[i].setPosition(rand() % 600, 0);
			plifes--;
		}
	}
}

void colBulletAsteroid(sf::Sprite &bullet, sf::Sprite asteroids[], int &score, sf::Sound &sound) {
	sf::Texture asteroidT = *asteroids[0].getTexture();
	for (int i = 0; i < 10; i++) {
		if (bullet.getPosition().x >= asteroids[i].getPosition().x
			&& bullet.getPosition().x <= (asteroids[i].getPosition().x + asteroidT.getSize().x)
			&& bullet.getPosition().y <= (asteroids[i].getPosition().y + asteroidT.getSize().y)) {

			sound.play();
			asteroids[i].setPosition(rand() % 600, 0);
			bullet.setPosition(-8, -8);
			score += 100;
		}
	}
}

void gameManager(int &plifes, int &score, bool &win, bool &lose) {
	if (plifes == 0)
		lose = true;
	if (score == 10000)
		win = true;
}

int main(){
	/*Windows*/
	int winWidth = WINWIDTH;
	int winHeight = WINHEIGHT;
	sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "Meteorite");
	window.setFramerateLimit(60);
	/*Player*/
	float speed = PSPEED;
	int pLifes = LIFES;
	int score = 0;
	sf::Texture playerT;
	playerT.loadFromFile("images/player.png");
	sf::Sprite player;
	player.setTexture(playerT);
	player.setPosition(winWidth / 2, winHeight - playerT.getSize().x - (playerT.getSize().x / 2));
	player.setScale(1.5, 1.5);
	/*Bullet*/
	float bulletSpeed = BSPEED;
	sf::Texture bulletT;
	bulletT.loadFromFile("images/bullet.png");
	sf::Sprite bullet;
	bullet.setTexture(bulletT);
	bullet.setPosition(-8,-8);
	bool bulletFlag = false;
	/*Random setup*/
	srand(time(0));
	/*Asteroids*/
	float aSpeed = (rand() % 500);
	sf::Texture asteroidT;
	asteroidT.loadFromFile("images/asteroid.png");
	sf::Sprite asteroids[10];
	for (int i = 0; i < 10; i++)
	{
		asteroids[i].setTexture(asteroidT);
		asteroids[i].setPosition(rand() % winHeight, 0);
	}
	/*UI*/
	/*font*/
	sf::Font font;
	font.loadFromFile("fonts/OpenSans.ttf");
	/*lifes*/
	sf::Text lifes;
	std::string life = "Lifes: " + std::to_string(pLifes);
	lifes.setFont(font);
	lifes.setString(life);
	lifes.setCharacterSize(24);
	lifes.setFillColor(sf::Color::Red);
	lifes.setStyle(sf::Text::Bold);
	lifes.setPosition(0, 0);
	/*Points*/
	sf::Text points;
	std::string point = "Points: " + std::to_string(score);
	points.setFont(font);
	points.setString(point);
	points.setCharacterSize(24);
	points.setFillColor(sf::Color::Red);
	points.setStyle(sf::Text::Bold);
	points.setPosition(600, 0);
	/*GameManager*/
	bool win = false;
	bool lose = false;
	/*Clock*/
	sf::Clock clock;
	float elapsed;
	/*sounds*/
	/*shot*/
	sf::SoundBuffer shotbuffer;
	shotbuffer.loadFromFile("sounds/playershot.wav");
	sf::Sound shotsound;
	shotsound.setBuffer(shotbuffer);
	/*asteroids*/
	sf::SoundBuffer astebuffer;
	astebuffer.loadFromFile("sounds/enemyDeath.wav");
	sf::Sound astesound;
	astesound.setBuffer(astebuffer);
	/*player*/
	sf::SoundBuffer plabuffer;
	plabuffer.loadFromFile("sounds/playerDeath.wav");
	sf::Sound plasound;
	plasound.setBuffer(plabuffer);
	/*Windows Loop*/
	while (window.isOpen()){
		/*get elapsed time*/
		elapsed = clock.getElapsedTime().asSeconds();
		clock.restart();
		/*UI update*/
		life = "Lifes: " + std::to_string(pLifes);
		lifes.setString(life);
		point = "Points: " + std::to_string(score);
		points.setString(point);
		/*Collisions*/
		ColPlayerAsteroid(player, pLifes, asteroids, plasound);
		colBulletAsteroid(bullet, asteroids, score, astesound);
		/*Game loop*/
		movement(player, (speed * elapsed));
		bulletShot(player.getPosition().x + 18, player.getPosition().y,bullet, bulletSpeed * elapsed, bulletFlag, shotsound);		
		asteroidUpdate(asteroids, winHeight, asteroidT, aSpeed, elapsed);
		gameManager(pLifes, score, win, lose);
		/*Events Loop*/
		sf::Event event;
		while (window.pollEvent(event))
			closeEvent(event, window);
		/*Window update*/
		window.clear();
		if (!win && !lose) {
			for (int i = 0; i < 10; i++)
				window.draw(asteroids[i]);
			window.draw(player);
			window.draw(bullet);
			window.draw(lifes);
			window.draw(points);
		}
		else {
			if (win) {
				lifes.setCharacterSize(68);
				lifes.setPosition(250, 300);
				lifes.setString("YOU WIN!!!");
				window.clear();
				window.draw(lifes);
				/*Events Loop*/
				while (window.pollEvent(event))
					closeEvent(event, window);
			}
			else {
				lifes.setCharacterSize(68);
				lifes.setPosition(250, 300);
				lifes.setString("YOU LOSE");
				window.clear();
				window.draw(lifes);				
				/*Events Loop*/
				while (window.pollEvent(event))
					closeEvent(event, window);
			}
		}
		window.display();
	}
	return 0;
}