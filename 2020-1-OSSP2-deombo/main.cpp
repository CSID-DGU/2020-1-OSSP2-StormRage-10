
#include "AirPlane.h"
#include <string>
#include <sstream>

Mix_Music *menu_music;//메뉴
Mix_Music *stage_music;//스테이지 음악

Mix_Chunk *game_over_sound;//게임오버 사운드
Mix_Chunk *selection_sound;//메뉴 선택음
Mix_Chunk *bullet_sound;//총알 발사음
Mix_Chunk *beam_sound;//총알2 발사음
Mix_Chunk *hit_sound;//플레이어 피격음
Mix_Chunk *explosion_sound1;//적 폭팔음
Mix_Chunk *explosion_sound2;//보스 폭팔음
Mix_Chunk *special_sound;//특수기 효과음
Mix_Chunk *item_sound;//아이템 획득음
Mix_Chunk *stage_clear_sound;//스테이지 클리어 사운드

SDL_Surface *shield;
SDL_Surface *screen;//실제 출력되는 화면
SDL_Surface *buffer;//화면에 그리기 전에 버퍼
SDL_Surface *background;//배경화면
SDL_Surface *background2;
SDL_Surface *background3;
SDL_Surface *intro;
SDL_Surface *explosion; //보스 몹 맞을 때 폭팔
SDL_Surface *life;
SDL_Surface *sapoint;
SDL_Surface *bullet;//총알 이미지
SDL_Surface *bullet_basic;
SDL_Surface *bullet_mini;//화면에 그리기 전에 버퍼
SDL_Surface *bullet_second;
SDL_Surface *bullet_boss;
SDL_Surface *message;
SDL_Surface *message2;
SDL_Surface *message3;
SDL_Surface *message4;
SDL_Surface *message5;
SDL_Surface *title_message;
SDL_Surface *plane;// 사용자 비행기 이미지
SDL_Surface *plane2;
SDL_Surface *plane3;
SDL_Surface *plane4;
SDL_Surface *plane5;
SDL_Surface *plane_1p;
SDL_Surface *plane_2p;
SDL_Surface *enemy[4];//회전하는 비행기 이미지
SDL_Surface *boom[11];// 폭발 이미지
SDL_Surface *frame;
SDL_Surface *frame2;
SDL_Surface *arrow;
SDL_Surface *pick1;
SDL_Surface *pick2;
SDL_Surface *enemy2;

SDL_Rect screen_rect;
SDL_Rect buffer_rect;

SDL_Event event;
TTF_Font *font;
TTF_Font *font2;
TTF_Font *font3;
SDL_Color textColor = {0, 0, 0};
SDL_Color textColor2 = {0, 0, 0};

Uint8 *keystates;

const int INITIAL_MODE = 10;
int EXIT = -1;
int craft;
int SA;
int SA2;
short mode;
short played_channel=-1;
short played_channel2=-1;



void sprite_surface(SDL_Surface* source, SDL_Rect tmp, SDL_Surface* destination, int w, int h, int step,int mode);
bool init();//변수들 초기화 함수
bool load_files();//이미지, 폰트,오디오 초기화 함수
bool SDL_free_all();// sdl 변수들 free 함수
bool menu();
bool menu2();
bool menu3();
bool game_over();
bool stage_clear();
void special_ability(int SA);
bool game_start();

void handle_resize(SDL_ResizeEvent &event){//화면의 크기가 resize 되면 이 함수가 호출됨
  
  SDL_FreeSurface(screen);//기존 screen을 free 해준다음.
  screen = SDL_SetVideoMode(event.w, event.h, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF |SDL_RESIZABLE);
  //재조정된 크기에 맞춰 다시 screen을 만든다.
  buffer_rect.w=event.w;//버퍼 rect의 width과 height을 조정해준다.
  buffer_rect.h=event.h;
  
}

