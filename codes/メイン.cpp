#include "DxLib.h"
#include <math.h>
#define PI 3.1415926535897932f
#include "関数.h"
#include <float.h>
void move_pos(VECTOR *pos,float move_span,int x,int y,int z,int up,int down)
{
	if(CheckHitKey(x)==1)
		pos->x+=(CheckHitKey(up)-CheckHitKey(down))*move_span;
	if(CheckHitKey(y)==1)
		pos->y+=(CheckHitKey(up)-CheckHitKey(down))*move_span;
	if(CheckHitKey(z)==1)
		pos->z+=(CheckHitKey(up)-CheckHitKey(down))*move_span;
	return;
}
struct game_control
{
	int num;
	int count;
	int buffer;
};

struct mouse
{
	int x;
	int y;
	int click_flag;
	int click_buffer;
	int position_buffer;
	int inout_flag;
	int inout_buffer;
};

int mouse_square(int MouseX,int MouseY,int x1,int y1,int x2,int y2,int BoxFlag)//マウスが指定範囲内に入ったときフラグオン
{
	int s=0;
	if(x1<=MouseX&&MouseX<=x2&&y1<=MouseY&&MouseY<=y2)
	{
		s=1;
	}
	if(BoxFlag==1)
	{
		DrawBox(x1,y1,x2,y2,GetColor(256,256,50),0);
	}
	return(s);
}
int mouse_click(struct mouse *m)//マウスクリック
{
	int s=0;
		
	if((GetMouseInput()&MOUSE_INPUT_LEFT)!=0)
		m->click_flag=1;
	else
		m->click_flag=0;

	s=get_change(m->click_flag,1,&m->click_buffer);

	return(s);
}
int draw_switch(int GH1,int GH2,int SH1,int SH2,int x,int y,struct mouse *m,int trans_flag)//スイッチ設置
{
	int size_x,size_y,s=0;
	GetGraphSize(GH2,&size_x,&size_y);
	DrawGraph(x,y,GH1,trans_flag);

	if(1==mouse_square(m->x,m->y,x,y,x+size_x,y+size_y,0)&&m->inout_flag!=x*y)
	{
		m->inout_buffer=0;
		m->inout_flag=x*y;
	}
	
	if(1==mouse_square(m->x,m->y,x,y,x+size_x,y+size_y,0))
		DrawGraph(x,y,GH2,trans_flag);
	if(x*y==m->inout_flag)
	{
	if(1==get_change(mouse_square(m->x,m->y,x,y,x+size_x,y+size_y,0),1,&m->inout_buffer))
			PlaySoundMem(SH1,DX_PLAYTYPE_BACK);
	}

	if(1==mouse_square(m->x,m->y,x,y,x+size_x,y+size_y,0)&&mouse_click(&*m)==1&&m->inout_buffer==1)
	{
		PlaySoundMem(SH2,DX_PLAYTYPE_BACK);
		s=1;
	}
	return(s);
}

