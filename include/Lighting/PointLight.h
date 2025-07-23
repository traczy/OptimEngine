#ifndef POINTLIGHT_H
#define POINTLIGHT_H

class PointLight
{

public:
    PointLight();
    PointLight(float x, float y, float z, float r, float g, float b);

    float getX() { return x; }
    float getY() { return y; }
    float getZ() { return z; }

    float getRed() { return r; }
    float getGreen() { return g; }
    float getBlue() { return b; }

    void setLocation(float x, float y, float z);

    /*!
        r, g, and b should be a float between 0 and 1
    */
    void setColor(float r, float g, float b);

private:
    float x;
    float y;
    float z;

    float r;
    float g;
    float b;

};

#endif // POINTLIGHT_H