void show_screen(){//SDL_flip을 직접적으로  호출하지 않고 show_screen을 통해 스케일링 된 화면을 flip해줌
  SDL_Surface* temp;
  
  double scale_x=(double)buffer_rect.w/SCREEN_WIDTH;
  double scale_y=(double)buffer_rect.h/SCREEN_HEIGHT;
  temp=zoomSurface(buffer,scale_x,scale_y,0);//버퍼로부터 확대한 Surface를 temp에 저장

  SDL_BlitSurface(temp,&buffer_rect,screen,&buffer_rect);//이 temp를 screen에 그림

  SDL_Flip(screen);//Filp을 통해 화면에 screen을 출력
  SDL_FreeSurface(temp);//temp를 반드시 free 해줘야 한다!!!
}
bool game_start()
{
  _bullets enemy_bullets;
  _bullets player_bullets;
  _bullets boss_bullets;
  _bullets mini_bullets;
  _bullets second_bullets;
  srand(time(NULL));

  int score = 0;
  int start_time = 0;
  int delay = 0;
  int count = 0;
  int shootcnt = 0;
  int shootcnt2 = 0;
  int background_count = 0;               //background 움직임 count
  int boom_mode = 0;

  short shield_counter=0;
  short RNG;//확률 변수를 저장할 변수
  //int flag = 0;
  //int flag2 = 0;
  //int flag3 = 0;

  int flag_sa = 0;
  int flag_sa2 = 0;
  int bound = -100;
  bool dead = false;
  bool dead2 = false;

  bool border_check=true;//경계 밖을 나간 것을 체크하기 위해 경계 안에 있는 경우 true
  bool border_check2=true;

  bool is_laser=false;
  bool is_laser2=false;
  bool is_boss_laser_harmful = false;

    //for obstacle
  int o_bound = 480+100;
  int hole;
  vector<Obstacle>::iterator obs_it;
  vector<Obstacle> Obs;

  vector<Enemy_standard_3>::iterator it3;
  vector<Enemy_standard_2>::iterator it2;
  vector<Enemy_standard>::iterator it;
  vector<BOOM>::iterator B_it;
  vector<special>::iterator it_sa;
  list<SDL_Rect>::iterator CB_it;

  vector<BOOM> mini_Boss_BOOM;//보스 폭발
  vector<BOOM> final_Boss_BOOM;//보스 폭발
  vector<BOOM> second_Boss_BOOM;// 폭발
  vector<BOOM> laser_Boss_B4;// 폭발
  vector<BOOM> B;//폭발
  vector<Enemy_standard> E;//기본1형 비행기
  vector<Enemy_standard_2> E2;// 2nd standard enemy
  vector<Enemy_standard_3> E3;// 3번째 적 공격시 4방향으로 총알 방출
  vector<special> sa_1;
  list<SDL_Rect> CB;//충돌 박스를 저장할 리스트

  SDL_Rect Border;//경계를 저장할 Border
  Border.x=(-PLAYER_WIDTH);
  Border.y=(-PLAYER_HEIGHT);
  Border.w=SCREEN_WIDTH + 2*PLAYER_WIDTH;
  Border.h=SCREEN_HEIGHT + 2*PLAYER_HEIGHT;

  //생성자에 사운드를 저장
  AirPlane A(bullet_sound,item_sound,hit_sound);//사용자 비행기
  AirPlane A2(bullet_sound,item_sound,hit_sound);
  AirPlane A3(bullet_sound,item_sound,hit_sound);
  Mini_Boss mini_boss(hit_sound);
  Second_Boss second_boss(hit_sound);
  Boss final_boss(hit_sound);
  Laser_Boss laser_boss(hit_sound);
  
  Health_item I;//체력 아이템
  Special_item I2;//스페셜 아이템
  Upgrade_item1 I3;//3방향 발사 업그레이드
  Upgrade_item2 I4;//직선 관통 레이저 업그레이드 
  Shield_item I5;//쉴드 아이템

  laser_bullet player_laser_bullet;
  laser_bullet player2_laser_bullet;
  mega_laser laser_boss_bullet;

  laser_boss_bullet.laser.env=false;

  player_laser_bullet.env=false;
  player2_laser_bullet.env=false;
  player_laser_bullet.offset.w=player2_laser_bullet.offset.w=5;
  player_laser_bullet.offset.h=player2_laser_bullet.offset.h=(Uint16)SCREEN_HEIGHT;

 
 

  if(Mix_PlayingMusic())
    Mix_HaltMusic();
  Mix_PlayMusic(stage_music,-1);//스테이지 음악 실행

  while(true){
    hole = rand()%4;

    RNG=rand();//매 프레임마다 확률 변수를 조정
    if(flag_sa < 10) flag_sa++;
    if(flag_sa2 <10) flag_sa2++;
    vector<special> t;
    it_sa = sa_1.begin();
    for(it_sa; it_sa != sa_1.end();it_sa++){
        if((*it_sa).pos() > -130) t.push_back(*it_sa);
    }
    sa_1= t;

    if(t.size()>0){
        vector<special>:: iterator ta;
        ta= (t.begin());
        bound = (*ta).pos();
    }

    //for obstacle
    vector<Obstacle> o;
    obs_it = Obs.begin();
    for(obs_it; obs_it != Obs.end(); obs_it++) {
      if((*obs_it).pos() < (480 + 130)) o.push_back(*obs_it);
      
    }
    Obs.clear();
    Obs = o;

    if(o.size()>0){
      vector<Obstacle>:: iterator oa;
      oa = (o.begin());
      o_bound = (*oa).pos();
    }

    //for obstacle
    if(o_bound > (480 + 130)) o_bound = 480+100;

    //for special
    if(bound < -130 ) bound = -100;

    if(count % 5 == 0) shootcnt = 0;
    if(count % 5 == 0) shootcnt2 = 0;
    if(count % 50 == 0)//50count마다 1기씩 생성
    {
      int i = rand()%2;
      int j = rand()%2;
      Enemy_standard tmp(i);
      Enemy_standard_2 tmp2(j);
      E.push_back(tmp);
      E2.push_back(tmp2);
    }
    else if(count % 75==0)
    {
      Enemy_standard_3 tmp3(rand()%2);
      E3.push_back(tmp3);
    }

    if(count > 0 && score > 1000 && count % 500 == 0)
    {
      Obstacle obstmp(0);
      Obstacle obstmp2(50);
      Obstacle obstmp3(100);
      Obstacle obstmp4(150);
      Obstacle obstmp5(200);
      Obstacle obstmp6(250);
      Obstacle obstmp7(300);
      Obstacle obstmp8(350);
      Obstacle obstmp9(400);
      Obstacle obstmp10(450);
      Obstacle obstmp11(500);
      Obstacle obstmp12(550);
      Obstacle obstmp13(600);
      Obstacle obstmp14(650);
      Obstacle obstmp15(700);

      Obs.push_back(obstmp);
      Obs.push_back(obstmp2);
      if(hole != 0) 
      {
        Obs.push_back(obstmp3);
        Obs.push_back(obstmp4);
      }
      Obs.push_back(obstmp5);
      if(hole != 1) 
      {
        Obs.push_back(obstmp6);
        Obs.push_back(obstmp7);
      }
      Obs.push_back(obstmp8);
      if(hole != 2) 
      {
        Obs.push_back(obstmp9);
        Obs.push_back(obstmp10);
      }
      Obs.push_back(obstmp11);
      if(hole != 3) 
      {
        Obs.push_back(obstmp12);
        Obs.push_back(obstmp13);
      }
      Obs.push_back(obstmp14);
      Obs.push_back(obstmp15);
    }

    start_time = SDL_GetTicks();//나중에 프레임 계산할 변수

   
    /////////////아이템 존재시 이동////////////////
    if(I.itm.size() > 0)
      I.control_item();

    if(I2.itm.size() > 0)
      I2.control_item();

    if(I3.itm.size() > 0)
      I3.control_item();

    if(I4.itm.size() > 0)
      I4.control_item();

    if(I5.itm.size() > 0)
      I5.control_item();
  //////////////총알 이동////////////////////////
    if(player_bullets.blt.size() > 0 )//총알들 위치 이동
      player_bullets.control_bullet();

    if(enemy_bullets.blt.size() > 0)//적 총알들 위치 이동
      enemy_bullets.control_bullet();

    if(boss_bullets.blt.size()>0)
      boss_bullets.control_bullet();

    if(mini_bullets.blt.size() > 0)
      mini_bullets.control_bullet();

    if(second_bullets.blt.size() > 0)
      second_bullets.control_bullet();
    
    if(dead != true &&(A.Got_shot(enemy_bullets,boss_bullets,mini_bullets,second_bullets, laser_boss_bullet.laser, is_boss_laser_harmful)||A.detect_collision(CB)
        ||A.check_in_border(Border,border_check))&& A.invisible_mode == 0)      //1 플레이어 피격 판정
    {
      //총알에 맞거나 충돌박스에 부딪치거나 경계밖으로 나갔을 시
      A.life--;
      A.invisible_mode = 1;
      if(!border_check)//경계 밖에 있는 경우
        A.set_pos(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);//비행기를 초기 위치로 돌려놓음
    }
    
    
    if(dead != true && A.detect_obstacle(Obs) && A.invisible_mode == 0)
    {
      A.pushed_by_obstacle(30);
    }

    if(dead2 != true && mode == 2 &&(A2.Got_shot(enemy_bullets,boss_bullets,mini_bullets,second_bullets, laser_boss_bullet.laser, is_boss_laser_harmful)
        ||A2.detect_collision(CB)||A2.check_in_border(Border,border_check2))&& A2.invisible_mode == 0)      //2 플레이어 피격 판정
    {
      //총알에 맞거나 충돌박스에 부딪치거나 경계밖으로 나갔을 시
      A2.life--;
      A2.invisible_mode = 1;
      if(!border_check2)//경계 밖에 있는 경우
         A.set_pos(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);//비행기를 초기 위치로 돌려놓음
      
    }

    if(E.size() > 0)
    {
      vector<Enemy_standard> v_tmp;


      for(it = E.begin(); it != E.end(); it++)//적 비행기들 피격 판정
      {
        Enemy_standard tmp(0);
        if(((*it).Got_shot(player_bullets)||(*it).Got_shot(player_laser_bullet)||(*it).Got_shot(player2_laser_bullet))||(bound < (*it).pos_y+32) && (bound+30 > (*it).pos_y))//비행기가 격추 당하면
        {
          if(I.itm.size() == 0&&RNG%4==0)//4분의 1확률로 아이템을 생성한다.
          {
            I.add_itm((*it).pos_x, (*it).pos_y, (*it).pos_x, (*it).pos_y + 20);
          }
          else if(I3.itm.size() == 0&&(RNG+1)%4==0)//4분의 1확률로 아이템을 생성한다.
          {
            I3.add_itm((*it).pos_x, (*it).pos_y, (*it).pos_x, (*it).pos_y + 20);
          }
          else if(I4.itm.size() == 0&&RNG%5==0)//5분의 1확률로 아이템을 생성한다.
          {
            I4.add_itm((*it).pos_x, (*it).pos_y, (*it).pos_x, (*it).pos_y + 20);
          }
          BOOM B_tmp((*it).Get_plane());
          B.push_back(B_tmp);
          (*it).~Enemy_standard();
          score += 100;
        }
        else
        {
          tmp = *it;
          v_tmp.push_back(tmp);
        }
      }

      E = v_tmp;

    }
    if(E2.size()>0)
    {
        vector<special> t;
        vector<Enemy_standard_2> v_tmp;
        it_sa = sa_1.begin();
        for(it2 = E2.begin(); it2 != E2.end(); it2++)//적 비행기들 피격 판정
        {
          Enemy_standard_2 tmp(0);
          if(((*it2).Got_shot(player_bullets)||(*it2).Got_shot(player_laser_bullet)||(*it2).Got_shot(player2_laser_bullet))|| (bound < (*it).pos_y+32) && (bound+30 > (*it).pos_y))
          {
            BOOM B_tmp((*it2).Get_plane());
            B.push_back(B_tmp);
            (*it2).~Enemy_standard_2();
            score += 100;
          if(I2.itm.size() == 0&&(RNG)%4==0)//4분의 1확률로 아이템을 생성한다.
          {
            I2.add_itm((*it2).pos_x, (*it2).pos_y, (*it2).pos_x, (*it2).pos_y + 20);
          }
          else if(I5.itm.size() == 0&&(RNG+1)%2==0)//5분의 1확률로 아이템을 생성한다.
          {
            I5.add_itm((*it2).pos_x, (*it2).pos_y, (*it2).pos_x, (*it2).pos_y + 20);
          }
          }
          else
          {
            tmp = *it2;
            v_tmp.push_back(tmp);
          }
        }

        E2=v_tmp;
    }
    
    if(E3.size()>0)
    {
        vector<special> t;
        vector<Enemy_standard_3> v_tmp;
        it_sa = sa_1.begin();
        for(it3 = E3.begin(); it3 != E3.end(); it3++)//적 비행기들 피격 판정
        {
          Enemy_standard_3 tmp(0);
          if(((*it3).Got_shot(player_bullets)||(*it3).Got_shot(player_laser_bullet)||(*it3).Got_shot(player2_laser_bullet))|| (bound < (*it3).pos_y+32) && (bound+30 > (*it3).pos_y))
          {
            SDL_Rect e_rect=(*it3).Get_plane();
            BOOM B_tmp(e_rect);
            B.push_back(B_tmp);
            enemy_bullets.add_blt(3,3,e_rect.x,e_rect.y);
            enemy_bullets.add_blt(3,-3,e_rect.x,e_rect.y);
            enemy_bullets.add_blt(-3,3,e_rect.x,e_rect.y);
            enemy_bullets.add_blt(-3,-3,e_rect.x,e_rect.y);

          if(I.itm.size() == 0&&(RNG)%3==0)//4분의 1확률로 아이템을 생성한다.
          {
            I.add_itm((*it3).pos_x, (*it3).pos_y, (*it3).pos_x, (*it3).pos_y + 20);
          }

            (*it3).~Enemy_standard_3();
            score += 100;
          }
          else
          {
            tmp = *it3;
            v_tmp.push_back(tmp);
          }
        }

        E3=v_tmp;
    }
    is_laser=mini_boss.Got_shot(player_laser_bullet,boom_mode,RNG);//레이저 공격에 맞은지 판별
    is_laser2=mini_boss.Got_shot(player2_laser_bullet,boom_mode,RNG);
    if(mini_boss.amount == 1 &&(mini_boss.Got_shot(player_bullets, boom_mode)||is_laser||is_laser2 )&& score >= 200) {
        BOOM tmp(mini_boss.Get_plane());
        tmp.three = boom_mode;
        mini_Boss_BOOM.push_back(tmp);
        if(is_laser)//레이저 공격이면
          mini_boss.loss_life(score,explosion_sound2,0.1);//프레임당 0.1데미지가 들어간다.
        if(is_laser2)
          mini_boss.loss_life(score,explosion_sound2,0.1);
        if(!is_laser&&!is_laser2)//일반 공격이면
          mini_boss.loss_life(score,explosion_sound2,1);//1의 데미지가 들어간다.
        if(mini_boss.life <= 0)//미니 보스 사망시
        {
          if(I2.itm.size() == 0 )
             I2.add_itm(mini_boss.pos_x, mini_boss.pos_y, mini_boss.pos_x, mini_boss.pos_y + 20);
          if(RNG%2==0)// 3방향 업글 아이템이 나오거나 레이저 업글 아이템이 나온다.
             I3.add_itm(mini_boss.pos_x, mini_boss.pos_y, mini_boss.pos_x+20, mini_boss.pos_y + 20);
          else
             I4.add_itm(mini_boss.pos_x, mini_boss.pos_y, mini_boss.pos_x+20, mini_boss.pos_y + 20);
        }
    }   // have to add the condition when the mini boss appear
    is_laser=second_boss.Got_shot(player_laser_bullet,boom_mode,RNG);
    is_laser2=second_boss.Got_shot(player2_laser_bullet,boom_mode,RNG);
    if(second_boss.amount == 1 && (second_boss.Got_shot(player_bullets, boom_mode)||is_laser||is_laser2 )&& score >= 200) {
        BOOM tmp(second_boss.Get_plane());
        tmp.three = boom_mode;
        second_Boss_BOOM.push_back(tmp);
        if(is_laser)//레이저 공격이면
          second_boss.loss_life(score,explosion_sound2,0.1);//프레임당 0.1데미지가 들어간다.
        if(is_laser2)
          second_boss.loss_life(score,explosion_sound2,0.1);
        if(!is_laser&&!is_laser2)//일반 공격이면
          second_boss.loss_life(score,explosion_sound2,1);//1의 데미지가 들어간다.
        if( second_boss.life <= 0)
        {
          if(I2.itm.size() == 0 )
            I2.add_itm(second_boss.pos_x, second_boss.pos_y, second_boss.pos_x, second_boss.pos_y + 20);
          if(RNG%2==0)
            I3.add_itm(second_boss.pos_x, second_boss.pos_y, second_boss.pos_x, second_boss.pos_y + 20);
          else
            I4.add_itm(second_boss.pos_x, second_boss.pos_y, second_boss.pos_x+20, second_boss.pos_y + 20);
        }
    }   // have to add the condition when the mini boss appear
    is_laser=laser_boss.Got_shot(player_laser_bullet,boom_mode,RNG);
    is_laser2=laser_boss.Got_shot(player2_laser_bullet,boom_mode,RNG);
    if(laser_boss.Got_shot(player_bullets, boom_mode)||is_laser||is_laser2) // have to add the condition when the mini boss appear
    {
      BOOM tmp(laser_boss.Get_plane());
      tmp.three = boom_mode;
      laser_Boss_B4.push_back(tmp);
      if(is_laser)
          laser_boss.loss_life(score,explosion_sound2,0.1,laser_boss_bullet.laser);
      if(is_laser2)
          laser_boss.loss_life(score,explosion_sound2,0.1,laser_boss_bullet.laser);
      if(!is_laser&&!is_laser2)
          laser_boss.loss_life(score,explosion_sound2,1,laser_boss_bullet.laser);
      if( laser_boss.life <= 0)
        {
          if(I.itm.size() == 0 )
            I.add_itm(laser_boss.pos_x, laser_boss.pos_y, laser_boss.pos_x, laser_boss .pos_y+ 20);
          
          laser_boss_bullet.~mega_laser();
        }
    }

    is_laser=final_boss.Got_shot(player_laser_bullet,boom_mode,RNG);
    is_laser2=final_boss.Got_shot(player2_laser_bullet,boom_mode,RNG);
    if(final_boss.amount == 1 && (final_boss.Got_shot(player_bullets, boom_mode)||is_laser||is_laser2)&& score >= 2000) // have to add the condition when the mini boss appear
    {
      BOOM tmp(final_boss.Get_plane());
      tmp.three = boom_mode;
      final_Boss_BOOM.push_back(tmp);
      if(is_laser)
          final_boss.loss_life(score,explosion_sound2,0.1);
      if(is_laser2)
          final_boss.loss_life(score,explosion_sound2,0.1);
       if(!is_laser&&!is_laser2)
          final_boss.loss_life(score,explosion_sound2,1);
    }

 

    if(A.Got_item(I.itm))//체력 아이템 획득시
    {
      if(A.life < 3)
      {
        A.increaseLife();
      }
    }

    if(A.Got_item(I2.itm))//스페셜 아이템 획득시
    {
      if(A.SA_count < 3)
      {
        A.increaseSA();
      }
    }

    if(A.Got_item(I3.itm))//3방향 발사 업그레이드 아이템 획득시
    {
      A.bullet_mode=2;
    }

    if(A.Got_item(I4.itm))//레이저 업그레이드 아이템 획득시
    {
      A.bullet_mode=3;
    }
    if(A.Got_item(I5.itm)&&!A.shield_mode)//쉴드 아이템 획득시
    {
      A.shield_mode=true;
    }
    else
      A.shield(shield_counter);

    if(final_boss.amount == 0)
    {
      stage_clear();
      break;
    }

    if(sa_1.size()>0){
        it_sa = sa_1.begin();
        enemy_bullets.eliminate(*it_sa);
        boss_bullets.eliminate(*it_sa);
        mini_bullets.eliminate(*it_sa);
        second_bullets.eliminate(*it_sa);
    }

    //for obstacle
    if(Obs.size()>0) {
      obs_it = Obs.begin();
    }

    if(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT)//버튼 누르면 꺼저야 되는데 안 꺼짐 수정 사항
			   break;
      if(event.type==SDL_VIDEORESIZE)
        handle_resize(event.resize);
    }

    keystates = SDL_GetKeyState(NULL);
    if(!CB.empty())
      CB.clear();

 
    if(keystates[SDLK_ESCAPE])
      break;
    /////////적기 이동과 충돌 판정 추가/////////
    if(E.size() > 0)//적 비행기 이동 및 발사
    {
      for(it = E.begin(); it != E.end(); it++)
      {
        CB.push_back((*it).control_plane(enemy_bullets));
        //충돌 판정을 위해 offset을 list에 저장
      }
    }
    if(E2.size() >0){
      for(it2 = E2.begin(); it2 != E2.end(); it2++)
      {
         CB.push_back((*it2).control_plane(enemy_bullets));
      }
    }
    if(E3.size() >0){
      for(it3 = E3.begin(); it3 != E3.end(); it3++)
      {
         CB.push_back((*it3).control_plane());
      }
    }

    //////////////////////////////////////////
    if(sa_1.size() >0)
    {
        for(it_sa = sa_1.begin(); it_sa != sa_1.end(); it_sa++){
            (*it_sa).control_bullet();
        }
    }

    //for obstacle
    if(Obs.size() >0)
    {
        for(obs_it = Obs.begin(); obs_it != Obs.end(); obs_it++){
            (*obs_it).control_bullet();
        }
    }
    //////////////1p플레이어 조작//////////////
    if(keystates[SDLK_UP])
      A.control_plane(0,-4,player_laser_bullet);

    if(keystates[SDLK_DOWN])
      A.control_plane(0, 4,player_laser_bullet);

    if(keystates[SDLK_LEFT])
      A.control_plane(-4, 0,player_laser_bullet);

    if(keystates[SDLK_RIGHT])
      A.control_plane(4, 0,player_laser_bullet);


    if((keystates[SDLK_o] && dead != true))
    {
      if(shootcnt == 0) {
          A.shooting(player_bullets,player_laser_bullet);
          if(player_laser_bullet.env&&played_channel==-1)
            played_channel=Mix_PlayChannel(-1,beam_sound,-1);
          shootcnt = 1;
      }
    }
    else{ 
      if(player_laser_bullet.env==false)
        played_channel=-1;
      player_laser_bullet.env=false;
      if(played_channel!=-1)
        Mix_HaltChannel(played_channel);
    }
    /*
    if(mode ==2&&keystates[SDLK_f] && dead2 != true)
      {
        if(shootcnt2 == 0) {
            A2.shooting(player_bullets,player2_laser_bullet);
            shootcnt2 = 1;
        }
      }
    */
 
    if(keystates[SDLK_p] && flag_sa == 10 && dead != true)    /// SHOULD HAVE FLAG TO A ABILITY IS USED NUMEROUS TIMES BY PRESSING ONCE.
    {

        if(A.SA_count >0){
           Mix_PauseMusic();//스테이지 음악을 PAUSE 하고
           Mix_VolumeChunk(special_sound,MIX_MAX_VOLUME);
           Mix_PlayChannel(-1,special_sound,0);//효과음 출력
           A.SA_count --; //// Put image for SA
           flag_sa = 0;
           switch (SA){
           case 0:
               {
                   special tmp(0,0);
                   special tmp2(100,0);
                   special tmp3(200,0);
                   special tmp4(300,0);
                   special tmp5(400,0);
                   special tmp6(500,0);
                   special tmp7(600,0);
                   special tmp8(700,0);
                   sa_1.push_back(tmp);
                   sa_1.push_back(tmp2);
                   sa_1.push_back(tmp3);
                   sa_1.push_back(tmp4);
                   sa_1.push_back(tmp5);
                   sa_1.push_back(tmp6);
                   sa_1.push_back(tmp7);
                   sa_1.push_back(tmp8);
               break;
              }
               case 1:
              {
                  special tmp(0,1);
                  special tmp2(100,1);
                  special tmp3(200,1);
                  special tmp4(300,1);
                  special tmp5(400,1);
                  special tmp6(500,1);
                  special tmp7(600,1);
                  special tmp8(700,1);
                  sa_1.push_back(tmp);
                  sa_1.push_back(tmp2);
                  sa_1.push_back(tmp3);
                  sa_1.push_back(tmp4);
                  sa_1.push_back(tmp5);
                  sa_1.push_back(tmp6);
                  sa_1.push_back(tmp7);
                  sa_1.push_back(tmp8);
                  break;
              }
               case 2:
              {
                  special tmp(0,2);
                  special tmp2(100,2);
                  special tmp3(200,2);
                  special tmp4(300,2);
                  special tmp5(400,2);
                  special tmp6(500,2);
                  special tmp7(600,2);
                  special tmp8(700,2);
                  sa_1.push_back(tmp);
                  sa_1.push_back(tmp2);
                  sa_1.push_back(tmp3);
                  sa_1.push_back(tmp4);
                  sa_1.push_back(tmp5);
                  sa_1.push_back(tmp6);
                  sa_1.push_back(tmp7);
                  sa_1.push_back(tmp8);
                   break;
              }
               case 3:
              {
                  special tmp(0,3);
                  special tmp2(100,3);
                  special tmp3(200,3);
                  special tmp4(300,3);
                  special tmp5(400,3);
                  special tmp6(500,3);
                  special tmp7(600,3);
                  special tmp8(700,3);
                  sa_1.push_back(tmp);
                  sa_1.push_back(tmp2);
                  sa_1.push_back(tmp3);
                  sa_1.push_back(tmp4);
                  sa_1.push_back(tmp5);
                  sa_1.push_back(tmp6);
                  sa_1.push_back(tmp7);
                  sa_1.push_back(tmp8);
                  break;
              }
               case 4:
              {
                  special tmp(0,4);
                  special tmp2(100,4);
                  special tmp3(200,4);
                  special tmp4(300,4);
                  special tmp5(400,4);
                  special tmp6(500,4);
                  special tmp7(600,4);
                  special tmp8(700,4);
                  sa_1.push_back(tmp);
                  sa_1.push_back(tmp2);
                  sa_1.push_back(tmp3);
                  sa_1.push_back(tmp4);
                  sa_1.push_back(tmp5);
                  sa_1.push_back(tmp6);
                  sa_1.push_back(tmp7);
                  sa_1.push_back(tmp8);
                  break;
              }
           }
        }
    }
    //////////////2p플레이어 조작//////////////
      if(mode == 2 && dead2 != true)
      {
        if(keystates[SDLK_w])
            A2.control_plane(0,-4,player2_laser_bullet);

          if(keystates[SDLK_s])
            A2.control_plane(0, 4,player2_laser_bullet);

          if(keystates[SDLK_a])
            A2.control_plane(-4, 0,player2_laser_bullet);

          if(keystates[SDLK_d])
            A2.control_plane(4, 0,player2_laser_bullet);
        if(keystates[SDLK_f])
          {
            if(shootcnt == 0) {
                A2.shooting(player_bullets,player2_laser_bullet);
                if(player2_laser_bullet.env&&played_channel2==-1)
                  played_channel2=Mix_PlayChannel(-1,beam_sound,-1);
                shootcnt = 1;
            }
          }
        else{
          player2_laser_bullet.env=false;
          if(player2_laser_bullet.env==false)
             played_channel2=-1;
          player2_laser_bullet.env=false;
          if(played_channel2!=-1)
            Mix_HaltChannel(played_channel2);
          }
         
          if(mode == 2 && keystates[SDLK_g])    /// SHOULD HAVE FLAG TO AVOID SPECIAL ABILITY IS USED NUMEROUS TIMES BY PRESSING ONCE.
          {
             
              if(A2.SA_count >0 && flag_sa2 == 10){
                 Mix_PauseMusic();
                 Mix_VolumeChunk(special_sound,MIX_MAX_VOLUME);
                 Mix_PlayChannel(-1,special_sound,0);
                 A2.SA_count --; //// Put image for SA
                 flag_sa2= 0;
                 switch (SA2){
                 case 0:
                     {
                         special tmp(0, 0);
                         special tmp2(100, 0);
                         special tmp3(200, 0);
                         special tmp4(300, 0);
                         special tmp5(400, 0);
                         special tmp6(500, 0);
                         special tmp7(600, 0);
                         special tmp8(700, 0);
                         sa_1.push_back(tmp);
                         sa_1.push_back(tmp2);
                         sa_1.push_back(tmp3);
                         sa_1.push_back(tmp4);
                         sa_1.push_back(tmp5);
                         sa_1.push_back(tmp6);
                         sa_1.push_back(tmp7);
                         sa_1.push_back(tmp8);
                     break;
                    }
                     case 1:
                    {
                        special tmp(0, 1);
                        special tmp2(100, 1);
                        special tmp3(200, 1);
                        special tmp4(300, 1);
                        special tmp5(400, 1);
                        special tmp6(500, 1);
                        special tmp7(600, 1);
                        special tmp8(700, 1);
                        sa_1.push_back(tmp);
                        sa_1.push_back(tmp2);
                        sa_1.push_back(tmp3);
                        sa_1.push_back(tmp4);
                        sa_1.push_back(tmp5);
                        sa_1.push_back(tmp6);
                        sa_1.push_back(tmp7);
                        sa_1.push_back(tmp8);
                        break;
                    }
                     case 2:
                    {
                        special tmp(0, 2);
                        special tmp2(100, 2);
                        special tmp3(200, 2);
                        special tmp4(300, 2);
                        special tmp5(400, 2);
                        special tmp6(500, 2);
                        special tmp7(600, 2);
                        special tmp8(700, 2);
                        sa_1.push_back(tmp);
                        sa_1.push_back(tmp2);
                        sa_1.push_back(tmp3);
                        sa_1.push_back(tmp4);
                        sa_1.push_back(tmp5);
                        sa_1.push_back(tmp6);
                        sa_1.push_back(tmp7);
                        sa_1.push_back(tmp8);
                         break;
                    }
                     case 3:
                    {
                        special tmp(0, 3);
                        special tmp2(100, 3);
                        special tmp3(200, 3);
                        special tmp4(300, 3);
                        special tmp5(400, 3);
                        special tmp6(500, 3);
                        special tmp7(600, 3);
                        special tmp8(700, 3);
                        sa_1.push_back(tmp);
                        sa_1.push_back(tmp2);
                        sa_1.push_back(tmp3);
                        sa_1.push_back(tmp4);
                        sa_1.push_back(tmp5);
                        sa_1.push_back(tmp6);
                        sa_1.push_back(tmp7);
                        sa_1.push_back(tmp8);
                        break;
                    }
                     case 4:
                    {
                        special tmp(0, 4);
                        special tmp2(100, 4);
                        special tmp3(200, 4);
                        special tmp4(300, 4);
                        special tmp5(400, 4);
                        special tmp6(500, 4);
                        special tmp7(600, 4);
                        special tmp8(700, 4);
                        sa_1.push_back(tmp);
                        sa_1.push_back(tmp2);
                        sa_1.push_back(tmp3);
                        sa_1.push_back(tmp4);
                        sa_1.push_back(tmp5);
                        sa_1.push_back(tmp6);
                        sa_1.push_back(tmp7);
                        sa_1.push_back(tmp8);
                        break;
                    }
                 }
              }
          }
          
          
      }

          if(mode ==2&&keystates[SDLK_g])    /// SHOULD HAVE FLAG TO AVOID SPECIAL ABILITY IS USED NUMEROUS TIMES BY PRESSING ONCE.
          {
             
              if(A2.SA_count >0&& flag_sa2==10){
                 Mix_PauseMusic();
                 Mix_VolumeChunk(special_sound,MIX_MAX_VOLUME);
                 Mix_PlayChannel(-1,special_sound,0);
                 
                 A2.SA_count --; //// Put image for SA
                 switch (SA2){
                 case 0:
                     {
                         special tmp(0, 0);
                         special tmp2(100, 0);
                         special tmp3(200,0);
                         special tmp4(300,0);
                         special tmp5(400,0);
                         special tmp6(500,0);
                         special tmp7(600,0);
                         special tmp8(700,0);
                         sa_1.push_back(tmp);
                         sa_1.push_back(tmp2);
                         sa_1.push_back(tmp3);
                         sa_1.push_back(tmp4);
                         sa_1.push_back(tmp5);
                         sa_1.push_back(tmp6);
                         sa_1.push_back(tmp7);
                         sa_1.push_back(tmp8);
                     break;
                    }
                     case 1:
                    {
                        special tmp(0,1);
                        special tmp2(100,1);
                        special tmp3(200,1);
                        special tmp4(300,1);
                        special tmp5(400,1);
                        special tmp6(500,1);
                        special tmp7(600,1);
                        special tmp8(700,1);
                        sa_1.push_back(tmp);
                        sa_1.push_back(tmp2);
                        sa_1.push_back(tmp3);
                        sa_1.push_back(tmp4);
                        sa_1.push_back(tmp5);
                        sa_1.push_back(tmp6);
                        sa_1.push_back(tmp7);
                        sa_1.push_back(tmp8);
                        break;
                    }
                     case 2:
                    {
                        special tmp(0,2);
                        special tmp2(100,2);
                        special tmp3(200,2);
                        special tmp4(300,2);
                        special tmp5(400,2);
                        special tmp6(500,2);
                        special tmp7(600,2);
                        special tmp8(700,2);
                        sa_1.push_back(tmp);
                        sa_1.push_back(tmp2);
                        sa_1.push_back(tmp3);
                        sa_1.push_back(tmp4);
                        sa_1.push_back(tmp5);
                        sa_1.push_back(tmp6);
                        sa_1.push_back(tmp7);
                        sa_1.push_back(tmp8);
                        break;
                    }
                     case 3:
                    {
                        special tmp(0,3);
                        special tmp2(100,3);
                        special tmp3(200,3);
                        special tmp4(300,3);
                        special tmp5(400,3);
                        special tmp6(500,3);
                        special tmp7(600,3);
                        special tmp8(700,3);
                        sa_1.push_back(tmp);
                        sa_1.push_back(tmp2);
                        sa_1.push_back(tmp3);
                        sa_1.push_back(tmp4);
                        sa_1.push_back(tmp5);
                        sa_1.push_back(tmp6);
                        sa_1.push_back(tmp7);
                        sa_1.push_back(tmp8);
                        break;
                    }
                     case 4:
                    {
                        special tmp(0,4);
                        special tmp2(100,4);
                        special tmp3(200,4);
                        special tmp4(300,4);
                        special tmp5(400,4);
                        special tmp6(500,4);
                        special tmp7(600,4);
                        special tmp8(700,4);
                        sa_1.push_back(tmp);
                        sa_1.push_back(tmp2);
                        sa_1.push_back(tmp3);
                        sa_1.push_back(tmp4);
                        sa_1.push_back(tmp5);
                        sa_1.push_back(tmp6);
                        sa_1.push_back(tmp7);
                        sa_1.push_back(tmp8);
                        break;
                    }
                 }
              }
          }
          /* 왜 코드가 중복돼서 삽입되어 있는거야..
          if(mode ==2&&keystates[SDLK_w])
            A2.control_plane(0,-4);

          if(mode ==2&&keystates[SDLK_s])
            A2.control_plane(0, 4);

          if(mode ==2&&keystates[SDLK_a])
            A2.control_plane(-4, 0);

          if(mode ==2&&keystates[SDLK_d])
            A2.control_plane(4, 0);
          */


    if(A.invisible_mode == 1)//투명화 상태, 투명도 조절
      A.invisible(plane_1p);

    if( mode ==2&&A2.invisible_mode == 1)//투명화 상태, 투명도 조절
      A2.invisible(plane_2p);
     //////////////배경을 움직이게 하는 부분//////////////
    if(background_count++ != 480);
    else
      background_count = 0;

    //이미지 그리는 부분
    apply_surface(0, -480 + background_count, background2,buffer,NULL);//백그라운드 그리는거
    apply_surface(0, 0 + background_count, background,buffer,NULL);//백그라운드 그리는거
    enemy_bullets.bullet_apply_surface(bullet_basic, buffer,NULL);//적 총알들
    boss_bullets.bullet_apply_surface(bullet_boss, buffer, NULL);
    second_bullets.bullet_apply_surface(bullet_second, buffer, NULL);
    mini_bullets.bullet_apply_surface(bullet_mini, buffer, NULL);
    player_bullets.bullet_apply_surface(bullet, buffer, NULL);//사용자 총알들
    if(dead != true)
    {
      A.plane_apply_surface(plane_1p, buffer, NULL); //사용자 비행기
      if(A.shield_mode)
        apply_surface(A.get_shield_x(),A.get_shield_y(),shield,buffer,NULL);
    }
    else if (dead == true) {
      A.~AirPlane();
    }

    if(mode == 2 && dead2 != true) {
      
      A2.plane_apply_surface(plane_2p, buffer,NULL);
      if(A2.shield_mode)
        apply_surface(A2.get_shield_x(),A2.get_shield_y(),shield,buffer,NULL);
    } //사용자 비행기
    /////////////아이템 그리는 부분///////////////
    if(I.itm.size()!=0)
    {
      I.item_apply_surface(I.item, buffer, NULL);
    }

    if(I2.itm.size()!=0)
    {
      I2.item_apply_surface(I2.item, buffer, NULL);
    }

    if(I3.itm.size()!=0)
    {
      I3.item_apply_surface(I3.item, buffer, NULL);
    }
    if(I4.itm.size()!=0)
    {
      I4.item_apply_surface(I4.item, buffer, NULL);
    }
    if(I5.itm.size()!=0)
    {
      I5.item_apply_surface(I5.item, buffer, NULL);
    }
    ///////////적 비행기 그리는 부분///////////////
    if( E.size() > 0)//적 비행기
    {
      for( it = E.begin(); it != E.end(); it++)
      {
        (*it).enemy_apply_surface(enemy, buffer, NULL);
      }
    }
    if( E2.size() > 0)
    {
      for( it2 = E2.begin(); it2 != E2.end(); it2++)
      {
        (*it2).enemy_apply_surface(buffer, NULL);
      }
    }
    if( E3.size() > 0)
    {
      SDL_Rect tmp;
      tmp.x=0;
      tmp.y=4;
      tmp.w=32;
      tmp.h=28;
      for( it3 = E3.begin(); it3 != E3.end(); it3++)
      {
        (*it3).enemy_apply_surface(buffer, &tmp);
      }
    }
    if(sa_1.size() >0)
    {
        for(it_sa = sa_1.begin(); it_sa != sa_1.end(); it_sa++){
            (*it_sa).apply_surface(buffer, NULL);
        }
    }

    //for obstacle
    if(Obs.size() >0)
    {
        for(obs_it = Obs.begin(); obs_it != Obs.end(); obs_it++){
            (*obs_it).apply_surface(buffer, NULL);
        }
    }
        //////////////보스 추가 조건//////////////

    if(laser_boss.amount == 1 && score >= 6000) {
      laser_boss.enemy_apply_surface(buffer,NULL);
      CB.push_back(laser_boss.control_plane(laser_boss_bullet.laser));
    }

    if(mini_boss.amount == 1 && score >= 13000){
      mini_boss.enemy_apply_surface(buffer, NULL);
      CB.push_back(mini_boss.control_plane(mini_bullets));
    } // have to add the condition when the mini boss appear

    if(second_boss.amount == 1 && score >= 1200){
      second_boss.enemy_apply_surface(buffer, NULL);
      CB.push_back(second_boss.control_plane(second_bullets)); 
    }//세컨드 보스 등장 조건

    if(final_boss.amount == 1 && score >= 20000){
      final_boss.enemy_apply_surface(buffer, NULL);
      CB.push_back(final_boss.control_plane(boss_bullets));
    } // have to add the condition when the mini boss appear
  
    if( B.size() > 0)//폭발
    {
      vector<BOOM> B_tmp;

      for(B_it = B.begin(); B_it != B.end(); B_it++)
      {
        if((*B_it).b.count <  11)
        {
          (*B_it).boom_apply_surface(boom,buffer,NULL,explosion_sound1);
          B_tmp.push_back(*B_it);
        }
        else
        {
          (*B_it).~BOOM();
        }
      }
      B = B_tmp;
    }

    if(mini_Boss_BOOM.size() > 0)                                    //보스 맞을 때 폭발 구현
    {
      vector<BOOM> B_tmp;

      for(B_it = mini_Boss_BOOM.begin(); B_it != mini_Boss_BOOM.end(); B_it++)
      {
        if((*B_it).b.count <  8)
        {
          sprite_surface(buffer,mini_boss.Get_plane(), explosion, 8, 1, (*B_it).b.count, (*B_it).three);
          (*B_it).b.count++;
          B_tmp.push_back(*B_it);
        }

        else
        {
          (*B_it).~BOOM();
        }
      }
      mini_Boss_BOOM = B_tmp;
    }
    if(second_Boss_BOOM.size() > 0)                                    //보스 맞을 때 폭발 구현
    {
      vector<BOOM> B_tmp;

      for(B_it = second_Boss_BOOM.begin(); B_it != second_Boss_BOOM.end(); B_it++)
      {
        if((*B_it).b.count <  8)
        {
          sprite_surface(buffer,second_boss.Get_plane(), explosion, 8, 1, (*B_it).b.count, (*B_it).three);
          (*B_it).b.count++;
          B_tmp.push_back(*B_it);
        }

        else
        {
          (*B_it).~BOOM();
        }
      }
      second_Boss_BOOM = B_tmp;
    }
    if(final_Boss_BOOM.size() > 0)                                    //보스 맞을 때 폭발 구현
    {
      vector<BOOM> B_tmp;

      for(B_it = final_Boss_BOOM.begin(); B_it != final_Boss_BOOM.end(); B_it++)
      {
        if((*B_it).b.count <  8)
        {
          sprite_surface(buffer, final_boss.Get_plane(), explosion, 8, 1, (*B_it).b.count,(*B_it).three);
          (*B_it).b.count++;
          B_tmp.push_back(*B_it);
        }
        else
        {
          (*B_it).~BOOM();
        }
      }
      final_Boss_BOOM = B_tmp;
    }


    if(A.life == 0 && mode == 1)//생명력 0
    {
      return game_over();

    }

    if(A.life == 1)//생명력 1
      apply_surface(550, 10, life, buffer,NULL);
    else if(A.life == 2)
    {
      apply_surface(550, 10, life, buffer,NULL); apply_surface(580, 10, life, buffer,NULL);
    }
    else if(A.life == 3)
    {
      apply_surface(550, 10, life, buffer,NULL); apply_surface(580, 10, life, buffer,NULL); apply_surface(610, 10, life, buffer,NULL);
    }
    
    if(mode == 2)
    {
      if(A.life == 0 && A2.life == 0)
      {
       return game_over();
    
      }
      else if(A.life == 0 && A2.life > 0)//생명력 0
      {
        dead = true;
      }

      else if(A2.life == 0 && A.life > 0)//생명력 0
      {
        dead2 = true;
      }

      if(A2.life == 1)//생명력 1
        apply_surface(550, 30, life, buffer,NULL);
      else if(A2.life == 2)
      {
        apply_surface(550, 30, life, buffer,NULL); apply_surface(580, 30, life, buffer,NULL);
      }
      else if(A2.life == 3)
      {
        apply_surface(550, 30, life, buffer,NULL); apply_surface(580, 30, life, buffer,NULL); apply_surface(610, 30, life, buffer,NULL);
      }
    }

    if (mode == 1) {
      if(A.SA_count == 1)//생명력 1
        apply_surface(550, 450, sapoint, buffer,NULL);
        else if(A.SA_count == 2)
        {
          apply_surface(550, 450, sapoint, buffer,NULL); apply_surface(580, 450, sapoint, buffer,NULL);
        }
        else if(A.SA_count == 3)
        {
          apply_surface(550, 450, sapoint, buffer ,NULL); apply_surface(580, 450, sapoint, buffer,NULL); apply_surface(610, 450, sapoint, buffer,NULL);
        }
      }

    if (mode == 2)
    {
      if(A.SA_count == 1)//생명력 1
        apply_surface(550, 430, sapoint, buffer,NULL);
      else if(A.SA_count == 2)
      {
        apply_surface(550, 430, sapoint, buffer,NULL); apply_surface(580, 430, sapoint, buffer,NULL);
      }
      else if(A.SA_count == 3)
      {
        apply_surface(550, 430, sapoint, buffer ,NULL); apply_surface(580, 430, sapoint, buffer,NULL); apply_surface(610, 430, sapoint, buffer,NULL);
      }

      if(A2.SA_count == 1)//생명력 1
        apply_surface(550, 450, sapoint, buffer,NULL);
      else if(A2.SA_count == 2)
      {
        apply_surface(550, 450, sapoint, buffer,NULL); apply_surface(580, 450, sapoint, buffer,NULL);
      }
      else if(A2.SA_count == 3)
      {
        apply_surface(550, 450, sapoint, buffer ,NULL); apply_surface(580, 450, sapoint, buffer,NULL); apply_surface(610, 450, sapoint, buffer,NULL);
      }
    }
    ////////////레이저 그래픽 구현 파트///////////////
    if(player_laser_bullet.env&&A.bullet_mode==3)//레이저 발사시 
      
      SDL_FillRect(buffer,&(player_laser_bullet.offset),SDL_MapRGB(buffer->format,200,0,0));/*SDL_MapRGB(buffer->format,200,0,0));*/ //레이저의 범위에 해당하는 부분을 옅은 붉은 색으로 칠함
    if(player2_laser_bullet.env&&A2.bullet_mode==3)
      SDL_FillRect(buffer,&(player2_laser_bullet.offset),SDL_MapRGB(buffer->format,200,0,0));//레이저의 범위에 해당하는 부분을 옅은 붉은 색으로 칠함
    if(laser_boss_bullet.laser.env && laser_boss_bullet.laser.offset.w <= 30)
    {
      is_boss_laser_harmful = false;
      laser_boss_bullet.show_effect(buffer,NULL);
    }
    else if(laser_boss_bullet.laser.env && laser_boss_bullet.laser.offset.w >30)
    {
      is_boss_laser_harmful = true;
      laser_boss_bullet.show_effect(buffer,NULL);
      //SDL_FillRect(buffer,&(laser_boss_bullet.offset),SDL_MapRGB(buffer->format,100,0,100));
    }

    ostringstream sc;
    sc<< score;
    message5 = TTF_RenderText_Solid(font3, sc.str().c_str(), textColor);
    apply_surface(0, 0, message5, buffer, NULL);

    //fps 계산
    delay = 1000/40 - (SDL_GetTicks() - start_time);
    if(delay > 0)
      SDL_Delay(delay);
   
    show_screen();
      //SDL_Flip(buffer);
     count ++;
  
    
  }
  return false;
}
int main()
{
 

 //_special special_one;
  init();//초기화 함수
  load_files();//이미지,폰트,bgm 로드하는 함수
  bool retry=false;
  bool run=true;
  run=menu();
  if(EXIT == 1||!run)
  {
    SDL_free_all();
    return 0;
  }
  run=menu3();
  if(!run)
  {
    SDL_free_all();
    return 0;
  }
  run=menu2();
  if(!run)
  {
    SDL_free_all();
    return 0;
  }
  do{
    retry=game_start();
  }while(retry);
  SDL_free_all();
  return 0;
}




