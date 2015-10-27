#include "AppDelegate.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Director *      g_director;
FileUtils *     g_file;
Size            g_win_size;
Size            g_vis_size;
SimpleAudioEngine *g_audio;

char            g_server_ip[32] = "192.168.191.3";
int             g_volume = 50;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize global objects
    g_director = Director::getInstance();
    g_file = FileUtils::getInstance();
    g_audio = SimpleAudioEngine::getInstance();

    {
        g_audio->preloadBackgroundMusic("bg1.mp3");
        g_audio->preloadBackgroundMusic("bg2.mp3");
        const char *sound_file[] = {
            "girl_aida",
            "girl_boxing",
            "girl_death",
            "girl_kick",
            "girl_special",

            "man_aida",
            "man_boxing",
            "man_death",
            "man_kick",
            "man_special",

            "man2_aida",
            "man2_boxing",
            "man2_death",
            "man2_kick",
            "man2_special"
        };
        for (int i = 0; i < 15; i++) {
            char buf[32];
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            sprintf(buf, "%s.%s", sound_file[i], "ogg");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            sprintf(buf, "%s.%s", sound_file[i], "wav");
#endif
            g_audio->preloadEffect(buf);
        }
    }

    auto glview = g_director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("GrassWar");
        g_director->setOpenGLView(glview);
    }

	g_vis_size = g_director->getVisibleSize();
    g_win_size = g_director->getWinSize();

    // turn on display FPS
    //g_director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    g_director->setAnimationInterval(1.0 / 60);

    auto screenSize = glview->getFrameSize();
    auto designSize = Size(960, 640);

    if (screenSize.height > 320) {
        auto resourceSize = Size(960, 640);
        g_director->setContentScaleFactor(resourceSize.height/designSize.height);
    }

    // Set the design resolution
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);

    {
        g_file->addSearchPath("scene/dialog_scene");
        g_file->addSearchPath("scene/menu_scene");
        g_file->addSearchPath("scene/room_scene");
        g_file->addSearchPath("scene/room_list_scene");
        g_file->addSearchPath("scene/role_scene");
        g_file->addSearchPath("scene/setting_scene");
        g_file->addSearchPath("scene/game_scene");
        g_file->addSearchPath("scene/help_scene");

        g_file->addSearchPath("Particle3D/materials");
        g_file->addSearchPath("Particle3D/scripts");
        g_file->addSearchPath("Particle3D/textures");

        g_file->addSearchPath("character");
        g_file->addSearchPath("fonts");
        g_file->addSearchPath("sound");
    }

    // create a scene. it's an autorelease object
    g_director->runWithScene(MenuScene::createScene());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
