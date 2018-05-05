#include <SFML/Graphics.hpp>
#define WINWIDTH 800;
#define WINHEIGHT 600;
#define PSPEED 300;
#define BSPEED 1000;

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

int main(){
	/*Windows*/
	int winWidth = WINWIDTH;
	int winHeight = WINHEIGHT;
	sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "Meteorite");
	window.setFramerateLimit(60);
	/*Player*/
	float speed = PSPEED;
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
	/*Clock*/
	sf::Clock clock;
	float elapsed;
	/*Windows Loop*/
	while (window.isOpen()){
		/*get elapsed time*/
		elapsed = clock.getElapsedTime().asSeconds();
		clock.restart();
		/*Game loop*/
		movement(player, (speed * elapsed));
		bulletShot(player.getPosition().x + 18, player.getPosition().y,bullet, bulletSpeed * elapsed, bulletFlag);
		/*Events Loop*/
		sf::Event event;
		while (window.pollEvent(event))
			closeEvent(event, window);
		/*Window update*/
		window.clear();
		window.draw(player);
		window.draw(bullet);
		window.display();
	}
	return 0;
}