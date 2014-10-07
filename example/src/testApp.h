#pragma once

#include "ofMain.h"

#define U_POINTS	4
#define V_POINTS	4
#define POINT_DIM	3
#define TEX_DIM		2
#define TEX_POINTS	2
#define TEX_CORNERS	4
#define GRID_RES	20

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void mouseMoved(int x, int y);
		void mousePressed(int x, int y, int button);
		void mouseDragged(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		void drawCtlPoints();
		void resetMesh();
		void resetQuad();
		void updateMesh();
		void updateQuad();

		ofPoint mouseToMesh(int x, int y);
		
		ofPoint src[TEX_CORNERS];
		ofPoint corners[TEX_CORNERS];
		ofMatrix4x4 quadWarping;
		ofMatrix4x4 quadWarpingT;
		ofMatrix4x4 quadWarpingI;

		ofPoint controlPoints[U_POINTS][V_POINTS];
		ofPoint controlPointsQW[U_POINTS][V_POINTS];

		ofPixels image;
		ofTexture texture;
		
		GLenum drawMode;

		GLuint textures[1];

		bool editMode;
		bool editCorners;
		int editMeshSelection;
		int editQuadSelection;
};
