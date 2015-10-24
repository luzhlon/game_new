#ifndef __WORLD_H__
#define __WORLD_H__

#include <functional>

#include "cocos2d.h"
#include "DrawNode3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

USING_NS_CC;

//#define VIEW_TEST
#define MAX_POINT_NUM 20              // ������
#define MAX_GOODS_COUNT 8             // �����Ʒ����
#define MAX_CAMERA_HEIGHT 300         // Camera���߶�
#define MIN_CAMERA_HEIGHT 80          // Camera��͸߶�
#define INTERVAL_GEN_GOODS 9.f        // ������Ʒ��ʱ����

class QuatNode {
public:
    static QuatNode *Import(char *file);

    QuatNode();
    QuatNode(QuatNode *parent, Vec2& d, Vec2& u);

    int Parse(const char *p);          // ����������ײ��Ϣ���ļ�
    bool Export(char *file);           // ����ǰ��ײ��Ϣ�������ļ�
    void Export(FILE *f);

    bool Split(Vec2& sp);              // ����һ�������ָǰ����
    void unSplit();                    // ɾ���ָ��˵�������`
    ~QuatNode();

    QuatNode *getChild(Vec2& pos);     // �ݹ���������
    QuatNode *getChildOnce(Vec2& pos); // ���ݹ�������
    bool contained(Vec2& pos);         // �Ƿ�����õ�
    inline bool block() {              // �Ƿ�Ϊ��������
        return _block;
    }
    inline void block(bool b) {        // ����Ϊ(��)��������
        _block = b;
    }
    inline bool split() {              // �Ƿ��Ѿ��ָ�
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
        CAMERA_FREE = (int)CameraFlag::USER1,       // �����ӽ������
        CAMERA_FIX = (int)CameraFlag::USER2         // �̶��ӽ������
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
    Camera *camera();                                  // ��ǰ���
    void show_click(const Vec3& v);                    // ��ʾ���Ч��
    bool conv2space(Vec3& v); //ignore v.z
    void draw_grid(float cell = 10.f,
                   float height = 0.f);                // ������
    void add_skybox();                                 // ��պ���

    inline void begin_gen_goods() {                    // ��ʼ������Ʒ
        // ÿ�� INTERVAL_GEN_GOODS ���Ⲣ����һ��goods
        schedule(schedule_selector(World::update_goods),
                                   INTERVAL_GEN_GOODS); 
    }

    void camera_zoom(float factor);                    // ������ͷ
    void camera_move(Vec2& factor);                    // �Ƶ���ͷ
    void camera_follow(Node *node);                    // ��ͷ����Node

    bool load_goods(char *file);                       // ������Ʒ
    void load_collision(char *file);                   // ������ײ����

    bool is_collision(Vec2 &pos);                      // �������Ƿ�����ײ����
    inline bool is_collision(Vec3& pos) {
        Vec2 v2(pos.x, pos.z);
        return is_collision(v2);
    }

    void set_position(Node*, Vec2& pos);               // ����һ��Node�������е�λ��
    void add_thing(Node *node,                         // �����������һ������
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

    void update_goods(float dt);                       // [��ʱ��]���������е���Ʒ

    int goods_count();
    void dec_goods(int index);                         // ��������ɾ��һ����Ʒ
    void add_goods(GoodsBase *good);                   // �����������һ����Ʒ
    bool get_goods(Vec2& pos, Goods *);                // ��ȡָ��λ�õ���Ʒ

private:
	World();

    Terrain *_terrain;                      // ����
    Camera *_camera_free;                   // �����ӽ����
    Camera *_camera_fix;                    // �̶��ӽ����
    QuatNode *_colli_root;                  // ����ײ����

    DrawNode3D *_drawNode;
    PUParticleSystem3D *_pu_click_point;

    Goods *_goods[MAX_POINT_NUM];
    std::function<void(GoodsBase *)> _on_gen_goods  = nullptr;
};

#endif /* __WORLD_H__ */
