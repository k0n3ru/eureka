//#include <stdio.h>
#include <set>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <string> 

// to convert between radians and degrees
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
typedef std::pair<b2Fixture*, b2Fixture*> fixturePair;

// Variable declarations. All seven tetris types are given an identifier number here.
// Same for the three body types: tetris, boat and water.

enum {
  shapeI,
  shapeJ,
  shapeL,
  shapeO,
  shapeS,
  shapeT,
  shapeZ,
  waterType = 0,
  boatType,
  tetrisType,
  window_width = 500,
  window_height = 500,
  tile_size = 10
};

float corner = 0.5f; // to ensure there are no unnecessary colisions, it is adviceable to bevel the sharp angles
                     // of a polygon. This is how much it will be clipped off

b2Vec2 gravity = b2Vec2(0, 9.8f);  
b2World* m_world = new b2World(gravity);  // World generation with gravity
b2BodyDef bodyDef;                        // Since body definitions are reuseable, we only need one instance for all objects
b2FixtureDef fixtureDef;                  // That's the same for fixtures defs and polygon shapes.
b2PolygonShape polygonShape;

// a template of the class the contains a box2d body

struct bodyTemplate {

  b2Body* body;
  sf::Color color;
  int type;

};

// devrised case of bodyTemplate to hold tetris bodies.

struct tetris: public bodyTemplate{

  

  tetris (int shape){ //to create a new tetris, you need to specify which of the seven types is to be created

    type = tetrisType;
    
    // set body defs properties
    bodyDef.type = b2_dynamicBody; 
    bodyDef.position.Set(window_width/2, 0);

    body = m_world->CreateBody(&bodyDef); // attach body to world
    body->SetLinearVelocity(b2Vec2 (0,tile_size)); // body settings

    // set fixture defs properties
    fixtureDef.density = 0.4f;
    fixtureDef.isSensor = false;

    body->SetUserData(this); // put a pointer inside the body which points back to the tetris class that contains it.

    //set a random colour
    srand (time(NULL));
    color.r = (rand()%255);
    srand (time(NULL)*2);
    color.g = (rand()%255);
    srand (time(NULL)*3);
    color.b = (rand()%255);
    color.a = 255;

    //different the tetris types will require the four sqaures to be attached to the body differently

    switch (shape) {
    case shapeI:
      // these functions will each attach a box at these locations.
      attachBox(b2Vec2(tile_size,0));
      attachBox(b2Vec2(tile_size*3,0));
      attachBox(b2Vec2(-tile_size,0));
      attachBox(b2Vec2(-tile_size*3,0));    
      break;
    case shapeJ:
      attachBox(b2Vec2(0,0));
      attachBox(b2Vec2(0,tile_size*2));
      attachBox(b2Vec2(0,-tile_size*2));
      attachBox(b2Vec2(-tile_size*2,tile_size*2));
      break;
    case shapeL:
      attachBox(b2Vec2(0,0));
      attachBox(b2Vec2(0,tile_size*2));
      attachBox(b2Vec2(0,-tile_size*2));
      attachBox(b2Vec2(tile_size*2,tile_size*2));
      break;
    case shapeO:
      attachBox(b2Vec2(tile_size,tile_size));
      attachBox(b2Vec2(tile_size,-tile_size));
      attachBox(b2Vec2(-tile_size,tile_size));
      attachBox(b2Vec2(-tile_size,-tile_size));
      break;
    case shapeS:
      attachBox(b2Vec2(tile_size,0));
      attachBox(b2Vec2(-tile_size,0));
      attachBox(b2Vec2(tile_size,tile_size*2));
      attachBox(b2Vec2(-tile_size,-tile_size*2));
      break;
    case shapeT:
      attachBox(b2Vec2(0,0));
      attachBox(b2Vec2(0,-tile_size*2));
      attachBox(b2Vec2(-tile_size*2,0));
      attachBox(b2Vec2(tile_size*2,0));
      break;
    case shapeZ:
      attachBox(b2Vec2(tile_size,0));
      attachBox(b2Vec2(-tile_size,0));
      attachBox(b2Vec2(-tile_size,tile_size*2));
      attachBox(b2Vec2(tile_size,-tile_size*2));
      break;
    }

  }

