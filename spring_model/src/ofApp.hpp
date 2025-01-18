#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    void mousePressed(int x, int y, int button) override;
    void mouseDragged(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;

private:
    void generateSphere(float radius, int resolution);
    void updateVerticesVerlet();

    ofEasyCam cam;
    ofLight light;
    ofMaterial material;

    ofMesh mesh;
    std::vector<glm::vec3> origin_positions;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> previousPositions; // For Verlet integration
    std::vector<glm::vec3> velocities;
    std::unordered_map<int, std::vector<int>> neighbors;

    int selectedVertex = -1;
};