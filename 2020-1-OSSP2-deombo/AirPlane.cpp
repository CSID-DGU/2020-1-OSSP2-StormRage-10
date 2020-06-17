#include "AirPlane.h"

void Health_item::add_itm(int x, int y, int ply_x, int ply_y)
{
  item = load_image("assets/tem_li.png");
  SDL_SetColorKey(item, SDL_SRCCOLORKEY,SDL_MapRGB(item->format,0,0,0));
  items tmp(x,y,ply_x,ply_y);
  itm.push_back(tmp);
}

void Health_item::item_apply_surface(SDL_Surface *item, SDL_Surface* destination, SDL_Rect* clip)  // item들 그리기
{
  for(vector<items>::iterator iter = itm.begin(); iter!= itm.end(); iter++)
  {
    SDL_BlitSurface( item, clip, destination, &(*iter).offset);
  }
}

void Health_item::control_item()
{
  vector<items> temp;
  for(vector<items>::iterator iter = itm.begin(); iter != itm.end(); iter++)
  {
    items tmp((*iter).move_x,(*iter).move_y,(*iter).offset.x,(*iter).offset.y + 2);
    if( 0 < tmp.offset.x + 9 && tmp.offset.x< SCREEN_WIDTH && -5 <= tmp.offset.y  && tmp.offset.y < SCREEN_HEIGHT)
      temp.push_back(tmp);
  }
  itm = temp;
}

Health_item::~Health_item()
{
    SDL_FreeSurface(item);
}

void Special_item::add_itm(int x, int y, int ply_x, int ply_y)
{
  item = load_image("assets/tem_sa.png");
  SDL_SetColorKey(item, SDL_SRCCOLORKEY,SDL_MapRGB(item->format,0,0,0));
  items tmp(x, y,ply_x,ply_y);
  itm.push_back(tmp);
}

void Special_item::item_apply_surface(SDL_Surface *item, SDL_Surface* destination, SDL_Rect* clip)  // item들 그리기
{
  for(vector<items>::iterator iter = itm.begin(); iter!= itm.end(); iter++)
  {
    SDL_BlitSurface( item, clip, destination, &(*iter).offset);
  }
}

void Special_item::control_item()
{
  vector<items> temp;
  for(vector<items>::iterator iter = itm.begin(); iter != itm.end(); iter++)
  {
    items tmp((*iter).move_x,(*iter).move_y,(*iter).offset.x,(*iter).offset.y + 2);
    if( 0 < tmp.offset.x + 9 && tmp.offset.x< SCREEN_WIDTH && -5 <= tmp.offset.y  && tmp.offset.y < SCREEN_HEIGHT)
      temp.push_back(tmp);
  }

  itm = temp;
}

Special_item::~Special_item()
{
    SDL_FreeSurface(item);
}

void Upgrade_item1::add_itm(int x, int y, int ply_x, int ply_y)
{
  item = load_image("assets/tem_sh.png");
  SDL_SetColorKey(item, SDL_SRCCOLORKEY,SDL_MapRGB(item->format,0,0,0));
  items tmp(x,y,ply_x,ply_y);
  itm.push_back(tmp);
}

void Upgrade_item1::item_apply_surface(SDL_Surface *item, SDL_Surface* destination, SDL_Rect* clip)  // item들 그리기
{
  for(vector<items>::iterator iter = itm.begin(); iter!= itm.end(); iter++)
  {
    SDL_BlitSurface( item, clip, destination, &(*iter).offset);
  }
}

void Upgrade_item1::control_item()
{
  vector<items> temp;
  for(vector<items>::iterator iter = itm.begin(); iter != itm.end(); iter++)
  {
    items tmp((*iter).move_x,(*iter).move_y,(*iter).offset.x,(*iter).offset.y + 2);
    if( 0 < tmp.offset.x + 9 && tmp.offset.x< SCREEN_WIDTH && -5 <= tmp.offset.y  && tmp.offset.y < SCREEN_HEIGHT)
      temp.push_back(tmp);
  }

  itm = temp;
}

Upgrade_item1::~Upgrade_item1()
{
    SDL_FreeSurface(item);
}

void Upgrade_item2::add_itm(int x, int y, int ply_x, int ply_y)
{
  item = load_image("assets/tem_sh.png");
  SDL_SetColorKey(item, SDL_SRCCOLORKEY,SDL_MapRGB(item->format,0,0,0));
  items tmp(x,y,ply_x,ply_y);
  itm.push_back(tmp);
}

