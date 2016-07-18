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
	sf::VertexArray* quads_[NUM_QUADS];
};


}//namespace
#endif
