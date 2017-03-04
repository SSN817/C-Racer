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


float succession_change(float start,float end,int  div_num,int count)
{
	//*count++;
	float s;
	s=start;
	if(div_num<=count)
	{
		s=end;
	}
	else
	{
		s=start+((end-start)/div_num)*count;
	}
	return(s);
}

int get_change(int check_num,int change, int *box)
{
	int s;
	if(check_num-*box==change)
	{
		s=1;
	}
	else
	{
		s=0;
	}
	*box=check_num;
	return(s);
}

void limit_section(int *count ,int sma,int bi ,int loopflag)
{
	if(loopflag==0)
	{
		if(*count>bi)
			*count=bi;
		if(*count<sma)
			*count=sma;
	}
	if(loopflag==1)
	{
		if(*count>bi)
			*count=sma;
		if(*count<sma)
			*count=bi;
	}
	return;
}


void show_vector(int n,VECTOR a,int r,int g,int b)
{

	DrawFormatString(0,18*n,GetColor(r,g,b),"%f,%f,%f",a.x,a.y,a.z);

	return;
}


void draw_vector(VECTOR start_pos ,VECTOR draw_vec,int r, int g, int b)
{
	int color;
	VECTOR v;
	color=GetColor(r,g,b);

	v=VScale(VNorm(draw_vec),-1.0);

	DrawCone3D(VAdd(start_pos,draw_vec),VAdd(VAdd(start_pos,draw_vec),VScale(v,5.0)),2.0,6,color,color,1);
	DrawCapsule3D(start_pos,VAdd(VAdd(start_pos,draw_vec),VScale(v,3.5)),0.7,6,color,color,1);

	return;
}

VECTOR vector_resolve(VECTOR R,VECTOR X,VECTOR Y,VECTOR Z,float X_flag,float Y_flag,float Z_flag)
{
	float s,t,u;
	MATRIX M;
	VECTOR A;
	A=VGet(0.0,0.0,0.0);

	X=VNorm(X);
	Y=VNorm(Y);
	Z=VNorm(Z);

	M=MGetIdent();//’PˆÊs—ñŽæ“¾

	M.m[0][0]=X.x;
	M.m[1][0]=X.y;
	M.m[2][0]=X.z;

	M.m[0][1]=Y.x;
	M.m[1][1]=Y.y;
	M.m[2][1]=Y.z;

	M.m[0][2]=Z.x;
	M.m[1][2]=Z.y;
	M.m[2][2]=Z.z;

	M=MInverse(M);//‹ts—ñŽæ“¾

	s=M.m[0][0]*R.x+M.m[0][1]*R.y+M.m[0][2]*R.z;
	t=M.m[1][0]*R.x+M.m[1][1]*R.y+M.m[1][2]*R.z;
	u=M.m[2][0]*R.x+M.m[2][1]*R.y+M.m[2][2]*R.z;

A=VAdd(VAdd(VScale(Y,t*Y_flag),VScale(Z,u*Z_flag)),VScale(X,s*X_flag));

	return(A);
}

void vector_log(VECTOR a[],int num,VECTOR b,int flag)
{
	int i;
	if(flag==-1)
	{
		for(i=0;i<num;i++)
		{
			a[i]=b;
		}
	}
	for(i=num-1;i>0;i--)
	{
		a[i]=a[i-1];
	}
	return;
}

void draw_Z_box(VECTOR x,VECTOR y,VECTOR z,VECTOR sp)
{
	VECTOR p[8];
	int i[12],j,n=0;
	p[0]=VAdd(VAdd(x,y),VAdd(z,sp));
	p[1]=VAdd(VSub(y,x),VAdd(z,sp));
	p[2]=VSub(VAdd(z,sp),VAdd(x,y));
	p[3]=VAdd(VSub(x,y),VAdd(z,sp));

	p[4]=VAdd(VAdd(x,y),sp);
	p[5]=VAdd(VSub(y,x),sp);
	p[6]=VSub(sp,VAdd(x,y));
	p[7]=VAdd(VSub(x,y),sp);

	i[0]=DrawLine3D(p[0],p[1],GetColor(255,0,0));
	i[1]=DrawLine3D(p[1],p[2],GetColor(255,0,0));
	i[2]=DrawLine3D(p[2],p[3],GetColor(255,0,0));
	i[3]=DrawLine3D(p[3],p[0],GetColor(255,0,0));

	i[4]=DrawLine3D(p[0],p[4],GetColor(255,0,0));
	i[5]=DrawLine3D(p[1],p[5],GetColor(255,0,0));
	i[6]=DrawLine3D(p[2],p[6],GetColor(255,0,0));
	i[7]=DrawLine3D(p[3],p[7],GetColor(255,0,0));

	i[8]=DrawLine3D(p[4],p[5],GetColor(255,0,0));
	i[9]=DrawLine3D(p[5],p[6],GetColor(255,0,0));
	i[10]=DrawLine3D(p[6],p[7],GetColor(255,0,0));
	i[11]=DrawLine3D(p[7],p[4],GetColor(255,0,0));
for(j=0;j<12;j++)
{
n=n+i[j];
}
if(n!=0)
{
n=n;
n=n;
}

	return;
}
void rota_box(VECTOR x1,VECTOR *x2,VECTOR y1,VECTOR *y2,VECTOR z1,VECTOR z2)
{
	int t=0;
	t=0;
MATRIX M;

if(VSize(VScale(VSub(z1,z2),1/VSize(z1)))<0.01)
M=MGetIdent();
else
M=MGetRotVec2(z1,z2);

*x2=VTransform(x1,M);
*y2=VTransform(y1,M);
return;
}

int check_box(VECTOR pos,struct z_box a)
{
int s=-1;
VECTOR R,X,Y,Z;
R=VSub(pos,a.start_pos);
X=vector_resolve(R,a.X,a.Y,a.Z,1.0,0.0,0.0);
Y=vector_resolve(R,a.X,a.Y,a.Z,0.0,1.0,0.0);
Z=vector_resolve(R,a.X,a.Y,a.Z,0.0,0.0,1.0);

if(VSize(X)<VSize(a.X)&&VSize(Y)<VSize(a.Y)&&VSize(Z)<VSize(a.Z))
s=a.num;
else
s=-1;
return(s);
}


