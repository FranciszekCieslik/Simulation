#include "ofApp.hpp"
#include <limits>

//--------------------------------------------------------------
void ofApp::setup()
{
    ofBackground(30);
    ofSetFrameRate(60);
    ofEnableDepthTest();

    light.setPosition(200, 300, 400);

    // Generowanie sfery
    float radius = 200.0f;
    int resolution = 20;
    generateSphere(radius, resolution);

    // Inicjalizacja pozycji wcześniejszych (Verlet)
    previousPositions = positions;
    origin_positions = positions;
    velocities.resize(positions.size(), glm::vec3(0));
}

//--------------------------------------------------------------
void ofApp::update()
{
    updateVerticesVerlet();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cam.begin();

    light.enable();
    material.begin();

    mesh.drawWireframe();

    material.end();
    light.disable();

    // Rysowanie wybranego wierzchołka
    if (selectedVertex >= 0)
    {
        ofSetColor(255, 50, 50);
        ofDrawSphere(positions[selectedVertex], 0.05);
        ofSetColor(255);
    }

    cam.end();
}

//--------------------------------------------------------------
void ofApp::generateSphere(float radius, int resolution)
{
    mesh.clear();
    positions.clear();
    neighbors.clear();

    // Tworzenie sfery (siatki trójkątów)
    for (int y = 0; y <= resolution; y++)
    {
        for (int x = 0; x <= resolution; x++)
        {
            float theta = ofMap(x, 0, resolution, 0, TWO_PI);
            float phi = ofMap(y, 0, resolution, 0, PI);

            glm::vec3 pos = {
                radius * sin(phi) * cos(theta),
                radius * sin(phi) * sin(theta),
                radius * cos(phi)};
            positions.push_back(pos);
            mesh.addVertex(pos);
            mesh.addNormal(glm::normalize(pos));
        }
    }

    // Tworzenie indeksów i sąsiedztwa
    int numVertices = resolution + 1;
    for (int y = 0; y < resolution; y++)
    {
        for (int x = 0; x < resolution; x++)
        {
            int i0 = y * numVertices + x;
            int i1 = i0 + 1;
            int i2 = (y + 1) * numVertices + x;
            int i3 = i2 + 1;

            mesh.addTriangle(i0, i1, i2);
            mesh.addTriangle(i1, i3, i2);

            neighbors[i0].push_back(i1);
            neighbors[i0].push_back(i2);
            neighbors[i1].push_back(i0);
            neighbors[i1].push_back(i3);
            neighbors[i2].push_back(i0);
            neighbors[i2].push_back(i3);
            neighbors[i3].push_back(i1);
            neighbors[i3].push_back(i2);
        }
    }
}

//--------------------------------------------------------------

void ofApp::updateVerticesVerlet()
{
    float springConstant = 0.4f;      // Stała sprężystości
    float dampingFactor = 0.98f;      // Współczynnik tłumienia
    float deltaTime = 1.0f / 60.0f;   // Stały krok czasowy
    float returnForceStrength = 0.2f; // Siła powrotu do pozycji początkowej

    for (int i = 0; i < positions.size(); i++)
    {
        glm::vec3 current = positions[i];
        glm::vec3 previous = previousPositions[i];

        // Siły działające na wierzchołek
        glm::vec3 acceleration(0.0f);

        // Iteracja po sąsiadach
        for (int neighbor : neighbors[i])
        {
            glm::vec3 neighborPos = positions[neighbor];
            glm::vec3 direction = neighborPos - current;
            float distance = glm::length(direction);

            if (distance > 0)
            {
                float restLength = glm::length(origin_positions[neighbor] - origin_positions[i]);
                float maxDistance = 2.0f * restLength; // Maksymalna odległość
                float minDistance = 0.5f * restLength; // Minimalna odległość

                // Sprężyna działa, tylko jeśli odległość wykracza poza zakres [minDistance, maxDistance]
                if (distance > maxDistance)
                {
                    direction = glm::normalize(direction);
                    acceleration += springConstant * (distance - maxDistance) * direction;
                }
                else if (distance < minDistance)
                {
                    direction = glm::normalize(direction);
                    acceleration += springConstant * (distance - minDistance) * -direction;
                }
            }
        }

        // Siła powrotu do pozycji początkowej
        glm::vec3 toOrigin = origin_positions[i] - current;
        acceleration += returnForceStrength * toOrigin;

        // Dodanie tłumienia
        glm::vec3 velocity = (current - previous) * dampingFactor;

        // Aktualizacja pozycji wierzchołka
        glm::vec3 newPosition = current + velocity + acceleration * deltaTime * deltaTime;

        // Zapis nowego stanu
        previousPositions[i] = current;
        positions[i] = newPosition;

        // Aktualizacja pozycji wierzchołka w siatce
        mesh.setVertex(i, newPosition);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    glm::vec2 mousePos(x, y);
    float closestScreenDistance = std::numeric_limits<float>::max();
    float closestDepth = std::numeric_limits<float>::max();
    int closestIndex = -1;

    for (int i = 0; i < positions.size(); i++)
    {
        // Rzutowanie wierzchołka na ekran
        glm::vec3 screenPos = cam.worldToScreen(positions[i]);

        // Odległość na płaszczyźnie ekranu
        float screenDistance = glm::distance(glm::vec2(screenPos.x, screenPos.y), mousePos);

        // Głębokość (im mniejsze Z, tym bliżej kamery)
        float depth = screenPos.z;

        // Aktualizacja najbliższego wierzchołka
        if (screenDistance < closestScreenDistance ||
            (screenDistance == closestScreenDistance && depth < closestDepth))
        {
            closestScreenDistance = screenDistance;
            closestDepth = depth;
            closestIndex = i;
        }
    }

    // Ustawienie wybranego wierzchołka, jeśli znaleziono
    if (closestIndex >= 0 && closestScreenDistance < 10.0f)
    { // Próg odległości na ekranie
        selectedVertex = closestIndex;
        cam.disableMouseInput();
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    if (selectedVertex >= 0)
    {
        positions[selectedVertex] = cam.screenToWorld(glm::vec3(x, y, 0));
        mesh.setVertex(selectedVertex, positions[selectedVertex]);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    selectedVertex = -1;
    cam.enableMouseInput();
}
