#include <iostream>

#include "camera\Camera.h"

using namespace std;

void main()
{
    cout << "Program was initialized successfully!" << endl;
    Camera::getCamera().renderScene();
}