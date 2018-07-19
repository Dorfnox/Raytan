#include <QApplication>

#include "Window.h"
#include "Raytan.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

#include "Sphere.hpp"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "Cube.hpp"
#include "Subtraction.hpp"

static void	add_to_scene(Scene& scene)
{
	scene.SetAmbient(glm::dvec3(0.05, 0.05, 0.05));
	scene.lights.push_back((Light){{-1, 0, 0}, {1, 1, 1}});

	Material m1;
	m1.diffuse = 1;
	m1.reflect = 0;
	m1.refract = 0;
	m1.refractiveIndex = 1.4;
	m1.materialSampler = nullptr;
	m1.color = glm::dvec3(1, 1, 0.5);
	m1.colorSampler = new Sampler("assets/image.png");
	m1.normalSampler = nullptr;

	Material m2;
	m2.diffuse = 0.2;
	m2.reflect = 0;
	m2.refract = 0.8;
	m2.refractiveIndex = 1.4;
	m2.materialSampler = nullptr;
	m2.color = glm::dvec3(0.4, 1, 0.4);
	m2.colorSampler = nullptr;
	m2.normalSampler = nullptr;

	Material m3;
	m3.diffuse = 1;
	m3.reflect = 0;
	m3.refract = 0;
	m3.refractiveIndex = 1.4;
	m3.materialSampler = nullptr;
	m3.color = glm::dvec3(0.1, 0.4, 0.9);
	m3.colorSampler = nullptr;
	m3.normalSampler = nullptr;
	
	Sphere *s1 = new Sphere;
	s1->position = glm::dvec3(0, 0, 0);
	s1->rotation = glm::dvec3(90, 0, 0);
	s1->material = m1;
	
	s1->radius = 0.1;

	scene.AddObject(s1);

	Plane *p1 = new Plane;
	p1->position = glm::dvec3(1, 0, 0);
	p1->rotation = glm::dvec3(0, 0, 90);
	p1->material = m1;
	
	scene.AddObject(p1);

	Cylinder *c1 = new Cylinder;
	c1->position = glm::dvec3(0, 0.4, 0);
	c1->rotation = glm::dvec3(30, 40, 140);
	c1->material = m2;

	c1->radius = 0.04;

	scene.AddObject(c1);

	Cone *cone1 = new Cone;
	cone1->position = glm::dvec3(0, -0.3, 0);
	cone1->rotation = glm::dvec3(0, 40, 40);
	cone1->material = m2;

	cone1->angle = 10;

	scene.AddObject(cone1);

	Cube *cube = new Cube;
	cube->position = glm::dvec3(0, 0.3, -0.3);
	cube->rotation = glm::dvec3(0, 0, 0);
	cube->material = m3;

	cube->size = glm::dvec3(0.2, 0.1, 0.3);

	scene.AddObject(cube);
	
}

int	main(int argc, char *argv[])
{
	/*
	** Later the camera a scene will be read from
	** the scene file here.
	*/
	Scene scene;

	add_to_scene(scene);
	
	glm::dvec3 pos = {-1.3, 0, 0};
	glm::dvec3 dir = {1, 0, 0};
	Camera camera(pos, glm::normalize(dir), glm::dvec3(0, 1, 0), 45, 1);
	
	// Initialize Qt Application with the scene and camera
	QApplication qt(argc, argv);
	Window window(scene, camera);
	window.show();
	return qt.exec();
}
