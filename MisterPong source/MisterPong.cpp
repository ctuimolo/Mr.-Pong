////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>


////////////////////////////////////////////////////////////
// Entry point of application
////////////////////////////////////////////////////////////
int main() {

	std:: cout << "Playing \"Mr. Pong!\"...\n\n";
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    //==================================//
	//          GAME CONSTANTS          //
	//==================================//
    const float pi = 3.14159f;
    const int gameWidth = 640;
    const int gameHeight = 480;
    sf::Vector2f paddleSize(20, 60);
    float ballRadius = 10.0f;
	int player01score = 0;
	int player02score = 0;
	int maxScore      = 99;
	int mrPongTimerMAX = 800;
	int mrPongStayMAX  = 1800;
	
	//==================================//
	//        MR Pong Controller        //
	//==================================//
	bool mrPongIsOn   = false;
	bool mrPongSound  = false;
	int mrPongTimer = mrPongTimerMAX;
	int mrPongStay  = mrPongStayMAX;

    // Initialize game window
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "Mr. Pong",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

	//===================================//
	//        Load images to be used     //
	//===================================//
	// Background image
	sf::Texture backgroundImage;
	if (!backgroundImage.loadFromFile("resources/background.png"))
		return EXIT_FAILURE;

	// Mr. Pong "walking" sprite sheet
	sf::Texture mrPongSheet;
	if (!mrPongSheet.loadFromFile("resources/MisterPongSheet.png"))
		return EXIT_FAILURE;
	int pongSheetDelay = 16;

	// Mr. Pong "pumping" animation sheet
	sf::Texture pumpUpSheet;
	if (!pumpUpSheet.loadFromFile("resources/pumpUp.png"))
		return EXIT_FAILURE;
	int pumpSheetDelay = 4;
	bool pumpSheetIsPlaying = false;

	// Mr.Pong "exploding" animation image
	sf::Texture explodeImage;
	if (!explodeImage.loadFromFile("resources/explode.png"))
		return EXIT_FAILURE;

	// Load images onto sprites used in the game
	//=====================================//
	//    Background sprite attributes     //
	//=====================================//
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundImage);
	int explosionTimer = 100;
	//=== ^^^ Background attributes ^^^ ===//


	//====================================//
	//        Mr. Pong attributes         //
	//====================================//
	sf::Sprite mrPongSprite;
	mrPongSprite.setTexture(mrPongSheet);
	mrPongSprite.setOrigin(12, 16);
	int walkFrameNumber = 0;
	int walkFrameDelay  = pongSheetDelay;
	int pumpFrameNumber = 0;
	int pumpFrameDelay  = pumpSheetDelay;
	int direction   = 0;
	int changeTimer = 50;
	//=== ^^^ Mr. Pong attributes ^^^ ===//

	sf::Sprite explosionSprite;
	explosionSprite.setTexture(explodeImage);
	explosionSprite.setOrigin(20,20);

	//====================================//
	//  Pumping up animation attributes   //
	//====================================//
	sf::Sprite pumpUpSprite;
	pumpUpSprite.setTexture(pumpUpSheet);
	pumpUpSprite.setOrigin(20,30);
	int destroyTimer = 0;
	//=== ^^^ Pump up animation   ^^^ ===//

    // Load the sounds used in the game
    sf::SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
        return EXIT_FAILURE;
    sf::Sound ballSound(ballSoundBuffer);

	sf::SoundBuffer thumpSoundBuffer;
    if (!thumpSoundBuffer.loadFromFile("resources/thump.wav"))
        return EXIT_FAILURE;
    sf::Sound thumpSound(thumpSoundBuffer);

	sf::SoundBuffer scoreSoundBuffer;
	if (!scoreSoundBuffer.loadFromFile("resources/score.wav"))
		return EXIT_FAILURE;
	sf::Sound scoreSound(scoreSoundBuffer);

	sf::SoundBuffer loseSoundBuffer;
	if (!loseSoundBuffer.loadFromFile("resources/lose.wav"))
		return EXIT_FAILURE;
	sf::Sound loseSound(loseSoundBuffer);

	sf::SoundBuffer chargeSoundBuffer;
	if (!chargeSoundBuffer.loadFromFile("resources/charge.wav"))
		return EXIT_FAILURE;
	sf::Sound chargeSound(chargeSoundBuffer);
	bool chargeSoundIsPlaying = false;

	sf::SoundBuffer shootSoundBuffer;
	if (!shootSoundBuffer.loadFromFile("resources/shoot.wav"))
		return EXIT_FAILURE;
	sf::Sound shootSound(shootSoundBuffer);

	sf::SoundBuffer stepSoundBuffer;
	if (!stepSoundBuffer.loadFromFile("resources/step.wav"))
		return EXIT_FAILURE;
	sf::Sound stepSound(stepSoundBuffer);
	stepSound.setLoop(true);
	bool stepSoundIsPlaying = false;

	sf::SoundBuffer appearSoundBuffer;
	if (!appearSoundBuffer.loadFromFile("resources/appear.wav"))
		return EXIT_FAILURE;
	sf::Sound appearSound(appearSoundBuffer);

	sf::SoundBuffer awaySoundBuffer;
	if (!awaySoundBuffer.loadFromFile("resources/away.wav"))
		return EXIT_FAILURE;
	sf::Sound awaySound(awaySoundBuffer);


    // Create the left paddle
    sf::RectangleShape leftPaddle;
    leftPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    leftPaddle.setOutlineThickness(0);
    leftPaddle.setOutlineColor(sf::Color::White);
    leftPaddle.setFillColor(sf::Color(255, 255, 255));
    leftPaddle.setOrigin(paddleSize / 2.f);

    // Create the right paddle
    sf::RectangleShape rightPaddle;
    rightPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    rightPaddle.setOutlineThickness(0);
    rightPaddle.setOutlineColor(sf::Color::White);
    rightPaddle.setFillColor(sf::Color(255, 255, 255));
    rightPaddle.setOrigin(paddleSize / 2.f);

    // Create the ball
    sf::CircleShape ball;
    ball.setRadius(ballRadius - 3);
    ball.setOutlineThickness(0);
    ball.setOutlineColor(sf::Color::White);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(ballRadius / 2, ballRadius / 2);

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/Mecha_Bold.ttf"))
        return EXIT_FAILURE;

    // Initialize the pause message
    sf::Text pauseMessage;
    pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(20);
    pauseMessage.setPosition(30.f, 50.f);
    pauseMessage.setColor(sf::Color::White);
    pauseMessage.setString(
		"                                               ====  MR. PONG!  ====\n\n "
		"Mr. Pong!\" is a pong-like made in C++ with SFML2.3.1\n\n"

		"Use the up and down arrow keys to move your paddle\n"
		"After a goal is made, Mr. Pong will occasionally wander the screen\n\n"
		"This is Mr. Pong ---->\n\n"
		
		"               He is an angry little guy, but don't mind him.\n"
		"               He'll probably leave on is own if you ignore him.\n"
		"               Just don't bump him or he's bound to do something brash.\n\n\n\n"
		
		"                                       << Press SPACEBAR to start! >>");

	// Initialize player score messages
	sf::Text player01scoreText;
	player01scoreText.setFont(font);
    player01scoreText.setCharacterSize(50);
    player01scoreText.setPosition(220.f, 30.f);
    player01scoreText.setColor(sf::Color::White);
    player01scoreText.setString(std::to_string(static_cast<long long>(player01score)));

	sf::Text player02scoreText;
	player02scoreText.setFont(font);
    player02scoreText.setCharacterSize(50);
    player02scoreText.setPosition(gameWidth - 270.f, 30.f);
    player02scoreText.setColor(sf::Color::White);
    player02scoreText.setString(std::to_string(static_cast<long long>(player02score)));


    // Define the paddles properties
    sf::Clock AITimer;
    const sf::Time AITime   = sf::seconds(0.01f);
    const float paddleSpeed = 400.f;
    float rightPaddleSpeed  = 20.f;
    float ballSpeed         = 400.f;
	float ballMaxSpeed      = 640.f;
	float ballMinSpeed      = 400.f;
    float ballAngle         = 0.f;
	float ballLock          = true;
	float going2explode     = false;

    sf::Clock clock;
    bool isPlaying = false;
	
	//==================================//
	//           Main game loop         // 
	//    active while window is open   //
	//==================================//
    while (window.isOpen()) {
        // Handle events
        sf::Event event;

		//=================================//
		//        Animation handler        //
		//     Specifically for Mr. Pong   //
		//=================================//
		if(walkFrameDelay > 0) {
			--walkFrameDelay;
		}else {
			walkFrameDelay = pongSheetDelay;
			if(walkFrameNumber < 3)
				++walkFrameNumber;
			else walkFrameNumber = 0;
		}

		if(pumpFrameDelay > 0) {
			--pumpFrameDelay;
		}else {
			pumpFrameDelay = pumpSheetDelay;
			if(pumpFrameNumber < 1)
				++pumpFrameNumber;
			else pumpFrameNumber = 0;
		}
		//=== ^^^ Animation handler ^^^ ===//

		//=================================//
		//           Event handler         //
		//        Poll events and react    //
		//=================================//
        while (window.pollEvent(event)){
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
			   ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
                window.close();
                break;
            }

            // Space key pressed: play
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space)) {
                if (!isPlaying) {

                    // (re)start the game
                    isPlaying = true;
                    clock.restart();

                    // Reset the position of the paddle, position of the ball, and ball speed
                    leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                    rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                    ball.setPosition(gameWidth / 2, gameHeight / 2);
					ballSpeed = ballMinSpeed;

                    // Reset the ball angle
                    do {
                        // Make sure the ball initial angle is not too much vertical
                        ballAngle = (std::rand() % 360) * 2 * pi / 360;
                    } while (std::abs(std::cos(ballAngle)) < 0.7f);
                }
            }
        }
		//=== ^^^  Event handler  ^^^ ===//

		//=========================================//
		//        If "isPlaying" is true           //
		//                                         //
		//=========================================//
        if (isPlaying) {
            float deltaTime = clock.restart().asSeconds();

			//================================//
			//        Control Mr. Pong        //
			//================================//
			if(mrPongIsOn) {
				if(mrPongStay > 0) {
					--mrPongStay;
					--changeTimer;

					if(ball.getPosition().x - ballRadius < mrPongSprite.getPosition().x + 24 &&
						ball.getPosition().x - ballRadius > mrPongSprite.getPosition().x - 24 &&
						ball.getPosition().y + ballRadius >= mrPongSprite.getPosition().y - 24 &&
						ball.getPosition().y - ballRadius <= mrPongSprite.getPosition().y + 24) {
						
						mrPongStay  = mrPongStayMAX;
						changeTimer = 50;
						mrPongIsOn  = false;
						going2explode = true;
						ball.setPosition(mrPongSprite.getPosition().x, mrPongSprite.getPosition().y);
						pumpUpSprite.setPosition(mrPongSprite.getPosition().x, mrPongSprite.getPosition().y);
						ballAngle = (std::rand() % 360) * 2 * pi / 360;
						ballSpeed = 1000;
					}

					switch(direction) {
					case 0 : if(mrPongSprite.getPosition().x < gameWidth - 50) 
							mrPongSprite.setPosition(mrPongSprite.getPosition().x + 1, 
								 mrPongSprite.getPosition().y); break;
					case 1 : if(mrPongSprite.getPosition().x > 50) 
						mrPongSprite.setPosition(mrPongSprite.getPosition().x - 1, 
								 mrPongSprite.getPosition().y); break;
					case 2 : if(mrPongSprite.getPosition().y < gameHeight - 50) 
						mrPongSprite.setPosition(mrPongSprite.getPosition().x, 
								 mrPongSprite.getPosition().y + 1); break;
					case 3 : if(mrPongSprite.getPosition().y > 50)  
						mrPongSprite.setPosition(mrPongSprite.getPosition().x, 
								 mrPongSprite.getPosition().y - 1); break;
					}
				}else {
					awaySound.play();
					mrPongStay  = mrPongStayMAX;
					changeTimer = 50;
					mrPongIsOn  = false;
				}
				if(changeTimer == 0) {
					changeTimer = 100;
					direction = std::rand() % 3;
				}
			}else {
				if((mrPongTimer > 0) && ((player01score > 0) || (player02score > 0))) 
					--mrPongTimer;
				if(mrPongTimer == 0) {
					appearSound.play();
					mrPongTimer = mrPongTimerMAX;
					mrPongIsOn  = true;
					mrPongSprite.setPosition(std::rand() % (gameWidth - 400) + 200, 
						std::rand() % (gameHeight - 200) + 100);
					direction = std::rand() % 3;
				}
			}
			//=== ^^^ Control Mr. Pong ^^^ ===//


			//================================//
			//      Player controllers        //
			//================================//
            // Move the player's paddle
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
			   (leftPaddle.getPosition().y - paddleSize.y / 2 > 0.0f)) {
				leftPaddle.move(0.f, -paddleSpeed * deltaTime);
				ballLock = (ballLock ? false: false);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
			   (leftPaddle.getPosition().y + paddleSize.y / 2 < gameHeight)) {
                leftPaddle.move(0.f, paddleSpeed * deltaTime);
				ballLock = (ballLock ? false: false);
            }

            // Move the computer's paddle
            if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 0.0f)) ||
               ((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < gameHeight))) {
                rightPaddle.move(0.f, rightPaddleSpeed * deltaTime);
            }

            // Update the computer's paddle direction according to the ball position
            if (AITimer.getElapsedTime() > AITime) {
                AITimer.restart();
                if (ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleSize.y / 3)
                    rightPaddleSpeed = paddleSpeed;
                else if (ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleSize.y / 3)
                    rightPaddleSpeed = -paddleSpeed;
                else
                    rightPaddleSpeed = 0.f;
            }
			//=== ^^^ Player controllers ^^^ ===//

			//==================================//
			//         Ball controller          //
			//==================================//
            // Move the ball
            float factor = ballSpeed * deltaTime;
            if(!ballLock && !going2explode) ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);

            // Check collisions between the ball and the screen
			// Left side goal
            if (ball.getPosition().x - ballRadius < 0.f) {

                // (re)start the game
                clock.restart();
				loseSound.play();
				if(player02score < maxScore) ++player02score;

                // Reset the position of the paddles and ball and ball speed
                leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                ball.setPosition(leftPaddle.getPosition().x + paddleSize.x, gameHeight / 2);
				ballSpeed = ballMinSpeed;
				ballLock = true;


                // Reset the ball angle
                ballAngle = pi / 2 * (std::rand() % 60 - 30) / 50;
            }
			// Right side goal
            if (ball.getPosition().x + ballRadius > gameWidth) {
                // (re)start the game
                clock.restart();
				scoreSound.play();
				if(player01score < maxScore) ++player01score;

                // Reset the position of the paddles and ball and ball speed
                leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                ball.setPosition(rightPaddle.getPosition().x - paddleSize.x - 10, gameHeight / 2);
				ballSpeed = ballMinSpeed;
				ballLock = true;

                // Reset the ball angle
                ballAngle = pi - pi / 2 * (std::rand() % 60 - 30) - 50;
            }

			if (ball.getPosition().y - ballRadius < 0.f) {
                thumpSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
            }
            
			if (ball.getPosition().y + ballRadius > gameHeight) {
                thumpSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
            }

            // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
               ball.getPosition().x - ballRadius > leftPaddle.getPosition().x - paddleSize.x / 2 &&
               ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
               ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2) {
               
				ballAngle = pi / 2 * (ball.getPosition().y - leftPaddle.getPosition().y) / 50;

				if(ballSpeed < ballMaxSpeed) ballSpeed += 20.0f;
                ballSound.play();
                ball.setPosition(leftPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
            }

            // Right Paddle
            if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
               ball.getPosition().x + ballRadius < rightPaddle.getPosition().x + paddleSize.x / 2 &&
               ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
               ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 2) {
                
				ballAngle = pi - pi / 2 * (ball.getPosition().y - rightPaddle.getPosition().y) / 50;

				if(ballSpeed < ballMaxSpeed) ballSpeed += 20.0f;
                ballSound.play();
                ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
            }
			//=== ^^^   Ball controller   ^^^ ===//
        }

        // Clear the window
        window.clear(sf::Color(0, 0, 0));

        if (isPlaying) {
            // Draw the paddles and the ball and the score
					window.draw(backgroundSprite);
			window.draw(ball);
			window.draw(leftPaddle);
            window.draw(rightPaddle);
			
			mrPongSprite.setTextureRect(sf::IntRect(walkFrameNumber*24, 0, 24, 32));
			if(mrPongIsOn)	{
				window.draw(mrPongSprite);
				if(!stepSoundIsPlaying) {
					stepSound.play();
					stepSoundIsPlaying = true;
				}
			}else {
				stepSound.stop();
				stepSoundIsPlaying = false;
			}
			pumpUpSprite.setTextureRect(sf::IntRect(pumpFrameNumber*40, 0, 40, 40));
			if(going2explode) {
				if(explosionTimer > 0) {
					window.draw(pumpUpSprite);
					--explosionTimer;
					if(!chargeSoundIsPlaying) {
						chargeSound.play();
						chargeSoundIsPlaying = true;
					}
				}else {
					chargeSoundIsPlaying = false;
					going2explode = false;
					destroyTimer = 32;
					explosionTimer = 100;
					shootSound.play();
				}
			}

			if(destroyTimer > 0) {
				--destroyTimer;
				explosionSprite.setPosition(mrPongSprite.getPosition().x, mrPongSprite.getPosition().y);
				window.draw(explosionSprite);
			}



			window.draw(player01scoreText);
			window.draw(player02scoreText);
			player01scoreText.setString(std::to_string(static_cast<long long>(player01score)));
		    player02scoreText.setString(std::to_string(static_cast<long long>(player02score)));

        }else {
            // Draw the pause message
            window.draw(pauseMessage);
			mrPongSprite.setTextureRect(sf::IntRect(walkFrameNumber*24, 0, 24, 32));
			mrPongSprite.setPosition(220, 210);
			window.draw(mrPongSprite);
        }

        // Display things on screen
        window.display();
    }

	std::cout << "Thank you for playing my game!\n";

    return EXIT_SUCCESS;
}