bool init()
{//고칠 것: if문 추가해서 init했을 때 실패하면 false반환하게끔
  SDL_Init(SDL_INIT_EVERYTHING);//SDL initialize
  TTF_Init();
  Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);//오디오를 스테레오로 하여서 연다

  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF |SDL_RESIZABLE);
  buffer=SDL_CreateRGBSurface( SDL_SWSURFACE,SCREEN_WIDTH, SCREEN_HEIGHT,
                              screen->format->BitsPerPixel,screen->format->Rmask,
                              screen->format->Gmask,screen->format->Bmask,
                              screen->format->Amask);
  buffer_rect.x=0;
  buffer_rect.y=0;
  buffer_rect.w=SCREEN_WIDTH;
  buffer_rect.h=SCREEN_HEIGHT;
  
  screen_rect.x=0;
  screen_rect.y=0;
  screen_rect.w=SCREEN_WIDTH;
  screen_rect.h=SCREEN_HEIGHT;
  //SDL_RESIZABLE 추가하여 윈도우 크기 조절을 가능하게 함
  SDL_WM_SetCaption("deombo", NULL);
  if(screen==NULL)
  {
    return false;
  }
  else
    return true;
}
//return true;

bool load_files()
{//고칠 것: if문 추가해서 init했을 때 실패하면 false반환하게끔
  explosion = load_image("assets/explosion.png");

  life = load_image("assets/life.gif");                   //life
  background = load_image("assets/background.png");//배경화면
  background2 = load_image("assets/background2.png");//배경화면
  intro = load_image("assets/intro.jpg");
  bullet = load_image("assets/BULLET.png");// 총알 이미지
  bullet_basic = load_image("assets/bullet.gif");
  bullet_boss = load_image("assets/bossbullet.png");
  bullet_second = load_image("assets/bossbullet.png");
  bullet_mini = load_image("assets/bossbullet.png");
  
  plane = load_image("assets/p2.gif");// 사용자 비행기 이미지
  plane2 = load_image("assets/aircraft1.png");
  plane3 = load_image("assets/aircraft3.png");
  plane4 = load_image("assets/aircraft5.png");
  plane5 = load_image("assets/aircraft6.png");
  plane_2p = load_image("assets/aircraft3.png");
  pick1 = load_image("assets/p2.png");
  pick2 = load_image("assets/aircraft6.png");
  frame = load_image("assets/blueframe.png");
  frame2 = load_image("assets/redframe.png");
  arrow = load_image("assets/arrow.png");
  sapoint = load_image("assets/sapoint1.png");
  shield =load_image("assets/shield_effect.png");

  font = TTF_OpenFont("assets/Terminus.ttf", 22);//작은 안내문 폰트
  font2 = TTF_OpenFont("assets/Starjout.ttf", 84);//제목 폰트
  font3 = TTF_OpenFont("assets/Starjout.ttf",24);

  //오디오 로드
  menu_music=Mix_LoadMUS("assets/Audio/menu_music.mp3");
  stage_music=Mix_LoadMUS("assets/Audio/battle_music.wav");
  
  game_over_sound=Mix_LoadWAV("assets/Audio/gameover.wav");
  selection_sound=Mix_LoadWAV("assets/Audio/selection.wav");
  bullet_sound=Mix_LoadWAV("assets/Audio/laser_shot.wav");
  beam_sound=Mix_LoadWAV("assets/Audio/beam_sound.wav");
  hit_sound=Mix_LoadWAV("assets/Audio/hit.wav");
  explosion_sound1=Mix_LoadWAV("assets/Audio/explosion1.aiff");
  explosion_sound2=Mix_LoadWAV("assets/Audio/explosion2.wav");
  special_sound=Mix_LoadWAV("assets/Audio/special.wav");
  item_sound=Mix_LoadWAV("assets/Audio/get_item.wav");
  stage_clear_sound=Mix_LoadWAV("assets/Audio/clear.wav");
  Mix_VolumeChunk(beam_sound,40);
  Mix_VolumeChunk(explosion_sound1,60);

  for(int i = 0 ; i < 4; i++)
  {
    string str = "assets/E_";
    string str1 = to_string(i + 1);
    string str2 = ".gif";
    string str3 = str + str1 + str2;
    enemy[i] = load_image(str3);
    SDL_SetColorKey(enemy[i], SDL_SRCCOLORKEY,SDL_MapRGB(enemy[i]->format,255,255,255));
  }
  for(int i = 0 ; i < 11; i++)//폭발 이미지
  {
    string str = "assets/explosion/";
    string str1 = to_string(i);
    string str2 = ".gif";
    string str3 = str + str1 + str2;
    boom[i] =load_image(str3);
    SDL_SetColorKey(boom[i], SDL_SRCCOLORKEY,SDL_MapRGB(boom[i]->format,255,255,255));
  }
  
  SDL_SetColorKey(shield, SDL_SRCCOLORKEY,SDL_MapRGB(shield->format,0,0,0));
  SDL_SetColorKey(explosion, SDL_SRCCOLORKEY,SDL_MapRGB(explosion->format,0,0,0));
  SDL_SetColorKey(life, SDL_SRCCOLORKEY,SDL_MapRGB(life->format,255,255,255));
  SDL_SetColorKey(bullet_boss, SDL_SRCCOLORKEY, SDL_MapRGB(bullet_boss->format,0,0,0));
  SDL_SetColorKey(bullet_boss, SDL_SRCCOLORKEY, SDL_MapRGB(bullet_mini->format,0,0,0));
  SDL_SetColorKey(frame, SDL_SRCCOLORKEY,SDL_MapRGB(frame->format,0,0,0));
  SDL_SetColorKey(frame2, SDL_SRCCOLORKEY,SDL_MapRGB(frame2->format,0,0,0));
  SDL_SetColorKey(bullet, SDL_SRCCOLORKEY,SDL_MapRGB(bullet->format,0,0,0));
  SDL_SetColorKey(bullet_basic, SDL_SRCCOLORKEY, SDL_MapRGB(bullet_basic->format,255,255,255));
  SDL_SetColorKey(arrow, SDL_SRCCOLORKEY,SDL_MapRGB(arrow->format,0,0,0));
  SDL_SetColorKey(sapoint, SDL_SRCCOLORKEY,SDL_MapRGB(sapoint->format,255,255,255));
  
  
  return true;
}

