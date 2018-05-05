#include <SFML/Graphics.hpp>

int main(){
	/*Windows*/
	int winWidth = 800;
	int winHeight = 600;
	sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "Meteorite");
	window.setFramerateLimit(60);
	/*Windows Loop*/
	while (window.isOpen()){
		sf::Event event;
		/*Events Loop*/
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.display();
	}
	return 0;
}