void num_count(int *ten,int *one,int max)
{
	if(*one==max+1)
	{
		*one=0;
		*ten=*ten+1;
	}
	return;
}
void num_draw(int GH_num,int x, int y,int div_num,int draw_num)
{
	int size_x,size_y;
	GetGraphSize(GH_num,&size_x,&size_y);

	DrawRectGraph(x,y,(size_x/div_num)*draw_num,0,size_x/div_num,size_y,GH_num,1,0);
	
	return;
}
// プログラムは WinMain から始まります
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{


	ChangeWindowMode( TRUE ) ;//ホップアップウインドウで表示
	SetGraphMode(800,800,32);//解像度800×800
	if( DxLib_Init() == -1 )	// ＤＸライブラリ初期化処理
	{
		return -1 ;	// エラーが起きたら直ちに終了
	}

	car car1;
	int waitcount,waitspan2=20,waitspan1=20,i=0,j=0;
	int MH_car1,MH_sky,MH_ground,MH_tonnel,GHhandle,MH_tree;
	MH_car1=MV1LoadModel("車完成.mqo");
	MH_sky=MV1LoadModel("サーキット（空のみ）.mqo");
	MH_ground=MV1LoadModel("サーキット（地面のみ）.mqo");
	MH_tonnel=MV1LoadModel("サーキット（道路のみ）.mqo");
	MH_tree=MV1LoadModel("木.mqo");
	GHhandle=LoadGraph("ハンドル.png");
	


	////////////////////////////////////////ゲーム管理///////////////////////////////////////////
	game_control flag;
	flag.count=0;
	flag.num=0;
	mouse mouse1;
	mouse1.click_buffer=0;
	mouse1.click_flag=0;
	mouse1.inout_buffer=0;
	mouse1.inout_flag=0;
	mouse1.position_buffer=0;

	int GH_num_speed,GH_num_timer1,GH_num_timer2,GH_num_count,GH_kmh,GH_colon,GH_num_clear,GH_clear_colon,GH_time;
	int GH_switch_start1,GH_switch_start2;
	int GH_message_go,GH_message_goal,GH_message_clear,GH_message_out;

	int goflag=0;
	int outflag=0;
	int GH_title;
	int SH_click,SH_mouse;

	SH_click=LoadSoundMem("cursor27.wav");
	SH_mouse=LoadSoundMem("buble02.wav");
	GH_title=LoadGraph("題名.png");
	GH_switch_start1=LoadGraph("スイッチ始める1.bmp");
	GH_switch_start2=LoadGraph("スイッチ始める2.bmp");
	GH_num_speed=LoadGraph("数字（スピードメーター）.bmp");
	GH_num_timer1=LoadGraph("数字（タイム計測）.bmp");
	GH_num_timer2=LoadGraph("数字（結果発表）.bmp");
	GH_num_count=LoadGraph("321.bmp");
	GH_kmh=LoadGraph("kmh.bmp");
	GH_colon=LoadGraph("コロン.bmp");
	GH_message_go=LoadGraph("スタート.bmp");
	GH_message_goal=LoadGraph("ゴール.bmp");
	GH_message_clear=LoadGraph("結果発表.bmp");
	GH_num_clear=LoadGraph("数字（結果発表）.bmp");
	GH_clear_colon=LoadGraph("結果発表コロン.bmp");
	GH_time=LoadGraph("タイム.bmp");
	GH_message_out=LoadGraph("コースアウト！.bmp");

	int speed[5],time[6],count_down=0;
	float sp=0.0;
	for(i=0;i<10;i++)
	{
	speed[i%5]=0;
	time[i%6]=0;
	}
	
	
	///////////////////////////////////////ゲーム管理終わり////////////////////////////////////////
	
	
	
	
	ground ground1;//地面設定
	ground1.ground_X=VGet(1.0,0.0,0.0);
	ground1.ground_Y=VGet(0.0,1.0,0.0);
	ground1.ground_Z=VGet(0.0,0.0,1.0);



	car1.force=VGet(0.0,0.0,0.0);//車設定
	car1.position=VGet(1120.0,9.0,-720.0);
	car1.velocity=VGet(0.0,0.0,0.0);
	car1.acceleration=VGet(0.0,0.0,0.0);
	car1.direction_Z=VGet(1.0,0.0,0.0);
	car1.direction_Y=VGet(0.0,1.0,0.0);
	car1.direction_X=VGet(0.0,0.0,1.0);
	car1.tire_velocity=VGet(0.0,0.0,0.0);
	car1.friction=VGet(0.0,0.0,0.0);
	car1.tire_h_roll=0.0;
	car1.tire_t_roll=0.0;
	car1.weight=1600.0;
	car1.u_max=1.2;
	car1.u_move=0.5;
	car1.fricflag=1;
	car1.position_buffer=car1.position;





	float accel=0.0f;
	int rotaflag=0;//車の回転
	VECTOR hit_vec[20];
	float hit_angle=0.0;


	int count;//カメラ設定
	count=0;
	int cflag=0;

	float c=0.0;
	float camera_y=0.0,camera_z=0.0,camera_p=0.0,camera_t=0.0;//カメラ設定
	int change[20],c_n=20;
	change[0]=0;
	change[1]=0;
	VECTOR c_p[20],c_y[20],c_z[20];//カメラ設定
	
	

	VECTOR camera_pos,camera_angle,light_pos;//区間、ライト編集


	camera_pos=VGet(0.0,120.0,-600.0);
	camera_angle=VGet(0.0,0.0,0.0);
	light_pos=VGet(0.0,120.0,-600.0);

		float va=0.0,ha=0.0,ta=0.0;




	int g,n=0,nn1=0,nn2=0,s=0,q=0,qq=0;
	z_box box[100];
	for(g=0;g<100;g++)
	{
		box[g].num=g;
		box[g].X=VGet(500.0,0.0,0.0);	
		box[g].Y=VGet(0.0,500.0,0.0);
		box[g].Z=VGet(0.0,0.0,50.0);
		box[g].XX=box[g].X;
		box[g].YY=box[g].Y;
		box[g].ZZ=box[g].Z;
		box[g].end_pos=VGet(0.0,100.0,-200.0);
		box[g].start_pos=VGet(10.0,100.0,-200.0);
	}


	MV1SetupCollInfo(MH_ground,-1,32,8,32);
	MV1SetupCollInfo(MH_tonnel,-1,32,8,32);
	SetDrawScreen( DX_SCREEN_BACK ); //描画先を裏画面に設定

	SetCameraNearFar(15.0,20000.0);
	// Ｚバッファを有効にする
	SetUseZBuffer3D( TRUE ) ;

	// Ｚバッファへの書き込みを有効にする
	SetWriteZBuffer3D( TRUE ) ;

	FILE *bbb=fopen("確認ベクトル","rb+");
	if(bbb==NULL)
		return(0);
	fread(box,sizeof(z_box),100,bbb);
	fclose(bbb);

	vector_log(c_p,c_n,car1.position,-1);
	vector_log(c_y,c_n,car1.direction_Y,-1);
	vector_log(c_z,c_n,car1.direction_Z,-1);







	while(ProcessMessage()!=-1)//*****************************************************メインループ**********************************************
	{
		GetMousePoint(&mouse1.x,&mouse1.y);

		ClearDrawScreen(); // 画面を消す
		waitcount=GetNowCount();//ループを開始した時の時刻を得る
//*******************************************************************************スタート画面****************************************************
		if(flag.num==0)
		{
				car1.force=VGet(0.0,0.0,0.0);//車設定
				car1.position=VGet(1120.0,9.0,-720.0);
				car1.velocity=VGet(0.0,0.0,0.0);
				car1.acceleration=VGet(0.0,0.0,0.0);
				car1.direction_Z=VGet(1.0,0.0,0.0);
				car1.direction_Y=VGet(0.0,1.0,0.0);
				car1.direction_X=VGet(0.0,0.0,1.0);
				car1.tire_velocity=VGet(0.0,0.0,0.0);
				car1.friction=VGet(0.0,0.0,0.0);
				car1.tire_h_roll=0.0;
				car1.tire_t_roll=0.0;
				car1.weight=800.0;
				car1.u_max=1.2;
				car1.u_move=0.5;
				car1.fricflag=1;
			
				for(i=0;i<10;i++)
				{
					speed[i%5]=0;
					time[i%6]=0;
				}	
			DrawBox(0,0,800,800,GetColor(255,255,255),1);
			DrawGraph(0,-60,GH_title,0);
			if(1==draw_switch(GH_switch_start1,GH_switch_start2,SH_mouse,SH_click,200,670,&mouse1,0))
			{
				flag.num=2;
				flag.count=0;
			}
		}
//*******************************************************************************スタート画面終わり**********************************************
//*******************************************************************************説明************************************************************
		if(flag.num==1)
		{



		}
//*****************************************************************************説明終わり********************************************************
//**********************************************************************************本編***********************************************************
		if(flag.num==2)
		{
			car1.force=VGet(0.0,0.0,0.0);//力の初期化

			//////////////////////////////当たり判定計算//////////////////////////////////
			car1.line0.pos_start=car1.position;
			car1.line0.pos_end=VAdd(car1.position,VScale(car1.direction_Y,-9.0f));
			car1.line1.pos_start=VAdd(car1.position,VAdd(VScale(car1.direction_Z,19.6f),VScale(car1.direction_X,8.1f)));
			car1.line1.pos_end=VAdd(VScale(car1.direction_Z,-43.0f),car1.line1.pos_start);
			car1.line2.pos_start=VAdd(car1.position,VAdd(VScale(car1.direction_Z,19.6f),VScale(car1.direction_X,-8.1f)));
			car1.line2.pos_end=VAdd(VScale(car1.direction_Z,-43.0f),car1.line2.pos_start);
			car1.line3.pos_start=VAdd(car1.position,VAdd(VScale(car1.direction_Z,19.6f),VScale(car1.direction_X,-8.1f)));
			car1.line3.pos_end=VAdd(VScale(car1.direction_X,16.2f),car1.line3.pos_start);
			car1.line4.pos_start=VAdd(car1.position,VAdd(VScale(car1.direction_Z,-23.31f),VScale(car1.direction_X,-8.1f)));
			car1.line4.pos_end=VAdd(VScale(car1.direction_X,16.2f),car1.line4.pos_start);

			//draw_vector(car1.position,VSub(VAdd(car1.position,VScale(car1.direction_Z,20.0f)),car1.position),255,255,255);
			//draw_vector(car1.position,VSub(VAdd(car1.position,VScale(car1.direction_Y,-9.0)),car1.position),  0,255,0);


			car1.coll=MV1CollCheck_Line(MH_tonnel,-1,car1.line1.pos_start,car1.line1.pos_end);
			car1.line1.hit_flag=car1.coll.HitFlag;
			if(car1.coll.HitFlag==1)
			{
				car1.line1.hit_norm=car1.coll.Normal;
				hit_vec[0]=car1.coll.HitPosition;
			}
			else
			{
				car1.line1.hit_norm=VGet(0.0f,0.0f,0.0f);
				hit_vec[0]=VGet(0.0f,0.0f,0.0f);
			}
			car1.coll=MV1CollCheck_Line(MH_tonnel,-1,car1.line2.pos_start,car1.line2.pos_end);
			car1.line2.hit_flag=car1.coll.HitFlag;
			if(car1.coll.HitFlag==1)
			{
				car1.line2.hit_norm=car1.coll.Normal;
				hit_vec[1]=car1.coll.HitPosition;
			}
			else
			{
				car1.line2.hit_norm=VGet(0.0f,0.0f,0.0f);
				hit_vec[1]=VGet(0.0f,0.0f,0.0f);
			}
			car1.coll=MV1CollCheck_Line(MH_tonnel,-1,car1.line3.pos_start,car1.line3.pos_end);
			car1.line3.hit_flag=car1.coll.HitFlag;
			if(car1.coll.HitFlag==1)
			{
				car1.line3.hit_norm=car1.coll.Normal;
				hit_vec[2]=car1.coll.HitPosition;
			}
			else
			{
				car1.line3.hit_norm=VGet(0.0f,0.0f,0.0f);
				hit_vec[2]=VGet(0.0f,0.0f,0.0f);
			}
			car1.coll=MV1CollCheck_Line(MH_tonnel,-1,car1.line4.pos_start,car1.line4.pos_end);
			car1.line4.hit_flag=car1.coll.HitFlag;
			if(car1.coll.HitFlag==1)
			{
				car1.line4.hit_norm=car1.coll.Normal;
				hit_vec[3]=car1.coll.HitPosition;
			}
			else
			{
				car1.line4.hit_norm=VGet(0.0f,0.0f,0.0f);
				hit_vec[3]=VGet(0.0f,0.0f,0.0f);
			}

			car1.line1.hit_flag=car1.line1.hit_flag+car1.line2.hit_flag+car1.line3.hit_flag+car1.line4.hit_flag;

			if(car1.line1.hit_flag>0)
			{
				car1.coll.Normal=VScale(VAdd(VAdd(car1.line1.hit_norm,car1.line2.hit_norm),VAdd(car1.line3.hit_norm,car1.line4.hit_norm)),1.0f/car1.line1.hit_flag);
				car1.coll.HitPosition=VScale(VAdd(VAdd(hit_vec[0],hit_vec[1]),VAdd(hit_vec[2],hit_vec[3])),1.0f/car1.line1.hit_flag);
				hit_vec[10]=VNorm(car1.coll.Normal);
				hit_vec[11]=VNorm(car1.velocity);
				hit_angle=acos(VDot(hit_vec[11],hit_vec[10]));
				if(hit_angle>PI/2)
				{
					hit_vec[11]=VScale(hit_vec[11],tan(PI-hit_angle));
					hit_vec[11]=VAdd(VScale(hit_vec[10],2.0),hit_vec[11]);
					hit_vec[11]=VNorm(hit_vec[11]);
					car1.velocity=VScale(VScale(hit_vec[11],VSize(car1.velocity)),0.6f);
				}
				hit_vec[4]=VSub(car1.position,car1.coll.HitPosition);
				hit_vec[5]=VSub(car1.position_buffer,car1.coll.HitPosition);
				hit_vec[4]=VScale(car1.coll.Normal,VDot(hit_vec[4],car1.coll.Normal)/VDot(hit_vec[4],hit_vec[4]));
				hit_vec[5]=VScale(car1.coll.Normal,VDot(hit_vec[5],car1.coll.Normal)/VDot(hit_vec[5],hit_vec[5]));				
				hit_vec[6]=VSub(hit_vec[5],hit_vec[4]);
				car1.position=VAdd(car1.position,hit_vec[6]);
				
					car1.coll=MV1CollCheck_Line(MH_tonnel,-1,car1.line1.pos_start,car1.line1.pos_end);
					car1.line1.hit_flag=car1.coll.HitFlag;
					if(car1.coll.HitFlag==1)
					{
						car1.line1.hit_norm=car1.coll.Normal;
						hit_vec[0]=car1.coll.HitPosition;
					}
					else
					{
						car1.line1.hit_norm=VGet(0.0f,0.0f,0.0f);
						hit_vec[0]=VGet(0.0f,0.0f,0.0f);
					}
					car1.coll=MV1CollCheck_Line(MH_tonnel,-1,car1.line2.pos_start,car1.line2.pos_end);
					car1.line2.hit_flag=car1.coll.HitFlag;
					if(car1.coll.HitFlag==1)
					{
						car1.line2.hit_norm=car1.coll.Normal;
						hit_vec[1]=car1.coll.HitPosition;
					}
					else
					{
						car1.line2.hit_norm=VGet(0.0f,0.0f,0.0f);
						hit_vec[1]=VGet(0.0f,0.0f,0.0f);
					}
					car1.coll=MV1CollCheck_Line(MH_tonnel,-1,car1.line3.pos_start,car1.line3.pos_end);
					car1.line3.hit_flag=car1.coll.HitFlag;
					if(car1.coll.HitFlag==1)
					{
						car1.line3.hit_norm=car1.coll.Normal;
						hit_vec[2]=car1.coll.HitPosition;
					}
					else
					{
						car1.line3.hit_norm=VGet(0.0f,0.0f,0.0f);
						hit_vec[2]=VGet(0.0f,0.0f,0.0f);
					}
					car1.coll=MV1CollCheck_Line(MH_tonnel,-1,car1.line4.pos_start,car1.line4.pos_end);
					car1.line4.hit_flag=car1.coll.HitFlag;
					if(car1.coll.HitFlag==1)
					{
						car1.line4.hit_norm=car1.coll.Normal;
						hit_vec[3]=car1.coll.HitPosition;
					}
					else
					{
						car1.line4.hit_norm=VGet(0.0f,0.0f,0.0f);
						hit_vec[3]=VGet(0.0f,0.0f,0.0f);
					}				
			car1.line1.hit_flag=car1.line1.hit_flag+car1.line2.hit_flag+car1.line3.hit_flag+car1.line4.hit_flag;
			if(car1.line1.hit_flag>0)
				car1.position=car1.position_buffer;
						
						
						
				






				//if(VDot(car1.velocity,car1.coll.Normal)<0)
				//	car1.force=VAdd(car1.force,VScale(VNorm(car1.coll.Normal),VSize(car1.velocity)*100000.0f));
			}
			car1.position_buffer=car1.position;//車の位置保存



			car1.coll_g=MV1CollCheck_Line(MH_ground,-1,car1.line0.pos_start,car1.line0.pos_end);//車と地面;
			if(car1.coll_g.HitFlag==1)
			{
			car1.velocity=vector_resolve(car1.velocity,car1.direction_X,car1.direction_Y,car1.direction_Z,1.0,0.0,1.0);

			car1.position=VAdd(car1.coll_g.HitPosition,VScale(VNorm(car1.coll_g.Normal),9.0));

			car1.rota_matrix=MGetRotVec2(car1.direction_Y,car1.coll_g.Normal);
			if(VSize(VSub(car1.direction_Y,car1.coll_g.Normal))<0.001)
				car1.rota_matrix=MGetIdent();
				car1.direction_Z=VTransform(car1.direction_Z,car1.rota_matrix);
				car1.direction_X=VTransform(car1.direction_X,car1.rota_matrix);
				car1.direction_Y=VTransform(car1.direction_Y,car1.rota_matrix);
			}
			car1.coll=MV1CollCheck_Line(MH_tonnel,-1,car1.line0.pos_start,car1.line0.pos_end);//車とコース;
			if(car1.coll.HitFlag==1)
			{
				car1.velocity=vector_resolve(car1.velocity,car1.direction_X,car1.direction_Y,car1.direction_Z,1.0,0.0,1.0);

				car1.position=VAdd(car1.coll.HitPosition,VScale(VNorm(car1.coll.Normal),9.0));

				car1.rota_matrix=MGetRotVec2(car1.direction_Y,car1.coll.Normal);
				if(VSize(VSub(car1.direction_Y,car1.coll.Normal))<0.001)
					car1.rota_matrix=MGetIdent();

				car1.direction_Z=VTransform(car1.direction_Z,car1.rota_matrix);
				car1.direction_X=VTransform(car1.direction_X,car1.rota_matrix);
				car1.direction_Y=VTransform(car1.direction_Y,car1.rota_matrix);
			}




//////////////////////////////当たり判定計算終わり////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////車の向き計算///////////////////////////////////////////////////////////////////////////////////////////////////

			if(mouse1.x>=450||mouse1.x<=350)
			{
			rotaflag=1;
			if(mouse1.x<=350)
			rotaflag=-1;
			}
			else
			rotaflag=0;

			if(VSquareSize(car1.velocity)>=0.1&&car1.fricflag>=0)
			{
				car1.rota_matrix=MGetRotAxis(car1.direction_Y,(VSize(car1.velocity)*waitspan1/1000.0f)/(250.0f*(100000.0/(1.0+(mouse1.x-400)*(mouse1.x-400))))*rotaflag);
			car1.direction_Z=VTransform(car1.direction_Z,car1.rota_matrix);
			car1.direction_X=VTransform(car1.direction_X,car1.rota_matrix);
			//car1.velocity=VTransform(car1.velocity,car1.rota_matrix);




			car1.friction=VScale(car1.velocity,tan((VSize(car1.velocity)*waitspan1/1000.0f)/250.0f*rotaflag)*100.0f/waitspan1*car1.weight);


			}



//////////////////////////////車の向き計算終わり////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////外力計算///////////////////////////////////////////////////////////////////////////////////////////////////////
			car1.direction_Z=VNorm(car1.direction_Z);
			if(flag.count>=1000/waitspan1*6&&outflag==0)
			{
				if(CheckHitKey(KEY_INPUT_F)==1)//精神を加速させろ！！
				{
					if(VSize(car1.velocity)<=800)
						accel=300.0;
					if(800<VSize(car1.velocity)&&VSize(car1.velocity)<=1500)
						accel=-1.0f/5.0f*VSize(car1.velocity)+300.0f;
					if(1500<VSize(car1.velocity))
						accel=0.0f;

					car1.force=VAdd(car1.force,VScale(car1.direction_Z,car1.weight*accel));	
				}	
				if(CheckHitKey(KEY_INPUT_B)==1)//&&VSquareSize(car1.velocity)>0.0f)//ブレーキ
						car1.force=VSub(car1.force,VScale(car1.direction_Z,car1.weight*300.0f));
			}

			car1.force=VSub(car1.force,VScale(VScale(ground1.ground_Y,car1.weight),400.0f));//重力


/////////////////////////////////タイヤの状態計算///////////////////////////////////////////////////////////////////////////////////////////////
			if(car1.coll.HitFlag==0&&car1.coll_g.HitFlag==0)//浮いている？
			{
			car1.fricflag=-1;
			}
			else
			{
			car1.fricflag=0;
			if(VSize(car1.friction)>=car1.u_max*car1.weight*300.0)//静止摩擦力の限界を超えている？
			car1.fricflag=0;
			if(VSize(vector_resolve(car1.velocity,car1.direction_X,car1.direction_Y,car1.direction_Z,1.0,0.0,0.0))<60.0)//速度のx成分がある一定値を下回ったら静止摩擦力に戻す
			car1.fricflag=1;


			if(car1.fricflag==1)
			car1.velocity=vector_resolve(car1.velocity,car1.direction_X,car1.direction_Y,car1.direction_Z,0.0,1.0,1.0);//静止摩擦力により速度のｘ成分は０になる
			if(car1.fricflag==0&&VSize(car1.velocity)>=0.5)
			car1.force=VSub(car1.force,VScale(VNorm(car1.velocity),car1.u_move*car1.weight*180.0));//動摩擦力が働く

			}

/////////////////////////////////タイヤの状態計算終わり//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////外力計算終わり///////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////加速度・速度・変位計算////////////////////////////////////////////////////////////////////////////////////////////
			car1.acceleration=VScale(car1.force,1.0f/car1.weight);
			car1.velocity=VAdd(car1.velocity,VScale(car1.acceleration,waitspan1/1000.0f));
			car1.position=VAdd(car1.position,VScale(car1.velocity    ,waitspan1/1000.0f));
		}
//////////////////////////////加速度・速度・変位計算終わり////////////////////////////////////////////////////////////////////////////////////
//*************************************************************本編終わり*****************************************************************************
//*************************************************************結果発表*******************************************************************************
		if(flag.num==3)
		{
			DrawBox(0,0,800,800,GetColor(255,255,255),1);
			num_draw(GH_num_clear,460,400,10,time[0]);
			DrawGraph(420,410,GH_clear_colon,1);
			num_draw(GH_num_clear,370,400,10,time[1]);
			num_draw(GH_num_clear,325,400,10,time[2]);
			DrawGraph(300,410,GH_clear_colon,1);
			num_draw(GH_num_clear,250,400,10,time[3]);
			num_draw(GH_num_clear,200,400,10,time[4]);
			DrawGraph(50,100,GH_message_clear,1);

			if(CheckHitKey(KEY_INPUT_BACK)==1&&CheckHitKey(KEY_INPUT_TAB)==1)
				flag.num=0;
		}


//*************************************************************結果発表終わり*************************************************************************

////////////////////////////////////////////カメラ////////////////////////////////////////////////////////////////////////////////////////////
		c_p[0]=car1.position;//カメラ座標セット
		c_y[0]=car1.direction_Y;
		c_z[0]=car1.direction_Z;
		c_n=7;
		vector_log(c_p,c_n,car1.position,0);
		vector_log(c_y,c_n,car1.direction_Y,0);
		vector_log(c_z,c_n,car1.direction_Z,0);

		if(cflag==1)
			count++;
		if(cflag==0)
			count--;

		limit_section(&count,0,20,0);
		cflag+=get_change(CheckHitKey(KEY_INPUT_C),1,&change[1]);
		limit_section(&cflag,0,1,1);

		camera_y=succession_change(0.0f,40.0f,20,count);
		camera_z=succession_change(0.0f,-100.0f,20,count);
		camera_p=succession_change(0.0f,1.0f,20,count);
		camera_t=succession_change(0.0f,1.0f,20,count);

		if(flag.num==2)
		{
			i=SetCameraPositionAndTarget_UpVecY( VAdd(VAdd(c_p[0],VScale(VSub(c_p[c_n-1],c_p[0]),camera_p)),VAdd(VScale(c_z[c_n-1],camera_z),VScale(c_y[c_n-1],camera_y))),
			VAdd(		VScale(car1.position,camera_t),
			VScale(VAdd(VAdd(VScale(car1.direction_Z,100.0f),VScale(car1.direction_Y,5.0f)),car1.position),1.0f-camera_t)));//カメラ
		}

////////////////////////////////////////////////カメラ/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////描画///////////////////////////////////////////////////////////////////////////////////////////
		if(flag.num==2)
		{

			if(CheckHitKey(KEY_INPUT_R)==1&&CheckHitKey(KEY_INPUT_O)==1)
				flag.num=0;


			MV1DrawModel(MH_tonnel);
			MV1DrawModel(MH_ground);
			MV1DrawModel(MH_sky);

			MV1DrawModel(MH_tree);
			//DrawFormatString(0,0,GetColor(255,255,255),"%d",n);


			ChangeLightTypePoint(VGet(0.0f,4000.0f,0.0f),180000.0f,0.8f,0.0f,0.0f);

			MV1SetPosition(MH_car1,car1.position);//変位セット
			MV1SetRotationZYAxis(MH_car1,car1.direction_Z,car1.direction_Y,0.0);//車ベクトルのセット
			MV1DrawModel(MH_car1);
			DrawBox(0,600,800,800,GetColor(0,0,0),1);
			if(mouse1.y<=700)
				DrawRotaGraph2(400,700,77,77,1.0,-PI/2+acos((-mouse1.x+400)/sqrt((double)((mouse1.x-400)*(mouse1.x-400)+(mouse1.y-700)*(mouse1.y-700)))),GHhandle,1);
			else
				DrawRotaGraph2(400,700,77,77,1.0,-PI/2-acos((-mouse1.x+400)/sqrt((double)((mouse1.x-400)*(mouse1.x-400)+(mouse1.y-700)*(mouse1.y-700)))),GHhandle,1);


			/*DrawFormatString(0,40,GetColor(255,255,255),"%f",VSize(car1.velocity));
			DrawFormatString(0,60,GetColor(255,255,255),"%f",VSize(car1.force));
			DrawFormatString(0,80,GetColor(255,255,255),"%f",VSize(car1.friction));
			DrawFormatString(0,100,GetColor(255,255,255),"%d",car1.fricflag);
			*/
			/////////////////////////////////////順位確認////////////////////////////////////////////////////////////////////////////////////////////
			s=-1;
			for(g=0;g<29;g++)
			{
				if(s<check_box(car1.position,box[g]))
				s=check_box(car1.position,box[g]);
			
			}
			/*
			if(s==-1)
				DrawFormatString(0,20,GetColor(255,255,255),"コースアウト");
			else
				DrawFormatString(0,20,GetColor(255,255,255),"第%d区間です",s);
			*/
			if(s!=-1)
				qq=s;
			//////////////////////////////////////順位確認終わり/////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////数字描画///////////////////////////////////////////////////////////////////////////////////////////
			if(s==-1)
			{
				outflag=1;
			}
			if(outflag>=1)
			{
				if(outflag==1)
				{
					if(qq==0)
						qq=1;
					car1.position=box[qq].start_pos;
					car1.velocity=VGet(0.0,0.0,0.0);
					car1.rota_matrix=MGetRotVec2(car1.direction_Z,box[qq].Z);
					car1.direction_X=VTransform(car1.direction_X,car1.rota_matrix);
					car1.direction_Y=VTransform(car1.direction_Y,car1.rota_matrix);				
					car1.direction_Z=VTransform(car1.direction_Z,car1.rota_matrix);		
				}

				outflag++;
				if(outflag<=1000/waitspan1*3)
					DrawGraph(150,350,GH_message_out,1);
			}
			
			if(1==get_change(s,-28,&flag.buffer)&&goflag==0)

			{
			goflag=1;
			}

			if(goflag>=1)
			{
				DrawGraph(200,350,GH_message_goal,1);
				goflag++;
				if(goflag>1000/waitspan1*3)
				{
					goflag=0;
					flag.count=0;
					flag.num=3;
				}	
			}
			if(outflag>=1000/waitspan1*3)
				outflag=0;
					
			flag.count++;
			if(0==flag.count%(100/waitspan1)&&flag.count>=1000/waitspan1*6)
				time[0]++;

			if(1000/waitspan1*2<=flag.count&&flag.count<1000/waitspan1*3)
				num_draw(GH_num_count,320,300,3,2);
			if(1000/waitspan1*3<=flag.count&&flag.count<1000/waitspan1*4)
				num_draw(GH_num_count,320,300,3,1);
			if(1000/waitspan1*4<=flag.count&&flag.count<1000/waitspan1*5)
				num_draw(GH_num_count,320,300,3,0);
			if(1000/waitspan1*5<=flag.count&&flag.count<1000/waitspan1*6)
				DrawGraph(280,300,GH_message_go,1);


			num_count(&time[1],&time[0],9);
			num_count(&time[2],&time[1],9);
			num_count(&time[3],&time[2],5);
			num_count(&time[4],&time[3],9);
			num_count(&time[5],&time[4],5);

			DrawGraph(430,0,GH_time,1);
			num_draw(GH_num_timer1,745,0,10,time[0]);
			DrawGraph(715,0,GH_colon,1);
			num_draw(GH_num_timer1,695,0,10,time[1]);
			num_draw(GH_num_timer1,665,0,10,time[2]);
			DrawGraph(650,0,GH_colon,1);
			num_draw(GH_num_timer1,630,0,10,time[3]);
			num_draw(GH_num_timer1,600,0,10,time[4]);

			for(i=0;i<4;i++)
			{
			speed[i]=0;
			}
			sp=VSize(car1.velocity)/28.0*3.6;
			for(i=0;i<2000;i++)
			{
				sp=sp-1.0f;
				if(sp<0)
					break;
				speed[0]++;
				num_count(&speed[1],&speed[0],9);
				num_count(&speed[2],&speed[1],9);
				num_count(&speed[3],&speed[2],9);
			}

			num_draw(GH_num_speed,640,640,10,speed[0]);
			num_draw(GH_num_speed,570,640,10,speed[1]);
			num_draw(GH_num_speed,500,640,10,speed[2]);
			DrawGraph(720,710,GH_kmh,1);

//////////////////////////////////////数字描画////////////////////////////////////////////////////////////////////////////////////////////
		}
		/*
		if(flag.count==2)
		{
			for(g=0;g<29;g++)
			{
				draw_Z_box(box[g].X,box[g].Y,box[g].Z,box[g].start_pos);//確認ベクトル描画
			}
		}
		*/
///////////////////////////////////////////////////////描画終わり///////////////////////////////////////////////////////////////////////

		waitspan2=waitspan1;//毎ループごとに一定時間を設定
		if(GetNowCount()-waitcount>=waitspan1)
			waitspan2=0;//設定した一定時間を処理時間が上回る場合待たずに次へ
		else
			waitspan2=waitspan2-(GetNowCount()-waitcount);//それ以外の場合設定した一定時間から処理時間を差し引いた分だけ待つ
		WaitTimer(waitspan2);
		ScreenFlip();//裏画面反映
	}
	WaitKey() ;// キー入力待ち
	DxLib_End() ;// ＤＸライブラリ使用の終了処理
	return 0 ;	// ソフトの終了 
}