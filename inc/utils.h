
namespace Eureka {
template <typename T>
class ShapeProperties {
public:
	float x() { return T.getOrigin().x ;}
	float y() {return T.getOrigin().y ;};
	float left();
	float right();
	float top();
	float bottom();

}
} //namespace