  //Normally simply creating a box is good enough. However, according to iForce2d,
  //sharp corners will lead to a lot of unnecassary collisions due to the overlapping AABBs,
  //which create lot of problems. One solution is to bevel the sharp edges,
  //which makes the squares have 8 vertices each.
  void attachBox(b2Vec2 position) {
    b2Vec2 vertices[8];
    vertices[0].Set(-tile_size + corner + position.x, tile_size + position.y);
    vertices[1].Set(-tile_size + position.x, tile_size - corner + position.y);
    vertices[2].Set(-tile_size + position.x, -tile_size + corner + position.y);
    vertices[3].Set(-tile_size + corner + position.x, -tile_size + position.y);
    vertices[4].Set(tile_size - corner + position.x, -tile_size + position.y);
    vertices[5].Set(tile_size + position.x, -tile_size + corner + position.y);
    vertices[6].Set(tile_size + position.x, tile_size - corner + position.y);
    vertices[7].Set(tile_size - corner + position.x, tile_size + position.y);
    polygonShape.Set(vertices, 8); // attach the vertices to a polygon
    fixtureDef.shape = &polygonShape; // attach the polygon to a fixture
    body->CreateFixture(&fixtureDef); // attach the fixture to the body
  }

  // destructor class of the tetris when it sinks deep into water
  ~tetris() {
    body->GetWorld()->DestroyBody(body );
  }

};

//Similiar class but for a boat not tetris.

struct boat: public bodyTemplate{

  boat (){

    type = boatType;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(window_width/2, window_height - 60);
    body = m_world->CreateBody(&bodyDef);

    polygonShape.SetAsBox(tile_size*15,tile_size);
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 0.5f;
    fixtureDef.isSensor = false;
    body->CreateFixture(&fixtureDef);

    polygonShape.SetAsBox(tile_size, tile_size*3, b2Vec2(-tile_size*14,-tile_size*2),0 );
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 0.5f;
    fixtureDef.isSensor = false;
    body->CreateFixture(&fixtureDef);

    polygonShape.SetAsBox(tile_size, tile_size*3, b2Vec2(tile_size*14,-tile_size*2),0 );
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 0.5f;
    fixtureDef.isSensor = false;
    body->CreateFixture(&fixtureDef);

    color = sf::Color::Red;
    body->SetUserData(this);
  }

};

//Similar class but for water. Because it interacts with other bodies
//but does not collide with them, it is set to be a sensor.

struct water: public bodyTemplate{

  water (){

    type = waterType;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(window_width/2, window_height - 36);
    body = m_world->CreateBody(&bodyDef);
    polygonShape.SetAsBox(tile_size*50, 36);
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 2;
    fixtureDef.isSensor = true;
    body->CreateFixture(&fixtureDef);
    color = sf::Color::Transparent;
    body->SetUserData(this);
  }

};

// The first list below records in one cycle which pairs of bodies have collided and
// generated buoyancy to the boat. Another one records
// all the tetris pieces that have sunk deep into the water and are scheduled to be deleted.
// These declarations need to be here as they have to be after the body class definition
// but before the contact listener class.
std::set<fixturePair> m_fixturePairs;
std::set<tetris*> tetrisScheduledForRemoval;

//contact listener basically runs after everytime a collision has just happened. It can tell you which
//pair of bodies have collided. You can specify what will happen upon that.
//here we are using it to detect the collision between the water and the boat.
//We put it into a list so as to calculate the buoyancy that pushes the boat up.
class ContactListener : public b2ContactListener
{

