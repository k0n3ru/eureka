#ifndef EUREKA_SHAPES_H
#define EUREKA_SHAPES_H

#include <SFML/Graphics.hpp>

namespace Eureka {

const int NUM_QUADS = 4;
const int QUAD_SIDE = 20; // px?

enum TetrominoType
{
	SHAPE_I,
	SHAPE_J,
	SHAPE_L,
	SHAPE_Z,
/*	SHAPE_S,
	SHAPE_T,
	SHAPE_O
*/
};

class Tetromino:public sf::Drawable, public sf::Transformable
{
public:
	Tetromino(sf::Vector2f,TetrominoType);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const ;
	void build(sf::Vector2f position,TetrominoType type);

	TetrominoType type_;
	sf::VertexArray* quad_[NUM_QUADS];
};


class Block
{

public:
	Block(sf::Vector2f size){
		size_ = size;
		rectangle_ = new sf::RectangleShape(size_);
	}
	void draw(){ return;}
private:
	bool canMove(sf::Vector2f new_pos_){return true;}
	sf::RectangleShape* rectangle_;
	sf::Vector2f size_;

};

/*
class Piece
{
public:
	// collection of blocks
}
*/

}//namespace
#endif
