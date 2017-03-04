#include "DxLib.h"
#include <math.h>

struct hit_line
{
VECTOR pos_start;
VECTOR pos_end;
VECTOR pos_hit;
VECTOR hit_norm;
int hit_flag;
};

struct car
{
	VECTOR direction_Z;
	VECTOR direction_Y;
	VECTOR direction_X;
	VECTOR force;
	VECTOR acceleration;
	VECTOR velocity;
	VECTOR position;
	VECTOR position_buffer;
	VECTOR friction;
	VECTOR tire_velocity;
	struct hit_line line0;
	struct hit_line line1;
	struct hit_line line2;
	struct hit_line line3;
	struct hit_line line4;


	MV1_COLL_RESULT_POLY coll;
	MV1_COLL_RESULT_POLY coll_g;

	MATRIX rota_matrix;

	float weight;
	float u_max;
	float u_move;
	float tire_t_roll;
	float tire_h_roll;

	int fricflag;
};
struct z_box
{
	VECTOR start_pos;
	VECTOR end_pos;

	VECTOR X;
	VECTOR Y;
	VECTOR Z;
	VECTOR XX;
	VECTOR YY;
	VECTOR ZZ;

	int num;
};
struct ground
{
	VECTOR ground_X;
	VECTOR ground_Y;
	VECTOR ground_Z;
};


float succession_change(float start,float end,int  div_num,int count);//連続した値の変化を得る

int get_change(int check_num,int change, int *box);//値の変化を知る

void limit_section(int *count ,int sma,int bi ,int loopflag);//値をループさせる

void show_vector(int n,VECTOR a,int r,int g,int b);//ベクトルの成分を表示する

void draw_vector(VECTOR start_pos ,VECTOR draw_vec,int r, int g, int b);//ベクトルを描画する

VECTOR vector_resolve(VECTOR R,VECTOR X,VECTOR Y,VECTOR Z,float X_flag,float Y_flag,float Z_flag);//あるベクトルを指定した３つの軸に分解し、指定した軸について値を返す

void vector_log(VECTOR a[],int num,VECTOR b,int flag);//連続したベクトルの値の保持

void draw_Z_box(VECTOR x,VECTOR y,VECTOR z,VECTOR sp);//区間を表示

void rota_box(VECTOR x1,VECTOR *x2,VECTOR y1,VECTOR *y2,VECTOR z1,VECTOR z2);//区間を回転させる

int check_box(VECTOR pos,struct z_box a);//区間内にいるかいないかを判定

//void move_pos(VECTOR *pos,float move_span,char x,char y,char z,char up,char down);//座標を移動させる