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


float succession_change(float start,float end,int  div_num,int count);//�A�������l�̕ω��𓾂�

int get_change(int check_num,int change, int *box);//�l�̕ω���m��

void limit_section(int *count ,int sma,int bi ,int loopflag);//�l�����[�v������

void show_vector(int n,VECTOR a,int r,int g,int b);//�x�N�g���̐�����\������

void draw_vector(VECTOR start_pos ,VECTOR draw_vec,int r, int g, int b);//�x�N�g����`�悷��

VECTOR vector_resolve(VECTOR R,VECTOR X,VECTOR Y,VECTOR Z,float X_flag,float Y_flag,float Z_flag);//����x�N�g�����w�肵���R�̎��ɕ������A�w�肵�����ɂ��Ēl��Ԃ�

void vector_log(VECTOR a[],int num,VECTOR b,int flag);//�A�������x�N�g���̒l�̕ێ�

void draw_Z_box(VECTOR x,VECTOR y,VECTOR z,VECTOR sp);//��Ԃ�\��

void rota_box(VECTOR x1,VECTOR *x2,VECTOR y1,VECTOR *y2,VECTOR z1,VECTOR z2);//��Ԃ���]������

int check_box(VECTOR pos,struct z_box a);//��ԓ��ɂ��邩���Ȃ����𔻒�

//void move_pos(VECTOR *pos,float move_span,char x,char y,char z,char up,char down);//���W���ړ�������