#ifndef __WORLD_H__
#define __WORLD_H__

#include <functional>

#include "cocos2d.h"
#include "DrawNode3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

USING_NS_CC;

//#define VIEW_TEST
#define MAX_POINT_NUM 20              // 最大点数
#define MAX_GOODS_COUNT 8             // 最大物品数量
#define MAX_CAMERA_HEIGHT 300         // Camera最大高度
#define MIN_CAMERA_HEIGHT 80          // Camera最低高度
#define INTERVAL_GEN_GOODS 9.f        // 产生物品的时间间隔

class QuatNode {
public:
    static QuatNode *Import(char *file);

    QuatNode();
    QuatNode(QuatNode *parent, Vec2& d, Vec2& u);

    int Parse(const char *p);          // 解析包含碰撞信息的文件
    bool Export(char *file);           // 将当前碰撞信息导出到文件
    void Export(FILE *f);

    bool Split(Vec2& sp);              // 根据一个点来分割当前区域
    void unSplit();                    // 删除分割了的子区域`
    ~QuatNode();

    QuatNode *getChild(Vec2& pos);     // 递归获得子区域
    QuatNode *getChildOnce(Vec2& pos); // 不递归子区域
    bool contained(Vec2& pos);         // 是否包含该点
    inline bool block() {              // 是否为阻塞区域
        return _block;
    }
    inline void block(bool b) {        // 设置为(非)阻塞区域
        _block = b;
    }
    inline bool split() {              // 是否已经分割
        return !_split.isZero();
    }

    bool _block = false;               // is block area
 
    Vec2 dd, uu; /* u: up, d: down */
    Vec2 _split = Vec2::ZERO;          /* point which splited the area */
    QuatNode *Parent = nullptr; // 
    QuatNode *_child[2][2];  // x,y

#ifdef VIEW_TEST
    void DrawBlock(float height = 0);
    void DrawSplit(float height = 0);
    void DrawOutline(float height = 0);
    void DrawHeight(DrawNode3D *draw);

    inline void Clear() {
        _draw_node->clear();
    }

    static Color4F s_color;
    DrawNode3D *_draw_node = nullptr;
#endif
};

struct GoodsBase {
    enum Type {
        GRASS = 0,
        SHOES,
        WEAPON,

        TYPE_NUMBER
    };

    Type type;
    int  count;
    int  index;
};

struct Goods : public GoodsBase {
    Vec2 get_position() {
        auto p3 = sprite->getPosition3D();
        return Vec2(p3.x, p3.z);
    }

    Goods();
    Goods(GoodsBase *);
    ~Goods();

    Sprite3D *sprite = nullptr;
};

class World : public Node
{
public:
    enum {
        CAMERA_FREE = (int)CameraFlag::USER1,       // 自由视角摄像机
        CAMERA_FIX = (int)CameraFlag::USER2         // 固定视角摄像机
    };

    static Vec3 s_camera_offset;
	static World *getInstance();

    inline Terrain *terrain() {
        return _terrain;
    }
    inline float height(float x, float z) {
        return _terrain->getHeight(x, z);
    }
    inline DrawNode3D *draw_node() {
        return _drawNode;
    }
    Camera *camera();                                  // 当前相机
    void show_click(const Vec3& v);                    // 显示点击效果
    bool conv2space(Vec3& v); //ignore v.z
    void draw_grid(float cell = 10.f,
                   float height = 0.f);                // 画格子
    void add_skybox();                                 // 天空盒子

    inline void begin_gen_goods() {                    // 开始产生物品
        // 每隔 INTERVAL_GEN_GOODS 秒检测并生成一次goods
        schedule(schedule_selector(World::update_goods),
                                   INTERVAL_GEN_GOODS); 
    }

    void camera_zoom(float factor);                    // 伸缩镜头
    void camera_move(Vec2& factor);                    // 移到镜头
    void camera_follow(Node *node);                    // 镜头跟随Node

    bool load_goods(char *file);                       // 加载物品
    void load_collision(char *file);                   // 加载碰撞区域

    bool is_collision(Vec2 &pos);                      // 所给点是否在碰撞区域
    inline bool is_collision(Vec3& pos) {
        Vec2 v2(pos.x, pos.z);
        return is_collision(v2);
    }

    void set_position(Node*, Vec2& pos);               // 设置一个Node在世界中的位置
    void add_thing(Node *node,                         // 向世界中添加一个物体
                   float x = 0.f, float z = 0.f);
    inline void add_thing(Node *node, Vec2& pos) {
        return add_thing(node, pos.x, pos.y);
    }
    inline void add_thing(Node *node, Vec3& pos) {
        return add_thing(node, pos.x, pos.z);
    }
    inline void on_gen_goods(std::function<void(GoodsBase*)> cb) {
        _on_gen_goods = cb;
    }

    void update_goods(float dt);                       // [定时器]更新世界中的物品

    int goods_count();
    void dec_goods(int index);                         // 从世界中删除一个物品
    void add_goods(GoodsBase *good);                   // 向世界中添加一个物品
    bool get_goods(Vec2& pos, Goods *);                // 获取指定位置的物品

private:
	World();

    Terrain *_terrain;                      // 地形
    Camera *_camera_free;                   // 自由视角相机
    Camera *_camera_fix;                    // 固定视角相机
    QuatNode *_colli_root;                  // 根碰撞区域

    DrawNode3D *_drawNode;
    PUParticleSystem3D *_pu_click_point;

    Goods *_goods[MAX_POINT_NUM];
    std::function<void(GoodsBase *)> _on_gen_goods  = nullptr;
};

#endif /* __WORLD_H__ */
