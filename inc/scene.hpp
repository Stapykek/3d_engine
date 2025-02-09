#pragma once
#include <SFML/Graphics.hpp>
#include <camera.hpp>

namespace mt
{
	class Scene
	{
	public:
		Scene(int width, int height);
		~Scene();
		void LifeCycle();
		void SceneFromFile(std::string name);
		void Ellipsoid(Point center, double r);
	private:
		std::unique_ptr<Camera> m_camera;

		int m_width;
		int m_height;

		std::unique_ptr<sf::RenderWindow> m_window;
		std::unique_ptr<sf::Texture> m_texture;
		std::unique_ptr<sf::Sprite> m_sprite;

		Point* m_points = nullptr;
		int m_size = 0;
	};
}