void Upgrade_item2::item_apply_surface(SDL_Surface *item, SDL_Surface* destination, SDL_Rect* clip)  // item들 그리기
{
  for(vector<items>::iterator iter = itm.begin(); iter!= itm.end(); iter++)
  {
    SDL_BlitSurface( item, clip, destination, &(*iter).offset);
  }
}

void Upgrade_item2::control_item()
{
  vector<items> temp;
  for(vector<items>::iterator iter = itm.begin(); iter != itm.end(); iter++)
  {
    items tmp((*iter).move_x,(*iter).move_y,(*iter).offset.x,(*iter).offset.y + 2);
    if( 0 < tmp.offset.x + 9 && tmp.offset.x< SCREEN_WIDTH && -5 <= tmp.offset.y  && tmp.offset.y < SCREEN_HEIGHT)
      temp.push_back(tmp);
  }

  itm = temp;
}

Upgrade_item2::~Upgrade_item2()
{
    SDL_FreeSurface(item);
}
AirPlane::AirPlane(Mix_Chunk* shooting,Mix_Chunk* got,Mix_Chunk* hit)
{
  pos_x = SCREEN_WIDTH / 2;//처음 시작 위치 지정
  pos_y = SCREEN_HEIGHT / 2;//처음 시작 위치 지정
  
  offset.x = pos_x;
  offset.y = pos_y;
  offset.w=PLAYER_WIDTH;
  offset.h=PLAYER_HEIGHT;
  //플레이어 사운드 설정
  shooting_sound=shooting;
  get_sound=got;
  hit_sound=hit;

  bullet_mode =3;//총알 모드는 기본적으로 1임
  life = 3;
  SA_count = 3;
  invisible_mode = 0;
}

AirPlane::~AirPlane()
{

}

void AirPlane::shooting(_bullets &player_bullets,laser_bullet &player_laser_bullet)
{
  if(bullet_mode==1){//기본 총알
    Mix_PlayChannel(-1,shooting_sound,0);//총알 발사음 출력
    player_bullets.add_blt( 0, -10,pos_x+5,pos_y - 15);
  }
  else if(bullet_mode==2){//3 방향 발사 모드
    Mix_PlayChannel(-1,shooting_sound,0);//총알 발사음 출력
    player_bullets.add_blt( 7, -7,pos_x+11,pos_y - 20, -45);
    player_bullets.add_blt( 0, -10,pos_x+5,pos_y - 15);
    player_bullets.add_blt(-7, -7,pos_x-13,pos_y - 20, 45);
  }
  else if(bullet_mode==3)
  {
    player_laser_bullet.env=true;
    player_laser_bullet.offset.w=3;
    player_laser_bullet.offset.h=SCREEN_HEIGHT;
    player_laser_bullet.offset.x= pos_x+9;
    player_laser_bullet.offset.y= pos_y-player_laser_bullet.offset.h;
  }
}

void AirPlane::plane_apply_surface(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
  offset.x = pos_x;
  offset.y = pos_y;
  SDL_BlitSurface( source, clip, destination, &offset );
}
void AirPlane::control_plane(int x, int y)
{
  /*
  if( pos_x + x >= 0 && (pos_x + PLAYER_WIDTH + x) <= SCREEN_WIDTH &&
      pos_y + y >= 0 && (pos_y + PLAYER_HEIGHT + y) < SCREEN_HEIGHT-6)//화면 범위 안
  {
    pos_x += x;
    pos_y += y;
  }
  */
    pos_x += x;
    pos_y += y;
}
void AirPlane::invisible(SDL_Surface *plane)
{
  static int count = 0;                               //투명화 지속 시간
  static int i =0;                                    //투명도

  if(count++ != 60)
  {
    if( count <= 10)
    {
      i += 25;
      SDL_SetAlpha(plane,SDL_SRCALPHA,i);
    }
    else if( 10 < count && count <= 20)
    {
      i -= 25;
      SDL_SetAlpha(plane,SDL_SRCALPHA,i);
    }
    else if( 20 < count && count <= 30)
    {
      i += 25;
      SDL_SetAlpha(plane,SDL_SRCALPHA,i);
    }
    else if( 30 < count && count <= 40)
    {
      i -= 25;
      SDL_SetAlpha(plane,SDL_SRCALPHA,i);
    }
    else if( 40 < count && count <= 50)
    {
      i += 25;
      SDL_SetAlpha(plane,SDL_SRCALPHA,i);
    }
    else
    {
      i -= 25;
      SDL_SetAlpha(plane,SDL_SRCALPHA,i);
    }
  }

  else                                               //불투명화,count 초기화
  {
    count = 0;
    this->invisible_mode = 0;
    SDL_SetAlpha(plane,SDL_SRCALPHA,255);
  }
}

