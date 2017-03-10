#include "ofMain.h"

#include "ofxAlembic.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
    ofxAlembic::Reader metaball;
    ofxAssimpModelLoader torus;
    ofShader shader;
    ofSpherePrimitive lightsphere;
    ofVboMesh vbomesh;
    float angle;
    
    static const int PARTICLE_NUM = 2000;
    ofVboMesh vbomesh_particle;
    ofBoxPrimitive box;
    ofVec3f particlePos[PARTICLE_NUM];
    ofVec3f particleRot[PARTICLE_NUM];
    float particleSize[PARTICLE_NUM];
    
    ofxPanel gui;
    ofxFloatSlider speed;
    ofxFloatSlider box_size;
    
    ofEasyCam cam;
    bool bHide;
    
    //--------------------------------------------------------------
    void setup()
    {
        shader.load("shaders/flat");
        metaball.open("model/metaball.abc");
        metaball.dumpNames();
        
        torus.loadModel("model/torus.obj");
        torus.setScale(100, 100, 100);
        vbomesh = torus.getMesh(0);
        vbomesh.setMode(OF_PRIMITIVE_TRIANGLES);
        lightsphere.set(1, 5);
        
        
        box.set(15.0f);
        vbomesh_particle = box.getMesh();
        
        float R = 500;
        for(int i = 0; i < PARTICLE_NUM; i++)
        {
            particlePos[i] = ofVec3f(ofRandom(-R, R), ofRandom(-R, R), ofRandom(-R, R));
        }
        
        gui.setup();
        gui.add(speed.setup("speed", 0, 0, 10));
        gui.add(box_size.setup("box_size", 0, 0, 2));
        
    }
    
    //--------------------------------------------------------------
    void update()
    {
        float time = ofGetElapsedTimef();
        float t = fmodf(time, metaball.getMaxTime());
        metaball.setTime(t);
        
        angle += 0.05;
        angle = fmodf(angle, 360);
        
        ofVec3f lightPos = ofVec3f(200 * sin(time*0.1) * ofNoise(time*0.1+0.1), 0, 200 * cos(time*0.1) * ofNoise(time*0.1));
        lightsphere.setPosition(lightPos);
        
    }
    
    //--------------------------------------------------------------
    void draw()
    {
        
        ofBackground(0);
        glEnable(GL_DEPTH_TEST);
        
        float time = ofGetElapsedTimef();
        ofMesh mesh;
        metaball.get("/Metaball/MetaballShape", mesh);
        
        cam.begin();
        shader.begin();
        ofMatrix4x4 model;
        model.scale(0.1, 0.1, 0.1);
        model.translate(0, 0, 0);
        
        shader.setUniform3f("lightPos", lightsphere.getPosition() * ofGetCurrentViewMatrix());
        shader.setUniformMatrix4f("model", model);
        shader.setUniformMatrix4f("view", ofGetCurrentViewMatrix());
        shader.setUniformMatrix4f("projection", cam.getProjectionMatrix());
        shader.setUniform1i("normalReverse", 0);
        shader.setUniform1i("u_col", 0);
        mesh.draw();
        
        {
            ofMatrix4x4 model;
            model.scale(100, 100, 100);
            model.rotate(angle, 0, 1, 0);
            model.translate(900, 0, 0);
            shader.setUniformMatrix4f("model", model);
            shader.setUniform1i("normalReverse", 1);
            shader.setUniform1i("u_col", 1);
            vbomesh.draw();
        }
        
        
        
        {
            ofMatrix4x4 model;
            model.translate(lightsphere.getPosition());
            shader.setUniformMatrix4f("model", model);
            lightsphere.draw();
        }
        
        float angle = fmodf(time * 2, 360);
        for(int i = 0; i < PARTICLE_NUM; i++)
        {
            particlePos[i].z += speed * 4.0;
            ofMatrix4x4 model;
            model.scale(box_size, box_size, box_size);
            model.rotate(angle + i, 1.0, 0.4, 0.1);
            model.translate(particlePos[i]);
            
            shader.setUniformMatrix4f("model", model);
            shader.setUniform1i("u_col", 2);
            vbomesh_particle.draw();
            
            if(particlePos[i].z > 600) particlePos[i].z = -600;
        }
        
        shader.end();
        cam.end();
        
        
        glDisable(GL_DEPTH_TEST);
        if(!bHide){
            gui.draw();
        }
        
    }
    
    //--------------------------------------------------------------
    void keyPressed(int key)
    {
        if(key == 'h')
        {
            bHide = !bHide;
        }
        
    }
    
    //--------------------------------------------------------------
    void keyReleased(int key)
    {
        
    }
    
    //--------------------------------------------------------------
    void mouseMoved(int x, int y )
    {
        
    }
    
    //--------------------------------------------------------------
    void mouseDragged(int x, int y, int button)
    {
        
    }
    
    //--------------------------------------------------------------
    void mousePressed(int x, int y, int button)
    {
        
    }
    
    //--------------------------------------------------------------
    void mouseReleased(int x, int y, int button)
    {
        
    }
    
    //--------------------------------------------------------------
    void mouseEntered(int x, int y)
    {
        
    }
    
    //--------------------------------------------------------------
    void mouseExited(int x, int y)
    {
        
    }
    
    //--------------------------------------------------------------
    void windowResized(int w, int h)
    {
        
    }
    
    //--------------------------------------------------------------
    void gotMessage(ofMessage msg)
    {
        
    }
    
    //--------------------------------------------------------------
    void dragEvent(ofDragInfo dragInfo)
    {
        
    }
};


//==============================================================
int main()
{
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 1);
    settings.width = 1280;
    settings.height = 720;
    settings.resizable = false;
    ofCreateWindow(settings);
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
