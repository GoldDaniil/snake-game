#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace sf;

const int window_width = 800;
const int window_height = 700;
const float SPEED = 300.0f;//cкорость в пикселях/сек

void handle_input(Text& player, float delta_time) {//обработка ввода
    float move_distance = SPEED * delta_time;
    if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {
        player.move(0, -move_distance);
    }
    if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) {
        player.move(0, move_distance);
    }
    if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) {
        player.move(-move_distance, 0);
    }
    if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) {
        player.move(move_distance, 0);
    }
}

bool is_out_of_bounds(const Text& player) {//чек выхода за границу
    return (player.getPosition().x < 0 ||
            player.getPosition().x + player.getGlobalBounds().width > window_width ||
            player.getPosition().y < 0 ||
            player.getPosition().y + player.getGlobalBounds().height > window_height);
}

void update_player_style(Text& player, float time) {
    int red = static_cast<int>(128 + 127 * sin(time * 2));
    int green = static_cast<int>(128 + 127 * sin(time * 3));
    int blue = static_cast<int>(128 + 127 * sin(time * 4));
    player.setFillColor(Color(red, green, blue));
}

void run_game() {
    RenderWindow window(VideoMode(window_width, window_height), "move the '0'", Style::Titlebar | Style::Close);//создание окна
    window.setFramerateLimit(60);

    Font font;//шрифт
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "error\n";
        return;
    }

    Text player("0", font, 40);//создание 0
    player.setFillColor(Color::Green);
    player.setPosition(window_width / 2.0f, window_height / 2.0f);

    Clock game_сlock;
    Clock frame_сlock;

    Text timer_text("", font, 20);//отобржание времени
    timer_text.setFillColor(Color::White);
    timer_text.setPosition(10, 10);

    RectangleShape border(Vector2f(window_width - 10, window_height - 10));//рамка окна
    border.setPosition(5, 5);
    border.setOutlineThickness(5);
    border.setOutlineColor(Color::White);
    border.setFillColor(Color::Transparent);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        float delta_time = frame_сlock.restart().asSeconds();
        float time_elapsed = game_сlock.getElapsedTime().asSeconds();

        handle_input(player, delta_time);//ввод

        if (is_out_of_bounds(player)) {//границы выход
            cout << "game over! time survived: " << time_elapsed << " seconds\n";
            window.close();
        }

        update_player_style(player, time_elapsed);

        stringstream ss;//обновление таймера
        ss << "time: " << fixed << setprecision(2) << time_elapsed << " sec";
        timer_text.setString(ss.str());

        int border_color = static_cast<int>(128 + 127 * sin(time_elapsed * 2));//обновление рамки - пульсация
        border.setOutlineColor(Color(border_color, border_color, 255));

        window.clear(Color(50, 50, 150));//фон - градиент синих тонов
        window.draw(border);
        window.draw(player);
        window.draw(timer_text);
        window.display();
    }
}

int main() {
    run_game();
    return 0;
}
