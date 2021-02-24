#pragma once
#include <Rendering/Vec3.h>
#include <Rendering/Mesh.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

static double PI = 3.14159265359;
void solidCylinder(Mesh* obj, Vec3 from, Vec3 to, double fromRadius, double toRadius, int nbPointCircle = 10, float mergeDist = 0.00001f);