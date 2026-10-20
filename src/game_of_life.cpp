#include <iostream>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

constexpr int WIDTH { 800 };
constexpr int HEIGHT { 800 };
constexpr int X { 1000 };
constexpr int Y { 50 };
constexpr int FPS { 60 };
constexpr int sq_len { 20 };
constexpr int COLS = WIDTH / sq_len;
constexpr int elements = (HEIGHT / sq_len) * (WIDTH / sq_len);

inline int sq_id(const int &row, const int &col);
inline int sq_row(const int& id);
inline int sq_col(const int& id);

void update_cell_status(const int &row, const int&col, std::vector<int>&vec);

void render(sf::RenderWindow &window, 
			const std::vector<int>& status_vec, 
			const std::vector<sf::RectangleShape> &rects);

void print_curr_gen(const std::vector<std::vector<int>>& curr_gen_vec);

void update_current_gen(std::vector<std::vector<int>>& curr_gen_vec,
						const std::vector<int>&status_vec);


void neighbours(const std::vector<std::vector<int>>&curr_gen_vec,
				const std::vector<int>&status_vec);

bool check_status(const int& id, const std::vector<int>&status_vec);

int main(){
    
	sf::RenderWindow window(  sf::VideoMode({WIDTH, HEIGHT}), 
							  "Conway's GOL", 
						 	  sf::Style::Titlebar | sf::Style::Close
							);

	window.setPosition(sf::Vector2i({X, Y}));
	window.setFramerateLimit(FPS);

	sf::RectangleShape rectangle;
	rectangle.setSize({20.f, 20.f});
	rectangle.setFillColor(sf::Color::White);

	std::vector<sf::RectangleShape> rects;
	std::vector<std::vector<int>> curr_gen_vec(0, std::vector<int>(2));
	std::vector<int> status_vec(elements, 0);

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
					
					int col = click_x / sq_len;
					int row = click_y / sq_len;

					float rect_x = col * sq_len;
					float rect_y = row * sq_len;

					update_cell_status(row, col, status_vec);
					
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
		render(window, status_vec, rects);
        window.display();
    }
	update_current_gen(curr_gen_vec, status_vec);
	neighbours(curr_gen_vec, status_vec);
	return 0;
}

void neighbours(const std::vector<std::vector<int>>&curr_gen_vec,
				const std::vector<int>&status_vec){
	int offsets[8][2] = {
		{ 1,   1}, {  0,  1},
		{-1,   1}, { -1,  0},
		{-1,  -1}, {  0, -1},
		{ 1,  -1}, {  1,  0}
	};

	for(const auto& sq : curr_gen_vec){
		int sqID = sq[0];
		int row = sq_row(sqID);
		int col = sq_col(sqID);
		std::cout << "SQ_ID : " << sqID << " => " << (sq[1] == 1 ? "live" : "dead") << std::endl;
		for(const auto& offset : offsets){
			int targetCol = col + offset[0];
			int targetRow = row + offset[1];

			if(targetCol < 0 || targetCol >= 40){
				continue;
			}
			if(targetRow < 0 || targetRow >= 40){
				continue;
			}
			int neighbour_id = sq_id(targetRow, targetCol);
			std::cout << (check_status(neighbour_id, status_vec) ? "live" : "dead") << std::endl;
		}
	}
}

inline int sq_id(const int &row, const int &col){
	return row * COLS + col;
}
inline int sq_row(const int& id){
	return (id / COLS);
}
inline int sq_col(const int& id){
	return (id % COLS);
}
void update_cell_status(const int &row, const int&col, std::vector<int>&vec){
	int id = sq_id(row, col);
	switch (vec[id]){;
		case 0:
			vec[id] = 1;
			break;
	}
}

bool check_status(const int& id, const std::vector<int>&status_vec){
	return (status_vec[id] == 1 ? true : false);
}

void update_current_gen(std::vector<std::vector<int>>& curr_gen_vec,
						const std::vector<int>&status_vec){
	int id;
	int status;
	for(int i = 0; i < 40; i++){
		for(int j = 0; j < 40; j++){
			id = sq_id(i, j);
			status = (check_status(id, status_vec) ? 1 : 0);
			curr_gen_vec.emplace_back(std::vector<int>{id, status});
		}
	}
}

void render(sf::RenderWindow &window, 
			const std::vector<int>& status_vec, 
			const std::vector<sf::RectangleShape> &rects){

	for(const auto& status : status_vec){
		if(status == 1){
			for(const auto& shape : rects){
				window.draw(shape);
			}
		}else if(status == 0){
			continue;
		}
	}
}

void print_curr_gen(const std::vector<std::vector<int>>& curr_gen_vec){
	for(const auto& el : curr_gen_vec){
		std::cout << "SQ_ID : " << el[0] << " => " << (el[1] == 1 ? "live" : "dead") << std::endl;
	}
}