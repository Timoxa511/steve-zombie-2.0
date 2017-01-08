#include "TimaXLIbJustOneFunction.h"


#define ever (;;)


//-----------------------------------------------------------------------------

struct Textures
    {
    Image Steve;
    Image Zombie;
    Image Blood;
    };


struct Steve : public Hero
    {

    //---------------
    Steve (const char *Name, Image Picture, Vec pos, Vec V);
    virtual void Animation () const override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    };

struct Zombie : public Hero
    {
    int AnimationSpeedDivider;
    int   ControlSpeedMultiplier;
    //-----------------
    Zombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve);
    virtual void Animation () const override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    };

struct gmMouse : public Hero
    {
    //---------------
    gmMouse (const char *Name, Image Picture, Vec pos, Vec V);
    virtual void Animation () const override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    };

struct FastZombie : public Zombie
    {

    //-----------------------
    FastZombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve);
    };

struct animation : public Hero
    {
    animation (Image Picture, Vec pos);
    virtual void Animation () const override;
    virtual void Control   (int KeyStopMove = VK_SPACE) override;
    };

//-----------------------------------------------------------------------------


enum {LOSE, WIN, CONTINUE};

void StartCom ();
void PlsStopSpammingKeysGetYourHandsAwayFromTheKeyBoard_Arrrrrrrr ();




void SteveAnimationNumber_and_Moving_Connecting (Hero *Steve);


//void NoAnimation     (const Hero *);

void GeneralControl (Hero *object, int KeyStopMove);
void MouseControl (Hero *Mouse, int KeyStopMove);


void ZombieSpawn (Engine *engine, Image texture, Hero *Steve);
void Zombiezomb (Hero *victim, const Hero *daddy);
int  ZombieAteUrBrains (const Steve *steve, Engine *engbro, const Textures *textures,
                        int BrainsBeingEatenRadius, int KolBoZombieNeededForUrBrainsBeingEaten);

void GameProcces (const Textures *textures);

//-----------------------------------------------------------------------------

int main()
    {
    StartCom ();

    Textures textures = {Image ("steve.bmp", {4, 18}), Image ("Zombie.bmp", {3, 2}), Image ("BloodMishura.bmp", {3, 2})};

    GameProcces (&textures);

    textures.Steve.DeleteTex();
    textures.Zombie.DeleteTex();
    textures.Blood.DeleteTex();
    txEnd ();
    PlsStopSpammingKeysGetYourHandsAwayFromTheKeyBoard_Arrrrrrrr ();
    }

//-----------------------------------------------------------------------------

void GameProcces (const Textures *textures)
    {
    //!!!
    Engine MrEngine;
    //!!!

    gmMouse mouse ("Mouse", Image (),    Vec (),   Vec ());
    Steve  steve  ("Steve",  textures->Steve,  Vec (500, 500), Vec (0, 0));
    Zombie zombie ("Zombie", textures->Zombie, Vec (700, 700), Vec (0, 0), &steve);

    //Zombiezomb (&steve, &zombie);

    MrEngine.add (&steve);
    MrEngine.add (&zombie);
    MrEngine.add (&mouse);


    while (!GetAsyncKeyState (VK_ESCAPE))
        {

        MrEngine.Run();

        SteveAnimationNumber_and_Moving_Connecting (&steve);
        ZombieSpawn (&MrEngine, textures->Zombie, &steve);
        if (ZombieAteUrBrains (&steve, &MrEngine, textures, 30, 10) == LOSE) break;

        txSleep (20);
        Global_Mouse.Update();
        Global_Timer.Update();
        }
    }

//-----------------------------------------------------------------------------
int ZombieAteUrBrains (const Steve *steve, Engine *engbro, const Textures *textures,
                       int BrainsBeingEatenRadius, int KolBoZombieNeededForUrBrainsBeingEaten)
    {
    for (int i = 0; i < engbro->Get_KolBo_OfObjects(); i++)
        {
        if ((engbro->GetObject(i)->Type_ == Hero::NPC) &&
            (hypot (steve->pos_.x - engbro->GetObject(i)->pos_.x,
                    steve->pos_.y - engbro->GetObject(i)->pos_.y)) < BrainsBeingEatenRadius)
            {
            if (--KolBoZombieNeededForUrBrainsBeingEaten <= 0)
                {
                engbro->add (new animation (textures->Blood, steve->pos_));
                return LOSE;
                }
            }
        }
    return WIN ;
    }

