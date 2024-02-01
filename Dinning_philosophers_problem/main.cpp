#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <ctime>

const int num_philosophers = 5;
std::vector<std::mutex> forks(num_philosophers);
std::vector<sf::CircleShape> philosophers(num_philosophers);
std::vector<std::thread> threads;

std::mt19937 gen(std::random_device{}());
std::uniform_int_distribution<> action_distribution(1, 3); // 1 - Eat, 2 - Think, 3 - Wait


struct PhilosopherParams {
    int id;
    int timeEat;
    int timeThink;
};

void philosopher(const PhilosopherParams& params) {

    while (true) {
        int action = action_distribution(gen);
        if (action == 1) {
            std::unique_lock<std::mutex> left_fork(forks[params.id]);
            std::unique_lock<std::mutex> right_fork(forks[(params.id + 1) % num_philosophers]);
            // Eating
            philosophers[params.id].setFillColor(sf::Color::Green);
            std::this_thread::sleep_for(std::chrono::seconds(params.timeEat));
            // Thinking
            philosophers[params.id].setFillColor(sf::Color::White);
            left_fork.unlock();
            right_fork.unlock();
        } else if (action == 2) {
            // Thinking
            philosophers[params.id].setFillColor(sf::Color::White);
            std::this_thread::sleep_for(std::chrono::seconds(params.timeThink));
        } else {
            // Waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}

int main() {
    sf::Font font;
    font.loadFromFile("/Users/mariiarybak/Documents/2024/PRO_SO/Dinning_philosophers_problem/Gellisto.ttf");

    sf::Texture texture1;
    texture1.loadFromFile("/Users/mariiarybak/Documents/2024/PRO_SO/Dinning_philosophers_problem/Aristotle.bmp");
    sf::Texture texture2;
    texture2.loadFromFile("/Users/mariiarybak/Documents/2024/PRO_SO/Dinning_philosophers_problem/Socrates.jpg");
    sf::Texture texture3;
    texture3.loadFromFile("/Users/mariiarybak/Documents/2024/PRO_SO/Dinning_philosophers_problem/Plato.bmp");
    sf::Texture texture4;
    texture4.loadFromFile("/Users/mariiarybak/Documents/2024/PRO_SO/Dinning_philosophers_problem/Epicurus.bmp");
    sf::Texture texture5;
    texture5.loadFromFile("/Users/mariiarybak/Documents/2024/PRO_SO/Dinning_philosophers_problem/Pyrrho.bmp");


    sf::RenderWindow window(sf::VideoMode(700, 540), "Dining Philosophers");
    int timeEat, timeThink;
    srand(time(NULL));
    char choice;
    std::string names[5]={"Aristotle", "Socrates", "Plato", "Epicurus", "Pyrrho"};
    std::cout << "Would you like to set eat time and think time? ([Y/y]es/[N/n]o): ";
    std::cin >> choice;
    switch (choice) {
        case 'y':
        case 'Y':
            std::cout << "Enter eat time and think time (in seconds) (1 4): ";
            std::cin >> timeEat >> timeThink;
            break;
        case 'n':
        case 'N':
        default:
            std::cout << "Eat and think time will be set randomly (between 1 and 6 seconds delay).";
            timeEat = rand() % 5 + 1;
            timeThink = timeEat;
            break;
    }


    // Initialize philosophers and forks
    for (int i = 0; i < num_philosophers; ++i) {
        philosophers[i].setRadius(35);
        philosophers[i].setPosition(300 + 150 * std::cos(i * 2 * M_PI / num_philosophers),
                                    200 + 150 * std::sin(i * 2 * M_PI / num_philosophers));
        philosophers[i].setFillColor(sf::Color::White);
        if(i==0)
            philosophers[i].setTexture(&texture1);
        else if(i==1)
            philosophers[i].setTexture(&texture2);
        else if(i==2)
            philosophers[i].setTexture(&texture3);
        else if(i==3)
            philosophers[i].setTexture(&texture4);
        else if(i==4)
            philosophers[i].setTexture(&texture5);
    }

    // Create threads for philosophers
    for (int i = 0; i < num_philosophers; ++i) {
        PhilosopherParams params = {i, timeEat, timeThink};
        threads.emplace_back(philosopher, params);
    }

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        // Draw philosophers
        for (size_t i = 0; i < num_philosophers; ++i) {
            window.draw(philosophers[i]);
            sf::Text text(names[i], font, 18);
            text.setFillColor(sf::Color::White);
            text.setPosition(philosophers[i].getPosition().x, philosophers[i].getPosition().y + 70);
            window.draw(text);
        }

        sf::CircleShape table(60);
       // table.setFillColor(sf::Color(139, 69, 19));
        table.setPosition(280, 180);
        sf::Texture texture;
        texture.loadFromFile("/Users/mariiarybak/Documents/2024/PRO_SO/Dinning_philosophers_problem/table.bmp");
        table.setTexture(&texture);
        window.draw(table);

        window.display();
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}

