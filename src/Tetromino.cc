#include "shapes.h"

using namespace Eureka;

Tetromino::Tetromino(sf::Vector2f position,TetrominoType type)
{
	type_ = type;
/*	quad_[0] = new sf::VertexArray(sf::Quads,4);
	quad_[1] = new sf::VertexArray(sf::Quads,4);
	quad_[2] = new sf::VertexArray(sf::Quads,4);
	quad_[4] = new sf::VertexArray(sf::Quads,4);
*/	build(position,type);
}


// Builds a quad at given pos.Use only to build tetromino quads
void buildQuad(sf::VertexArray*& quad,sf::Vector2f position)
{
	quad = new sf::VertexArray(sf::Quads,4);
        (*quad)[0].position = sf::Vector2f(position.x,position.y);
        (*quad)[1].position = sf::Vector2f(position.x+QUAD_SIDE,position.y);
        (*quad)[2].position = sf::Vector2f(position.x+QUAD_SIDE,position.y+QUAD_SIDE);
        (*quad)[3].position = sf::Vector2f(position.x,position.y+QUAD_SIDE);

	// define its 4 texture coordinates
	(*quad)[0].color = sf::Color::Red;
	(*quad)[1].color = sf::Color::Green;
	(*quad)[2].color = sf::Color::Blue;
	(*quad)[3].color = sf::Color::Magenta;
}


void Tetromino::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//operate on all the quads in this tetromino
	for(int i=0;i<NUM_QUADS;i++)
	{
		target.draw(*quad_[i]);
	}
}

void Tetromino::build(sf::Vector2f position,TetrominoType type)
{
	switch(type)
	{
		case SHAPE_I:
			buildQuad(quad_[0],position);
			buildQuad(quad_[1],(*(quad_[0]))[3].position);
			buildQuad(quad_[2],(*(quad_[1]))[3].position);
			buildQuad(quad_[3],(*(quad_[2]))[3].position);
			break;
		case SHAPE_J:
			buildQuad(quad_[0],position);
			buildQuad(quad_[1],(*(quad_[0]))[3].position);
			buildQuad(quad_[2],(*(quad_[1]))[3].position);
			buildQuad(quad_[3],
			sf::Vector2f((*(quad_[2]))[0].position.x - QUAD_SIDE,
			(*(quad_[2]))[0].position.y));
			break;
		case SHAPE_L:
			buildQuad(quad_[0],position);
			buildQuad(quad_[1],(*(quad_[0]))[3].position);
			buildQuad(quad_[2],(*(quad_[1]))[3].position);
			buildQuad(quad_[3],
			sf::Vector2f((*(quad_[2]))[0].position.x + QUAD_SIDE,
			(*(quad_[2]))[0].position.y));
			break;
		case SHAPE_Z:
			buildQuad(quad_[0],position);
			buildQuad(quad_[1],(*(quad_[0]))[1].position);
			buildQuad(quad_[2],(*(quad_[1]))[3].position);
			buildQuad(quad_[3],(*(quad_[2]))[1].position);
			break;
		default:
			abort();

	}
}
