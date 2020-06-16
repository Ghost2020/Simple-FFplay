#ifndef QYUV420P_RENDER_H
#define QYUV420P_RENDER_H

#include <QOpenGlContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class QYUV420P_Render : public QOpenGLFunctions
{
public:
    QYUV420P_Render();
    ~QYUV420P_Render();

    //初始化gl
    void initialize();
    //刷新显示
    //void Render(uchar* py,uchar* pu,uchar* pv,int width,int height/*,int type*/);
    void Render(uchar** ptr,int width,int height/*,int type*/);

private:
    //shader程序
    QOpenGLShaderProgram m_program;
    //shader中yuv变量地址
    GLuint m_textureUniformY, m_textureUniformU , m_textureUniformV;
    //创建纹理
    GLuint m_idy , m_idu , m_idv;
};

#endif // QYUV420P_RENDER_H
