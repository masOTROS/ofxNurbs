#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void mouseDragged(int x, int y, int button);

		void drawCtlPoints();

		ofEasyCam cam;

		ofPixels image;
		
		GLUnurbsObj *theNurb;

		GLuint textures[1];

		bool editMode;
		int editSelection;
};
