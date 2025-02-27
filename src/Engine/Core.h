//
// Created by Sebastian on 12/23/2017.
//

#ifndef RENDERENGINE_CORE_H
#define RENDERENGINE_CORE_H

#include "RenderEngine/RenderEngine.h"
#include "PhysicsEngine/PhysicsEngine.h"
#include "AudioEngine/AudioEngine.h"


#include "../Main/initOpenGL/OpenGL_Deps.h"

class Core {
private:
    struct Properties{
        GLFWwindow *window; // Create glfw window
        int *screenWidth,*screenHeight;
        //OpenGL openGL_Program;
        bool isRunning;
        //all the stuff we might want to allow the game logic to modify but will take effect within the core engine
        bool isPaused;
        bool inMainMenu;
        bool isGameInitialized;
        bool isIngameMenuInitialized;
		bool isInUpgradeMenu;
    };
    Properties properties;
    const int MAX_PHYSICS_STEPS_PER_FRAME = 3;
    void runGame();
    void runMenu();
    void runPauseMenu();
    void runEndGameMenu();
	void runUpgradeMenu();
public:
    Core(int *screenWidth,int *screenHeight, GLFWwindow *window, bool gamePaused);
    void coreLoop();
    ~Core();
    static GLFWwindow* globalWindow;
	static int upgradeLizardId;
    static float simtimeSinceStartup;
    static float realtimeSinceStartup;
    static RenderEngine* renderEngine;
    static std::string dataPath;
    static std::vector<Camera*> cameras;
    static bool loadOnlyBoxes;
    static Entity* playerVehicle;
    static glm::vec3 menuBaseTextColor;
    static glm::vec3 menuSelectedTextColor;
};


#endif //RENDERENGINE_CORE_H
