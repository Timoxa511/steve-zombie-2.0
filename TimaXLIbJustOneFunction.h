#include "TXlib.h"

enum {NO = false, YES = true};
const double DT = 1.0;

template <typename T = double>

class Vector
    {
    public:
    T x, y;

    //-----------

    Vector ();
    Vector (T a, T b);
    Vector (POINT point);


    Vector <T> Normalize () const;
    double Angle () const;
    double Len   () const;

    Vector <T> operator + (Vector <T> that ) const;
    Vector <T> operator - (Vector <T> that) const;
    Vector <T> operator / (T divider) const;
    Vector <T> operator * (T multiplier) const;
    };

typedef Vector <> Vec;     //typedef какой-то_тип пробел название_синонима_к_этому_типу                      Vec = Vector <>

class Image
    {

    private:
    HDC Texture_;
    Vector <> Len_;
    Vector <int> nFrames_ ;

    //------------------------------------------

    public:
    explicit Image (const char FilePictureName [], Vector <int> nFrames = Vector <int> (1, 1)); //explicit - запрещяет неявный вызов конструктора с одним параметром (ибо если не защищать то могут пойти оч сложные ошибки)
    Image ();

    Vector <int> nFramesReturn () const;
    Vec GetLen () const;
    HDC GetTexture () const;

    void  Draw (Vec pos, int AnimationNumber, int t) const;
    void  DeleteTex ();

    };
class Hero;              // я обьявдяю класс (еще не целиком) что бы defcontrol работал так как он работает с классом героя и в классе героя же используется => что бы можно было бы указывать defcontrol  в конструкторе


class Hero
    {

    public:
    const char *Name_;
    Image Picture_;

    Vec pos_;
    Vec V_;

    int AnimationNumber_;
    int Type_;


    const Hero *victim_;

    //--------------------------------
    public:


    enum {NPC, CHARACTER, MOUSE, PICTURE};

    //---------------------

    Hero ();
    Hero (const char *Name, Image Picture, Vec pos, Vec V, int Type,
          const Hero *victim = NULL, int AnimationNumber = 0);

    Vec GetV () const;

    bool SetAnimation (int AnimationNumber);
    void Move (double dt);


    virtual void Control   (int KeyStopMove = VK_SPACE);
    virtual void Animation () const;
    };

class Mouse
    {
    public:
    Vec pos_;
    Vec LastClickPos_;
    int LastClickButtons_;

    //-----------------------

    Mouse ();

    Vec  GetPos();
    int  GetButtons();
    Vec  GetClickPos();
    int  GetClickButtons();
    void Update();
    };

class Time
    {
    private:
    int t_;
    //----
    public:
    Time ();
    int GetTime();
    void Update();
    };


const int N_OBJECTS = 4000;

class Engine
    {
    private:
    Hero* objects_[N_OBJECTS];
    int freePlace_;
    //--------------------------------------
    public:
    Engine ();
    int   GetObjectNumber (const char *name);
    Hero* GetObject (int num);
    int   Get_KolBo_OfObjects ();
    void  add (Hero *object);
    void  Run ();
    };


int DoubleCompareWithZero (double number);


//-----------------------------------------------------------------------------


 //
 //
 //
 //           ___________                    ____________
 //               <(|)>                               #@
 //
 //
 Mouse Global_Mouse;
 Time  Global_Timer;
 //
 //
 //
 //
 //          )________                            ______(
 //                   \_______          _________/
 //                           \________/
 //
 //
 //
 //
 //                                                                  Bill
                                                                                  //                | имеется   hero | имеется hero
                                                                                  //                | в форме обьекта| в форме указателя

                                                                                  // нужен в        |  hero          | *hero
                                                                                  // форме обьекта  |                |
//{ Engine::

Engine::Engine () :                                                               // нужен в        |  &hero         | hero                         |
    objects_  ({}),                                                               // форме указателя|
    freePlace_ (N_OBJECTS)
    {
    }
//-----------------------------------------------------------------------------

int Engine::GetObjectNumber (const char *name)
    {
    assert (name != NULL);

    for (int i = 0; i < N_OBJECTS; i++)
        {
        assert (objects_[i]->Name_ != NULL);

        if (strcmp (name, objects_[i]->Name_) == 0)
            {
            return i;
            }
        }
    return -1;
    }

//-----------------------------------------------------------------------------

Hero* Engine::GetObject (int num)
    {
    assert (0 <= num && num < N_OBJECTS);
    return objects_[num];
    }

//-----------------------------------------------------------------------------
int Engine::Get_KolBo_OfObjects ()
    {
    return (N_OBJECTS - freePlace_);
    }
