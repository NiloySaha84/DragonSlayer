// Dragon and Slayer skeleton and base classes.
//requires std=c++17

#include<iostream>
#include<SFML/Graphics.hpp>
#include<vector>
#include<tuple>     
#include<ctime>
#include<cmath>
#include<set>
#include "animatedgif.cpp"
#include<deque>
using namespace std;

const int wwidth = 1024; // window dimensions in pixels
const int wheight = 680;

const int vgap = 24; // space between lines (squares)
const int hgap = 24; 

const int mwidth = wwidth/hgap;  // matrix dimensions in squares
const int mheight = wheight/vgap;

const char DRAGON = 8;
const char EMPTY = 0;
const char FOOD = 16;
const char SLAYER = -128;
const int WITHINSIGHT = 12;
const char OBSTACLE = 14;

typedef tuple<int,int> coord;  // a coordinate is just a pair of ints

typedef deque<coord> COORDQUEUE;

// global matrix
char Matrix[mwidth][mheight] = {EMPTY};
 const int foods = 10; // number of food items;
const int obstacles = 5; // number of obstacles;

struct obstacle_base {
    set<coord> obstacle; // coordinates of food.
    
    AnimatedGIF obgif;
    sf::Vector2i obsize;
    sf::Sprite obsprite;
    
    
    
   
    obstacle_base() : obgif{AnimatedGIF("fire.gif")} {
        obsize = obgif.getSize();
        obsprite.scale(1.25f*hgap/obsize.x,vgap*1.25f/obsize.y);
        int j = 0;
        while (j<obstacles) {
          int x = rand()%mwidth;
          int y = rand()%mheight;
          if (Matrix[x][y]<DRAGON && Matrix[x][y]>=0) {
        Matrix[x][y] = OBSTACLE;
        obstacle.insert({x,y});
        j++;
          }
        } //while
    }
    
    void render(sf::RenderWindow& window){
        for(auto& [x,y] : obstacle) { // draw obstacles
            obsprite.setPosition(x*hgap-obsize.x/20,y*vgap-obsize.y/20);
            obgif.update(obsprite);
            window.draw(obsprite);
        }
    }
};
struct dragon_base {   // don't change this class, INHERIT it.
  COORDQUEUE Q;
  sf::CircleShape circ;
  AnimatedGIF headgif; 
  sf::Vector2i headsize;
  sf::Sprite headsprite;

  dragon_base() : headgif{AnimatedGIF("head1.gif")} {}
  dragon_base(int x, int y) : headgif{AnimatedGIF("head1.gif")}
  {
    circ.setRadius(hgap/2.0f);
    circ.setFillColor(sf::Color::Green);
    headsize = headgif.getSize();
    headsprite.scale(1.25f*hgap/headsize.x,vgap*1.25f/headsize.y);
    Q.push_front({x,y});
    Q.push_front({x+1,y});
    Q.push_front({x+2,y});    
  } // constructor sets initial position, length 1

  void render(sf::RenderWindow& window) {
    int x, y;
    // draw only head as animated gif
    tie(x,y) = Q[0];
    headsprite.setPosition(x*hgap-headsize.x/20,y*vgap-headsize.y/20);
    headgif.update(headsprite);
    window.draw(headsprite);  
    for (int i=1;i<Q.size();i++) {
      tie(x,y) = Q[i];
      circ.setPosition(x*hgap,y*vgap);
      window.draw(circ);
    }
  } //render
};  // dragon_base class (can be inherited by subclass)

struct dragon:dragon_base {
    int x0; int y0;
    dragon(int x, int y):dragon_base(x,y){
        x0 = x;
        y0 = y;
        //std::pair<int,int>x0,y0;
       // Matrix[x0][y0] = DRAGON;
    }
    
    
    void move(int dx, int dy){
        int new_x = x0 + dx;
        int new_y = y0 + dy;
       // Matrix[x0][y0] = DRAGON;
        
        if ((new_x >= 0 && new_x < mwidth) && (new_y >= 0 && new_y < mheight) && Matrix[new_x][new_y] != OBSTACLE) {
            if (Matrix[new_x][new_y] != DRAGON) {
                if (Matrix[new_x][new_y] == FOOD) {
                   dragon_base::Q.push_front({new_x,new_y});
                 // Matrix[new_x][new_y] ^= DRAGON;
                   Matrix[x0][y0] = EMPTY;
                   x0 = new_x;
                   y0 = new_y;
               }//Food
                 //if (Matrix[new_x][new_y] == EMPTY) {
                else{
                    dragon_base::Q.push_front({new_x,new_y});
                    dragon_base::Q.pop_back();
                    //Matrix[new_x][new_y] ^= DRAGON;
                    //Matrix[x0][y0] = EMPTY;
                    x0 = new_x;
                    y0 = new_y;
                }//Empty
                 
            }// Dragon
           
        }
    }//move
    
};