  void BeginContact(b2Contact* contact) {

    // when a collision has just happened, it will record down which two bodies have collided
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    // we can use the user data inside the body to find out that object contains them.
    tetris* tetrisInQuestionA = static_cast<tetris*>( fixtureA->GetBody()->GetUserData() );
    tetris* tetrisInQuestionB = static_cast<tetris*>( fixtureB->GetBody()->GetUserData() );

    // check if one of them is water (by checking if it's a sensor) and if the other is the boat
    // if yes, put them in the buoyancy list to calculate the buoyancy.
    if ( fixtureA->IsSensor() && tetrisInQuestionB->type == boatType) {
      m_fixturePairs.insert( std::make_pair(fixtureA, fixtureB) ); 
    } else if ( fixtureB->IsSensor() && tetrisInQuestionA->type == boatType ){
      m_fixturePairs.insert( std::make_pair(fixtureB, fixtureA) ); 
    }

  }

  // do the same thing when the two objects lose contact.
  // and remove the pair from the list that calculates buoyancy.
  void EndContact(b2Contact* contact) {
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    tetris* tetrisInQuestionA = static_cast<tetris*>( fixtureA->GetBody()->GetUserData() );
    tetris* tetrisInQuestionB = static_cast<tetris*>( fixtureB->GetBody()->GetUserData() );
    if ( fixtureA->IsSensor() && tetrisInQuestionB->type == boatType ) {
      m_fixturePairs.erase( std::make_pair(fixtureA, fixtureB) );
    } else if ( fixtureB->IsSensor() && tetrisInQuestionA->type == boatType ){
      m_fixturePairs.erase( std::make_pair(fixtureB, fixtureA) );
    }
  }
};

// the four functions below are taken from iforce2d to calculate the buoyancy. It makes use of the data
// in the list we have just generated. It involves calculating the overapping area of the two objects which
// in turn determines the buoyancy. It also locate the centre of mass of the boat and applies the
// buoyancy to it.
bool inside(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 p) {
  return (cp2.x-cp1.x)*(p.y-cp1.y) > (cp2.y-cp1.y)*(p.x-cp1.x);
}
b2Vec2 intersection(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 s, b2Vec2 e) {
  b2Vec2 dc( cp1.x - cp2.x, cp1.y - cp2.y );
  b2Vec2 dp( s.x - e.x, s.y - e.y );
  float n1 = cp1.x * cp2.y - cp1.y * cp2.x;
  float n2 = s.x * e.y - s.y * e.x;
  float n3 = 1.0 / (dc.x * dp.y - dc.y * dp.x);
  return b2Vec2( (n1*dp.x - n2*dc.x) * n3, (n1*dp.y - n2*dc.y) * n3);
}
bool findIntersectionOfFixtures(b2Fixture* fA, b2Fixture* fB, std::vector<b2Vec2>& outputVertices)
{
  //currently this only handles polygon vs polygon
  if ( fA->GetShape()->GetType() != b2Shape::e_polygon ||
    fB->GetShape()->GetType() != b2Shape::e_polygon )
    return false;

  b2PolygonShape* polyA = (b2PolygonShape*)fA->GetShape();
  b2PolygonShape* polyB = (b2PolygonShape*)fB->GetShape();

  //fill subject polygon from fixtureA polygon
  for (int i = 0; i < polyA->GetVertexCount(); i++)
    outputVertices.push_back( fA->GetBody()->GetWorldPoint( polyA->GetVertex(i) ) );

  //fill clip polygon from fixtureB polygon
  std::vector<b2Vec2> clipPolygon;
  for (int i = 0; i < polyB->GetVertexCount(); i++)
    clipPolygon.push_back( fB->GetBody()->GetWorldPoint( polyB->GetVertex(i) ) );

  b2Vec2 cp1 = clipPolygon[clipPolygon.size()-1];
  for (int j = 0; j < clipPolygon.size(); j++) {
    b2Vec2 cp2 = clipPolygon[j];
    if ( outputVertices.empty() )
      return false;
    std::vector<b2Vec2> inputList = outputVertices;
    outputVertices.clear();
    b2Vec2 s = inputList[inputList.size() - 1]; //last on the input list
    for (int i = 0; i < inputList.size(); i++) {
      b2Vec2 e = inputList[i];
      if (inside(cp1, cp2, e)) {
        if (!inside(cp1, cp2, s)) {
          outputVertices.push_back( intersection(cp1, cp2, s, e) );
        }
        outputVertices.push_back(e);
      }
      else if (inside(cp1, cp2, s)) {
        outputVertices.push_back( intersection(cp1, cp2, s, e) );
      }
      s = e;
    }
    cp1 = cp2;
  }

  return !outputVertices.empty();
}
b2Vec2 ComputeCentroid(std::vector<b2Vec2> vs, float& area)
{
  int count = (int)vs.size();
  b2Assert(count >= 3);

  b2Vec2 c;
  c.Set(0.0f, 0.0f);
  area = 0.0f;

  // pRef is the reference point for forming triangles.
  // Its location doesnt change the result (except for rounding error).
  b2Vec2 pRef(0.0f, 0.0f);

  const float32 inv3 = 1.0f / 3.0f;

  for (int32 i = 0; i < count; ++i)
  {
    // Triangle vertices.
    b2Vec2 p1 = pRef;
    b2Vec2 p2 = vs[i];
    b2Vec2 p3 = i + 1 < count ? vs[i+1] : vs[0];

    b2Vec2 e1 = p2 - p1;
    b2Vec2 e2 = p3 - p1;

    float32 D = b2Cross(e1, e2);

    float32 triangleArea = 0.5f * D;
    area += triangleArea;
    // Area weighted centroid
    c += triangleArea * inv3 * (p1 + p2 + p3);
  }

  // Centroid
  if (area > b2_epsilon)
    c *= 1.0f / area;
  else
    area = 0;
  return c;
}

