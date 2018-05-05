#include <SFML/Graphics.hpp>
#include <ctime>
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

void bulletShot(float playerX, float playerY, sf::Sprite &bullet, float bulletSpeed, bool &bulletFlag) {
	if (!bulletFlag) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			bullet.setPosition(playerX, playerY);
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

void asteroidUpdate(sf::Sprite asteroids[], float winHeight, sf::Texture asteroidT, float &aSpeed, float elapsed) {
	for (int i = 0; i < 10; i++) {
		aSpeed = (rand() % 500);
		if (asteroids[i].getPosition().y >(winHeight + asteroidT.getSize().y)) {
			asteroids[i].setPosition(rand() % 600, 0);
		}
		else asteroids[i].move(0, aSpeed * elapsed);
	}
}

void ColPlayerAsteroid(sf::Sprite &player, int &plifes, sf::Sprite asteroids[]) {
	sf::Texture asteroidT = *asteroids[0].getTexture();
	sf::Texture playerT = *player.getTexture();
	for (int i = 0; i < 10; i++) {
		if ((player.getPosition().x <= asteroids[i].getPosition().x)
			&& (player.getPosition().x + playerT.getSize().x >= asteroids[i].getPosition().x)
			&& player.getPosition().y <= (asteroids[i].getPosition().y + asteroidT.getSize().y)) {
			asteroids[i].setPosition(rand() % 600, 0);
			plifes--;
		}
	}
}

void colBulletAsteroid(sf::Sprite &bullet, sf::Sprite asteroids[], int &score) {
	sf::Texture asteroidT = *asteroids[0].getTexture();
	for (int i = 0; i < 10; i++) {
		if (bullet.getPosition().x >= asteroids[i].getPosition().x
			&& bullet.getPosition().x <= (asteroids[i].getPosition().x + asteroidT.getSize().x)
			&& bullet.getPosition().y <= (asteroids[i].getPosition().y + asteroidT.getSize().y)) {

			asteroids[i].setPosition(rand() % 600, 0);
			bullet.setPosition(-8, -8);
			score += 100;
		}
	}
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
	/*Clock*/
	sf::Clock clock;
	float elapsed;
	/*Windows Loop*/
	while (window.isOpen()){
		/*get elapsed time*/
		elapsed = clock.getElapsedTime().asSeconds();
		clock.restart();
		/*Collisions*/
		ColPlayerAsteroid(player, pLifes, asteroids);
		colBulletAsteroid(bullet, asteroids, score);
		/*Game loop*/
		movement(player, (speed * elapsed));
		bulletShot(player.getPosition().x + 18, player.getPosition().y,bullet, bulletSpeed * elapsed, bulletFlag);
		asteroidUpdate(asteroids, winHeight, asteroidT, aSpeed, elapsed);
		/*Events Loop*/
		sf::Event event;
		while (window.pollEvent(event))
			closeEvent(event, window);
		/*Window update*/
		window.clear();
		for (int i = 0; i < 10; i++)
			window.draw(asteroids[i]);
		window.draw(player);
		window.draw(bullet);
		window.display();
	}
	return 0;
}