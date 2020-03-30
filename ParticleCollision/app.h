#ifndef APPLICATION_H
#define APPLICATION_H
class Application
{
protected:
    int height, width;
    float nRange, timeinterval;
public:
    float getTimeinterval();
    int getwidth(), getheight();
    void setTimeinterval(float timeint);
    virtual void update()                     , 
                 display()                    ,
                 initGraphics()               ,
                 resize(int width, int height);
};
#endif //APPLICATION_H


    