SDL_Rect AirPlane::Get_plane()
{
  return offset;
}

bool AirPlane::Got_shot(_bullets &A,_bullets &B,_bullets &C,_bullets &D)
{
  vector<bullets>::iterator iter;
  vector<bullets> tmp;
  vector<bullets> tmp2;
  vector<bullets> tmp3;

  bool flag = false;

  for(iter = A.blt.begin(); iter != A.blt.end(); iter++)
  {
    if((pos_x + 18 < (*iter).offset.x || pos_y + 20 < (*iter).offset.y + 5) ||
    ((*iter).offset.x + 15 < pos_x + 9 || (*iter).offset.y + 10 < pos_y + 10))//안 맞았을 때
      tmp.push_back(*iter);
    else//맞았을때
    {
      Mix_PlayChannel(-1,hit_sound,0);//피격음 출력
      flag = true;
    }
  }
  A.blt = tmp;
  for(iter = B.blt.begin(); iter != B.blt.end(); iter++)
  {
    if((pos_x + 18 < (*iter).offset.x || pos_y + 20 < (*iter).offset.y + 5) ||
    ((*iter).offset.x + 15 < pos_x + 9 || (*iter).offset.y + 10 < pos_y + 10))//안 맞았을 때
      tmp2.push_back(*iter);
    else//맞았을때
    {
      Mix_PlayChannel(-1,hit_sound,0);//피격음 출력
      flag = true;
    }
  }
  B.blt = tmp2;


  for(iter = C.blt.begin(); iter != C.blt.end(); iter++)
  {
    if((pos_x + 18 < (*iter).offset.x || pos_y + 20 < (*iter).offset.y + 5) ||
    ((*iter).offset.x + 15 < pos_x + 9 || (*iter).offset.y + 10 < pos_y + 10))//안 맞았을 때
      tmp3.push_back(*iter);
    else//맞았을때
    {
      Mix_PlayChannel(-1,hit_sound,0);//피격음 출력
      flag = true;
    }
  }
  C.blt = tmp3;
  /*
  if(flag == true) {
    Mix_PlayChannel(-1,hit_sound,0);//피격음 출력
    return flag;
  }*/
  return flag;
}

bool AirPlane::Got_item(vector<items>& I)
{
  vector<items>::iterator iter;
  bool flag = false;

  for(iter = I.begin(); iter != I.end(); iter++)
  {
    if((pos_x + 18 < (*iter).offset.x + 9 || pos_y + 20 < (*iter).offset.y + 5) ||
    ((*iter).offset.x + 18 < pos_x + 9 || (*iter).offset.y + 10 < pos_y + 10));//안 맞았을 때
    else                                                                           //맞았을때
    {
      flag = true;
      I.clear();
      break;
    }
  }
  if(flag)
    Mix_PlayChannel(-1,get_sound,0);//아이템 획득 음 출력
  return flag;
}

bool AirPlane::detect_collision(list<SDL_Rect> C)
{
  list<SDL_Rect>::iterator iter;
  bool flag = false;

  for(iter = C.begin(); iter != C.end();iter++)
  {
    if(intersects((*iter),this->offset))//충돌시
    {
      flag = true;
      Mix_PlayChannel(-1,hit_sound,0);//피격음 출력
      break;
    }
  
  }
  return flag;
}

bool AirPlane::detect_collision(SDL_Rect C)
{
  if(intersects(C,this->offset))//충돌시
  {
      Mix_PlayChannel(-1,hit_sound,0);//피격음 출력
      return true;
  }
  return false;
}
bool AirPlane::check_in_border(SDL_Rect C,bool& flag)
{
  if(!(flag=intersects(C,this->offset)))//경계 밖에 있는 경우
  {
      Mix_PlayChannel(-1,hit_sound,0);//피격음 출력
      return true;
  }
  return false;
}
void AirPlane::increaseLife()
{
  life++;
}

void AirPlane::increaseSA()
{
  SA_count++;
}

void AirPlane::Got_shiled(SDL_Surface *plane)
{
  static int count = 0;                               //투명화 지속 시간
  static int i =0;                                    //투명도

  if(count++ != 120)
  {
      i -= 50;
      SDL_SetAlpha(plane,SDL_SRCALPHA,i);
  }
  else                                               //불투명화,count 초기화
  {
    count = 0;
    this->invisible_mode = 0;
    SDL_SetAlpha(plane,SDL_SRCALPHA,255);
  }
}

