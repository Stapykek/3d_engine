#include <scene.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
namespace mt
{
	Scene::Scene(int width, int height)
	{
		m_width = width;
		m_height = height;
		m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(m_width, m_height), "3D Engine");	//здесь создаётся окно
		m_texture = std::make_unique<sf::Texture>(); 													//текстура - изображение в которое идут все точки, которые просчитала камера 
		m_texture->create(m_width, m_height); 															//обозначаем размер текстуры
		m_sprite = std::make_unique<sf::Sprite>(*m_texture);											//спрайт - класс который можно отрисовать,
																										//в него мы подаём текстуру(картинку с камеры)
		Intrinsic intrinsic = { 960.0, 540.0, 960.0, 540.0 };
		Point position = { 0.0, 0.0, 0.0 };
		Angles angles = { 0.0,0.0,0.0 };
		m_camera = std::make_unique<Camera>(m_width, m_height, intrinsic, position, angles);
		/*std::make_unique<Type>() создаёт unique_ptr - один из "умных" указателей. Такие указатели (в отличие от необработанных)
		могут автоматически удалять объекты когда это нужно.unique_ptr владеет динамическим ресурсом, не даёт другим указателям
		указывать на него. Удаляет объект при его выходе из области видимости. Также unique_ptr запрещает копирование
		shared_ptr владеет динамическим ресурсом, но допускает множественные ссылки. Для того, чтобы понять, когда удалять объект, у него есть внутренний
		счётчик. Объект удаляется, когда счётчик становится равным 0. счётчик увеличивается на 1, когда создаётся новый shared_ptr, и уменьшается, когда
		этот shared_ptr выходит из области видимости.
		weak_ptr - по сути - shared_ptr, не увеличивающий внутренний счётчик. Пример использования: решение проблемы циклических ссылок.
		*/
		m_points = new Point[200000]; //у нас может быть всего 200000 точек.

		/*double r = 1;
		//здесь происходит генерация поверхностей. Я перенастроил их так, чтобы генерировался один эллипсоид
		for(double fi = 0; fi < 3.14; fi += 0.01)
			for (double teta = 0; teta < 3.1415; teta += 0.01)
			{
				m_points[m_size].x = r * sin(teta) * cos(fi);
				m_points[m_size].y = r * sin(teta) * sin(fi) + 5;
				m_points[m_size].z = r * cos(teta);
				m_size++;
			}

		for(double fi = 0; fi < 3.14; fi += 0.01)
			for (double teta = 0; teta < 3.1415; teta += 0.01)
			{
				m_points[m_size].x = -r * sin(teta) * cos(fi);
				m_points[m_size].y = -r * sin(teta) * sin(fi) + 5;
				m_points[m_size].z = -r * cos(teta);
				m_size++;
			}*/
		
	}
	Scene::~Scene()
	{
		if (m_points != nullptr)
			delete[] m_points;
	}

	void Scene::LifeCycle()
	{
		double y0 = 1;

		while (m_window->isOpen()) {
			sf::Event event;
			while (m_window->pollEvent(event))
				if (event.type == sf::Event::Closed)
					m_window->close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				//m_window->close();
				//m_window->create(sf::VideoMode(m_width, m_height), "3D Engine", sf::Style::Fullscreen);
				m_camera->dZ(0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				//m_window->close();
				//m_window->create(sf::VideoMode(m_width, m_height), "3D Engine");
				m_camera->dZ(-0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				m_camera->dX(-0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				m_camera->dX(0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				m_camera->dPitch(-0.02);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				m_camera->dPitch(0.02);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				m_camera->dRoll(-0.02);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				m_camera->dRoll(0.02);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
				m_window->close();
			}
			
			// #define WTF //здесь код для генерации поверхности, которая при этом постоянно удаляется от камеры..
			#ifdef WTF
			y0 += 0.02;
			m_size = 0;
			double r = 1;
			for (double fi = 0; fi < 6.28; fi += 0.01)
				for (double teta = 0; teta < 1.57; teta += 0.01)
				{
					m_points[m_size].x = r * sin(teta) * cos(fi);
					m_points[m_size].y = r * sin(teta) * sin(fi) + y0;
					m_points[m_size].z = r * cos(teta);
					m_size++;
				}

			// ������������� �����

			#endif
			for (int i = 0; i < m_size; i++)
				m_camera->ProjectPoint(m_points[i], { 0, 255 ,0, 255 });
			//здесь были прописаны значения 1920 1080, а не m_width и m_height из-за чего программа крашилась, если поставить не full hd разрешение.
			m_texture->update((uint8_t*)m_camera->Picture(), m_width, m_height, 0, 0); 
			m_camera->Clear();


			m_window->clear();
			m_window->draw(*m_sprite);

			m_window->display();

		}
	}

	void Scene::SceneFromFile(std::string name){
		std::ifstream in(name);

		if (!(in.is_open()))
			return;

		while(!in.eof()){
			Point tmp;
			in >> tmp.x;
			in >> tmp.y;
			in >> tmp.z;
			m_points[m_size] = tmp;
			m_size++;
		}
		in.close();
	}

	void Scene::Ellipsoid(Point center, double r){
		//здесь происходит генерация поверхностей. Я перенастроил их так, чтобы генерировался один эллипсоид
		for(double fi = 0; fi < 3.14; fi += 0.01)
			for (double teta = 0; teta < 3.1415; teta += 0.01)
			{
				double a;
				double b;
				double c;
				a = r * sin(teta) * cos(fi);
				b = r * sin(teta) * sin(fi);
				c = r * cos(teta);

				m_points[m_size].x = a + center.x;
				m_points[m_size].y = b + center.y;
				m_points[m_size].z = c + center.z;
				m_size++;
				m_points[m_size].x = -a + center.x;
				m_points[m_size].y = -b + center.y;
				m_points[m_size].z = -c + center.z;
				m_size++;
			}
	}
}