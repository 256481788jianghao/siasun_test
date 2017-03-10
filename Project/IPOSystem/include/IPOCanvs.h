#ifndef IPOCANVS_H
#define IPOCANVS_H

#include <wx/wx.h>


class IPOCanvs : public wxPanel
{
    public:
        IPOCanvs(wxFrame* parent,wxWindowID id,wxPoint p,wxSize size);
        virtual ~IPOCanvs();
        void DrawLine(wxPoint a,wxPoint b);
        void DrawLine(double x0,double y0,double x1,double y1);
        void setInfo(double x0,double max_x,double y0,double max_y){ m_maxX = max_x;m_x0 = x0;m_maxY = max_y;m_y0 = y0;};
        wxPoint mapXY(double x,double y);
        void ClearCanvas();
        void setVerse(bool flag);
    protected:
    private:
        wxSize m_size;
        wxClientDC dc;
        double m_maxX,m_maxY,m_x0,m_y0;
        bool m_verse;
        bool m_hasDrawT;
};

#endif // IPOCANVS_H