Enemy_standard_2::Enemy_standard_2(int mode)
{
  enemy = load_image("assets/3.gif");// 비행기 이미지
  //Setcolorkey는 네모난 그림에서 비행기로 쓸 그림 빼고 나머지 흰 바탕들만 투명하게 바꾸는거
  SDL_SetColorKey(enemy, SDL_SRCCOLORKEY,SDL_MapRGB(enemy->format,255,255,255));

  this->mode = mode;
  int y = rand()%100 + 10;
  if(mode == 0) pos_x = -ENEMY_WIDTH;//처음 시작 위치 지정
  else pos_x = SCREEN_WIDTH+ENEMY_WIDTH;
  pos_y = y;//처음 시작 위치 지정
  life = 1;
  count = 0;
  offset.w =ENEMY_WIDTH;
  offset.h=ENEMY_HEIGHT;
}

Enemy_standard_2::~Enemy_standard_2()
{
}

bool Enemy_standard_2::Got_shot(_bullets &A)
{
  vector<bullets>::iterator iter;
  vector<bullets> tmp;

  bool flag = false;

  for(iter = A.blt.begin(); iter != A.blt.end(); iter++)
  {
    if((pos_x + 25 < (*iter).offset.x + 3 || pos_y + 32 < (*iter).offset.y) ||
    ((*iter).offset.x + 13 < pos_x + 10 || (*iter).offset.y + 32 < pos_y))//안 맞았을 때
      tmp.push_back(*iter);
    else//맞았을때
    {
      flag = true;
    }
  }

  A.blt = tmp;

  return flag;
}

bool Enemy_standard_2::Got_shot(laser_bullet A){//레이저 빔에 맞을 때 판정
  if(A.env)
    return intersects(this->offset,A.offset);
  else
    return false;
};


bool Enemy_standard_2:: eliminate(int y){
    if(pos_y+32 > y) return true;
    else return false;
}

void Enemy_standard_2::shooting(_bullets &A)
{
  A.add_blt( 0, 5,pos_x + 2,pos_y + 15);
}

void Enemy_standard_2::enemy_apply_surface(SDL_Surface* destination, SDL_Rect* clip )
{
  
  offset.x = pos_x;
  offset.y = pos_y;
  SDL_BlitSurface(enemy, clip, destination, &offset );
}

SDL_Rect Enemy_standard_2::control_plane(_bullets &A)
{
  if(first_exe==true){
    pos = rand()%75+75;
    first_exe=false;
  }
  if(count % 30 == 0 ) this->shooting(A);
  if(count < pos){
      if(mode == 0) pos_x += 3;
      else pos_x -= 3;
  }
  else {
      if(mode == 0){
          pos_x -= 3;
          pos_y += 3;
      }
      else {
          pos_x += 3;
          pos_y += 3;
      }
  }
  offset.x = pos_x;
  offset.y = pos_y;
  count++;
  return this->offset;
}

SDL_Rect Enemy_standard_2::Get_plane()
{
  offset.x = pos_x;
  offset.y = pos_y;

  return offset;//오프셋(충돌 박스)를 리턴함
}

Enemy_standard::Enemy_standard(int mode)
{
  int x = rand()%200+80;
  int y = rand()%200+400;
  this->mode = mode;// 이동경로 mode
  if( mode == 0)
    pos_x = x;// 처음 시작 위치 지정
  else if(mode == 1)
    pos_x = y;
  pos_y = -ENEMY_HEIGHT;//처음 시작 위치 지정
  life = 1;

  offset.w =ENEMY_WIDTH;
  offset.h=ENEMY_HEIGHT;
}

Enemy_standard::~Enemy_standard()
{
}

bool Enemy_standard::Got_shot(_bullets &A)
{
  vector<bullets>::iterator iter;
  vector<bullets> tmp;

  bool flag = false;

  for(iter = A.blt.begin(); iter != A.blt.end(); iter++)
  {
      if((pos_x + 22 < (*iter).offset.x + 3 || pos_y + 32 < (*iter).offset.y) ||
      ((*iter).offset.x + 13 < pos_x + 10 || (*iter).offset.y + 32 < pos_y))//안 맞았을 때
      tmp.push_back(*iter);
    else//맞았을때
    {
      flag = true;
    }
     
  }
  A.blt = tmp;

  return flag;
}
bool Enemy_standard::Got_shot(laser_bullet A){//레이저 빔에 맞을 때 판정
  if(A.env)
    return intersects(this->offset,A.offset);
  else
    return false;
};
bool Enemy_standard:: eliminate(int y){
    if(pos_y+32 > y) return true;
    else return false;
}

