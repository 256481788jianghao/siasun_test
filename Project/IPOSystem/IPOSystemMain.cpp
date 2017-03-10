/***************************************************************
 * Name:      IPOSystemMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2017-03-08
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "IPOSystemMain.h"
#include <math.h>

#define STEPTIME 50
//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(IPOSystemFrame, wxFrame)
    EVT_TIMER(wxID_ANY,IPOSystemFrame::OnTimer)
    EVT_CLOSE(IPOSystemFrame::OnClose)
    EVT_MENU(idMenuQuit, IPOSystemFrame::OnQuit)
    EVT_MENU(idMenuAbout, IPOSystemFrame::OnAbout)
    EVT_BUTTON(idButtonStartOnClick, IPOSystemFrame::OnClickButtonStart)
    EVT_BUTTON(idButtonPauseOnClick, IPOSystemFrame::OnClickButtonPause)
    EVT_BUTTON(idButtonResetOnClick, IPOSystemFrame::OnClickButtonReset)
    EVT_BUTTON(idButtonResumeOnClick, IPOSystemFrame::OnClickButtonResume)
    EVT_BUTTON(idButtonSaveOnClick, IPOSystemFrame::OnClickButtonSave)
END_EVENT_TABLE()

IPOSystemFrame::IPOSystemFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title,wxPoint(0,0),wxSize(1280,720)),
    lastPoint(0,0),
    m_timer(this),
    m_curStatus(STATUS_STOP)
{
#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello Code::Blocks user!"),0);
    SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR
    wxTextCtrl *t1 = new wxTextCtrl(this,12000,"时间-位置",wxPoint(400,10));t1->SetEditable(false);
    wxTextCtrl *t2 = new wxTextCtrl(this,12000,"时间-速度",wxPoint(400,260));t2->SetEditable(false);
    wxTextCtrl *t3 = new wxTextCtrl(this,12000,"数据表",wxPoint(1000,10));t3->SetEditable(false);
    buttonSave = new wxButton(this, idButtonSaveOnClick, "保存", wxPoint(900, 580),wxSize(50,40));
    buttonStart = new wxButton(this, idButtonStartOnClick, "开始", wxPoint(750, 530),wxSize(50,40));
    buttonPause = new wxButton(this, idButtonPauseOnClick, "暂停", wxPoint(750, 580),wxSize(50,40));
    buttonResume = new wxButton(this, idButtonResumeOnClick, "恢复", wxPoint(810, 530),wxSize(50,40));
    buttonReset = new wxButton(this, idButtonResetOnClick, "重置", wxPoint(810, 580),wxSize(50,40));
    canvs_p = new IPOCanvs(this,idCanvsP,wxPoint(30, 50),wxSize(830,200));
    canvs_s = new IPOCanvs(this,idCanvsP,wxPoint(30, 300),wxSize(830,200));
    inputCurPosition = new IPOInput(this,idInputCurPosition,wxPoint(30, 530),"当前位置","mm","1000");
    inputDesPosition = new IPOInput(this,idInputCurPosition,wxPoint(272, 530),"目标位置","mm","1100");
    inputSpeed = new IPOInput(this,idInputCurPosition,wxPoint(514, 530),"速度","mm/s","20");
    inputPro = new IPOInput(this,idInputCurPosition,wxPoint(30, 590),"计算周期","ms","1000");
    inputAcsN = new IPOInput(this,idInputCurPosition,wxPoint(272, 590),"加速节拍","拍","50");
    inputDecN = new IPOInput(this,idInputCurPosition,wxPoint(514, 590),"减速节拍","拍","100");
    dataShow = new IPODataShow(this,idDataShow,wxPoint(900,50));
    m_robotInfo.timeCnt = 0;
    //makeInputEnalbe(false);
}


IPOSystemFrame::~IPOSystemFrame()
{
}
void IPOSystemFrame::showDiaLog(wxString info)
{
    wxMessageDialog *dia = new wxMessageDialog(this,info);
    dia->ShowModal();
}

void IPOSystemFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void IPOSystemFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void IPOSystemFrame::OnClickButtonStart(wxCommandEvent &event)
{
    if(m_curStatus == STATUS_STOP){
        m_startLessEnd = true;
        m_robotInfo.timeCnt = 0;
        m_robotInfo.startPos = inputCurPosition->getValue();
        m_robotInfo.curPos = m_robotInfo.startPos;
        m_robotInfo.endPos = inputDesPosition->getValue();

        if(m_robotInfo.startPos > m_robotInfo.endPos){
            m_startLessEnd = false;
        }

        m_robotInfo.desSpeed = inputSpeed->getValue();
        m_robotInfo.curSpeed = 0;
        m_robotInfo.pro = inputPro->getValue();
        m_robotInfo.acsCnt = inputAcsN->getValue();
        m_robotInfo.decCnt = inputDecN->getValue();

        if(m_robotInfo.desSpeed <= 0){
            showDiaLog("speed must > 0!!!");
            return;
        }
        if(m_robotInfo.pro <=0){
            showDiaLog("pro must > 0!!!");
            return;
        }
        if(m_robotInfo.acsCnt <=0){
            showDiaLog("acsCnt must > 0!!!");
            return;
        }
        if(m_robotInfo.decCnt <=0){
            showDiaLog("decCnt must > 0!!!");
            return;
        }

        if(abs(m_robotInfo.endPos - m_robotInfo.startPos) < 0.1){
            showDiaLog("dis too small    !!!!");
            return;
        }

        makeInputEnalbe(false);
        canvs_p->setVerse(!m_startLessEnd);
        canvs_s->setVerse(!m_startLessEnd);
        if(!m_startLessEnd){
            m_robotInfo.desSpeed *= -1;
        }
        m_robotInfo.accelerationUp = m_robotInfo.desSpeed/m_robotInfo.acsCnt;
        m_robotInfo.accelerationDown = (0-m_robotInfo.desSpeed)/m_robotInfo.decCnt;
        m_robotInfo.decPos = m_robotInfo.getDecPos();
        m_robotInfo.changeSpeed = 0;
        m_robotInfo.changeCnt = 0;
        m_robotInfo.changePos = m_robotInfo.startPos;

        m_timer.Start(m_robotInfo.pro);
        m_curStatus = STATUS_ACC;
        dataShow->appendData(m_robotInfo.timeCnt,m_robotInfo.curPos,m_robotInfo.curSpeed);
        canvs_s->setInfo(0,m_robotInfo.computePointSum(),0,m_robotInfo.desSpeed);
        canvs_p->setInfo(0,m_robotInfo.computePointSum(),m_robotInfo.startPos,m_robotInfo.endPos);
    }else{
        if(m_curStatus == STATUS_END){
            showDiaLog("please reset!!!!");
        }else{
            showDiaLog("robot is runing!!!!");
        }
    }

}
void IPOSystemFrame::OnClickButtonPause(wxCommandEvent &event)
{
    if(m_curStatus == STATUS_ACC || m_curStatus == STATUS_STABLE){
            m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
            m_robotInfo.changeCnt = m_robotInfo.timeCnt;
            m_robotInfo.changePos = m_robotInfo.curPos;
            m_curStatus = STATUS_DEC;
    }

}

void IPOSystemFrame::OnClickButtonResume(wxCommandEvent &event)
{
    if(m_startLessEnd){
        if(m_curStatus == STATUS_END && m_robotInfo.curPos < m_robotInfo.endPos){
            m_curStatus = STATUS_ACC;
            if(m_robotInfo.curPos > m_robotInfo.decPos){
                double leaveDis = abs(m_robotInfo.endPos - m_robotInfo.curPos);
                m_robotInfo.decPos = m_robotInfo.curPos + leaveDis*m_robotInfo.acsCnt/(m_robotInfo.acsCnt+m_robotInfo.decCnt);
            }
            m_timer.Start(m_robotInfo.pro);
        }else{
            if(m_curStatus == STATUS_DEC){
                showDiaLog("in dec step!!!");
            }else{
                showDiaLog("at end Position!!!");
            }
        }
    }else{
        if(m_curStatus == STATUS_END && m_robotInfo.curPos > m_robotInfo.endPos){
            m_curStatus = STATUS_ACC;
            if(m_robotInfo.curPos < m_robotInfo.decPos){
                double leaveDis = abs(m_robotInfo.endPos - m_robotInfo.curPos);
                m_robotInfo.decPos = m_robotInfo.curPos - leaveDis*m_robotInfo.acsCnt/(m_robotInfo.acsCnt+m_robotInfo.decCnt);
            }
            m_timer.Start(m_robotInfo.pro);
        }else{
            if(m_curStatus == STATUS_DEC){
                showDiaLog("in dec step!!!");
            }else{
                showDiaLog("at end Position!!!");
            }
        }
    }
}

void IPOSystemFrame::OnClickButtonReset(wxCommandEvent &event)
{
    if(m_curStatus == STATUS_END){
        m_curStatus = STATUS_STOP;
        makeInputEnalbe(true);
        dataShow->Clear();
        canvs_p->ClearCanvas();
        canvs_s->ClearCanvas();
    }
}

void IPOSystemFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void IPOSystemFrame::OnTimer(wxTimerEvent& event)
{
    double speedLastPoint = m_robotInfo.curSpeed;
    double posLastPoint = m_robotInfo.curPos-m_robotInfo.startPos;
    m_robotInfo.timeCnt++;
    if(m_curStatus == STATUS_ACC){
        m_robotInfo.curSpeed = m_robotInfo.accelerationUp*(m_robotInfo.timeCnt-m_robotInfo.changeCnt);
        m_robotInfo.curPos = m_robotInfo.changePos + 0.5*m_robotInfo.curSpeed*m_robotInfo.pro*0.001*(m_robotInfo.timeCnt-m_robotInfo.changeCnt);

        if(abs(m_robotInfo.curSpeed) >= abs(m_robotInfo.desSpeed)){
            m_robotInfo.curSpeed = m_robotInfo.desSpeed;
            m_curStatus = STATUS_STABLE;
            m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
            m_robotInfo.changeCnt = m_robotInfo.timeCnt;
        }
        if(m_startLessEnd && m_robotInfo.curPos >= m_robotInfo.decPos){
            m_curStatus = STATUS_DEC;
            //m_robotInfo.curPos = m_robotInfo.decPos;
            //m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
            adjustSpeedAndPos(1);
            m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
            m_robotInfo.changeCnt = m_robotInfo.timeCnt;
            m_robotInfo.changePos = m_robotInfo.curPos;

        }
        if(!m_startLessEnd && m_robotInfo.curPos <= m_robotInfo.decPos){
            m_curStatus = STATUS_DEC;
            //m_robotInfo.curPos = m_robotInfo.decPos;
            //m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
            adjustSpeedAndPos(1);
            m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
            m_robotInfo.changeCnt = m_robotInfo.timeCnt;
            m_robotInfo.changePos = m_robotInfo.curPos;
        }
    }else if(m_curStatus == STATUS_STABLE){
        m_robotInfo.curPos += m_robotInfo.desSpeed*m_robotInfo.pro*0.001;
        if(m_startLessEnd && m_robotInfo.curPos >= m_robotInfo.decPos){
            m_curStatus = STATUS_DEC;
            //m_robotInfo.curPos = m_robotInfo.decPos;
            //m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
            adjustSpeedAndPos(0);
            m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
            m_robotInfo.changeCnt = m_robotInfo.timeCnt;
            m_robotInfo.changePos = m_robotInfo.curPos;
        }
        if(!m_startLessEnd && m_robotInfo.curPos <= m_robotInfo.decPos){
            m_curStatus = STATUS_DEC;
            //m_robotInfo.curPos = m_robotInfo.decPos;
            //m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
            adjustSpeedAndPos(0);
            m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
            m_robotInfo.changeCnt = m_robotInfo.timeCnt;
            m_robotInfo.changePos = m_robotInfo.curPos;
        }
    }else if(m_curStatus == STATUS_DEC){
        m_robotInfo.curSpeed = m_robotInfo.changeSpeed + m_robotInfo.accelerationDown*(m_robotInfo.timeCnt-m_robotInfo.changeCnt);
        //m_robotInfo.curPos += (m_robotInfo.curSpeed+0.5*m_robotInfo.accelerationDown*m_robotInfo.pro*0.001)*m_robotInfo.pro*0.001;
        m_robotInfo.curPos = m_robotInfo.changePos + 0.5*(m_robotInfo.curSpeed+m_robotInfo.changeSpeed)*m_robotInfo.pro*0.001*(m_robotInfo.timeCnt-m_robotInfo.changeCnt);

        if(m_startLessEnd){
            if(m_robotInfo.curSpeed <= 0){
                m_robotInfo.curSpeed = 0;
                m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
                m_robotInfo.changeCnt = m_robotInfo.timeCnt;
                m_robotInfo.changePos = m_robotInfo.curPos;
                m_curStatus = STATUS_END;
                m_timer.Stop();
            }
        }else{
            if(m_robotInfo.curSpeed >= 0){
                m_robotInfo.curSpeed = 0;
                m_robotInfo.changeSpeed = m_robotInfo.curSpeed;
                m_robotInfo.changeCnt = m_robotInfo.timeCnt;
                m_robotInfo.changePos = m_robotInfo.curPos;
                m_curStatus = STATUS_END;
                m_timer.Stop();
            }
        }
    }
    canvs_s->DrawLine(m_robotInfo.timeCnt-1,speedLastPoint,m_robotInfo.timeCnt,m_robotInfo.curSpeed);
    canvs_p->DrawLine(m_robotInfo.timeCnt-1,posLastPoint,m_robotInfo.timeCnt,m_robotInfo.curPos-m_robotInfo.startPos);
    dataShow->appendData(m_robotInfo.timeCnt,m_robotInfo.curPos ,m_robotInfo.curSpeed);
}

void IPOSystemFrame::makeInputEnalbe(bool flag){
    inputCurPosition->SetModified(flag);
    inputDesPosition->SetModified(flag);
    inputSpeed->SetModified(flag);
    inputPro->SetModified(flag);
    inputAcsN->SetModified(flag);
    inputDecN->SetModified(flag);
}

void IPOSystemFrame::OnClickButtonSave(wxCommandEvent& event)
{
    wxDateTime time = wxDateTime::Now();
    wxString tmp = time.Format();
    tmp.Replace("/","_");
    tmp.Replace(" ","_");
    tmp.Replace(":","_");
    wxString filename = "./data_"+tmp+"_.txt";
    showDiaLog("save file to "+filename);
    dataShow->SaveFile(filename);
}

double IPOSystemFrame::adjustSpeedAndPos(int n)
{
    if(n == 0){
             m_robotInfo.curPos = m_robotInfo.decPos;
    }else{
        m_robotInfo.curSpeed = m_robotInfo.curSpeed*sqrt(abs(m_robotInfo.decPos-m_robotInfo.changePos)/abs(m_robotInfo.curPos-m_robotInfo.changePos));
        m_robotInfo.curPos = m_robotInfo.decPos;
    }
    return 0;
}
