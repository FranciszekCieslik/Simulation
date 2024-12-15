#include "ofApp.hpp"

Force ofApp::wind(
    [](const Particle &p)
    {
        return ofVec3f(ofRandom(-4, 4), ofRandom(0, 0), ofRandom(-4, 4)); // Siła eksplozji
    },
    [](const Particle &p)
    {
        return p.position.y > 0;
    });

Force ofApp::gravity(
    [](const Particle &p)
    {
        return ofVec3f(0, -0.1, 0); // Grawitacja
    },
    [](const Particle &p)
    {
        return p.position.y > 0; // Działa tylko nad ziemią
    });

Force ofApp::lift(
    [](const Particle &p)
    {
        return ofVec3f(0, ofRandom(0, 2), 0); // Siła unoszenia
    },
    [](const Particle &p)
    {
        return true; // Aktywne, gdy cząsteczka żyje
    });

Force ofApp::spiral(
    [](const Particle &p)
    {
        return ofVec3f(sin(ofGetElapsedTimef()), 0, cos(ofGetElapsedTimef())) * 0.2f; // Spiralny ruch
    },
    [](const Particle &p)
    {
        return p.lifeTime > 0; // Aktywne, gdy cząsteczka żyje
    });

ofApp::ofApp() : snow(600,
                      ofVec3f(0, 600, 0),     // posmin
                      ofVec3f(200, 600, 200), // posmax
                      ofVec2f(1, 3),          // radius
                      ofVec2f(1, 1),          // mass
                      ofVec2f(10, 30),        // LifeTime
                      ofVec3f(0, 10, 0),      // velocity
                      ofVec3f(250, 250, 250), // maxcolors
                      ofVec3f(250, 250, 250)  // mincolors
                      ),
                 fire(200,
                      ofVec3f(0, 20, 0),      // posmin
                      ofVec3f(200, 20, 200),  // posmax
                      ofVec2f(2, 2),          // radius
                      ofVec2f(1, 1),          // mass
                      ofVec2f(10, 20),        // LifeTime
                      ofVec3f(1, 0, 1),       // velocity
                      ofVec3f(250, 100, 100), // maxcolors
                      ofVec3f(250, 100, 100)  // mincolors
                 )
{
}
//--------------------------------------------------------------
void ofApp::setup()
{
    ofBackground(0);     // Czarny kolor tła
    ofEnableDepthTest(); // Włącz test głębokości dla 3D
    ofEnableLighting();  // Włącz system oświetlenia

    // Konfiguracja światła
    pointLight.setPointLight();                               // Punktowe źródło światła
    pointLight.setPosition(300, 300, 300);                    // Pozycja światła
    pointLight.setDiffuseColor(ofFloatColor(1.0, 0.8, 0.8));  // Kolor rozproszony (ambient)
    pointLight.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0)); // Kolor refleksyjny (specular)

    // Konfiguracja materiału
    material.setDiffuseColor(ofColor(100, 255, 100));  // Kolor rozproszony obiektu
    material.setSpecularColor(ofColor(255, 255, 255)); // Kolor refleksyjny obiektu
    material.setShininess(64);                         // Stopień połysku (im wyższa wartość, tym bardziej błyszczące)

    glowShader.load("glow.vert", "glow.frag");
}

//--------------------------------------------------------------
void ofApp::update()
{
    // Animacja pozycji światła
    float time = ofGetElapsedTimef();
    pointLight.setPosition(300 * cos(time), 300, 300 * sin(time));

    snow.update();
    fire.update();

    snow.useMethod([&](Particle &p)
                   {
                       wind.applyTo(p);
                       gravity.applyTo(p); });

    fire.useMethod([&](Particle &p)
                   { 
            lift.applyTo(p);
            float time = ofGetElapsedTimef();
            p.velocity.x += ofNoise(p.position.x * 0.1, time) * 0.5;
            p.velocity.z += ofNoise(p.position.z * 0.1, time) * 0.5;
            float lifeRatio = 1.0f - (p.lifeTime / 100.0f);
            p.color = ofColor(255, 165 * lifeRatio, 0, 255 * lifeRatio);
            p.radius *= 0.95; 
            p.velocity= p.velocity.normalize() * ofRandom(2, 5); 
            spiral.applyTo(p); });

    if (ofGetElapsedTimeMillis() % 2000 < 100)
    {
        snow.addParticle(500);
        fire.addParticle(500);
    }
    fire.addParticle(150);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cam.begin();         // Rozpocznij tryb kamery
    pointLight.enable(); // Włącz źródło światła
    material.begin();    // Ustaw materiał

    snow.draw(material);

    ofEnableBlendMode(OF_BLENDMODE_ADD); // Additive blending
    glowShader.begin();
    glowShader.setUniform3f("glowColor", 1.0, 0.5, 0.0); // Orange
    glowShader.setUniform1f("glowIntensity", 2.0);
    fire.draw(material);
    glowShader.end();
    ofDisableBlendMode();

    material.end();       // Wyłącz materiał
    pointLight.disable(); // Wyłącz światło
    cam.end();            // Zakończ tryb kamery
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}