bool SDL_free_all()
{//올바르게 free하면 true 반환하게 수정

  Mix_FreeMusic(menu_music);
  Mix_FreeMusic(stage_music);
  Mix_FreeChunk(game_over_sound);
  Mix_FreeChunk(selection_sound);
  Mix_FreeChunk(hit_sound);
  Mix_FreeChunk(explosion_sound1);
  Mix_FreeChunk(bullet_sound);
  Mix_FreeChunk(beam_sound);
  Mix_FreeChunk(explosion_sound2);
  Mix_FreeChunk(special_sound);
  Mix_FreeChunk(item_sound);
  Mix_FreeChunk(stage_clear_sound);
  Mix_CloseAudio();

  SDL_FreeSurface(shield);
  SDL_FreeSurface(plane);
  SDL_FreeSurface(bullet);
  SDL_FreeSurface(background);
  SDL_FreeSurface(background2);
  SDL_FreeSurface(background3);
  SDL_FreeSurface(explosion);
  SDL_FreeSurface(life);
  SDL_FreeSurface(sapoint);
  SDL_FreeSurface(bullet_basic);
  SDL_FreeSurface(bullet_mini);
  SDL_FreeSurface(bullet_second);
  SDL_FreeSurface(bullet_boss);
  SDL_FreeSurface(message);
  SDL_FreeSurface(message2);
  SDL_FreeSurface(message3);
  SDL_FreeSurface(message4);
  SDL_FreeSurface(message5);
  SDL_FreeSurface(title_message);
  SDL_FreeSurface(arrow);
  SDL_FreeSurface(pick1);
  SDL_FreeSurface(pick2);
  SDL_FreeSurface(frame);
  SDL_FreeSurface(frame2);
  SDL_FreeSurface(enemy2);
  SDL_FreeSurface(buffer);
  SDL_FreeSurface(plane);
  SDL_FreeSurface(plane2);
  SDL_FreeSurface(plane3);
  SDL_FreeSurface(plane4);
  SDL_FreeSurface(plane5);
  for(int i =0 ; i < 4; i++)
    SDL_FreeSurface(enemy[i]);
  for(int i = 0; i < 11; i++)
    SDL_FreeSurface(boom[i]);
  SDL_FreeSurface(screen);

  SDL_Quit();//init한 SDL 변수들 닫아주는겅 일걸,위의 freesurface랑 차이 모름

  return true;
}