void Enemy_standard::shooting(_bullets &A)
{
  A.add_blt( 0, 5,pos_x + 2,pos_y + 15);
}

void Enemy_standard::enemy_apply_surface(SDL_Surface* source[], SDL_Surface* destination, SDL_Rect* clip )
{
  int i = count % 4;
  offset.x = pos_x;
  offset.y = pos_y;
  SDL_BlitSurface(source[i], clip, destination, &offset );
}

SDL_Rect Enemy_standard::control_plane(_bullets &enemy)
{//y= 3일 때 속도가 적당.
  if(first_exe == true){
    pos = rand()%75+75;
    first_exe=false;
  }
  if(count % 30 == 0) this->shooting(enemy);
  if(count<pos) pos_y += 3;
  else{
      if(this->mode == 0){
          pos_x+=3;
          pos_y+=3;
      }
      else{
          pos_x-=3;
          pos_y+=3;
      }
  }
  ++count;
  offset.x = pos_x;
  offset.y = pos_y;
  return this->offset;
}

SDL_Rect Enemy_standard::Get_plane()
{
  offset.x = pos_x;
  offset.y = pos_y;
  return offset;
}
/*
//About Obstacle
Obstacle::Obstacle()
{
  int x = rand()%200+80;
  int y = rand()%200+400;

  /*this->mode = mode;// 이동경로 mode
  if( mode == 0)
    pos_x = x;// 처음 시작 위치 지정
  else if(mode == 1)
    pos_x = y;

  pos_y = -OBSTACLE_HEIGHT;//처음 시작 위치 지정
  //life = 1;

  offset.w =OBSTACLE_WIDTH;
  offset.h=OBSTACLE_HEIGHT;
}

Obstacle::~Obstacle()
{
}

bool Obstacle::Got_shot(_bullets &A)
{
  vector<bullets>::iterator iter;
  vector<bullets> tmp;

  bool flag = false;

  for(iter = A.blt.begin(); iter != A.blt.end(); iter++)
  {
    /*
      if((pos_x + 22 < (*iter).bullet_pos.x + 3 || pos_y + 32 < (*iter).bullet_pos.y) ||
      ((*iter).bullet_pos.x + 13 < pos_x + 10 || (*iter).bullet_pos.y + 32 < pos_y))//안 맞았을 때
      tmp.push_back(*iter);
     
    else//맞았을때
    {
      flag = true;
    }
    
  }

  A.blt = tmp;

  return flag;
}

void Obstacle::obstacle_apply_surface(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{

  offset.x = pos_x;
  offset.y = pos_y;
  SDL_BlitSurface(source, clip, destination, &offset );
}

SDL_Rect Obstacle::move_obstacle()
{//y= 3일 때 속도가 적당.
  if(first_exe == true){
    pos = rand()%75+75;
    first_exe=false;
  }
  
  if(count<pos) pos_y += 3;
  else{
          pos_x+=3;
          pos_y+=3;
  }
  ++count;
  return this->offset;
}

SDL_Rect Obstacle::Get_obstacle()
{
  offset.x = pos_x;
  offset.y = pos_y;
  return offset;
}
*/

Mini_Boss::Mini_Boss(Mix_Chunk* sound){
    mini_boss = load_image("assets/boss2.png");// 비행기 이미지
    //Setcolorkey는 네모난 그림에서 비행기로 쓸 그림 빼고 나머지 흰 바탕들만 투명하게 바꾸는거
    SDL_SetColorKey(mini_boss, SDL_SRCCOLORKEY,SDL_MapRGB(mini_boss->format,0,0,0));
    pos_x = 320;// 처음 시작 위치 지정
    pos_y = -MINI_BOSS_HEIGHT;//처음 시작 위치 지정
    life = 30;//has to be changed later (at least 70)
    count = 0;
    offset.w =MINI_BOSS_WIDTH;
    offset.h=MINI_BOSS_HEIGHT;
    hit_sound=sound;
    
}

Mini_Boss::~Mini_Boss(){
    this->amount--;
    delete this->mini_boss;
};

