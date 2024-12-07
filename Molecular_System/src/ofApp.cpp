#include "ofApp.hpp"

Force ofApp::wind(
    [](const Particle &p) -> ofVec3f
    {
        return ofVec3f(0.3f, 0.0f, 0.0f); // Stała siła w osi X
    },
    [](const Particle &p) -> bool
    {
        return p.position.y > 0.0f; // Wiatr działa tylko nad płaszczyzną Y=0
    });

Force ofApp::gravity(
    [](const Particle &p) -> ofVec3f
    {
        ofVec3f center(0, 0, 0);
        ofVec3f direction = center - p.position;
        float distance = direction.length();
        float strength = 9.8f / (distance * distance + 1.0f); // Prawo odwrotnego kwadratu
        direction.getNormalized();
        return direction * strength * 50;
    },
    [](const Particle &p) -> bool
    {
        return true;
    });

void ofApp::handleCollision(Particle &p1, Particle &p2)
{
    ofVec3f delta = p2.position - p1.position;
    float distance = delta.length();

    // Wektor normalny
    ofVec3f normal = delta.getNormalized();

    // Prędkości normalne
    ofVec3f v_n1 = normal * (p1.velocity.dot(normal));
    ofVec3f v_n2 = normal * (p2.velocity.dot(normal));

    // Prędkości styczne
    ofVec3f v_t1 = p1.velocity - v_n1;
    // ofVec3f v_t2 = p2.velocity - v_n2;

    // Nowe prędkości
    p1.velocity = v_t1 + v_n2;
    // p2.velocity = v_t2 + v_n1;

    // Korekta pozycji, aby uniknąć nakładania się kul
    float overlap = 0.5f * (p1.radius + p2.radius - distance);
    p1.position -= normal * overlap;
    // p2.position += normal * overlap;
}

ofApp::ofApp() : particles(1000,
                           ofVec3f(ofGetWindowWidth() - 40, ofGetWindowHeight() - 40, -ofGetWindowHeight()), // pos
                           ofVec2f(5, 20),                                                                   // radius
                           ofVec2f(1, 10),                                                                   // mass
                           ofVec2f(100, 200),                                                                // LifeTime
                           5.0f),                                                                            // velocity
                 sphere(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), ofColor(255, 0, 0), 250, 50, 100000)

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

    isDragging = false;
}

//--------------------------------------------------------------
void ofApp::update()
{
    // Animacja pozycji światła
    float time = ofGetElapsedTimef();
    pointLight.setPosition(300 * cos(time), 300, 300 * sin(time));

    particles.update();

    particles.useMethod([&](Particle &p)
                        { 
        wind.applyTo(p);
        gravity.applyTo(p);
        if(p.checkCollision(sphere)){
            handleCollision(p,sphere);
        }; });

    if(particles.size() < 700){
        particles.addParticle(300);
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cam.begin();         // Rozpocznij tryb kamery
    pointLight.enable(); // Włącz źródło światła
    material.begin();    // Ustaw materiał

    particles.draw(material);
    sphere.draw(material);

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
    if (isDragging)
    {
        sphere.position = ofVec3f(x, y, 0) + dragOffset; // Aktualizacja pozycji sfery
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    ofVec3f mousePosition(x, y, 0);
    if (mousePosition.distance(sphere.position) <= sphere.radius)
    {
        isDragging = true;
        dragOffset = sphere.position - mousePosition; // Ustawienie offsetu
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    isDragging = false; // Zatrzymanie przeciągania
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