bool menu()   // 처음 시작 메뉴
{
  textColor = {204, 255, 204};  // 안내 폰트 색깔
  textColor2 = {255, 255, 255}; // 제목 폰트 색깔
  bool quit = false;
  Mix_PlayMusic(menu_music,-1);
  Mix_VolumeMusic(128);
	while (quit == false)
	{
	
    message = TTF_RenderText_Solid(font, "Press space to start, esc key to quit", textColor); // space키는 시작 esc키는 종료
    message2 = TTF_RenderText_Solid(font2, "SPACE 2143", textColor2);  // 제목
    background = load_image("assets/menu3.png");  // 배경
		apply_surface(0, 0, background, buffer, NULL);
    apply_surface((640 - message->w) / 2, 280, message, buffer, NULL);
    apply_surface((640 - message2->w) / 2, 100, message2, buffer, NULL);
    
    show_screen();
			//SDL_Flip(buffer);
    if (SDL_PollEvent(&event))
		{
      if(event.type==SDL_VIDEORESIZE)
        handle_resize(event.resize);

			if (event.type == SDL_KEYDOWN)
			{
        Mix_PlayChannel(-1,selection_sound,0);
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:  // space 키가 눌리면 게임 배경 가져오고 게임 시작
        {
          quit = true;
          background = load_image("assets/background.png");
          break;
        }
        case SDLK_ESCAPE:  // esc 키 누르면 종료
        {
          EXIT = 1;
          quit = true;
          return false;
        }
				}
			}
			else if (event.type == SDL_QUIT)
			{
				quit = true;
        return false;
			}
		}
	}
  return true;
}

