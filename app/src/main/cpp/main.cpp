#include <jni.h>
#include <GLES3/gl32.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <iostream>
#include <vector>
#include "Program.h"
#include "Object3D.h"
#include "Camera.h"
#include "ObjectCreator.h"
#include "CameraControl.h"
#include "StaticModel.h"
AAssetManager* mgr = nullptr;
Program* program = nullptr;
Program* programD = nullptr;
Lighting* li = nullptr;
Object3D* terrain = nullptr;
Object3D* box = nullptr;
Camera* cam = nullptr;
CameraControl* cc = nullptr;
int widt, heigh;
bool isInitialized = false;
extern "C" {
    JNIEXPORT void JNICALL Java_com_dragonpav_dengine_Renderer_surfaceCreated(JNIEnv* env, jobject thiz) {
        AAsset* vertex = AAssetManager_open(mgr, "vertex.glsl", AASSET_MODE_UNKNOWN);
        AAsset* fragment = AAssetManager_open(mgr, "fragment.glsl", AASSET_MODE_UNKNOWN);
        AAsset* vertexDepth = AAssetManager_open(mgr, "vertex_depth.glsl", AASSET_MODE_UNKNOWN);
        AAsset* fragmentDepth = AAssetManager_open(mgr, "fragment_depth.glsl", AASSET_MODE_UNKNOWN);
        Shader vert(vertex, Shader::Type::Vertex);
        vert.compile();
        Shader frag(fragment, Shader::Type::Fragment);
        frag.compile();
        program = new Program(&vert, &frag);
        if (!program->link()) {
            __android_log_write(ANDROID_LOG_ERROR, "DEngine", "Failed to link a general program");
        }
        Shader vertD(vertexDepth, Shader::Type::Vertex);
        vertD.compile();
        Shader fragD(fragmentDepth, Shader::Type::Fragment);
        fragD.compile();
        programD = new Program(&vertD, &fragD);
        if (!programD->link()) {
            __android_log_write(ANDROID_LOG_ERROR, "DEngine", "Failed to link a depth program");
        }
        li = new Lighting(Vector3(0.5f, 2, 2), program);
        li->dirLight.specular.x = 0;
        li->dirLight.specular.y = 0;
        li->dirLight.specular.z = 0;
        li->pointLights[0].position.x = 0.5f;
        li->pointLights[0].position.y = 2;
        li->pointLights[0].position.z = 0;
        li->pointLights[0].specular.x = 0;
        li->pointLights[0].specular.y = 0;
        li->pointLights[0].specular.z = 0;
        RenderUtils* re = new RenderUtils(program, li, widt, heigh);
        cam = new Camera(re);
        cam->setPos(Vector3(4, 2, 2));
        AAsset* tex1 = AAssetManager_open(mgr, "sand.png", AASSET_MODE_BUFFER);
        Texture* sand = new Texture(program, tex1, Texture::Config());
        Texture* green = new Texture({0, 0xff, 0}, program, Texture::Config());
        terrain = new Object3D(ObjectCreator::createTerrain(Vector3(0, 0, 0), 200, 200, 0.03f, 0.02f, 0.4f, cam, sand));
        box = new Object3D(ObjectCreator::createBox(Vector3(0, 1, 0), Vector3(1, 1, 1), cam, green));
        cc = new CameraControl();
        cam->setCameraControl(cc);
        glEnable(GL_DEPTH_TEST);
    }
    JNIEXPORT void JNICALL Java_com_dragonpav_dengine_Renderer_surfaceChanged(JNIEnv* env, jobject thiz, jint width, jint height) {
        widt = width;
        heigh = height;
        glViewport(0, 0, width, height);
        cam->perspective(80, (float) widt / (float) heigh, 0.1f, 100.0f);
        isInitialized = true;
    }
    JNIEXPORT void JNICALL Java_com_dragonpav_dengine_Renderer_drawFrame(JNIEnv* env, jobject thiz) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (!isInitialized) return;
        li->beginShadows(programD);
        terrain->begin();
        terrain->render(programD, true);
        terrain->end();
        box->begin();
        box->render(programD, true);
        box->end();
        li->endShadows(widt, heigh, program);
        terrain->begin();
        terrain->render(program, false);
        terrain->end();
        box->begin();
        box->render(program, false);
        box->end();
    }
    JNIEXPORT void JNICALL Java_com_dragonpav_dengine_Renderer_setAssetMgr(JNIEnv* env, jobject thiz, jobject assetManager) {
        mgr = AAssetManager_fromJava(env, assetManager);
    }
    JNIEXPORT void JNICALL Java_com_dragonpav_dengine_Renderer_onTouch(JNIEnv* env, jobject thiz, jint action, jint x, jint y) {
        if (cc != nullptr && isInitialized) {
            cc->onTouch(action, x, y);
        }
    }
}