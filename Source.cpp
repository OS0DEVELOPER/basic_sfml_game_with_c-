#include <SFML/Graphics.hpp>
#include <iostream>
#include "Header.h"
#include "GameMusic.h"
using namespace std;

// Main Components of the Game
sf::Clock gameClock;
sf::Clock deltaTimeClock, bulletclock;;
const float GRAVITY = 800.0f;
const float playerSpeed = 200.0f;
const float jumpSpeed = -400.0f;
bool isOnGround = false;
bool isCollision = false;
bool isIdel = false;
bool isWalking = false;
sf::Vector2f playerPos;
sf::Vector2f enemyPos;
sf::FloatRect playerBounds;
sf::FloatRect enemyBounds;
float VelocityY = 0.0f;
const int totalGroundTiles = 25;
float Bulletspeed = 300.0f;
sf::Sprite bullet;
sf::Texture bulletImage;
std::vector<sf::Sprite> bullets;
float Velocity = 0.0f;
bool isBulletRelease = false;
float enemyVelocity = 0.0f;
float enemySpeed = 5;
bool isShoot = false;
int xStep = 1;


//Audio 
GameMusic gamesound;
//Audio

enum class Screen {
    MainMenu,
    Game
};

// Resources
sf::Texture backgroundImage, playImage, exitImage, continueImage, playerImage,GroundImage,getHome,enemyImage,skyImage;
sf::Sprite background, playButton, exitButton, continueButton, player,home,enemy,sky;
sf::Sprite ground[5][10];;
sf::IntRect rectSprite(0, 0, 20, 36);  // Player SpriteSheet
sf::Font text;
sf::Text comments;
//sf::IntRect idelSprite(0,0,20,36);
// Function to load resources
bool loadResources() {
    if (!backgroundImage.loadFromFile("Asserts/background.png") ||
        !getHome.loadFromFile("Asserts/getHomehouse.png") ||
        !bulletImage.loadFromFile("Asserts/bullet.png") ||
        !playImage.loadFromFile("Asserts/playButton.png") ||
        !continueImage.loadFromFile("Asserts/continueButton.png") ||
        !exitImage.loadFromFile("Asserts/exitButton.png") ||
        !GroundImage.loadFromFile("Asserts/groundImage.png") ||
        !enemyImage.loadFromFile("Asserts/SunMonster.png") ||
        !skyImage.loadFromFile("Asserts/gameSky.png") ||
        !playerImage.loadFromFile("Asserts/player_spritesheet.png")) {
        cout << "Error: Failed to load one or more assets!" << endl;
        return false;
    }
    if (!text.loadFromFile("F:/Fonts/BebasNeue-Regular.ttf")) {
        return false;
    }
    

    // Set up sprites
    background.setTexture(backgroundImage);
    playButton.setTexture(playImage);
    playButton.setPosition(500, 100);

    continueButton.setTexture(continueImage);
    continueButton.setPosition(500, 300);

    exitButton.setTexture(exitImage);
    exitButton.setPosition(500, 500);

    home.setTexture(getHome);
    home.setPosition(800,125);

    player.setTexture(playerImage);
    player.setTextureRect(rectSprite);
    player.setPosition(500, 500);

    enemy.setTexture(enemyImage);
    enemy.setPosition(600,200);

    sky.setTexture(skyImage);
    sky.setOrigin(400,500);
    comments.setFont(text);


    return true;
}
// Render UI (Main Menu)
void renderUI(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(playButton);
    window.draw(continueButton);
    window.draw(exitButton);

}
void LoadMap(sf::RenderWindow& window) {
    window.draw(sky);
    for (int row = 0; row < 5; ++row) {       // 5 layers
        for (int col = 0; col < 10; ++col) {  // 10 tiles per layer
            ground[row][col].setPosition(137 * col, 650 - row * 100); // Adjust y-position for layers
            ground[row][col].setTexture(GroundImage);
            window.draw(ground[row][col]);
        }
    }
    window.draw(home);
    
}
// Render Game Screen
void renderGameScreen(sf::RenderWindow& window) {
    
    float deltaTime = deltaTimeClock.restart().asSeconds();
    float deltatimebullet = bulletclock.getElapsedTime().asSeconds();
    if (!isOnGround) {
        VelocityY += GRAVITY * deltaTime;
        enemyVelocity += GRAVITY * deltaTime;
    }
    player.move(0, VelocityY * deltaTime);
    if (enemy.getPosition().x >= 1000) {
        xStep = -xStep; // Reverse direction (move left)
    }
    if (enemy.getPosition().x <= 0) {
        xStep = -xStep;  // Reverse direction (move right)
    }
    enemy.move(xStep, enemyVelocity * deltaTime);
    //Window boundry checking 
    // Restrict player within the window boundaries
    playerPos = player.getPosition();
    playerBounds = player.getGlobalBounds();

    enemyPos = enemy.getPosition();
    enemyBounds = enemy.getGlobalBounds();
    // Left boundary
    if (playerPos.x < 0) {
        player.setPosition(0, playerPos.y);
        
    }
    if (enemyPos.x < 0) {
        enemy.setPosition(0, enemyPos.y);
    }

    // Right boundary
    if (playerPos.x + playerBounds.width > 1080) {
        player.setPosition(1080 - playerBounds.width, playerPos.y);
       
    }
    if (enemyPos.x + enemyBounds.width > 1080 && !isShoot) {

        enemy.setPosition(1080 - enemyBounds.width, enemyPos.y);// 
    }
    // Top boundary (if needed, e.g., to restrict jumping too high)
    if (playerPos.y < 0) {
        player.setPosition(playerPos.x, 0);
    }

    // Bottom boundary (if needed, though the ground collision might already handle this)
    if (playerPos.y + playerBounds.height > 720) {
        player.setPosition(playerPos.x, 720 - playerBounds.height);
    }

    if (player.getGlobalBounds().intersects(home.getGlobalBounds())) {
        comments.setString("You Have Reach Home");
        comments.setFillColor(sf::Color::Green);
        comments.setPosition(window.getSize().x / 2,100);
        comments.setStyle(sf::Text::Bold);
    }
    // End of Boundry checking 

    //start of Ground collision checking
    isCollision = false;
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 10; ++col) {
            if (player.getGlobalBounds().intersects(ground[row][col].getGlobalBounds())) {
                isCollision = true;
                VelocityY = 0.0f;
                player.setPosition(player.getPosition().x, ground[row][col].getPosition().y - player.getGlobalBounds().height);
                break;
            }
            if (enemy.getGlobalBounds().intersects(ground[row][col].getGlobalBounds())) {
                enemyVelocity = 0.0f;
                enemy.setPosition(enemy.getPosition().x, ground[row][col].getPosition().y - enemy.getGlobalBounds().height);
            }
        }
        if (isCollision) break; // Exit the outer loop if collision is detected
    }
    isOnGround = isCollision;
    // end of checking of collision
    if (playerBounds.intersects(enemyBounds)) {
        comments.setFillColor(sf::Color::Red);
        comments.setPosition(window.getSize().x / 2, 100);
        comments.setString("Game Over!");
        comments.setStyle(sf::Text::Bold);
    }
    for (const auto& bullet : bullets) {
        if (bullet.getGlobalBounds().intersects(enemyBounds)) {
            enemy.move(enemySpeed, 0);
            isShoot = true;
            enemy.setPosition(2000,0);    
        }   
    }
    if (isWalking) {
        // then play walking animation
        if (gameClock.getElapsedTime().asSeconds() > 0.1f) {
            // Update sprite animation
            if (rectSprite.left >= 200 - 22) {
                rectSprite.left = 0;
            }
            else {
                rectSprite.left += 21;
            }
            gameClock.restart();
            player.setTextureRect(rectSprite);
            
        }
    }
    if (isIdel) {
        isWalking = false;
    }
    if (isBulletRelease) {
        bullet.setPosition(player.getPosition().x, player.getPosition().y);
        bullet.setTexture(bulletImage);
        bullets.push_back(bullet);
    }
        Velocity = Bulletspeed * deltatimebullet;
        for (auto it = bullets.begin(); it != bullets.end(); /* no increment here */) {
            it->move(Velocity, 0); // Move the bullet to the right
            bulletclock.restart();
            if (it->getPosition().x > 1010) {
                it = bullets.erase(it); // Remove the bullet and get the next valid iterator
                std::cout << "Bullet Erased";
            }
            else {       
                ++it; // Only increment if we didn't erase the element
            }

        }
    for (const auto& bullet : bullets) {
        
        window.draw(bullet);
    }
    window.draw(player); 
    window.draw(comments);
    window.draw(enemy);
}
// Main Function
int main() {
    sf::RenderWindow window(sf::VideoMode(1080, 720), "GetHome");
    window.setFramerateLimit(60);
    Practice obj;
    // Set the window icon
    sf::Image icon;
    if (!icon.loadFromFile("Asserts/logo.png")) {
        cout << "Error: Failed to load icon!" << endl;
        return -1;

    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load resources
    if (!loadResources()) {
        return -1;
    }
    
    gamesound.setVolume(50);
    Screen currentScreen = Screen::MainMenu;
// Audio Scean Change 
    bool isMainScean = true;
    bool isGameScean = false;

// Audio Scean change
    // Game Loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                gamesound.stopMusic();
            }
            
            // Handle input based on the current screen
            if (currentScreen == Screen::MainMenu) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    currentScreen = Screen::Game;
                    gamesound.playMusic();
                }
            }
            else if (currentScreen == Screen::Game) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    gamesound.stopMusic();
                    currentScreen = Screen::MainMenu;
                }
                
            }
            float deltaTime = deltaTimeClock.restart().asSeconds();
            // Handle continuous movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                player.move(playerSpeed * deltaTime, 0); // Move right
                isWalking = true;
            }
            else {          
                isIdel = true;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                isWalking = true;
                player.move(-playerSpeed * deltaTime, 0); // Move left
            }else {
                isIdel = true;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                // Move the player downward at a constant speed 
                float unitDown = 50;
                player.move(0,unitDown+deltaTime);               
            }   

            // Check for left mouse button click
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                    isBulletRelease = true;
                    
            }
            else {
                isBulletRelease = false;
            }

            // Handle jumping (only on KeyPressed, not continuous)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && isOnGround) {
                VelocityY += jumpSpeed;
                isOnGround = false;
            }
        }
        // Clear Window
        window.clear(sf::Color::Black);

        // Render based on the current screen
        if (currentScreen == Screen::MainMenu) {
            isMainScean = true;
            renderUI(window);
        }
        else if (currentScreen == Screen::Game) {
            isGameScean = true;
            LoadMap(window);           
            renderGameScreen(window);   
        }
 
        // Display rendered frame
        window.display();
    }
    return 0;
}