//-----------------------------------------------------------------------------

void Zombiezomb (Hero *victim, const Hero *daddy)
    {
    Zombie zomb ("Zombie", Image (), Vec (700, 700), Vec (0, 0), daddy);
    int t = *(int*)&zomb;
    *(int*)victim = t;
    victim->victim_ = daddy;
    }

//-----------------------------------------------------------------------------
void ZombieSpawn (Engine *engine, Image texture, Hero *Steve)
    {
    if (txMouseButtons () == 2)
        {
        Zombie *zom = (rand()%100 > 30)? (new Zombie     ("Zomb",     texture, txMousePos(), Vec (0, 0), Steve)) :
                                         (new FastZombie ("FastZomb", texture, txMousePos(), Vec (0, 0), Steve));
        engine->add (zom);
        }
    }

//-----------------------------------------------------------------------------

//{Steve::
Steve::Steve (const char *Name, Image Picture, Vec pos, Vec V) :
    Hero (Name, Picture, pos, V, CHARACTER)
    {}
//-----------------------------------------------------------------------------
void Steve::Animation () const
    {
    if (DoubleCompareWithZero (V_.x) != 0 && DoubleCompareWithZero (V_.y) != 0)
        {
        int t = Global_Timer.GetTime()/8;

        Picture_.Draw (pos_, AnimationNumber_, t);
        }

    else
        Picture_.Draw (pos_, AnimationNumber_, 0);

    }

//-----------------------------------------------------------------------------
void Steve::Control (int KeyStopMove )
    {
Vec VDist = Global_Mouse.GetPos() - pos_;

    if (VDist.Len() > 10)
        {
        Vec shortVector = VDist.Normalize() * (5*1.5 + 0.5);     //  V_ = !(Vec (txMousePos()) - pos_) * 15   | где ! - оператор нормализации
        V_ = shortVector;
        }
    else
        V_ = Vec (0, 0);


    if (GetAsyncKeyState ( KeyStopMove ))
        V_ = Vec (0, 0);
    }
//}
//-----------------------------------------------------------------------------

//{Zombie::
Zombie::Zombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve) :
    Hero (Name, Picture, pos, V, NPC, Steve),
    AnimationSpeedDivider (8),
      ControlSpeedMultiplier (5)
    {}

//-----------------------------------------------------------------------------
void Zombie::Animation () const
 {
    int t = Global_Timer.GetTime()/AnimationSpeedDivider;
    int ActiveDirection = (V_.x >= 0)?  1 : 0;

    Picture_.Draw (pos_, ActiveDirection, t);

    }

//-----------------------------------------------------------------------------
void Zombie::Control (int KeyStopMove)
    {
    Vec VDist = victim_->pos_ - pos_;

    if (VDist.Len() > 10)
        {
        Vec shortVector = VDist.Normalize () * ControlSpeedMultiplier;     //  V_ = !(Vec (txMousePos()) - pos_) * 15   | где ! - оператор нормализации
        V_ = shortVector;
        }
    else
        V_ = Vec (0, 0);


    if (GetAsyncKeyState ( KeyStopMove ))
        V_ = Vec (0, 0);
    }
//}
//-----------------------------------------------------------------------------

//{gmMouse::
gmMouse::gmMouse (const char *Name, Image Picture, Vec pos, Vec V):
    Hero (Name, Picture, pos, V, MOUSE)
    {}

//-----------------------------------------------------------------------------
void gmMouse::Animation () const
    {}

//-----------------------------------------------------------------------------
void gmMouse::Control (int KeyStopMove)
    {
    pos_ = Global_Mouse.GetClickPos();
    }

//}
//-----------------------------------------------------------------------------

