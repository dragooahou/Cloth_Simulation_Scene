#include "Rendering/RenderFunctions.h"


void solidCylinder(Mesh* obj, Vec3 from, Vec3 to, double fromRadius, double toRadius, int nbPointCircle, float mergeDist)
{
    // bon voila pleins de math sa mère bon courage ...

    // pour chaques couches
    Vec3 v = to - from; // vecteur directeur du cylindre
    double tDelta = (2.0 * PI) / ((double) nbPointCircle);
   
    // origine du cercle
    Vec3 o = from;

    // origine du cercle qui suit
    Vec3 o_n = to;

    //rayon
    double r = fromRadius;

    //rayon du cercle qui suis
    double r_n = toRadius;

    Vec3 lastPointCircleFrom;
    Vec3 lastPointCircleTo;
    Vec3 lastNormalPointCircleFrom;
    Vec3 lastNormalPointCircleTo;

    double k = 0;
    // pour chaques cercles
    for (size_t circle = 0; circle <= nbPointCircle; circle++)
    {
        double t = ((double)circle) * tDelta;

        double sinT = sin(t);
        double cosT = cos(t);

        double fu = std::atan((v.x * cosT  + v.z * sinT) / -v.y) + k * PI;
        double u = fu;

        double sinU = sin(u);
        double cosU = cos(u);

        Vec3 normalPointCircleFrom(
            cosT * cosU,
            sinU,
            sinT * cosU);

        Vec3 normalPointCircleTo(
            cosT * cosU,
            sinU,
            sinT * cosU);

        Vec3 pointCircleFrom(normalPointCircleFrom * r + o);
        Vec3 pointCircleTo(normalPointCircleTo * r_n + o_n);
        

        if (circle >= 1)
        {
            obj->addQuad(
                lastPointCircleFrom,
                lastPointCircleTo,
                pointCircleTo,
                pointCircleFrom,

                lastNormalPointCircleFrom,
                lastNormalPointCircleTo,
                normalPointCircleFrom,
                normalPointCircleTo,

                mergeDist);

            obj->texcoords.insert(obj->texcoords.end(), {(float)nbPointCircle/circle, 0});
            obj->texcoords.insert(obj->texcoords.end(), {(float)nbPointCircle/circle, 1});
            obj->texcoords.insert(obj->texcoords.end(), {(float)nbPointCircle/(circle+1), 1});
            obj->texcoords.insert(obj->texcoords.end(), {(float)nbPointCircle/(circle+1), 0});

        }

        lastPointCircleTo = pointCircleTo;
        lastPointCircleFrom = pointCircleFrom;
        lastNormalPointCircleTo = normalPointCircleTo;
        lastNormalPointCircleFrom = normalPointCircleFrom;

        //if (circle ==(int) (3.0*nbPointCircle / 4.0) k = 1;
    }


}