struct slayer_base {  // dragon slayer base class
  int x, y; // current position
  AnimatedGIF slayergif;
  sf::Vector2i slayersize;
  sf::Sprite slayersprite;

  slayer_base() : slayergif{AnimatedGIF("miner.gif")} {}
  slayer_base(int x0, int y0) : slayergif{AnimatedGIF("miner.gif")}
  {
    x = x0;  y = x0;
    slayersize = slayergif.getSize();
    slayersprite.scale(1.3f*hgap/slayersize.x,vgap*1.3f/slayersize.y);
    Matrix[x0][y0] ^= SLAYER;
  } // constructor

  void render(sf::RenderWindow& window) {
    slayersprite.setPosition(x*hgap-slayersize.x/20,y*vgap-slayersize.y/20);
    slayergif.update(slayersprite);
    window.draw(slayersprite);
  }
}; //slayer_base class

bool getting_chopped = false;

struct slayer : slayer_base {
    int randomstate;
    slayer(int x0, int y0) : slayer_base(x0, y0) {
        randomstate = 0;
    }

    double distance(int x2, int y2) {
        return sqrt(pow(x - x2, 2) + pow(y - y2, 2));
    }

    void move(int dx, int dy) {
        int new_x = 0; int new_y = 0;
        if ((new_x >= 0 && new_x < mwidth) && (new_y >= 0 && new_y < mheight) && Matrix[new_x][new_y] != OBSTACLE) {
            
            if (randomstate<=0 && distance(dx,dy)>WITHINSIGHT) {
                    new_x = x+rand() % 3 - 1;
                    new_y = y+rand() % 3 - 1;
                if ((new_x >= 0 && new_x < mwidth) && (new_y >= 0 && new_y < mheight)) {
                    x = new_x;
                    y = new_y;
                }
            }
            
            if (randomstate>0){
                new_x = x+rand() % 3 - 1;
                new_y = y+rand() % 3 - 1;
                if ((new_x >= 0 && new_x < mwidth) && (new_y >= 0 && new_y < mheight)) {
                    x = new_x;
                    y = new_y;
                }
                //for (int i=randomstate;i>=0;i--) {
                    randomstate--;
               // }
            }
            
            if (randomstate<=0 && distance(dx,dy)<=WITHINSIGHT){
                if(x<dx) {
                  new_x = x+1;
                    if ((new_x >= 0 && new_x < mwidth) && (new_y >= 0 && new_y < mheight)){
                        x = new_x;
                        //y++;
                    }
                }
                else if(x>dx){
                   new_x = x-1;
                    if ((new_x >= 0 && new_x < mwidth) && (new_y >= 0 && new_y < mheight)){
                        x = new_x;
                        //y++;
                    }
                }
                if(y>dy){
                    new_y = y-1;
                    if ((new_x >= 0 && new_x < mwidth) && (new_y >= 0 && new_y < mheight)){
                        y = new_y;
                    }
                }
               else if(y<dy){
                    new_y = y+1;
                   if ((new_x >= 0 && new_x < mwidth) && (new_y >= 0 && new_y < mheight)){
                       y = new_y;
                   }
                }
            }
             if (randomstate<=0 && distance(dx,dy)== 0) {
                randomstate = 20;
                getting_chopped = true;
                 //dragon0.truncate(dragon0.Q.size()/2);
            }
            else{
                getting_chopped = false;
            }
            
        }
    }
};

