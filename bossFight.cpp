#include "GameClasses.cpp"
//This is the first boss that will spawn after the first 4 levels
   void level1Boss(RenderWindow& window,int &levels,Spaceship &spaceship,vector<Bullets> &bullets,int &heart,int &score,int highScore,Picture hearts[],Text &scoretxt,Text &highscoretxt,Text &levelsTxt,Bullets &bullet,Clock &clock
    ,double movement){
        window.clear();
        bool a=false;
        Boss b1(window,"Boss.png");
        Clock bossShoot;
        vector<Bullets> bossBullets;
        Bullets bossBullet(window,"Bombs.png");
        Font font;
    if (!font.loadFromFile("AGENCYR.ttf")) {
        cout << "Error loading font" << endl;
    }
        Text Welcome = Text("Welcome to the first boss fight", font, 50);
        Welcome.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2);
        Clock TextTimer;
        while(TextTimer.getElapsedTime().asSeconds()<5){

            window.draw(Welcome);
            window.display();
        }
        window.clear();
        while(window.isOpen())
        {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (event.key.code == Keyboard::Space && (clock.getElapsedTime().asSeconds() > 1 )) {
            bullet.SetPosition((spaceship.getPosition().x)+50 , spaceship.getPosition().y - 10);
            bullets.push_back(bullet);
            bullet.drawTo(window);
            window.display();
            clock.restart();
    
            event.key.code = Keyboard::Unknown;
        }
        if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && spaceship.checkleft(movement)) {
            spaceship.move(-1 * movement, 0);
        }
        if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && spaceship.checkright(window, movement)) {
            spaceship.move(movement, 0);   
        }
        if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && spaceship.checkUp(movement,window)) {
            spaceship.move(0, -1 * movement);
     
        }
        if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && spaceship.checkdown(window, movement)) {
            spaceship.move(0, movement);
     
        }
        
        //This condition will check if the multiplier has ended
        window.clear();
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].getPosition().y < 0) {
                bullets.erase(bullets.begin() + i);
            }
            else {
                bullets[i].move(0, window.getSize().y * -0.02);
                bullets[i].drawTo(window);
            }
        }
   
        b1.script(window,spaceship,10);
        //This condition will check if the spaceship bullets has collided with the boss
        for(int i=0;i<bullets.size();i++){
            if(bullets[i].getGlobalBounds().intersects(b1.getGlobalBounds())){
                b1.incrementHits();
                bullets.erase(bullets.begin()+i); 
                if(b1.getHits()==5)
                {
                    levels++;
                    score+=500;
                    a=true;
                }               
            }
        }
        //This condition will check if the boss bullets has collided with the spaceship
        for(int i=0;i<bossBullets.size();i++){
            if(bossBullets[i].getGlobalBounds().intersects(spaceship.getGlobalBounds())){
                heart--;
                bossBullets.erase(bossBullets.begin()+i);
                
            }
        }
        //This condition will check if the boss bullets are out of bounds
        for(int i=0;i<bossBullets.size();i++){
            if(bossBullets[i].getPosition().y>window.getSize().y){
                bossBullets.erase(bossBullets.begin()+i);
            }
        }
        if (bossShoot.getElapsedTime().asSeconds() > 2) {
            bossBullet.SetPosition(b1.getPosition().x+50, b1.getPosition().y+50);
            bossBullets.push_back(bossBullet);
            bossShoot.restart();
        }
        if(a)
        {
            break;
        }
        for(int i=0;i<bossBullets.size();i++){
            if(bossBullets[i].getPosition().y>0){
                bossBullets[i].move(0,window.getSize().y*0.005);
                bossBullets[i].drawTo(window);
            }
            else{
                bossBullets.erase(bossBullets.begin()+i);
            }
        }
        spaceship.drawTo(window);
        for (int i = 0; i < heart; i++) {
            hearts[i].drawTo(window);
        }
        if (heart <= 0) {
            cout << "GAME OVER";
           
            if (score > highScore) {
                highScore = score;
                ofstream HighScore("highscore.txt", ios::app);
                if (HighScore.is_open()) {
                    HighScore << highScore << "\n";
                    HighScore.close();
                }

            }
            window.close();
        }
        scoretxt.setString("Score: " + to_string(score));
        highscoretxt.setString("High Score: " + to_string(highScore));
        levelsTxt.setString("Level: " + to_string(levels));
        window.draw(scoretxt);
        window.draw(highscoretxt);
        window.draw(levelsTxt);
        window.display();
        }
        TextTimer.restart();
        Text Outro = Text("Congratulations you have passed the first boss fight", font, 50);
                    
            Welcome.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2);
        window.clear();
        while(TextTimer.getElapsedTime().asSeconds()<5){
            window.draw(Outro);
            window.display();
        }
    }