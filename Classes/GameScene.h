#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "SceneLayer.h"
#include "DrawNode3D.h"
#include <functional>

class World;
class SkillButton;

class SkillButton : ui::Button {    // 技能按钮
public:
    static void onSkillClicked(Ref *ref);
    static SkillButton *SkillIt(Button *);

    void cool(float time);                    // 冷却
    bool is_cooling();              // 是否正在冷却
};

class GameScene : public SceneLayer {
public:
    static Scene* createScene();
    static void ui2gl(Vec2 &v);                       // ui坐标转换成opengl坐标

    CREATE_FUNC(GameScene);
    bool init() override;

    void load_world();                                // 加载3D世界
    void load_ui(Node *root);                         // 加载UI

    static GameScene *Instance;
    static Layout *s_layout_ui;
    static ImageView *s_image_direction;              // 小地图中的方向图标
    // *********************************************  // 读条
    static ImageView *s_image_progress;               
    static LoadingBar *s_load_progress;
    // *********************************************  // 魔力条
    static LoadingBar *s_load_magic;
    // *********************************************  // 4个技能按钮
    static SkillButton *s_btn_boxing;
    static SkillButton *s_btn_kick;
    static SkillButton *s_btn_special;
    static SkillButton *s_btn_speed;
    // ********************************************   // 比分Text
    static Text *s_text_score_red;
    static Text *s_text_score_blue;
    static Text *s_text_grass;
                                                             // 成绩 == Grass Number
    static void set_small_direction(float);                  // 设置小地图中的方向
    static void set_score_red(int);                          // 设置红队的成绩
    static void set_score_blue(int);                         // 设置蓝队成绩
    static void set_score(int);                              // 设置自己的成绩
private:
    inline static void show_progress(bool s) {               // 设置显示进度条
        s_image_progress->setVisible(s);
    }
    inline static void set_progress(float percent) {         // 设置进度条的进度
        s_load_progress->setPercent(percent);
    }
    float _prog_second;                                      // 进度条总耗时
    float _cur_second;                                       // 当前进度已耗时
public:
    std::function<void(float , bool end)> _prog_cb;          // 进度回调
    void begain_progress(float seconds,                      // 启动进度条
        std::function<void (float, bool end)> cb = nullptr);
    void break_progress();                                   // 打断进度条
    void update_progress(float dt);                          // [定时器]更新进度条
    //
	void onScrollTouched(Ref *, Widget::TouchEventType);
	void onLayerTouched(Ref *, Widget::TouchEventType);
    void onMouseScroll(Event* event);
    void onPickUpClicked(Ref *);
};

#endif