bool menu2()   // 싱글 플레이인지 멀티 플레이인지 고르는 메뉴
{
  int selectx = 25;
  int selectx2 = 525;
	bool quit = false;
  bool quit2 = false;
  bool choosen = false;
  bool choosen2 = false;
	while (quit == false || quit2 == false)
	{
		if (SDL_PollEvent(&event))
		{
      font = TTF_OpenFont("assets/Terminus.ttf", 36);//작은 안내문 폰트
      message = TTF_RenderText_Solid(font, "Choose your Aircraft", textColor); // space키는 시작 esc키는 종료
      message2 = TTF_RenderText_Solid(font3, "1P", textColor);
      message3 = TTF_RenderText_Solid(font3, "2P", textColor);
      message4 = TTF_RenderText_Solid(font3, "1p              2p", textColor);
      background = load_image("assets/background.png");  // 배경
			apply_surface(0, 0, background, buffer, NULL);
      apply_surface((640 - message->w) / 2, 90, message, buffer, NULL);
      apply_surface(selectx, 220, frame, buffer, NULL);
      apply_surface(selectx+30, 180, message2, buffer, NULL);
      if(mode == 2)
      {
        apply_surface(selectx2, 220, frame2, buffer, NULL);
        apply_surface(selectx2+30, 315, message3, buffer, NULL);
        apply_surface(200, 430, message4, buffer, NULL);
        apply_surface(202, 400, pick1, buffer, NULL);
        SDL_SetColorKey(pick1, SDL_SRCCOLORKEY,SDL_MapRGB(pick1->format,0,0,0));
        apply_surface(387, 400, pick2, buffer, NULL);
        SDL_SetColorKey(pick2, SDL_SRCCOLORKEY,SDL_MapRGB(pick2->format,0,0,0));
      }
      SDL_SetColorKey(plane, SDL_SRCCOLORKEY,SDL_MapRGB(plane->format,255,255,255));
      apply_surface(57, 255, plane, buffer, NULL);
      SDL_SetColorKey(plane2, SDL_SRCCOLORKEY,SDL_MapRGB(plane->format,0,0,0));
      apply_surface(182, 255, plane2, buffer, NULL);
      SDL_SetColorKey(plane3, SDL_SRCCOLORKEY,SDL_MapRGB(plane->format,0,0,0));
      apply_surface(307, 255, plane3, buffer, NULL);
      SDL_SetColorKey(plane4, SDL_SRCCOLORKEY,SDL_MapRGB(plane->format,0,0,0));
      apply_surface(432, 255, plane4, buffer, NULL);
      SDL_SetColorKey(plane5, SDL_SRCCOLORKEY,SDL_MapRGB(plane->format,0,0,0));
      apply_surface(557, 255, plane5, buffer, NULL);
      show_screen();
			//SDL_Flip(buffer);
      if(event.type==SDL_VIDEORESIZE)
        handle_resize(event.resize);
			if (event.type == SDL_KEYDOWN)
			{
        Mix_PlayChannel(-1,selection_sound,0);
				switch (event.key.keysym.sym)
				{

        case SDLK_SPACE:
        {
          if(choosen && choosen2)
          {
            quit = true;
            quit2 = true;
          }
          else
          {}
          break;
        }

				case SDLK_o:  // space 키가 눌리면 게임 배경 가져오고 게임 시작
        {
          choosen = true;
          quit = true;
          if(selectx == 25){
            plane_1p = load_image("assets/p2.png");
            pick1 = load_image("assets/p2.png");
            SDL_SetColorKey(plane, SDL_SRCCOLORKEY,SDL_MapRGB(plane->format,255,255,255));
            SA = 0;
          }

          else if(selectx == 150){
            plane_1p = load_image("assets/aircraft1.png");
            pick1 = load_image("assets/aircraft1.png");
            SDL_SetColorKey(plane, SDL_SRCCOLORKEY,SDL_MapRGB(plane->format,0,0,0));
            SA = 1;
          }
          else if(selectx == 275){
            plane_1p = load_image("assets/aircraft3.png");
            pick1 = load_image("assets/aircraft3.png");
            SDL_SetColorKey(plane, SDL_SRCCOLORKEY,SDL_MapRGB(plane->format,0,0,0));
            SA = 2;
          }
          else if(selectx == 400){
            plane_1p = load_image("assets/aircraft5.png");
            pick1 = load_image("assets/aircraft5.png");
            SDL_SetColorKey(plane, SDL_SRCCOLORKEY,SDL_MapRGB(plane->format,0,0,0));
            SA = 3;
          }
          else if(selectx == 525){
            plane_1p = load_image("assets/aircraft6.png");
            pick1 = load_image("assets/aircraft6.png");
            SDL_SetColorKey(plane, SDL_SRCCOLORKEY,SDL_MapRGB(plane->format,0,0,0));
            SA = 4;
          }
          if(mode == 1)
            quit2 = true;
          break;
        }

        case SDLK_f:  // space 키가 눌리면 게임 배경 가져오고 게임 시작
        {
          choosen2 = true;
          if(selectx2 == 25){
            plane_2p = load_image("assets/p2.png");
            pick2 = load_image("assets/p2.png");
            SDL_SetColorKey(plane_2p, SDL_SRCCOLORKEY,SDL_MapRGB(plane_2p->format,255,255,255));
            SA2 = 0;
          }

          else if(selectx2 == 150){
            plane_2p = load_image("assets/aircraft1.png");
            pick2 = load_image("assets/aircraft1.png");
            SDL_SetColorKey(plane_2p, SDL_SRCCOLORKEY,SDL_MapRGB(plane_2p->format,0,0,0));
            SA2 = 1;
          }
          else if(selectx2 == 275){
            plane_2p = load_image("assets/aircraft3.png");
            pick2 = load_image("assets/aircraft3.png");
            SDL_SetColorKey(plane_2p, SDL_SRCCOLORKEY,SDL_MapRGB(plane_2p->format,0,0,0));
            SA2 = 2;
          }
          else if(selectx2 == 400){
            plane_2p = load_image("assets/aircraft5.png");
            pick2 = load_image("assets/aircraft5.png");
            SDL_SetColorKey(plane_2p, SDL_SRCCOLORKEY,SDL_MapRGB(plane_2p->format,0,0,0));
            SA2 = 3;
          }
          else if(selectx2 == 525){
            plane_2p = load_image("assets/aircraft6.png");
            pick2 = load_image("assets/aircraft6.png");
            SDL_SetColorKey(plane_2p, SDL_SRCCOLORKEY,SDL_MapRGB(plane_2p->format,0,0,0));
            SA2 = 4;
          }


          break;
        }


        case SDLK_LEFT:  
        {
          if (selectx == 25)
          {}
          else
          {
            selectx -= 125;
          }
          break;
        }
        case SDLK_RIGHT:  
        {
          if(selectx == 525)
          {}
          else
          {
            selectx += 125;
          }
          break;
        }
        case SDLK_a: 
        {
          if (selectx2 == 25)
          {}
          else
          {
            selectx2 -= 125;
          }
          break;
        }
        case SDLK_d:  
        {
          if(selectx2 == 525)
          {}
          else
          {
            selectx2 += 125;
          }
          break;
        }


        case SDLK_ESCAPE:  // esc 키 누르면 종료
        {
          EXIT = 1;
          quit = true;
          return false;
        }
				}
			}
			else if (event.type == SDL_QUIT)
			{
				quit = true;
        return false;
			}
		}
	}
  return true;
}

