#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#define U_POINTS	4
#define V_POINTS	4
#define POINT_DIM	3
#define TEX_DIM		2
#define TEX_POINTS	2
#define GRID_RES	20

class ofxNurbs{
public:  
    
	void setup(int u_points, int v_points);
	void draw();
	void begin();
	void end();

	void reset();
		
	void mouseMoved(ofMouseEventArgs &args);
	void mousePressed(ofMouseEventArgs &args);
	void mouseDragged(ofMouseEventArgs &args);
	void mouseReleased(ofMouseEventArgs &args);
	
	void save(string saveFile = "nurbs.xml");
	void load(string loadFile = "nurbs.xml");
	void saveToXml(ofxXmlSettings& XML);
	void loadFromXml(ofxXmlSettings& XML);
	
	void enableEditMode();
	void disableEditMode();
	void toggleEditMode();
	bool getEditMode();
    
    void setControlPoint(int index, ofPoint position);
    ofPoint getControlPoint(int index);
        
    void setCornerSensibility(float sensibility);
    float getCornerSensibility();

private:
	void drawCtlPoints();
	void resetCtlPoints();
	void updateMesh();

	ofPoint mouseToMesh(int x, int y);

	int x, y;
	int  width; //width of the quad to work with
	int	 height; // height of the quad to work with

	GLfloat ctlpoints[U_POINTS][V_POINTS][POINT_DIM];
	ofPoint corners[4];
		
	GLenum drawMode;

	GLuint textures[1];

	bool editMode;
	int editSelection;
};