//{FastZombie::
FastZombie::FastZombie (const char *Name, Image Picture, Vec pos, Vec V, const Hero *Steve) :
    Zombie (Name, Picture, pos, V, Steve)
    {
    AnimationSpeedDivider /= 4;
      ControlSpeedMultiplier *= 1.5;
    }


//}
//-----------------------------------------------------------------------------

//{animation::

animation::animation (Image Picture, Vec pos) :
    Hero ("Picture", Picture, pos, Vec (), PICTURE)
    {}
//-----------------------------------------------------------------------------

void animation::Animation () const
    {
    int t = Global_Timer.GetTime()/3;
    Picture_.Draw (pos_, 0, t);
    }

//-----------------------------------------------------------------------------
void animation::Control (int KeyStopMove)
    {
    }
//}
//-----------------------------------------------------------------------------

void SteveAnimationNumber_and_Moving_Connecting (Hero *Steve)
    {

    //$ (Steve->GetV().Len()); $n;
    if (Steve->GetV().Len() <= DBL_EPSILON)
        {
        return;
        }


    double AnimationNumber = (360 - (Steve->GetV().Angle() + 90) )/20;
    AnimationNumber =  round (AnimationNumber);
    Steve->SetAnimation (( (int) AnimationNumber + 18)%18);


    }
//-----------------------------------------------------------------------------
void GeneralControl (Hero *object, int KeyStopMove)
    {
    Vec VDist = object->victim_->pos_ - object->pos_;

    if (VDist.Len() > 10)
        {
        Vec shortVector = VDist.Normalize () * ((object->Type_ == Hero::NPC)? 10 : 15);     //  V_ = !(Vec (txMousePos()) - pos_) * 15   | где ! - оператор нормализации
        object->V_ = shortVector;
        }
    else
        object->V_ = Vec (0, 0);


    if (GetAsyncKeyState ( KeyStopMove ))
        object->V_ = Vec (0, 0);
    }
//-----------------------------------------------------------------------------

void NoAnimation (const Hero */*object*/)
    {

    }
//-----------------------------------------------------------------------------


//{ slaves::


/*//-----------------------------------------------------------------------------

void CircleAnimation (const Hero *object, int Moving)
    {

    txBitBlt (txDC (), object->pos.x - object->Picture.Len.x/4/2, object->pos.y - object->Picture.Len.y/18/2,
                      object->Picture.Len.x/4, object->Picture.Len.y/18, object->Picture.Texture,
                      object->Picture.Len.x/4 * Moving, object->Picture.Len.y/18 * object->AnimationNumber);

    $ (object->AnimationNumber); $n;
    //$ (object->pos.y - object->Picture.Len.y/18/2); $n;
    //$ (object->Picture.Len.x/4);   $n;
    //$ (object->Picture.Len.y/18); $n;
    //$ (object->Picture.Len.y/18 * object->AnimationNumber);  $n;
    //getch ();

    }
            */
//-----------------------------------------------------------------------------

/*void SuperAnimation (const Hero *object, int Active)
    {
    int ActiveMirror = 0;
    if (object->V.x >= 0) ActiveMirror = 1; else ActiveMirror = 0;

    txAlphaBlend (txDC (), object->pos.x - object->Picture.Len.x/2, object->pos.y - object->Picture.Len.y/2,
                      object->Picture.Len.x, object->Picture.Len.y, object->Picture.Texture,
                      object->Picture.Len.x * Active, object->Picture.Len.y *(object->AnimationNumber * 2 + ActiveMirror));
    }
         */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------


void StartCom ()
    {
    txCreateWindow (1800, 1000);

    txSetFillColor     (RGB (255, 255, 255));
    txClear ();
    txSetColor         (RGB(0, 0, 0));

    txBegin ();
    //txDisableAutoPause();
    }

//-----------------------------------------------------------------------------

void PlsStopSpammingKeysGetYourHandsAwayFromTheKeyBoard_Arrrrrrrr ()
    {
    while (GetAsyncKeyState ('A') || GetAsyncKeyState ('S') ||
           GetAsyncKeyState ('W') || GetAsyncKeyState ('D'))
        Sleep (0);
    }

//}
//-----------------------------------------------------------------------------