bool menu3()   // 비행기 고르는 메뉴
{
  int selecty=210;
	bool quit = false;
  
	while (quit == false)
	{
		
      font = TTF_OpenFont("assets/Terminus.ttf", 36);//작은 안내문 폰트
      message = TTF_RenderText_Solid(font, "Choose the game mode", textColor); // space키는 시작 esc키는 종료
      message2 = TTF_RenderText_Solid(font3, "Single play", textColor);
      message3 = TTF_RenderText_Solid(font3, "Multi play", textColor);
      background = load_image("assets/intro.jpg");  // 배경
			apply_surface(0, 0, background, buffer, NULL);
      apply_surface((640 - message->w) / 2, 100, message, buffer, NULL);
      apply_surface((640 - message2->w) / 2, 210, message2, buffer, NULL);
      apply_surface((640 - message3->w) / 2, 300, message3, buffer, NULL);
      apply_surface(170, selecty-20, arrow, buffer, NULL);

      show_screen();
			//SDL_Flip(buffer);
    if (SDL_PollEvent(&event))
		{
      if(event.type==SDL_VIDEORESIZE)
        handle_resize(event.resize);
			if (event.type == SDL_KEYDOWN)
			{
        Mix_PlayChannel(-1,selection_sound,0);
				switch (event.key.keysym.sym)
				{

				case SDLK_o:  // space 키가 눌리면 게임 배경 가져오고 게임 시작
        {
          quit = true;
          
          if(selecty == 210)
            mode = 1;
          else if(selecty == 300)
            mode = 2;

          break;
        }
        case SDLK_UP:  // space 키가 눌리면 게임 배경 가져오고 게임 시작
        {
          if (selecty == 210)
          {}
          else
          {
            selecty -= 90;
          }
          break;
        }
        case SDLK_DOWN:  // space 키가 눌리면 게임 배경 가져오고 게임 시작
        {
          if(selecty == 300)
          {}
          else
          {
            selecty += 90;
          }
          break;
        }

        case SDLK_ESCAPE:  // esc 키 누르면 종료
        {
          EXIT = 1;
          quit = true;
          return false;
        }
				}
			}
			else if (event.type == SDL_QUIT)
			{
				quit = true;
        return false;
			}
		}
	}
  return true;
}