bool Mini_Boss::Got_shot(_bullets &A, int &x){
    vector<bullets>::iterator iter;
    vector<bullets> tmp;

    bool flag = false;

    for(iter = A.blt.begin(); iter != A.blt.end(); iter++)
    {
      if((pos_x + 240 < (*iter).offset.x + 9 || pos_y + 82 < (*iter).offset.y + 5) ||
      ((*iter).offset.x + 9 < pos_x + 9 || (*iter).offset.y + 5 < pos_y + 10))//안 맞았을 때
        tmp.push_back(*iter);
      else//맞았을때
      {
        Mix_PlayChannel(-1,hit_sound,0);//사운드 출력
        if((*iter).offset.x <= pos_x + MINI_BOSS_WIDTH / 5)
          x = 0;
        else if((*iter).offset.x <= pos_x + (MINI_BOSS_WIDTH / 5) * 2)
          x = 1;
        else if((*iter).offset.x <= pos_x + (MINI_BOSS_WIDTH / 5) * 3)
          x = 2;
        else if((*iter).offset.x <= pos_x + (MINI_BOSS_WIDTH / 5) * 4)
          x = 3;
        else
          x = 4;
        flag = true;
      }
    }

    A.blt = tmp;

    return flag;
};

bool Mini_Boss::Got_shot(laser_bullet A, int &x,short RNG){//레이저 빔에 맞을 때 판정

    bool flag=false;
    if(A.env){
      if(flag=intersects(this->offset,A.offset))//맞았을때
      {
        Mix_PlayChannel(-1,hit_sound,0);//사운드 출력
        x=RNG%5;
      }
    }
    return flag;
};

void Mini_Boss::shooting(_bullets &A){
    A.add_blt( 0, 5,pos_x + 125,pos_y + 82);
    A.add_blt( 3, 5,pos_x + 125,pos_y + 82);
    A.add_blt( -3, 5,pos_x + 125,pos_y + 82);
    A.add_blt( -6, 4,pos_x + 125,pos_y + 82);
    A.add_blt( 6, 4,pos_x + 125,pos_y + 82);
};

void Mini_Boss::enemy_apply_surface(SDL_Surface* destination, SDL_Rect* clip){
    //SDL_Rect offset;
    offset.y = pos_y;
    offset.x = pos_x;
    SDL_BlitSurface(mini_boss, clip, destination, &offset );
};
SDL_Rect Mini_Boss::control_plane(_bullets &A){
    if(cont_shoot>=1 && cont_shoot <= 30) {this->cont_shoot ++; if(cont_shoot%6==0)this->shooting(A);} // TO make gap between each bullets in one cycle
    if(cont_shoot > 30) {if(cont_shoot>=50)cont_shoot = 0;else cont_shoot++;} // To make gap between each cycles
    if(count % 30 == 0 ) {this->shooting(A); this->cont_shoot ++;}
    if(count < 50){
        pos_y += 3;
    }
    else
    {
        if(direction == 0){
            if(this->pos_x>550) direction =1;
            this->pos_x += 2;
        }
        else{
            if(this->pos_x<90) direction = 0;
            this->pos_x -= 2;
        }
    }
    count++;
    offset.x = pos_x;
    offset.y = pos_y;
    return this->offset;
};

SDL_Rect Mini_Boss::Get_plane()
{
  offset.x = pos_x;
  offset.y = pos_y;

  return offset;
}

void Mini_Boss::loss_life(int& score,Mix_Chunk* sound)
{
    this->life--;
    score +=50;
    if( this->life == 0) {
      Mix_PlayChannel(-1,sound,0);//보스 사망시 폭팔음 출력
      this->~Mini_Boss();
      score+=1000;
   }
}

/////////////////////////////////////////////////////////////////////////

Second_Boss::Second_Boss(Mix_Chunk* sound){
    Second_boss = load_image("assets/boss.png");// 비행기 이미지
    //Setcolorkey는 네모난 그림에서 비행기로 쓸 그림 빼고 나머지 흰 바탕들만 투명하게 바꾸는거
    SDL_SetColorKey(Second_boss, SDL_SRCCOLORKEY,SDL_MapRGB(Second_boss->format,0,0,0));
    pos_x = 320;// 처음 시작 위치 지정
    pos_y = -MINI_BOSS_HEIGHT;//처음 시작 위치 지정
    life = 35;//has to be changed later (at least 70)
    count = 0;
    offset.w =SECOND_BOSS_WIDTH;
    offset.h=SECOND_BOSS_HEIGHT;
    hit_sound=sound;
    
}

Second_Boss::~Second_Boss(){
    this->amount--;
    delete this->Second_boss;
};