//-----------------------------------------------------------------------------

void Engine::add (Hero *object)
    {

    assert (0 <= (Get_KolBo_OfObjects()) && (Get_KolBo_OfObjects()) < N_OBJECTS);
    objects_ [Get_KolBo_OfObjects()] = object;
    freePlace_--;
    }

//-----------------------------------------------------------------------------
void Engine::Run ()
    {
    txClear ();
    for (int i = 0; i < (N_OBJECTS - freePlace_); i++)
        {
        assert (0 <= i && i < (N_OBJECTS - freePlace_));
        objects_[i]->Control ();
        objects_[i]->Move (DT);
        objects_[i]->Animation ();
        }
    }
//}
//-----------------------------------------------------------------------------

//{

//{ Hero::
Hero::Hero () :
    Name_ ("Ama Default Fish, Booub"),

    Picture_ (),                    //Picture = Image ();              Image cfgyui ("fghjk"); Picture = cfgyui;         // кстати можно не писать Picture () потому что у image есть дефолт конструктор

    pos_ (),
    V_ (),

    AnimationNumber_ (0),
    Type_ (0),


    victim_ (NULL)
    {
    }

//-----------------------------------------------------------------------------

Hero::Hero (const char *Name, Image Picture, Vec pos, Vec V, int Type,
            const Hero *victim, int AnimationNumber) :

    Name_ (Name),
    Picture_ (Picture),
    pos_ (pos),
    V_ (V),
    AnimationNumber_ (AnimationNumber),
    Type_ (Type),
    victim_ (victim)
    {
    }


//-----------------------------------------------------------------------------
Vec Hero::GetV () const
    {
    return V_;
    }
//-----------------------------------------------------------------------------

void Hero::Move (double dt)
    {
    pos_ = pos_ + V_*dt;
    }

//-----------------------------------------------------------------------------
void Hero::Control (int KeyStopMove)
    {
    printf ("ama %s the Default horsie, boub, i cant moooove\n", Name_);
    }
//-----------------------------------------------------------------------------

bool Hero::SetAnimation (int AnimationNumber)
    {
    if (AnimationNumber >= Picture_.nFramesReturn().y || AnimationNumber < 0)
        {
        printf ("ERROR: wrong AnimationNumber\n"
                "       %s's AnimationNumber = %d\n"
                "       trying to set AnimationNumber = %d\n\n", Name_, Picture_.nFramesReturn().y, AnimationNumber);
        return NO;
        }

    AnimationNumber_ = AnimationNumber;

    return YES;
    }

//-----------------------------------------------------------------------------
void Hero::Animation () const
    {
    printf ("ama %s the Default horsie, boub, i cant be Animated\n", Name_);
    }

//}
//-----------------------------------------------------------------------------





//{ Image::
//-----------------------------------------------------------------------------
Image::Image () :
    Texture_ (NULL),
    Len_     (),
    nFrames_ ()
    {
    }
//-----------------------------------------------------------------------------

Image::Image (const char FilePictureName [], Vector <int> nFrames) :
    Texture_ (txLoadImage (FilePictureName)),
    Len_     (),
    nFrames_ (nFrames)
    {

    if (Texture_ == NULL)
        {
        char Message [999] = "";
        sprintf (Message, "Упс, %s не загружается", FilePictureName);
        txMessageBox (Message, "программа не работает", MB_ICONEXCLAMATION);;
        return;
        }

    HBITMAP hbitmap = (HBITMAP) Win32::GetCurrentObject (Texture_, OBJ_BITMAP);

    BITMAP bitmap = {};

    Win32::GetObject (hbitmap, sizeof(bitmap), &bitmap);

    Len_.x = bitmap.bmWidth;
    Len_.y = bitmap.bmHeight;

    }

//-----------------------------------------------------------------------------
Vector <int> Image::nFramesReturn ()   const
    {
    return nFrames_;
    }

//-----------------------------------------------------------------------------
Vec Image::GetLen () const
    {
    return Len_;
    }

//-----------------------------------------------------------------------------
HDC Image::GetTexture () const
    {
    return Texture_;
    }
//-----------------------------------------------------------------------------
void Image::DeleteTex ()
    {
    txDeleteDC (Texture_);
    Texture_ = NULL;
    }
