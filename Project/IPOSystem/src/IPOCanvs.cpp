#include "IPOCanvs.h"

#define OFFSETX 10
#define OFFSETY 10

IPOCanvs::IPOCanvs(wxFrame* parent,wxWindowID id,wxPoint p,wxSize size)
            :wxPanel(parent,id,p,size),
            m_size(size),
            dc(this)
{
    /*wxPen pen(*wxBLUE,1);
    dc.SetPen(pen);
    dc.DrawLine(wxPoint(OFFSETX,OFFSETY),wxPoint(OFFSETX,m_size.GetHeight()-OFFSETY));
    dc.DrawLine(wxPoint(OFFSETX,m_size.GetHeight()-OFFSETY),wxPoint(m_size.GetWidth()-OFFSETX,m_size.GetHeight()-OFFSETY));
    dc.SetPen(wxNullPen);*/
    m_hasDrawT = false;
}

IPOCanvs::~IPOCanvs()
{
    //dtor
}

wxPoint IPOCanvs::mapXY(double x,double y)
{
    int myX = (m_size.GetWidth() - 2*OFFSETX)*x/(m_maxX - m_x0);
    int myY = (m_size.GetHeight() - 2*OFFSETY)*y/(m_maxY - m_y0);
    return wxPoint(myX,myY);
}

void IPOCanvs::DrawLine(wxPoint a,wxPoint b)
{
    wxPoint start,end;
    if(!m_verse){
        start = wxPoint(a.x+OFFSETX,m_size.GetHeight()-OFFSETY-a.y);
        end = wxPoint(b.x+OFFSETX,m_size.GetHeight()-OFFSETY-b.y);
        if(!m_hasDrawT){
            wxPen pen(*wxBLUE,1);
            dc.SetPen(pen);
            dc.DrawLine(wxPoint(OFFSETX,OFFSETY),wxPoint(OFFSETX,m_size.GetHeight()-OFFSETY));
            dc.DrawLine(wxPoint(OFFSETX,m_size.GetHeight()-OFFSETY),wxPoint(m_size.GetWidth()-OFFSETX,m_size.GetHeight()-OFFSETY));
            dc.SetPen(wxNullPen);
        }
    }else{
        start = wxPoint(a.x+OFFSETX,OFFSETY+a.y);
        end = wxPoint(b.x+OFFSETX,OFFSETY+b.y);
        if(!m_hasDrawT){
            wxPen pen(*wxBLUE,1);
            dc.SetPen(pen);
            dc.DrawLine(wxPoint(OFFSETX,OFFSETY),wxPoint(OFFSETX,m_size.GetHeight()-OFFSETY));
            dc.DrawLine(wxPoint(OFFSETX,OFFSETY),wxPoint(m_size.GetWidth()-OFFSETX,OFFSETY));
            dc.SetPen(wxNullPen);
        }
    }
    m_hasDrawT = true;
    wxPen pen(*wxRED,1);
    dc.SetPen(pen);
    dc.DrawLine(start,end);
    dc.SetPen(wxNullPen);
}

void IPOCanvs::DrawLine(double x0,double y0,double x1,double y1)
{
    DrawLine(mapXY(x0,y0),mapXY(x1,y1));
}

void IPOCanvs::ClearCanvas()
{
    wxPen pen(*wxWHITE,1);
    dc.SetPen(pen);
    dc.DrawRectangle(wxPoint(0,0),m_size);
    dc.SetPen(wxNullPen);
    m_hasDrawT = false;
}

void IPOCanvs::setVerse(bool flag)
{
    m_verse = flag;
}
