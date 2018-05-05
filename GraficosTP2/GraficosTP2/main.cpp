#include <SFML/Graphics.hpp>

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

int main(){
	/*Windows*/
	int winWidth = 800;
	int winHeight = 600;
	sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "Meteorite");
	window.setFramerateLimit(60);
	/*Player*/
	float speed = 300;
	sf::Texture playerT;
	playerT.loadFromFile("images/player.png");
	sf::Sprite player;
	player.setTexture(playerT);
	player.setPosition(winWidth / 2, winHeight - playerT.getSize().x - 18);
	player.setScale(1.5, 1.5);
	/*Clock*/
	sf::Clock clock;
	float elapsed;
	/*Windows Loop*/
	while (window.isOpen()){
		/*get elapsed time*/
		elapsed = clock.getElapsedTime().asSeconds();
		clock.restart();
		/*Events Loop*/
		sf::Event event;
		while (window.pollEvent(event)){
			closeEvent(event, window);
			movement(player, (speed * elapsed));
		}
		window.clear();
		window.draw(player);
		window.display();
	}
	return 0;
}