//after all the definition, we now put it into practise.

int main()
{
  // Generate a window using SFML and an SFML shape object which is used to draw
  // everything in the window later on. Also create a font object. 
  sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "SFML Test", sf::Style::Default);
  sf::ConvexShape shapeToBeDrawn;
 // sf::Font font;
  //font.loadFromFile("Minecrafter_3.ttf");

  // a bunch of flags that keep track of the game and let us know if you have won or lost.
  int score = 0;
  bool allowedToWin = true;
  bool won = false;
  bool alreadyWon = false;
  bool allowedToLose = false;
  bool lost = false;
  bool exploded = false;
/*
  // Generation of the winning message using the font object.
  sf::Text youWon;
  youWon.setFont(font);
  youWon.setString("YOU WON!");
  youWon.setCharacterSize(69);
  youWon.setColor(sf::Color(255,200,0,255));
  youWon.setPosition(24,(window_height-69)/2);

  // Generation of the winning message using the font object.
  sf::Text youLost;
  youLost.setFont(font);
  youLost.setString("YOU LOST!");
  youLost.setCharacterSize(64);
  youLost.setColor(sf::Color(255,200,0,255));
  youLost.setPosition(24,(window_height-64)/2);

  // Generation of the score on screen.
  sf::Text score_text;
  score_text.setFont(font);
  score_text.setString("0");
  score_text.setCharacterSize(24);
  score_text.setColor(sf::Color(255,200,0,255));
  score_text.setPosition(24,window_height-48);

  // Generation of target number dispayed on screen.
  sf::Text goal;
  goal.setFont(font);
  goal.setString("TARGET 20");
  goal.setCharacterSize(24);
  goal.setColor(sf::Color(255,200,0,255));
  goal.setPosition(24,24);
*/
  // These clock objects are used to control how fast the keyboard gets updated,
  // because if the keyboard gets updated too fast (every millisecond),
  // the tetris piece will drop into the water before you can see it.
  clock_t timestamp = clock(); // Used to record time at any instance.
  clock_t interval; // Used to compare time passed after a certain instance.

  //attached the contact listenser to the world.
  ContactListener contactListenerInstance;
  m_world->SetContactListener(&contactListenerInstance);

  // now create the boat, the water and the first tetris that will fall from the sky
  // This tetris will take the form of one of the seven types randomly selected
  boat();
  water();
  srand (time(NULL));
  tetris* current_tetris = new tetris(rand()%7);

  // Now everything is initialised, we will put them in the loop.

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // makes the background yellow
    window.clear(sf::Color::Yellow);


    // in each cycle box2d will collect all user imputs, and then physics-simlutate it.
    // and then we can use the data generated by simluation to update our objects on the screen.
    // now lets look at the user inputs.

    // the user input here is basically controlling how the tetris is falling with keyboard.
    // so lets make a copy of the tetris's position and angle
    // which are to be manipulated.
    b2Vec2 position = current_tetris->body->GetPosition();
    float angle = current_tetris->body->GetAngle();

    // then the user makes it flyo left or right, or rotate or fall faster.
    // the keyboard will only update every 100, because as I have said,
    // we don't want it to be too fast.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      clock_t interval = clock() - timestamp;
      if (interval > 100) {
        position.x -= tile_size;
        timestamp = clock();
      }
    }  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      clock_t interval = clock() - timestamp;
      if (interval > 100) {
        position.x += tile_size;
        timestamp = clock();
      }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      position.y += 1;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      clock_t interval = clock() - timestamp;
      if (interval > 200) {
        angle += 90 * DEGTORAD;
        timestamp = clock();
      }
    }

    // the new altered posible and angle apply to the tetris.
    current_tetris->body->SetTransform(position,angle);

    // now all the user imput has been taken in this cycle,
    // box2d will do the physics simlulation
    // for example it will detect what objects have collided
    // and how they will react to each other
    // and ultimately do what we asked it to do in the contact listener
    m_world->Step((1.0f / 150), 10, 8);

    // now we loop through the list we have generated before
    // to see which pairs of object have collided and generated float force.
    // then we apply the buoyancy to the boat's centre of mass.
    // the whole while loop below is taken from iforce2D
    std::set<fixturePair>::iterator it = m_fixturePairs.begin();
    std::set<fixturePair>::iterator end = m_fixturePairs.end();
    while (it != end) {
      b2Fixture* fixtureA = it->first;
      b2Fixture* fixtureB = it->second;
      float density = fixtureA->GetDensity();
      std::vector<b2Vec2> intersectionPoints;
      if ( findIntersectionOfFixtures(fixtureA, fixtureB, intersectionPoints) ) {
        float area = 0;
        b2Vec2 centroid = ComputeCentroid( intersectionPoints, area); //this is the centre of mass of the boat
        float displacedMass = fixtureA->GetDensity() * area;
        b2Vec2 buoyancyForce = displacedMass * -gravity; // this is the amount of buoyancy to be applied.
        fixtureB->GetBody()->ApplyForce( buoyancyForce, centroid ,true); // applying the force.

        // the part below is just to make things look more realistic.
        b2Vec2 velDir = fixtureB->GetBody()->GetLinearVelocityFromWorldPoint( centroid ) -
        fixtureA->GetBody()->GetLinearVelocityFromWorldPoint( centroid );
        float vel = velDir.Normalize();
        float dragMag = fixtureA->GetDensity() * vel * vel;
        b2Vec2 dragForce = dragMag * -velDir;
        fixtureB->GetBody()->ApplyForce( dragForce, centroid ,true);
        float angularDrag = area * -fixtureB->GetBody()->GetAngularVelocity();
        fixtureB->GetBody()->ApplyTorque( angularDrag, true );
      }
      ++it;
    }

    // now we have to test if the falling tetris landed on the boat
    // if so we will create a new tetris from the sky
    // to do the test we access the falling tetris's contact list (a list of all collision in this cycle)
    // if it has hit the boat, it will generate a new tetris from the sky. score plus one upon that.
    // notice that if the collision happen to far up: (position.y < tile_size*5)
    // that means the user has stacked up a whole tower of tetris and the falling tetris has collided
    // with the top of the tower. in that case 'exploded' will be
    // triggered and you will immediately lose
    for (b2ContactEdge* edge = current_tetris->body->GetContactList(); edge; edge = edge->next) {
      srand (time(NULL));
      if (position.y < tile_size*5) {
        allowedToLose = true;
        exploded = true;
      }

      // obviously if you have won or lost there will be no more falling tetris
      // so this is not allow.
      if (!won && !lost) {
        srand (time(NULL));
        current_tetris = new tetris(rand()%7);
        ++ score;
      }
      break; // one collision detected is enough, so loop an break.
    }

    // now we have to create another list to keep track of tetris that have falling deep
    // in the water (> 500) and will be deleted. score minus one upon that. we loop through all the objects in the
    // world and test them one by one. since this is linked list, even though the object should be 
    // deleted, we cannot delete them yet. otherwise we will break the linked list
    // and cannot access the next item. We can only note them down in a
    // list and delete them later.
    for(b2Body* body = m_world->GetBodyList(); body; body = body->GetNext()) {
      tetris* tetrisInQuestion = static_cast<tetris*>( body->GetUserData() );
      b2Vec2 tetrisInQuestionPosition = tetrisInQuestion->body->GetPosition();
      if ((tetrisInQuestionPosition.y > 500 + tile_size*4 || tetrisInQuestionPosition.x > 500 + tile_size*4
        || tetrisInQuestionPosition.x < 0 - tile_size*4) && tetrisInQuestion->type != boatType && tetrisInQuestion!=current_tetris) {
          tetrisScheduledForRemoval.insert(tetrisInQuestion);
          -- score;
      }
    }

    // now we loop through the list we have just created and
    // delete what's inside and then clear the list.
    std::set<tetris*>::iterator it2 = tetrisScheduledForRemoval.begin();
    std::set<tetris*>::iterator end2 = tetrisScheduledForRemoval.end();
    for (; it2!=end2; ++it2) {
      tetris* dead_tetris = *it2;
      delete dead_tetris;
    }
    tetrisScheduledForRemoval.clear();

    // now all the calculations are done
    // we can draw them onto the screen.
    // again we access the world's body list,
    // and for each body we access its distinctive colour
    // and access each of its fixtures.
    // we turn the "sf::ConvexShape shapeToBeDrawn" that we created earlier (which is reuseable) into
    // the same shape as the fixture and draw it on to the screen.
    // we do this until or everything is drawn.
    for(b2Body* body = m_world->GetBodyList(); body; body = body->GetNext()) {
      tetris* tetrisInQuestion = static_cast<tetris*>( body->GetUserData() );
      if (tetrisInQuestion != NULL) shapeToBeDrawn.setFillColor(tetrisInQuestion->color);
      shapeToBeDrawn.setOutlineThickness(1);
      shapeToBeDrawn.setOutlineColor(sf::Color(250, 150, 100));
      for(b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        if(fixture->GetType() == b2Shape::e_polygon) {
          b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
          size_t point_count = shape->GetVertexCount();
          shapeToBeDrawn.setPointCount(point_count);
          for(int i = 0; i < point_count; i++) {
            b2Vec2 v = shape->GetVertex(i);
            shapeToBeDrawn.setPoint(i, sf::Vector2f(body->GetWorldVector(v).x + body->GetPosition().x, body->GetWorldVector(v).y + body->GetPosition().y));
          }
          window.draw(shapeToBeDrawn);
        }
      }

    }

    // after drawing the objects, we have to draw the texts too
    // here we turn the score from int to string and draw the string.
    std::ostringstream oss;
    oss<< score;
    std::string result = oss.str();
    //score_text.setString(result);
    //window.draw(score_text);
    //window.draw(goal);
    
    // now according to what score we have, we can display on screen if you have won or lost.
    // you are only allowed to lose once your score goes above 5
    // that's to make sure you will not lose when you are starting up and only have one piece
    // and you lose that piece.
    if(score > 5) allowedToLose = true;

    // if you have more than 20, you win and have 'alreadyWon'
    // so will continue to win even if the score drops below 20 again
    if((allowedToWin == true && score >= 20) || alreadyWon) {
      won = true;
      allowedToLose = false;
      alreadyWon = true;
      //window.draw(youWon);
    }

    // if you have gone above 5 and drop back to zero or have accidently built a tall tower that touches the sky
    // you will lose.
    if (allowedToLose == true && (score <= 0 || exploded)) {
      lost = true;
      allowedToWin = false;
     // window.draw(youLost);
    }

    window.display();
  }

  return 0;
}

