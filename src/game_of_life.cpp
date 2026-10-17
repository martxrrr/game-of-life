#include <array>
#include <iostream>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

void neighbours(int &col, int& row);

int main(){
    constexpr int WIDTH { 800 };
	constexpr int HEIGHT { 800 };

	constexpr int X { 1000 };
	constexpr int Y { 50 };

	constexpr int FPS { 60 };

	sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Conway's GOL", sf::Style::Titlebar | sf::Style::Close);
	window.setPosition(sf::Vector2i({X, Y}));
	window.setFramerateLimit(FPS);

	sf::RectangleShape rectangle;
	rectangle.setSize({20.f, 20.f});
	rectangle.setFillColor(sf::Color::White);
	std::vector<sf::RectangleShape> rects;


    while(window.isOpen()){
        while(const std::optional event = window.pollEvent()){
            if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
				if(keyPressed->scancode == sf::Keyboard::Scancode::Escape){
					window.close();
				}
			}
			if(const auto* mouse_click = event->getIf<sf::Event::MouseButtonPressed>()){
				if(mouse_click->button == sf::Mouse::Button::Left){
					sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
					int click_x = mouse_pos.x;
					int click_y = mouse_pos.y;
					
					int col = click_x / 20;
					int row = click_y / 20;

					float rect_x = col * 20;
					float rect_y = row * 20;

					neighbours(col, row);
					
					rectangle.setPosition({rect_x, rect_y});
					rects.emplace_back(rectangle);
				}
			}
        }
		sf::VertexArray hor(sf::PrimitiveType::Lines, 2);
		sf::VertexArray ver(sf::PrimitiveType::Lines, 2);

        window.clear();

		for(int i = 0; i <= 800; i+=20){
			float inc = i;
			hor[0].position = sf::Vector2f(0.f, inc);
			hor[1].position = sf::Vector2f(800.f, inc);

			window.draw(hor);
		}
		for(int j = 0; j <= 800; j+=20){
			float inc = j;
			ver[0].position = sf::Vector2f(inc, 0.f);
			ver[1].position = sf::Vector2f(inc, 800.f);

			window.draw(ver);
		}
		for(const auto& shape : rects){
			window.draw(shape);
		}

        window.display();
    }
}

void neighbours(int &col, int& row){
	int offsets[8][2] = {
		{ 1,   1}, {  0,  1},
		{-1,   1}, { -1,  0},
		{-1,  -1}, {  0, -1},
		{ 1,  -1}, {  1,  0}
	};
	for(const auto& offset : offsets){
		int targetCol = col + offset[0];
		int targetRow = row + offset[1];

		if(targetCol < 0 || targetCol >= 40){
			continue;
		}
		if(targetRow < 0 || targetRow >= 40){
			continue;
		}

		std::cout << "row : " << targetRow << " col : " << targetCol << std::endl;
	}
}