//-----------------------------------------------------------------------------
void Image::Draw (Vec pos, int AnimationNumber, int t) const
    {
    int CurrentFrame = t % nFrames_.x;
    Vector <int> FrameLen ((int) (Len_.x/nFrames_.x), (int) (Len_.y/nFrames_.y));

    txTransparentBlt (txDC (), pos.x - FrameLen.x/2, pos.y - FrameLen.y/2, FrameLen.x, FrameLen.y,
                      Texture_, FrameLen.x * CurrentFrame, FrameLen.y * AnimationNumber);
    }

//-----------------------------------------------------------------------------
//}
//-----------------------------------------------------------------------------




//{Vector::

template <typename T>
Vector <T>::Vector () :
    x (0),
    y (0)
    {
    }

//-----------------------------------------------------------------------------
template <typename T>
Vector <T>::Vector (POINT point) :
    x (point.x),
    y (point.y)
    {
    }

//-----------------------------------------------------------------------------

template <typename T>
Vector <T>::Vector (T a, T b) :
    x (a),
    y (b)
    {
    }

//-----------------------------------------------------------------------------
template <typename T>
Vector <T> Vector <T>::operator - (Vector <T> that) const
                                                                            //Vector C = Minus (a, b);
                                                                            //Vector C = a.Minus (b);
    {
    Vector <T> res (x - that.x, y - that.y);

    return res;
    }


//-----------------------------------------------------------------------------
template <typename T>
Vector <T> Vector <T>::operator + (Vector <T> that ) const
                                                                            //Vector C = Minus (a, b);
                                                                            //Vector C = a.Minus (b);
    {
    Vector <T> res (this->x + that.x, this->y + that.y);

    return res;
    }
//-----------------------------------------------------------------------------

template <typename T>
Vector <T> Vector <T>::operator / (T divider) const
                                                                            //Vector C = Minus (a, b);                                                                         //Vector C = a.Minus (b);
    {
    Vector <T> res (x/divider, y/divider);

    return res;
    }


//-----------------------------------------------------------------------------

template <typename T>
double Vector <T>::Len () const
    {
    double VectorLen = hypot (x, y);
    return VectorLen;
    }

//-----------------------------------------------------------------------------
template <typename T>
Vector <T> Vector <T>::Normalize () const
    {
    double len = Len();
    if (len < DBL_EPSILON)
        return Vector <T> (0, 0);

    Vector <T> NormalizedVector = *this/len;
    return NormalizedVector;

    }

//-----------------------------------------------------------------------------
template <typename T>
Vector <T> Vector <T>::operator * (T multiplier) const
    {
    Vector <T> res (x * multiplier, y * multiplier);

    return res;
    }


//-----------------------------------------------------------------------------
template <typename T>
double Vector <T>::Angle () const
    {
    Vec promezshootochnoe_nazvanie = this->Normalize ();
    double angle = acos (promezshootochnoe_nazvanie.x);

    angle = angle/M_PI*180;
    if (y > 0) angle = -angle + 360;

    return angle;
    }


//}
//-----------------------------------------------------------------------------




//{Mouse::
Mouse::Mouse () :
    pos_ (),
    LastClickPos_ ()
    {}


//-----------------------------------------------------------------------------
int Mouse::GetButtons()
    {
    return txMouseButtons();
    }

//-----------------------------------------------------------------------------
Vec Mouse::GetPos()
    {
    return Vec (txMousePos());
    }

//-----------------------------------------------------------------------------
Vec Mouse::GetClickPos()
    {
    return LastClickPos_;
    }

//-----------------------------------------------------------------------------
int  Mouse::GetClickButtons()
    {
    return LastClickButtons_;
    }
//-----------------------------------------------------------------------------
void Mouse::Update()
    {
    if (Global_Mouse.GetButtons() == 1)
        LastClickPos_ = GetPos();

    LastClickButtons_ = txMouseButtons ();
    }

//-----------------------------------------------------------------------------

//}
//-----------------------------------------------------------------------------




//{Time
Time::Time () :
    t_ (0)
    {}
//-----------------------------------------------------------------------------

int Time::GetTime()
    {
    return t_;
    }

//-----------------------------------------------------------------------------

void Time::Update()
    {
    t_++;
    }
//}
//-----------------------------------------------------------------------------


//----------------'''------------------''----------------''---------------''-----------------'''--------------'''---------------'''--------------


int DoubleCompareWithZero (double number)
    {
    if (number > DBL_EPSILON) return +1;

    if (number < DBL_EPSILON) return -1;                  // DCWZ ( 0.0000000000000001) = 0
                                                          // DCWZ (-0.0000000000000001) = 0
    return 0;                                             // DCWZ ( 0.01) = +1
                                                          // DCWZ (-0.01) = -1
    }

//-----------------------------------------------------------------------------


//}
























