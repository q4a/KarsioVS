//
// Created by Sebastian on 12/23/2017.
//
// Modified by Brian on 03/03/2018
// Based on implementation by Cody Claborn, at: https://codyclaborn.me/tutorial/making-a-basic-fmod-audio-engine-in-c.html
//

#ifndef RENDERENGINE_AUDIOENGINE_H
#define RENDERENGINE_AUDIOENGINE_H

#include "fmod_studio.hpp"
#include "fmod_studio.h"
#include <string>
#include <map>
#include <vector>
#include "..\glm\glm.hpp"
#include <math.h>
#include <iostream>
#include "..\AudioObserver.h"
#include <algorithm>

struct Implementation {
    Implementation();
    ~Implementation();

    void update();

    FMOD::Studio::System* mpStudioSystem;
    FMOD::System* mpSystem;

    FMOD::ChannelGroup *musicChannels;
    FMOD::ChannelGroup *soundChannels;

    int mnNextChannelId;

    typedef std::map<std::string, FMOD::Sound*> SoundMap;
    typedef std::map<int, FMOD::Channel*> ChannelMap;
    typedef std::map<std::string, FMOD::Studio::EventInstance*> EventMap;
    typedef std::map<std::string, FMOD::Studio::Bank*> BankMap;

    BankMap mBanks;
    EventMap mEvents;
    SoundMap mSounds;
    ChannelMap mChannels;    

};

class AudioEngine {
    static AudioEngine* globalInstance;
public:
    static void init();
    static void update();
    static void shutdown();
    static int ErrorCheck(FMOD_RESULT result);
    float musicVol = 0.5f;
    float soundVol = 0.5f;
    int musicChannel = -1;

    glm::vec3 listenerPos = glm::vec3(0, 0, 0);
    glm::vec3 lastPos = glm::vec3(0, 0, 0);
    glm::vec3 vel = glm::vec3(0,0,0);
    glm::vec3 forward = glm::vec3(0,0,1);
    glm::vec3 up = glm::vec3(0, 1, 0);
    

    AudioObserver* audioObserver = new AudioObserver();

    AudioEngine* getInstance();

    void setMusicVol(float newVol);
    void setSoundVol(float newVol);

    void bindObserver(AudioObserver* anObserver);
    void updateListenerPos(glm::vec3 newPos, glm::vec3 newfwd, glm::vec3 newUp, double frametime);
    void loadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
    void loadEvent(const std::string& strEventName);
    void loadSound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
    void loadMusic(const std::string& strMusicName);
    void unloadSound(const std::string& strSoundName);
    void update3dListener();
    int playSounds(const std::string& strSoundName, const glm::vec3& vPos, float fVolumedB);
    void playEvent(const std::string& strEventName);
    void stopChannel(int nChannelId);
    void stopEvent(const std::string& strEventName, bool bImmediate = false);
    void getEventParameter(const std::string& strEventName, const std::string& strEventParameter, float* parameter);
    void setEventParameter(const std::string& strEventName, const std::string& strParameterName, float fValue);
    void stopAllChannels();
    void setChannel3dPosition(int nChannelId, const glm::vec3& vPosition);
    void setChannelVolume(int nChannelId, float fVolumedB);
    bool isPlaying(int nChannelId) const;
    bool isEventPlaying(const std::string& strEventName) const;
    float dbToVolume(float db);
    float volumeTodb(float volume);
    FMOD_VECTOR vectorToFmod(const glm::vec3& vPosition);
};


#endif //RENDERENGINE_AUDIOENGINE_H
