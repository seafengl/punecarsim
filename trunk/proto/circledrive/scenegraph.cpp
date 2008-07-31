
/* scenegraph.cpp: Functions for creating and managing the scenegraph. */

#include<osg/Node>
#include<osg/Geode>
#include<osg/Geometry>
#include<osg/ShapeDrawable>
#include<osg/MatrixTransform>
#include<osg/NodeCallback>
#include<osg/Notify>
#include<osg/Timer>
#include<osg/LineWidth>
#include<vector>
#include<sstream>
#include<iostream>
#include <cmath>
#include "scenegraph.h"
#include "car.h"

/* Create scene graph. */
osg::ref_ptr<osg::Group> create_scene_graph(Car * car)
{
	osg::ref_ptr<osg::Group> root_group = new osg::Group;

	osg::ref_ptr<osg::Geode> road_geode = new osg::Geode;
	road_geode->addDrawable(draw_road(R1, R2).get());

	/* 1. Add a circular road. */
	root_group->addChild(road_geode.get());

	/* 2. Add many cylinders. */
	for(int cc = 1; cc <= 10; cc++) {

		double theta = 2.0 * M_PI/10 * cc;
		root_group->addChild(draw_cylinder(R1, theta).get());
	}

	/* 3. Draw the car (dashboard, wheels etc). */
	root_group->addChild(car->draw().get());

	return root_group.get();
}

/* Create cylinders on both sides of the road. */
osg::ref_ptr<osg::MatrixTransform> draw_cylinder(double rad, double theta)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::ShapeDrawable> sd;

	osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3f(), 0.5, 5.0);
	sd = new osg::ShapeDrawable(cylinder.get());
	geode->addDrawable(sd.get());

	/* Kepp the cylinder at desired position. */
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	mt->setDataVariance(osg::Object::STATIC);

	osg::Matrix m;
	double x1 = 0.0, y1 = 0.0;

	x1 = rad * cos(theta);
	y1 = rad * sin(theta);
	m.makeTranslate(x1, y1, 2.5);
	mt->setMatrix(m);

	/* Add geode to matrix transform object. */
	mt->addChild(geode.get());

	return mt.get();
}


/* Draw the road. */
osg::ref_ptr<osg::Geometry> draw_road(double r1, double r2)
{
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	double theta, x1, y1, x2, y2;

	for(int i = 0; i <= QUAD_COUNT; i++) {

		theta = 2.0 * M_PI/QUAD_COUNT * i;
		x1 = r1 * cos(theta);
		y1 = r1 * sin(theta);
		x2 = r2 * cos(theta);
		y2 = r2 * sin(theta);

		v->push_back(osg::Vec3(x1, y1, 0));
		v->push_back(osg::Vec3(x2, y2, 0));
	}

 	geom->setVertexArray(v.get());
	osg::ref_ptr<osg::DrawArrays> da;
	da = new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, QUAD_COUNT*2); 
	geom->addPrimitiveSet(da.get());
		
	/* Color of the road. */
	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
	c->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));

	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	geom->setColorArray(c.get());

	return geom.get();
}