bool game_over()  // 사용자 죽었을 시 나타나는 게임오버 창
{
  if(Mix_PlayingMusic())
    Mix_HaltMusic();//음악 정지
  Mix_HaltChannel(-1);//모든 사운드 채널 정지
  font = TTF_OpenFont("assets/Terminus.ttf", 24);//작은 안내문 폰트
  background = load_image("assets/gameover.png");
  message2 = TTF_RenderText_Solid(font2, "Game over", textColor2);
  apply_surface(0, 0, background, buffer, NULL);
  message = TTF_RenderText_Solid(font, "Press space to restart, esc key to quit", textColor);
  apply_surface((640 - message->w) / 2, 280, message, buffer, NULL);
  apply_surface((640 - message2->w) / 2, 100, message2, buffer, NULL);
  show_screen();
	//SDL_Flip(buffer);
  Mix_PlayChannel(-1,game_over_sound,0);
	while (true)
	{
		if (SDL_PollEvent(&event))
		{
      if(event.type==SDL_VIDEORESIZE)
        handle_resize(event.resize);
			if (event.type == SDL_KEYDOWN)
			{
        Mix_PlayChannel(-1,selection_sound,0);
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE://esc 키가 눌리면 종료
        {

          return false;
        }
        case SDLK_SPACE:
        {
          background = load_image("assets/background.png");
          return true;

        }
				default:{}
			  }
		
	  	}
      else if (event.type == SDL_QUIT)
			{

        return false;
			}
  	}
  }
}

bool stage_clear()  // 나중에 bosscounter == 0 되면 stage clear 되도록 창 띄우기
{
	bool quit = false;
  Mix_HaltChannel(-1);
  Mix_HaltMusic();
  Mix_PlayChannel(-1,stage_clear_sound,0);
  message = TTF_RenderText_Solid(font, "Stage Clear!", textColor);
  background = load_image("assets/menu3.png");
  apply_surface(0, 0, background, buffer, NULL);
  apply_surface((640 - message->w) / 2, 480/2 - message->h, message, buffer, NULL);
	show_screen();
  //SDL_Flip(buffer);
	while (quit == false)
	{
		if (SDL_PollEvent(&event))
		{
      if(event.type==SDL_VIDEORESIZE)
        handle_resize(event.resize);
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE://esc 키가 눌리면 종료
        {
          quit = true;
          return false;
        }
				default:
					break;
				}
			}
			else if (event.type == SDL_QUIT)
			{
				quit = true;
        return false;
			}
		}
	}
  return true;
}

void sprite_surface( SDL_Surface *buffer, SDL_Rect tmp, SDL_Surface* surface, int w, int h, int step , int mode1)
{
	SDL_Rect rectDst, rectSrc;
  if(mode1 == 0)
  {
    rectDst = tmp;
    rectDst.x -= 10;
  }

  else if(mode1 == 1)
  {
    rectDst = tmp;
    rectDst.x += 60;
    rectDst.y += 18;
  }

  else if(mode1 == 2)
  {
    rectDst = tmp;
    rectDst.x += 110;
    rectDst.y += 32;
  }
  else if(mode1 == 3)
  {
    rectDst = tmp;
    rectDst.x += 160;
    rectDst.y += 18;
  }
  else if(mode1 == 4)
  {
    rectDst = tmp;
    rectDst.x += 210;
  }

  rectSrc.x = (step % w) * surface->w/w;      //분할된 이미지 선택
  rectSrc.y = (step / w) * surface->h/h;
  rectSrc.w = surface->w/w;                   //분할된 이미지 선택
  rectSrc.h = surface->h/h;
  SDL_BlitSurface(surface, &rectSrc, buffer, &rectDst);
}
