#pragma once
#include "ofMain.h"
#include "Particles.hpp"
#include "Particle.hpp"
#include "Force.hpp"

class ofApp : public ofBaseApp
{
public:
	ofApp();
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	Particles snow; // Cząsteczki
	bool isDragging;	// Czy aktualnie przeciągamy sferę
	ofVec3f dragOffset; // Offset pomiędzy punktem kliknięcia a środkiem sfery

	static Force wind;
	static Force gravity;

	ofLight pointLight;	 // Punktowe źródło światła
	ofMaterial material; // Materiał dla obiektów
	void handleCollision(Particle &p1, Particle &p2);

	ofEasyCam cam; // Kamera łatwa w obsłudze
};
