#include <SFML/Graphics.hpp>
#include <Scene.hpp>

int main()
{
	mt::Scene scene(1920,1080);
	scene.SceneFromFile("input.txt");
	// scene.Ellipsoid({3,3,3}, 2);
	scene.LifeCycle();
	return 0;
}