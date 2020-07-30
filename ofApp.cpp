#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofColor color;
	vector<ofColor> color_list;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		color_list.push_back(color);
	}

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	
	for (int k = 0; k < 230; k++) {

		auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));

		ofMesh mesh;
		vector<glm::vec3> right, left, frame;

		glm::vec3 last_location;
		float last_theta;

		for (int i = 0; i < 25; i++) {

			auto location = glm::vec3(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i) * 0.0035), 0, 1, -350, 350), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i) * 0.0035), 0, 1, -350, 350), 0);
			auto next = glm::vec3(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i + 1) * 0.0035), 0, 1, -350, 350), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i + 1) * 0.0035), 0, 1, -350, 350), 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(i, 0, 25, 0, 8) * cos(theta + PI * 0.5), ofMap(i, 0, 25, 0, 8) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(i, 0, 25, 0, 8) * cos(theta - PI * 0.5), ofMap(i, 0, 25, 0, 8) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
		}

		for (int i = 0; i < right.size(); i++) {

			mesh.addVertex(left[i]);
			mesh.addVertex(right[i]);
		}


		for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

			mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
			mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
		}

		mesh.addVertex(last_location);
		int index = mesh.getNumVertices();
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(8 * cos(theta), 8 * sin(theta), 0));
			frame.push_back(last_location + glm::vec3(8 * cos(theta), 8 * sin(theta), 0));
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
		}

		ofSetColor(color_list[(int)ofRandom(color_list.size())]);
		mesh.draw();

		ofSetColor(239);
		ofNoFill();
		ofBeginShape();
		ofVertices(frame);
		reverse(right.begin(), right.end());
		ofVertices(right);
		ofVertices(left);
		ofEndShape();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}