int main(int argc, char** argv)
{
    using namespace sf;
    srand((unsigned int)time(0) % 10000); //seed random number generator
    RenderWindow window(VideoMode(wwidth, wheight), "Dragon");
    window.setFramerateLimit(4);
    
    Vertex hlines[2*mheight]; // 2 vertices for each line
    Vertex vlines[2*mwidth]; // 2 vertices for each line    

    auto color = Color(255,255,0,128); //::Yellow;
    for (int i=0; i < (2*mheight); i += 2) { // horizontal lines
        hlines[i] = Vertex(Vector2f(0.0f, i*vgap/2), color);
        hlines[i+1] = Vertex(Vector2f(wwidth-1, i*vgap/2), color);
    }
    for (int i=0; i < 2*mwidth; i += 2) { // vertical lines
        vlines[i] = Vertex(Vector2f(i*hgap/2, 0.0f), color);
        vlines[i+1] = Vertex(Vector2f(i*hgap/2, wheight-1), color);
    }

    dragon dragon0(10,10);
    int dx = 0, dy = 0; // dragon movement vector
    slayer slayer0(25,35);
    obstacle_base obstacle0;
    set<coord> food; // coordinates of food.
    int i = 0;  // set foods
    while (i<foods) {
      int x = rand()%mwidth;
      int y = rand()%mheight;
      if (Matrix[x][y]<DRAGON && Matrix[x][y]>=0) {
	Matrix[x][y] = FOOD;
	food.insert({x,y});
	i++;
      }
    } //while

    CircleShape fcirc(hgap/4.0);       //graphical representation of food
    fcirc.setFillColor(Color::White);
    
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
	    if (event.type==sf::Event::KeyPressed) {
	      if (event.key.code==sf::Keyboard::Up) { 
		dy = -0;  dx = -1;
              dragon0.move(dy,dx);
              int Dfx = get<0>(dragon0.Q.front()); int Dfy = get<1>(dragon0.Q.front());
         
              if (Matrix[Dfx][Dfy] == FOOD) {
                  // Remove the food item from the set
                  food.erase({dx, dy});}
              //slayer0.move(dragon0.Q.front().first,dragon0.front().second);
	      }
	      else if (event.key.code==sf::Keyboard::Down) { //down
		dy = 0; dx=1;
              
                  dragon0.move(dy,dx);
              
              int Dfx = get<0>(dragon0.Q.front()); int Dfy = get<1>(dragon0.Q.front());
            
              if (Matrix[Dfx][Dfy] == FOOD) {
                  // Remove the food item from the set
                  food.erase({dx, dy});}
              //slayer0.move(dragon0.Q.front().first,dragon0.front().second);
	      }
	      else if (event.key.code==sf::Keyboard::Left) { //left
		dx = 0; dy=-1;
              dragon0.move(dy,dx);
              int Dfx = get<0>(dragon0.Q.front()); int Dfy = get<1>(dragon0.Q.front());
              
              if (Matrix[Dfx][Dfy] == FOOD) {
                  // Remove the food item from the set
                  food.erase({dx, dy});}
              //slayer0.move(dragon0.Q.front().first,dragon0.front().second);
	      }
	      else if (event.key.code==72) { //right
		dx=0; dy = 1;
              dragon0.move(dy,dx);
              int Dfx = get<0>(dragon0.Q.front()); int Dfy = get<1>(dragon0.Q.front());
             
              if (Matrix[Dfx][Dfy] == FOOD) {
                  // Remove the food item from the set
                  food.erase({dx, dy});}
              //slayer0.move(dragon0.Q.front().first,dragon0.front().second);
	      }
	      else if (event.key.code==57) {  //stop
		dx = dy = 0;
              dragon0.move(dy,dx);
	      }
            
            
	    }// keyboard event handler
        }
        int Dx = get<0>(dragon0.Q.at(dragon0.Q.size()/2)); int Dy = get<1>(dragon0.Q.at(dragon0.Q.size()/2));
        slayer0.move(Dx,Dy);
        int Dfx = get<0>(dragon0.Q.front()); int Dfy = get<1>(dragon0.Q.front());
      
        if (Matrix[Dfx][Dfy] == FOOD) {
            // Remove the food item from the set
            food.erase({Dfx, Dfy});}
    
        
       if (getting_chopped && dragon0.Q.size()!=0) {
                dragon0.Q.resize(dragon0.Q.size()/2);
                
            }
        if ((dragon0.Q.size()==1) || (food.size()==0)){
            window.close();
        }
        window.clear(sf::Color::Black); // erase background
        window.draw(vlines, 2*mwidth, sf::Lines); //draw lines
        window.draw(hlines, 2*mheight, sf::Lines); //draw lines
	for(auto& [x,y] : food) { // draw food items
	  fcirc.setPosition(x*hgap+hgap/4.0, y*vgap + vgap/4.0);
	  window.draw(fcirc);
        
	}

	dragon0.render(window);
	slayer0.render(window);
    obstacle0.render(window);
        window.display();
    } // main event loop

    return EXIT_SUCCESS;
}//main