bool Second_Boss::Got_shot(_bullets &A, int &x){
    vector<bullets>::iterator iter;
    vector<bullets> tmp;

    bool flag = false;

    for(iter = A.blt.begin(); iter != A.blt.end(); iter++)
    {
      if((pos_x + 240 < (*iter).offset.x + 9 || pos_y + 82 < (*iter).offset.y + 5) ||
      ((*iter).offset.x + 9 < pos_x + 9 || (*iter).offset.y + 5 < pos_y + 10))//안 맞았을 때
        tmp.push_back(*iter);
      else//맞았을때
      {
        Mix_PlayChannel(-1,hit_sound,0);//사운드 출력
        if((*iter).offset.x <= pos_x + SECOND_BOSS_WIDTH / 5)
          x = 0;
        else if((*iter).offset.x <= pos_x + (SECOND_BOSS_WIDTH / 5) * 2)
          x = 1;
        else if((*iter).offset.x <= pos_x + (SECOND_BOSS_WIDTH / 5) * 3)
          x = 2;
        else if((*iter).offset.x <= pos_x + (SECOND_BOSS_WIDTH / 5) * 4)
          x = 3;
        else
          x = 4;
        flag = true;
      }
    }

    A.blt = tmp;

    return flag;
};
void Second_Boss::shooting(_bullets &A){
    A.add_blt( 0, 5,pos_x + 125,pos_y + 82);
    A.add_blt( 3, 5,pos_x + 125,pos_y + 82);
    A.add_blt( -3, 5,pos_x + 125,pos_y + 82);
    A.add_blt( -6, 4,pos_x + 125,pos_y + 82);
    A.add_blt( 6, 4,pos_x + 125,pos_y + 82);
};

void Second_Boss::enemy_apply_surface(SDL_Surface* destination, SDL_Rect* clip){
    //SDL_Rect offset;
    offset.y = pos_y;
    offset.x = pos_x;
    SDL_BlitSurface(Second_boss, clip, destination, &offset );
};
SDL_Rect Second_Boss::control_plane(_bullets &A){
    if(cont_shoot>=1 && cont_shoot <= 30) {this->cont_shoot ++; if(cont_shoot%6==0)this->shooting(A);} // TO make gap between each bullets in one cycle
    if(cont_shoot > 30) {if(cont_shoot>=50)cont_shoot = 0;else cont_shoot++;} // To make gap between each cycles
    if(count % 30 == 0 ) {this->shooting(A); this->cont_shoot ++;}
    if(count < 50){
        pos_y += 3;
    }
    else
    {
        if(direction == 0){
            if(this->pos_x>550) direction =1;
            this->pos_x += 2;
        }
        else{
            if(this->pos_x<90) direction = 0;
            this->pos_x -= 2;
        }
    }
    count++;
    return this->offset;
};

SDL_Rect Second_Boss::Get_plane()
{
  offset.x = pos_x;
  offset.y = pos_y;

  return offset;
}

void Second_Boss::loss_life(int& score,Mix_Chunk* sound)
{
    this->life--;
    score +=50;
    if( this->life == 0) {
      Mix_PlayChannel(-1,sound,0);//보스 사망시 폭팔음 출력
      this->~Second_Boss();
      score+=1000;
   }
}
////////////////////////////////////////////////////////////////////////

Boss::Boss(Mix_Chunk* sound){
    mini_boss = load_image("assets/boss4.png");// 비행기 이미지
    //Setcolorkey는 네모난 그림에서 비행기로 쓸 그림 빼고 나머지 흰 바탕들만 투명하게 바꾸는거
    pos_x = 280;// 처음 시작 위치 지정
    SDL_SetColorKey(mini_boss, SDL_SRCCOLORKEY,SDL_MapRGB(mini_boss->format,255,255,255));
    pos_y = -MINI_BOSS_HEIGHT;//처음 시작 위치 지정
    life = 60;//has to be changed later (at least 70)
    offset.w =BOSS_WIDTH;
    offset.h=BOSS_HEIGHT;
    hit_sound=sound;
}

Boss::~Boss(){
    this->amount--;
    delete this->mini_boss;
};

bool Boss::Got_shot(_bullets &A, int& x){
    vector<bullets>::iterator iter;
    vector<bullets> tmp;

    bool flag = false;

    for(iter = A.blt.begin(); iter != A.blt.end(); iter++)
    {
      if((pos_x + 135 < (*iter).offset.x + 9 || pos_y + 85 < (*iter).offset.y + 5) ||
      ((*iter).offset.x + 9 < pos_x + 9 || (*iter).offset.y + 5 < pos_y + 10))//안 맞았을 때
        tmp.push_back(*iter);
      else//맞았을때
      {
        Mix_PlayChannel(-1,hit_sound,0);//사운드 출력
        if((*iter).offset.x <= pos_x + BOSS_WIDTH / 5)
          x = 0;
        else if((*iter).offset.x <= pos_x + (BOSS_WIDTH / 5) * 2)
          x = 1;
        else if((*iter).offset.x <= pos_x + (BOSS_WIDTH / 5) * 3)
          x = 2;
        else if((*iter).offset.x <= pos_x + (BOSS_WIDTH / 5) * 4)
          x = 3;
        else
          x = 4;

        flag = true;
      }
    }

    A.blt = tmp;

    return flag;
};
bool Boss::Got_shot(laser_bullet A, int &x,short RNG){//레이저 빔에 맞을 때 판정

    bool flag=false;
    if(A.env){
      if(flag=intersects(this->offset,A.offset))//맞았을때
      {
        Mix_PlayChannel(-1,hit_sound,0);//사운드 출력
        x=RNG%5;
      }
    }
    return flag;
};
void Boss::shooting(_bullets &A){

    //rhombus
    A.add_blt( 0, 5,pos_x + 75,pos_y + 50);
    A.add_blt( 1, 4,pos_x + 75,pos_y + 50);
    A.add_blt( -1, 4,pos_x + 75,pos_y + 50);
    A.add_blt( -2, 3,pos_x + 75,pos_y + 50);
    A.add_blt( 2, 3,pos_x + 75,pos_y + 50);
    A.add_blt( 3, 2,pos_x + 75,pos_y + 50);
    A.add_blt( -3, 2,pos_x + 75,pos_y + 50);
    A.add_blt( 4, 1,pos_x + 75,pos_y + 50);
    A.add_blt( -4, 1,pos_x + 75,pos_y + 50);
    A.add_blt( 5, 0,pos_x + 75,pos_y + 50);
    A.add_blt( -5, 0,pos_x + 75,pos_y + 50);
    A.add_blt( -4, -1,pos_x + 75,pos_y + 50);
    A.add_blt( 4, -1,pos_x + 75,pos_y + 50);
    A.add_blt( -3, -2,pos_x + 75,pos_y + 50);
    A.add_blt( 3, -2,pos_x + 75,pos_y + 50);
    A.add_blt( -2, -3,pos_x + 75,pos_y + 50);
    A.add_blt( 2, -3,pos_x + 75,pos_y + 50);
    A.add_blt( -1, -4,pos_x + 75,pos_y + 50);
    A.add_blt( 1, -4,pos_x + 75,pos_y + 50);
    A.add_blt( 0, -5,pos_x + 75,pos_y + 50);

    //cross
    /*A.add_blt( 3, 0,pos_x + 75,pos_y + 50);
    A.add_blt( 4, 0,pos_x + 75,pos_y + 50);
    A.add_blt( -3, 0,pos_x + 75,pos_y + 50);
    A.add_blt( -4, 0,pos_x + 75,pos_y + 50);*/

    //A.add_blt( 10, 0,pos_x + 35,pos_y + 50);
    //A.add_blt( -10, 0,pos_x + 35,pos_y + 50);
};
void Boss::enemy_apply_surface(SDL_Surface* destination, SDL_Rect* clip){
    //SDL_Rect offset;
    offset.y = pos_y;
    offset.x = pos_x;
    SDL_BlitSurface(mini_boss, clip, destination, &offset );
};
SDL_Rect Boss::control_plane(_bullets &A){
    if(cont_shoot>=1 && cont_shoot <15) {this->cont_shoot ++; if(cont_shoot%3==0)this->shooting(A);}
    if(cont_shoot >=15) cont_shoot = 0;
    if(count % 30 == 0 ) {this->shooting(A); this->cont_shoot ++;}
    if(count < 50){
        pos_y += 3;
    }
    else
    {
        if(direction == 0){
            if(this->pos_x>550) direction =1;
            this->pos_x += 2;
        }
        else{
            if(this->pos_x<90) direction = 0;
            this->pos_x -= 2;
        }
    }
    count++;
    offset.x = pos_x;
    offset.y = pos_y;
    return this->offset;
};

SDL_Rect Boss::Get_plane()
{
  offset.x = pos_x;
  offset.y = pos_y;

  return offset;
}
void Boss::loss_life(int& score,Mix_Chunk* sound)
{
    this->life--;
    score += 50;
    if( this->life == 0) {
      Mix_PlayChannel(-1,sound,0);
      this->~Boss();
      score+=3000;
  }
}
